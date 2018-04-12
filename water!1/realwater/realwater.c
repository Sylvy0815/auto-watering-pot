#include <stdio.h>    // ������ sprintf()�� ���� #include ����
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>
#include "lcd.h"

#define f_cpu 16000000UL
int ValvePin = 4;                // Solenoid valve connected to pin 4
int MoistPin = 5;
int val = 0;

void setup()                    // run once, when the sketch starts
{
  pinMode(ValvePin, OUTPUT);      // sets the digital pin as output
//  pinMode(SwPin, INPUT);
//  pinMode(LEDPin, OUTPUT);
  Serial.begin(9600);
  buttonIs = digitalRead(SwPin); //Read the initial state of the switch!
}

//===========Functions=====================//
//void getButton() { 
//  buttonWas = buttonIs; // Set the old state of the button to be the current state since we're creating a new current state.
//  buttonIs = digitalRead(SwPin); // Read the button state
//} 

void openValve(){
    digitalWrite(ValvePin, HIGH);
//    digitalWrite(LEDPin,HIGH);
    Serial.println("Valve Open");
}

void closeValve(){
  digitalWrite(ValvePin, LOW);
//  digitalWrite(LEDPin,LOW);
  Serial.println("Valve closed");
}
  
//=================Main Loop===================//
void loop()                     // run over and over again
{
// getButton();
  
  if((buttonIs==1)&&(buttonWas==0)) { 
    openValve();
      }
  
  val = analogRead(MoistPin);
  
  if((val > 500)){ 
    closeValve();
      }
}

//=====================Main====================//
int main(void)
{
	int i;
	 
	char lcd_string[2][MAX_LCD_STRING];	//LCD ��¿� ���Ǵ� ����
	LCD_init();		//LCD �ʱ�ȭ

	sprintf(lcd_string[0], "LCD Test");
	LCD_str_write(0, 0, lcd_string[0]);

	sprintf(lcd_string[0], "Microprocessor");
	LCD_str_write(1, 0, lcd_string[0]);

	return 0;
}