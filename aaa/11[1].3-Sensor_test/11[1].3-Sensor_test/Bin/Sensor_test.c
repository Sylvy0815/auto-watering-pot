/**********************************************************************************
 
 ���ϸ� : Sensor_test.c
 
 �� �� : 
  �½��� �����κ��� �µ��� �������� TextLCD�� ����Ѵ�.
 
 ��Ʈ����
 1) ��ƮD 0~1�� SENSOR����� TMP_SCK,TMP_SDA�ɿ� �����Ѵ�.
    PD0 -> TMP_SCK
    PD1 -> TMP_SDA
  
 
 2) ��ƮC 0~7���� TEXT LCD����� D0~D7�� �����Ѵ�.
    ��ƮB 5~7���� TEXT LCD����� RS,RW,E �� �����Ѵ�.
    PC0~7 -> D0~D7
    PD5  -> RS
    PD6  -> RW
    PD7  -> E
 

�ҽ� ����
1) AVR,  SHT11 ���� , lcd, ADC ����� �����Ѵ�.
 ARRAY_SIZE(array): �迭�� ũ�� ������ش�.
 DELAY_MS(x) : 200ms �� �Ѱ迴�� _delay_ms()�Լ��� Ȯ���� ���� �Լ�

2) �µ� �Ǵ� ���� ���� lcd�� ����ϴ� �Լ�
3) sht11 ���� �ʱ�ȭ �� ���� �Լ�
4) ADC �ʱ�ȭ  �Լ�
5) TextLCD �ʱ�ȭ �Լ�
6) SHT11 �� ���� �µ����� �����Ѵ�.
7) SHT11 �� ���� �������� �����Ѵ�.
8) ���ܼ� ���� ���� �����Ѵ�.
9) ������������ �����Ѵ�.
10) ������ ����,�µ����� LCD�� ����Ѵ�

***************************************************************/

// 1)
#include<avr/io.h>
#include<util/delay.h>
#include<avr/interrupt.h>
#include"lib_sensor.h"
#include"lcd.h"
#include"avr_lib.h"
//////////////////////////////////////////////////////////////�������


#define ARRAY_SIZE(array)	(sizeof(array)/sizeof(array[0]))
#define DELAY_MS(x)  {static unsigned int _i; \
					for(_i=0;_i<x;_i++) \
							_delay_ms(1); }

u16 Ultra=0,Cds=0;

char buf[10]={0},buf1[10]={0};
unsigned char timer0cnt=0,time_s=0,time_m=0,time_h=0,time_D=0;
SIGNAL(SIG_OVERFLOW0);
// 2)
void printf_2dot1(u08 sense,u16 sense_temp);

//////////////////////////////////////////////
/*                  ���� 
                                         */
int main(){


 DDRE=0xff;
 TCCR0 = 0x07;
 TCNT0 = 0xff-80;
 TIMSK|= 1<<TOIE0;
 TIFR |=1<<TOV0;


 volatile u16 temp,humi;
 
 // 3)
 initialize_sht11_hanback();
 start_sht11_sensor(); 

 // 4)
 
 
 // 5)
 lcdInit(); 

 lcdPrintData("By. LAPOTE",sizeof("Sensor_Test")-1); 
 
 DELAY_MS(500);
   
 lcdClear();

 while(1){
 
  //�µ� ���� 
  // 6)
  PORTE=0xc0;
  if(time_s>=4)
  {
  PORTE|=0x30;
  }

  temp = get_sht11_hanback_data(TEMP);
  DELAY_MS(100);
  if(temp/10>=30)
  {
  PORTE|=0x03;
  _delay_ms(200);
  }
  else if(temp/10<=20)
  {
  PORTE|=0x03;
  _delay_ms(200);
  }
  else
  { 
  PORTE|=0x00;
  _delay_ms(200);
  }
  //���� ���� 
  // 7)
  humi = get_sht11_hanback_data(HUMI);
  DELAY_MS(100); 
 if(humi/10>=50)
  {
  PORTE|=0x0c;
  _delay_ms(200);
  }
  else if(humi/10<=20)
  {
  PORTE|=0x0c;
  _delay_ms(200);
  }
  else
  { 
  PORTE|=0x00;
  _delay_ms(200);
  }
  lcdGotoXY(0,0); 
 
  // 10)
  printf_2dot1(TEMP,temp);
 
  lcdGotoXY(0,1);
 
  printf_2dot1(HUMI,humi); 
 
  DELAY_MS(300);
  if(time_s>=10)
  {
   PORTE|=0xc0;
   _delay_ms(200);
   }
   else
   {
   PORTE|=0x00;
   }
  
  
 
 }
 
 return 0;
}

/*�����Լ� ���� */
/////////////////////////////////////////////////////////////////////
void printf_2dot1(u08 sense,u16 sense_temp) {
 u08 s100,s10;

 if(sense == TEMP) lcdPrintData(" Temp: ",7);
 else if(sense == HUMI) lcdPrintData(" Humi: ",7);
 
 s100 = sense_temp/100;
 
 if(s100> 0) lcdDataWrite(s100+'0');
 else lcdPrintData(" ",1);
 
 s10 = sense_temp%100;
 lcdDataWrite((s10/10)+'0');
 lcdPrintData(".",1);
 lcdDataWrite((s10%10)+'0');


 
}

SIGNAL(SIG_OVERFLOW0)
{

  cli();
   TCNT0 = 0xff-80;
   timer0cnt++;
   
   if(timer0cnt==90)
   {
    timer0cnt=0;
   	time_s++;
	if(time_s>=60)
	{	
	time_m++;
	time_s=0;
	}
	if(time_m>=59)
	{
	time_h++;
	time_m=0;
	}
	if(time_h>=24)
	{
	time_h=0;
	time_D++;
	}

	
	
   }
   sei();
}


