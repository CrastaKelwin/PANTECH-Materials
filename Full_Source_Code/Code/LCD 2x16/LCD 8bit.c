/********************************************************************
#######################################################################
##																	 ##
## Important : Not to be Disclosed to the Clients..					 ##
## This is transferred into as an object File and...				 ##
## is included only for the Company's Development Purpose...		 ##
##												  					 ##
##		© Pantech Solutions | All rights Reserved.					 ##
##																	 ##
#######################################################################
	2X16 char LCD Demo Program in 8-bit Mode
 	----------------------------------------
  	
	Description   | Display Simple text message.

	Test configuration:
	-------------------
	Oscillator:      12.0 MHz (cclk = 60.0 MHz, Fcco = 240.0 MHz)

	Connections
	-----------
	Data Lines(P0.0--0.7)			
	P0.12 	-	RS	|	P0.13	-	RW	|	P0.14	-	EN
/********************************************************************/

#include <LPC213x.H>
#include "LCD8.H"

extern unsigned char msg[16] = {" LPC21XX ARM ADB"};	//msg
extern unsigned char msg1[16]= {"    LCD Demo!   "};	//msg1

void DelayMs()
{
	unsigned int i,j;
	for(i=0;i<100;i++)
		for(j=0;j<2000;j++);
}

void main()
{
	LCD_Config(&IOPIN0,0);
	while(1)
	{
		;	
	}
}

