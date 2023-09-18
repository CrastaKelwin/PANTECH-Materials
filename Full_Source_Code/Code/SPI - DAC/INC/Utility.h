#ifndef __UTILITY_H
#define __UTILITY_H


void hex2str (unsigned char *p, unsigned long intnum, unsigned char len);
unsigned char int2str (unsigned char *p, signed long intnum);

unsigned char Lo(unsigned long val);
unsigned char Hi(unsigned long val);
unsigned char Higher(unsigned long val);
unsigned char Highest(unsigned long val);

void delay_Nx10cyc (unsigned long cnt);


#endif
