#include <lpc214x.h>
#include <stdio.h>

#define LED_ROW			0xFFFFFFFF		// LED as P1.16 to P1.23

void DelayMs(unsigned int ms);

int main(void)
{
		PINSEL2=4;
		PINSEL1=0;
		IODIR0  = LED_ROW;			//Configure P1.20 Output

		while(1)				//loop forever
		{
			IOSET0 	= LED_ROW;
			DelayMs(200);
			IOCLR0 	= LED_ROW;
			DelayMs(200);
		}
}

void DelayMs(unsigned int ms)                     //Delay at 30Mhz frquency
{
	long i,j;
	for (i = 0; i < ms; i++ )
	for (j = 0; j < 6659; j++ );
}
