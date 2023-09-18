/********************************************************************
	DAC Test Examples
 	--------------------------
  	
	Description
	-----------
	Measure Analog voltage at pin p0.28
	ADC output given to DAC's input

	Test configuration:
	-------------------
	Oscillator	:   12.0 MHz (cclk = 60.0 MHz, Fcco = 240.0 MHz)
	ADC			:	4.28MHz (VPBclock/7,   30MHZ/7)

	Connections - ADC0.1		
	-------------------------
	P0.28 	-	AD0.1			P0.25	-	DAC O/p
/********************************************************************/
#include <lpc213x.h>
#include <stdio.h>

void DelayMs(unsigned int);

//-------------------------
//main program starts here
//-------------------------
void main(void)
{
	unsigned long val;

	PINSEL0 = 0x00000005;
	PINSEL1 = 0X01080000;
	VPBDIV 	= 	0x02;						//Set the Pclk to 30 Mhz

	U0LCR = 0x83;						
	U0DLL = 195;							//9600 baud rate
	U0LCR = 0x03;

	AD0CR	=	0x00270402;
	AD0CR	|=	0X01000000;
	IO0DIR  = 	0x0FFF7030;
	printf("******************************************************\n");
	printf("ADC I/P Adjust Pot(R14) and Measure P0.25 for DAC O/P \n");
	printf("******************************************************\n");

	while(1)
	{
		do
		{
		    val = AD0DR;                   	// Read A/D Data Register 
		}     
		while ((val & 0x80000000) == 0);  //Wait for the conversion to complete
		val = ((val >> 6) & 0x03FF)<<16;  //Extract the A/D result
		DelayMs(20);
		printf("ADC I/P: %d \r",val>>16);
		val = val>>16; 
		val = val<<6;
		DACR = (val & 0x0000FFC0);
		DelayMs(50);
	}
}

//Delay Routine
void DelayMs(unsigned int count) 	
{  
									// mSec Delay 11.0592 Mhz 
    unsigned int i,j;		       		// Keil v7.5a 
    for(i=0;i<count;i++)
	{
		for(j=0;j<1000;j++);
	}
}
 
