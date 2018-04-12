#include"avr_lib.h"

void Printf_Attach(void (*userFunc)(u08 *data) ){
	print_Data = userFunc;

}
void us_delay(unsigned short time_us){
	
	register u08 i;

	for(i=0; i < time_us ; i++){		//4 
		asm volatile("PUSH R0");		//2
		asm volatile("POP R0");			//2	 = 8cycle = 1.08us
									
	}

}
void ms_delay(u16 time_ms){
	register u16 i;

	for(i=0; i < time_ms ; i++){	
		us_delay(250);	
		us_delay(250);	
		us_delay(250);	
		us_delay(250);	// 1us * 1000 = 1ms
		
	}
}

//spi

void AIC23B_spi_write(u08 cntl,u08 data){
	

	SPDR = cntl;
	while((SPSR & 0x80)==0x00);
	PORTB	&=	~(0x01);

	SPDR	=	data;
	while((SPSR & 0x80)==0x00);
	PORTB	|=0x01;

	ms_delay(10);

}

void spi_write(u08 data){

	SPDR	=	data;
	while((SPSR & 0x80)==0x00);

}

u08 cal_bit_place(u08 bit){ //매개변수 bit 를 처음엔 char 형으로 했으나, 
							// 0x08 가 char 형으로는 	음수값으로 대처해버려 -0 으로 인식했었다.
							//그래서 unsigned char 형으로 변환 
	char i=1;

	while(1){
		if(bit>0x01)
			bit=bit>>1;
		else if(bit==0x01)
			return i;
		else
			return 0;

			i++;
	}
}



void sprint_u16(u16 data, u08 * str){
			
	if(data>10000){
		str[0] = (data/10000)+'0';
		str[1] = ((data/1000)%10)+'0';
		str[2] = ((data/100)%10)+'0';
		str[3] = ((data/10) %10 )+'0';	
		str[4] = (data%10)+'0';	
		str[5] = '\0';	
	}
	else if(data>1000){
		str[0] = (data/1000)+'0';
		str[1] = ((data/100)%10)+'0';
		str[2] = ((data/10)%10)+'0';
		str[3] = (data%10)+'0';	
		str[4] = '\0';	
	}
	else if(data>100){
		str[0] = (data/100)+'0';
		str[1] = ((data/10)%10)+'0';		
		str[2] = (data%10)+'0';	
		str[3] = '\0';	
	}
	else if(data>10){
		str[0] = ((data/10))+'0';
		str[1] = (data%10)+'0';	
		str[2] = '\0';	
	}
	else if(data>0){
		str[0] = (data)+'0';	
		str[1] = '\0';	
	}
	else	
		str[0] = '\0';	
}

void SPI_Init(){
	DDRB	|=	0x07; //우선 포트를 설정 miso 입력설정 
	PORTB	|=	0x01; // ~cs = 1 
	
	SPCR	=	0x50;
	SPSR	=	0x00;
}

void init_UART1(u08 bps)   // 시리얼 함수 초기화
{

    DDRD &= 0xFB;
	  DDRD |= 0x08;     // PE1(TXD) 출력(4), PE0(RXD) 입력(3)


     UCSR1A=0x00;
     UCSR1B=0x18;   // Receive enable, Transmitte enable
     UCSR1C=0x06;   // 비동기 방식, No parity bit, 1 stop bit   

     UBRR1H=0x00;
     UBRR1L=bps;   // 115200

}

u08 getch_u1(){

	unsigned char data;

	while((UCSR1A & 0x80)==0);

	data=UDR1;

	UCSR1A	|=	0x80;

	return data;
}

void putch_u1(unsigned char data)
{
	while((UCSR1A & 0x20)==0x00);
	UDR1=data;
  
	UCSR1A |= 0x20;         // 클리어 UDRE0
}

void uart1_Str(char *str){
	u08 i=0;
	while(str[i]!='\0')
		putch_u1(str[i++]);
}

u16 square(u08 x,u08 y){
	u08 i;
	u16 _ret=1;
	for(i=0;i<y;i++)
		_ret *= x;
	return _ret;
}


void Printf(unsigned char *fmt, ...){
	
		
	unsigned char *str,*ptr;
	unsigned char i;
	va_list arg;     	
	
	unsigned char ptrBuf[32];
	unsigned int data;
	unsigned char tmp;
	unsigned char base;
	float decimal;
	unsigned char flag=0,dec_cnt=0;
	unsigned char buf[64]={0};
	

	str = buf;//직접 buf에 쓰면되지만 포인터 주소가 이동하므로 출력준비가 모두끝났을떄
			// 즉, str에 모든 문자열이 복사된후, buf를 출력하면, str 맨처음부터 출력된다.	
	
	va_start ( arg, fmt );// 최초의 매개변수,fmt 다음의 매개변수의 포인터를 arg에 넘겨준다.
		
	for(;*fmt;*fmt++){		//  *fmp != '\0' 
		if(*fmt != '%' ){	//
			*str++ = *fmt;
			continue;
		}
		fmt++;

		if(*fmt >= '0' && *fmt <= '9'){
			dec_cnt = *fmt -'0';
			fmt++;
		}
		else 
			dec_cnt=3;

		i=0;
			switch(*fmt){
				case 'c':					
					*str++ = va_arg(arg,char);	//현재 매개변수 포인터에서  1byte 값을 읽어들인후 다음 매개변수로 이동 
				continue;

				case 's':
					ptr = va_arg(arg,char*);	
					while((*str++ = *ptr++)!='\0');
					str--;	//문자열 \0 제거 
				continue;
			
				case 'd':					
					data = va_arg(arg,unsigned int);	
					base = 10;
				break;
				
				case 'b':					
					data = va_arg(arg,unsigned int);	
					base = 2;
				break;

				case 'x':					
					data = va_arg(arg,unsigned int);	
					base = 16;
				break;

				case 'f':
					decimal =  va_arg(arg,float);				
					
					if(decimal >= 1.0){ //1.0보다 큰경우
						
						ptr = &ptrBuf[31];
						data = 	decimal;	
				
						do{			
							tmp = data%10 + '0';
							*ptr-- = tmp;
							i++;		

						}while(data/=10);
	
					while(i--)
			 			*str ++ = *(++ptr);
						}//end 1.0
			
						else //1.0보다 작은경우 
							*str++='0';					
						if(dec_cnt==0)	continue;				

						dec_cnt = (dec_cnt>3?3:dec_cnt);

						*str ++ = '.'; //소수점 을 찍는다 					
						
						data = 	decimal; 										
						data = (decimal*square(10,dec_cnt)) - (data*square(10,dec_cnt));	
						//소수점 %x 에서 x자리 까지만 구하기 위한 공식
					
						ptr = &ptrBuf[31];
									
						for(i=0;i<dec_cnt;i++){					
							tmp = (data)%10 + '0';
							data/=10;
							*ptr-- = tmp;								
						}

						for(i=dec_cnt;i>0;i--)
				 			*str ++ = *(++ptr);
					
					continue; //end 'f' 
								
					default:
					continue;
			}//END CASE
	


		//숫자변환 
		ptr = &ptrBuf[31];		
		
		do{
			tmp = data%base;
	
			if(tmp >9)
				tmp += 'A'-10;
			else
				tmp += '0';

			*ptr-- = tmp;
			i++;
			
			
			
		}while(data/=base);
	
		while(i--)
			 *str ++ = *(++ptr);
		
	}//end for
	
	*str ='\0'; //문자열 끝선언
			
	print_Data(buf);
	
	va_end(arg);
}//end func
