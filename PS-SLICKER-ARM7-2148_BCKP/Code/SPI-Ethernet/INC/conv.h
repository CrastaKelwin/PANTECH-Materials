#ifndef __CONV_H
#define __CONV_H


void ByteToStr(unsigned char input, char *output);
void WordToStr(unsigned long input, char *output);
void ShortToStr(signed char input, char *output);
void IntToStr(signed long input, char *output);
void LongToStr(signed long input, char *output);
unsigned char Bcd2Dec(unsigned char bcdnum);
unsigned char Dec2Bcd(unsigned char decnum);
unsigned long Bcd2Dec16(unsigned long bcdnum);
unsigned Dec2Bcd16(unsigned long decnum);
char *rtrim(char *ptr);


#endif
