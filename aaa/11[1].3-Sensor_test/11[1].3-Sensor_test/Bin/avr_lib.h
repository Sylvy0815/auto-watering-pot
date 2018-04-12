#ifndef _JIHUN_
#define _JIHUN_

#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>

typedef unsigned char  u08;
typedef   signed char  s08;
typedef unsigned short u16;
typedef   signed short s16;
typedef unsigned long  u32;
typedef   signed long  s32;
typedef unsigned long long u64;
typedef   signed long long s64;

#define TRUE	1	
#define FALSE	0

#define MAX_U08	255
#define MAX_U16	65535
#define MAX_U32	4294967295

// maximum values that can be held
// by signed data types (8,16,32bits)
#define MIN_S08	-128
#define MAX_S08	127
#define MIN_S16	-32768
#define MAX_S16	32767
#define MIN_S32	-2147483648
#define MAX_S32	2147483647

//uart 속도 7372800hz
#define UART_9600	47
#define UART_57600	7
#define UART_115200	3

//	매개변수 함수모음
typedef char	*va_list;
#define _ADDRESSOF(v)	(&(v))
#define _INTSIZEOF(n)	((sizeof(n)+sizeof(int)-1) & ~(sizeof(int)-1))

#define va_start(ap,v)	(ap=(va_list)_ADDRESSOF(v)+_INTSIZEOF(v))

#define va_arg(ap,t)	(*(t*)((ap+=_INTSIZEOF(t))-_INTSIZEOF(t)))

#define va_end(ap)	(ap=(va_list)0)

void(*print_Data)(u08 *data);
//printf함수 마지막 출력함수 이름 ...취양에 맞게 변환할것

#ifndef outb
	#define	outb(addr, data)	addr = (data)
#endif

#ifndef inb
	#define	inb(addr)			(addr)
#endif
#ifndef BV
	#define BV(bit)			(1<<(bit))
#endif

#ifndef cbi
	#define cbi(reg,bit)	reg &= ~(BV(bit))
#endif

#ifndef sbi
	#define sbi(reg,bit)	reg |= (BV(bit))
#endif

#ifndef cli
	#define cli()			__asm__ __volatile__ ("cli" ::)
#endif

#ifndef sei
	#define sei()			__asm__ __volatile__ ("sei" ::)
#endif

#ifndef MIN
	#define MIN(a,b)			((a<b)?(a):(b))
#endif

#ifndef MAX
	#define MAX(a,b)			((a>b)?(a):(b))
#endif

#ifndef ABS
	#define ABS(x)				((x>0)?(x):(-x))
#endif

#ifndef ARRAY_SIZE
	#define ARRAY_SIZE(array)	(sizeof(array)/sizeof(array[0]))
#endif

#ifndef read_XRAM
	#define read_XRAM(address)        (*(volatile unsigned char *)(0x8000+address))
#endif

#ifndef write_XRAM
	#define write_XRAM(address,value) ((*(volatile unsigned char *)(0x8000+address))=value)
#endif

#ifndef DELAY_MS
#define DELAY_MS(x)  {static unsigned int _i; \
					for(_i=0;_i<x;_i++) \
							_delay_ms(1); }
#endif							
//delay
#define CYCLES_PER_US ((F_CPU+500000)/1000000) 	// cpu cycles per microsecond
// CPU clock speed
//#define F_CPU        16000000               		// 16MHz processor
//#define F_CPU        14745000               		// 14.745MHz processor
//#define F_CPU        8000000               		// 8MHz processor
//#define F_CPU        7372800               		// 7.37MHz processor
//#define F_CPU        4000000               		// 4MHz processor
//#define F_CPU        3686400               		// 3.69MHz processor

//딜레이 계열 
void us_delay(unsigned short time_us);
void ms_delay(u16 time_ms);


// AIC23B SPI 통신
void AIC23B_spi_write(u08 cntl,u08 data);

//normal SPI 통신
void spi_write(u08 data);

//지훈 std함수
u08 cal_bit_place(u08 bit);
void uart1_Str(char *str);
void sprint_u16(u16 data, u08 * str); //u16이내의 정수를 스트링으로 변환

void init_UART1(u08 bps);   // 시리얼 함수 초기화
void SPI_Init();

void print_init();

void Printf(unsigned char *fmt, ...);
u08 getch_u1();

void Printf_Attach(void (*userFunc)(u08 *data) );
	
#endif
