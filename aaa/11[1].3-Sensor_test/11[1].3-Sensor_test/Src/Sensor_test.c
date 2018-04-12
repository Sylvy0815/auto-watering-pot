/**********************************************************************************
 
 파일명 : Sensor_test.c
 
 동 작 : 
  온습도 센서로부터 온도값 센서값을 TextLCD에 출력한다.
 
 포트연결
 1) 포트D 0~1을 SENSOR모듈의 TMP_SCK,TMP_SDA핀에 연결한다.
    PD0 -> TMP_SCK
    PD1 -> TMP_SDA
  
 
 2) 포트C 0~7핀은 TEXT LCD모듈의 D0~D7에 연결한다.
    포트B 5~7핀은 TEXT LCD모듈의 RS,RW,E 에 연결한다.
    PC0~7 -> D0~D7
    PD5  -> RS
    PD6  -> RW
    PD7  -> E
 

소스 설명
1) AVR,  SHT11 센서 , lcd, ADC 헤더를 선언한다.
 ARRAY_SIZE(array): 배열의 크기 계산해준다.
 DELAY_MS(x) : 200ms 가 한계였던 _delay_ms()함수를 확장한 선언 함수

2) 온도 또는 습도 값을 lcd에 출력하는 함수
3) sht11 센서 초기화 및 시작 함수
4) ADC 초기화  함수
5) TextLCD 초기화 함수
6) SHT11 로 부터 온도값을 추출한다.
7) SHT11 로 부터 습도값을 추출한다.
8) 적외선 센서 값을 추출한다.
9) 조도센서값을 추출한다.
10) 추출한 습도,온도값을 LCD에 출력한다

***************************************************************/

// 1)
#include<avr/io.h>
#include<util/delay.h>
#include"lib_sensor.h"
#include"lcd.h"
#include"avr_lib.h"

#define ARRAY_SIZE(array)	(sizeof(array)/sizeof(array[0]))
#define DELAY_MS(x)  {static unsigned int _i; \
					for(_i=0;_i<x;_i++) \
							_delay_ms(1); }

u16 Ultra=0,Cds=0;

char buf[10]={0},buf1[10]={0};

// 2)
void printf_2dot1(u08 sense,u16 sense_temp);


int main(){

 volatile u16 temp,humi;
 
 // 3)
 initialize_sht11_hanback();
 start_sht11_sensor(); 

 // 4)
 
 
 // 5)
 lcdInit(); 

 lcdPrintData("Sensor_Test",sizeof("Sensor_Test")-1); 
 
 DELAY_MS(500);
   
 lcdClear();

 while(1){
 
  //온도 측정 
  // 6)
  temp = get_sht11_hanback_data(TEMP);
  DELAY_MS(100);
 
  //습도 측정 
  // 7)
  humi = get_sht11_hanback_data(HUMI);
  DELAY_MS(100); 
 
  lcdGotoXY(0,0); 
 
  // 10)
  printf_2dot1(TEMP,temp);
 
  lcdGotoXY(0,1);
 
  printf_2dot1(HUMI,humi); 
 
  DELAY_MS(300);
 
 }
 return 0;
}


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


