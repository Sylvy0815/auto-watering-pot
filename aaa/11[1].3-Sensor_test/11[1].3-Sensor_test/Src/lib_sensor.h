#ifndef _LIB_SENSOR_
#define  _LIB_SENSOR_

#include<avr/io.h>
#include"avr_lib.h"


//===================================================================
//
// adc_sht11.h (@chlee)
//
//===================================================================
// Copyright 2006, HANBACK
//===================================================================
// 
// Author(s): Cheol-Hee,Lee
// Date     : 2005-01-6
// Purpose  : ZigbeX definitions for sht11 of qplusn
// Usage    : This file is used in nano qplus OS
//
//===================================================================

/* adc_sht11.h - ZigbeX definitions for adc of MCU board.      */

#define   SHT11_INTERRUPT_NUM      1
#define   SHT11_INTERRUPT_EDGE     0x0c

#define   SHT11_INTERRUPT_ENABLE()\
    do {\
          EICRA &= ~SHT11_INTERRUPT_EDGE;\
          EIMSK |= BV(SHT11_INTERRUPT_NUM);\
    } while(0)
        
#define   SHT11_INTERRUPT_DISABLE()\
    do {\
          EICRA &= ~BV(SHT11_INTERRUPT_NUM+2);\
          EIMSK &= ~BV(SHT11_INTERRUPT_NUM);\
    } while(0)
/* Interrupt Service Routine */
/* External Interrupt 1 */

void sht11_external_isr(void);
void start_sht11_sensor(void);
u16 get_sht11_hanback_data(u08 type);

u08 s_write_byte(u08 value);
u08 s_read_byte(u08 ack);
void s_transstart(void);
void s_connectionreset(void);
u08 s_softreset(void);
u08 s_read_statusreg(u08 *p_value, u08 *p_checksum);
u08 s_write_statusreg(u08 *p_value);
u08 s_measure(u16 *p_value, u16 *p_checksum, u08 mode);
void calc_sth11(u16 p_humidity ,u16 p_temperature);
float calc_dewpoint(float h,float t);
void initialize_sht11_hanback(void);
void sht11_delay(unsigned short time_us);


enum {DEW,TEMP,HUMI};


#endif 
