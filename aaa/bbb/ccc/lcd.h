#ifndef __LCD_H__
#define __LCD_H__

#define MAX_LCD_STRING 0x40

extern void gen_E_strobe(void);
extern void wait_BusyFlag(void);
extern void LCD_command(unsigned char data);
extern void LCD_data_write(unsigned char data);
extern void LCD_init(void);
extern void set_cursor(unsigned int row, unsigned int col);
extern void LCD_str_write(unsigned int row, unsigned int col, char *str);
#endif
