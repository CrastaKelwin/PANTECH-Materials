/**********************************************************************************
;    Filename:	    SPI.c                                           
;                                         							  
;    Author:       M.Sukumar                                          
;    Company:      PANTECH SOLUTIONS                                  
;    Notes:		   Program to Demonstrate software SPI using MCP4921 DAC       
				   CS,SCL,MISO,MOSI Pins of CONFIG switch should be    
;				   Turned ON before loading the Program                  
;**********************************************************************************/
#include <LPC214x.H>
 
//SPI lines 
#define CS    0x20000000				//Chip select ON RC0	
#define MOSI  0x00000040					//Master Out Slave In
#define MISO  0x00000020				//Master in slave out
#define SCK   0x00000010					//Clock

unsigned int Square=0;
void SPi_init(void);
void SPi_WRITE(unsigned short);

void Square_Wave(void);
void Ramp_Wave(void);
void DelayMs(unsigned int Ms);

int main()
{
	DelayMs(100);
	PINSEL0 = 0;
	IODIR0  = ( CS | SCK | MISO | MOSI);								
	SPi_init();
	DelayMs(100);
	while(1)
	{	
		if(Square==1) Square_Wave();
		else Ramp_Wave();
	}
}	

void SPi_init()
{
	IOSET0 |= CS;								//Make CS pin high
	IOCLR0 |= MOSI | SCK;						//Clear input pin
}

void SPi_WRITE(unsigned short Addr)
{										//Rise CS and pull down again
	unsigned int i;
	IOCLR0 |=CS;	
	for(i=0;i<16;i++)					//Send Data's
	{
		if(Addr & 0x8000) 
			IOSET0 |=MOSI;
		else   
			IOCLR0 |=MOSI;
		IOSET0 |=SCK;
		Addr=Addr<<1;
		IOCLR0 |=SCK; 						
	}
	IOSET0 |=CS;
}

void Square_Wave(void)
{
	SPi_WRITE(0x3fff);			
	DelayMs(500);
	SPi_WRITE(0x3000);
	DelayMs(500);
}

void Ramp_Wave(void)
{
	unsigned int i;
	for(i=0;i<0x0fff;i++)
		SPi_WRITE(0x3000|i);			

	for(i=0x0fff;i>0;i--)
		SPi_WRITE(0x3000|i);			
}

void DelayMs(unsigned int Ms)
{
	int delay_cnst;
	while(Ms>0)
	{
		Ms--;
		for(delay_cnst = 0;delay_cnst <220;delay_cnst++);
	}
}

