#include <lpc214x.h>
#include <stdio.h>
#include <math.h>

#define Rad   0.0174532925

#define SPI0_SEL			 (1<<29)
#define SPI0_MSTR			 0x20
#define SPI0_BE				 0x04
#define SPIF					(1<<7)

void SPI0_Init(void);
void SPI_Send(unsigned int);
void sin_function(void);
void Delay(void);

int teta,DAC_In;
float sine_val;

int main()
{
		SPI0_Init();
	
		while(1)
		{
			sin_function();
		}
}

void SPI0_Init()

{
		S0SPCR 	= 0x00;
		PINSEL0 = ((0x01<<8)| (0x01<<10)| (0x01<<12));   //SCK,MISO,MOSI
		IODIR0 	= SPI0_SEL;
    IOSET0 	= SPI0_SEL;
	
		S0SPCCR |= 0x8;
		S0SPCR 	|= SPI0_MSTR|SPI0_BE;
		
}

void SPI_Send(unsigned int data)

{
		IOCLR0 	|= SPI0_SEL;
		S0SPDR  |= (0x3000 |(data & 0xFFFF));
		while ( !(S0SPSR & SPIF));
		IOSET0 	|= SPI0_SEL;
}

void sin_function()
{
	  int teta ;
	
		for(teta=0;teta<360;teta++)	
		{
			sine_val = (2.5 * sin(teta*Rad));
			DAC_In	 = ((sine_val * 4096 /5)+2048);
			SPI_Send(DAC_In);
			Delay();
		}
}

void Delay()
{
	unsigned int i,j;
	for(i=0;i<25;i++)
		for(j=0;j<1000;j++);
}
