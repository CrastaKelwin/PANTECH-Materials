/********************************************************************
	ADC Test Examples
 	--------------------------
  	
	Description
	-----------
	ADC 0.1- variey the trimpot, corresponding values send to serial port.

	Test configuration:
	-------------------
	Oscillator	:   12.0 MHz (cclk = 60.0 MHz, Fcco = 240.0 MHz)
	ADC			:	4.28MHz (VPBclock/7,   30MHZ/7)

	Connections - ADC0.1		(Adc0.0 is not availabel in LPC2138)
	--------------------------
	P0.28 	-	AD0.1			
/********************************************************************/

#include <lpc213x.h>
#include <stdio.h>



//------------------------------
//	Serial Port Initialize
//------------------------------
void serial_init(void)
{
  PINSEL0 = 0x00000005;           /* Enable RxD0 and TxD0                	*/
  U0LCR = 0x83;                   /* 8 bits, no Parity, 1 Stop bit          */
  U0DLL = 195;                     /* 9600 Baud Rate @ 12MHz VPB Clock       */
  U0LCR = 0x03;                   /* DLAB = 0        						*/
}


//------------------------
//Delay Routine start here
//------------------------
void delay(int n)
{
	int i,j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<0x5000;j++)
		{;}
	}
}	 



//------------------------
//Main program starts here
//------------------------
void main(void)
{

	unsigned long val;
	PINSEL0 =	0X00000005;		//Enable RXD0 and TXD0
	PINSEL1 =	0X01000000;		//Enable ADC
	VPBDIV 	= 	0x02;			//Set the cclk to 30 Mhz
	AD0CR	=	0x00270602;		//ADC configuration bits
	AD0CR	|=	0X01000000;		//start ADC now
	IO0DIR  = 	0x0FFF7030;

	serial_init();				//serial initialization

	
	while(1)
	{
		do
		{
		    val = AD0DR;                   	// Read A/D Data Register 
		}     
		while ((val & 0x80000000) == 0);  	//Wait for the conversion to complete
		val = ((val >> 6) & 0x03FF);
		delay(1);
		printf("value : %d\n",val);
		delay(1);
	 }
}


