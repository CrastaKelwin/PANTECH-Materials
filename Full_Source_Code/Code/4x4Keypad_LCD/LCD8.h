#ifndef __LCD8_H

#define __LCD8_H

void lcd_initialize(unsigned long *);
void delay(unsigned int);
void lcd_cmd(unsigned long *, unsigned char, unsigned char);
void lcd_data(unsigned long *, unsigned char, unsigned char);
void LCD_Config (unsigned long *, unsigned char);
void lcd_display(unsigned long *, unsigned char);

#endif