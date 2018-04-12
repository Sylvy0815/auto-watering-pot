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

// Solenoid Valve. Valve Output port
#define VO PB0
 
// Humidity check
#define CHECK_INTERVAL 30000000			// 5 minutes == 30000000

// Water pump control
#define AUTO_STOP_INTERVAL 6000000		// 1 minute == 6000000
#define HUMIDITY_DRY 300
#define HUMIDITY_HUMID 700

// LCD_print(line, message)
void LCD_print(unsigned int line, unsigned char msg[MAX_LCD_STRING]) {
	unsigned char lcd_string[2][MAX_LCD_STRING];

	sprintf(lcd_string[0], msg);
	LCD_str_write(line, 0, lcd_string[0]);
}

void adc_init(void)
{
	DDRB = 0xFF;	// Solenoid Valve output port
	DDRF = 0x00;	// ADC input port
	ADCSRA = 0xA5;	// 11100101 ADC enable, free running mode, single conversion mode, auto trigger, 500kHz (prescaler: 32)
	SFIOR &= 0x1F;	// ADMUX Activate. Free Running mode
}

uint16_t get_adc(uint8_t channel)
{
	uint16_t result = 0;

	ADCSRA |= (1 << ADSC);	// start conversion

	ADCSRA |= (1 << ADIF);	// ADC restart
	while (!((ADCSRA >> ADIF) & 1));	// stand by until ADC completed
	result += ADC;						// resolving Power: 1024


	ADCSRA &= (~(1 << ADSC));	// stop conversion

	return result;
}

void loop(void) {
	int humi;
	char lcd_string[2][MAX_LCD_STRING];

	adc_init();		// ADC initialization
	ADMUX = 0x40;	// Channel: 0
	ADCSRA = 0xE7;		// 11100111 ADC control and Status Register A

	while(1){
		humi = get_adc(0); 
		_delay_ms(100000);		// 1 second
		if (humi<HUMIDITY_DRY){
			LCD_print(1, "Hum: Dry soil");
			PORTB = (PORTB & 0) | 1 << VO;	// turn on the Solenoid Valve
			_delay_ms(AUTO_STOP_INTERVAL);	// wait 5 minutes
		}
		else if(HUMIDITY_DRY<=humi && humi<HUMIDITY_HUMID){
			LCD_print(1, "Hum: Humid soil");
		}
		else if(HUMIDITY_HUMID<humi){
			LCD_print(1, "Hum: In water");
		}
	}
}  // End of loop()



//****************************** MAIN **********************************
#define f_cpu 16000000UL

int main(void)
{
	LCD_init();
	LCD_print(0, "Auto flowerpot");
	loop();

	return 0;
}
