#include <avr/io.h>
#include "lcd.h"

#define RS PD5
#define RW PD6
#define E  PD7

void gen_E_strobe(void)
{
	volatile int i;

	PORTD |= 1<<E; 			// E 신호를 High로
	for(i=0; i<10; i++);	// E 스트로브 신호를 일정기간 High로 유지
	PORTD &= ~(1<<E);		// E 신호를 Low로
}

void  wait_BusyFlag(void)
{
	volatile int i;
	unsigned char bf;

	DDRC = 0x0;		// 포트 C를 입력핀으로 설정
	PORTD = (PORTD & ~(1<<RS)) | 1<<RW; // RS <- Low, RW <- High
	
	do
	{
		PORTD |= 1<<E;			// E 신호를 High로
		for(i=0; i<10; i++);	// E 스트로브 신호를 일정기간 High로 유지
		bf = PINC & 1<<PC7;		// busy flag를 읽어 냄.
		PORTD &= ~(1<<E);		// E 신호를 Low로
	}while(bf);					// bf값이 0이 아니면 busy, 0이 될때 까지 반복
}

void LCD_command(unsigned char data)
{
	wait_BusyFlag();			// busy flag가 0이 될 때까지 대기
	DDRC = 0xFF;				// 포트 C를 출력핀으로 설정
	PORTC = data;				// 데이터 출력
	PORTD &= ~(1<<RS | 1<<RW);	// RS <- 0, RW <- 0
	gen_E_strobe();				// E 스트로브 신호 만들기
}

void LCD_data_write(unsigned char data)
{
	wait_BusyFlag();
	DDRC = 0xFF;
	PORTC = data;
	PORTD = (PORTD | 1<<RS) & ~(1<<RW); // RS <- 1, RW <- 0
	gen_E_strobe();
}

void LCD_init(void)
{
	DDRD |= 1<<RS | 1<<RW | 1<<E; 		// RS, RW, E 핀을 출력핀으로 설정

	PORTD &= ~(1<<RS | 1<<E | 1<<RW);	// 초기 RS, E, RW <- 0

	LCD_command(0x3C);
	LCD_command(0x02);
	LCD_command(0x01);
	LCD_command(0x06);
	LCD_command(0x0F);
}

void set_cursor(unsigned int row, unsigned int col)
{
	LCD_command(0x80 + (row % 2) * 0x40 + (col % 0x40));
}

void LCD_str_write(unsigned int row, unsigned int col, char *str)
{
	int i;

	set_cursor(row, col);
	for(i=0; (i+col < MAX_LCD_STRING) && (str[i] != '\0'); i++)
		LCD_data_write(str[i]);
}
