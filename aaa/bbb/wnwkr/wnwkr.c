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


void loop(void) {
	int i=1;
	int humi=1;
	char lcd_string[2][MAX_LCD_STRING];
	
	while(i>0){

		_delay_ms(100000);
		if (i>0 && i<300){
			sprintf(lcd_string[0], "%3d: Dry soil", i);
		}
		else if(i>=300 && i<700){
			sprintf(lcd_string[0], "%3d: Humid soil", i);
		}
		else if(i>=700){
			sprintf(lcd_string[0], "%3d: In water", i);
		}
		LCD_str_write(1, 0, lcd_string[0]);
	
		i=i+1;
	}
}



//****************************** MAIN **********************************
#define f_cpu 16000000UL

int main(void)
{
	char lcd_string[2][MAX_LCD_STRING];
	LCD_init();

	LCD_print(0, "Auto flowerpot");
	_delay_ms(600000);
	setup();
	loop();

	return 0;
}
