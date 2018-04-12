//////////////////////////////////////////////////////////

// 1. Sends registration code to server at start-up and in every 30 min
// 2. Sends humidity of soil to server every 5 min
// 3. Responds at ping command from remote.
// 4. When it receives control command from server, feed water to flowerpot
//    And send response with result code
//////////////////////////////////////////////////////////

#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>
#include "lcd.h"

// Humity Sensor
#define HI PF1
// Solenoid Valve
#define VO PB1
 
// Humidity check
#define CHECK_INTERVAL 6000000  // 5 minutes == 30000000  1 minute == 6000000
unsigned long prevReadTime = 0;

// Water pump control
#define AUTO_STOP_INTERVAL 1500
#define HUMIDITY_THRESHOLD 250
int isValveOn = 0;
unsigned long prevValveTime = 0;


void setup(void) {
  // initialization
  DDRF = 0x00;	// Humidity Sensor
  DDRB = 0xFF;			// Solenoid Valve
}
// LCD print
void LCD_print(unsigned int line, unsigned char msg[MAX_LCD_STRING]) {
	unsigned char lcd_string[2][MAX_LCD_STRING];

	sprintf(lcd_string[0], msg);
	LCD_str_write(line, 0, lcd_string[0]);
}

uint16_t get_adc(uint8_t channel)
{
	uint16_t result = 0;
	
	ADCSRA |= (1 << ADSC);	// start conversion

		ADCSRA |= (1 << ADIF);	// ADC 재시작		
		while( !((ADCSRA >> ADIF) & 1) );	// ADC 완료할때까지 대기
		result += ADC;						// 1024분해능으로 데이터를 받아온다.


	ADCSRA &= (~(1 << ADSC));	// stop conversion

	return result;
}

void loop(void) {
	int i=1;
	int humi=1;
	char lcd_string[2][MAX_LCD_STRING];
	
	while(i>0){
		PORTB =1;		// HIGH
		_delay_ms(100000);
		PORTB =0;		// LOW

		_delay_ms(100000);

		sprintf(lcd_string[0], "time: %3d", i);
		LCD_str_write(1, 0, lcd_string[0]);
	
		i=i+1;
	}
	while(1){
		sprintf(lcd_string[0], "hum: %4d", humi);
		LCD_str_write(1, 0, lcd_string[0]);
		_delay_ms(100000);
		get_adc(0);
	}


	while(1){
		humi = get_adc(0);	// Get Humidity

		if (humi < HUMIDITY_THRESHOLD) {
			LCD_print(0, "Auto flowerpot");
			LCD_print(1, "Start pumping....");
			PORTB = (PORTB & 0) | 1<<VO;	// turn on the Solenoid Valve

			_delay_ms(AUTO_STOP_INTERVAL);	// wait 5 minutes

			LCD_print(1, "Stop pumping.....");
			PORTB = (PORTB & 0) | 0<<VO;	// turn off the Solenoid Valve
			_delay_ms(5000);
		}
			
		sprintf(lcd_string[0], "Humidity: %d%%", humi);
		LCD_str_write(1, 0, lcd_string[0]);
	}

}  // End of loop()



//****************************** MAIN **********************************
#define f_cpu 16000000UL

int main(void)
{
	char lcd_string[2][MAX_LCD_STRING];
	setup();
	LCD_init();

	LCD_print(0, "Auto flowerpot");
	setup();
	loop();

	return 0;
}
