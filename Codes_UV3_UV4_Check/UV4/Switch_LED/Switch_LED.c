#include<lpc214x.h>							 

#define LED0    0x010000
#define LED1    0x020000
#define LED2    0x040000
#define LED3    0x080000
#define LED4    0x100000
#define LED5    0x200000
#define LED6    0x400000
#define LED7    0x800000

unsigned int SW_RD;

int main()
{
	PINSEL2=4;
	IODIR1  =   0xFF0000;									//All the GPIO pins configured 	as output
	IODIR0  =   0x000000;									//All the GPIO pins configured 	as output
	
	while(1)
	{	
		SW_RD    = ((IOPIN0 & 0xFF0000)>>16 & 0xFF);
		if(SW_RD&0x80) IOSET1 = LED0;	else IOCLR1 = LED0;
		if(SW_RD&0x40) IOSET1 = LED1;	else IOCLR1 = LED1;
		if(SW_RD&0x20) IOSET1 = LED2;	else IOCLR1 = LED2;
		if(SW_RD&0x10) IOSET1 = LED3;	else IOCLR1 = LED3;
		if(SW_RD&0x08) IOSET1 = LED4;	else IOCLR1 = LED4;
		if(SW_RD&0x04) IOSET1 = LED5;	else IOCLR1 = LED5;
		if(SW_RD&0x02) IOSET1 = LED6;	else IOCLR1 = LED6;
		if(SW_RD&0x01) IOSET1 = LED7;	else IOCLR1 = LED7;
	}
}

