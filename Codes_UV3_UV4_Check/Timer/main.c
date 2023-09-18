/***********************************************************************************************
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	Important Note ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||;
;||~																						~||;
;||~		This part of the software coding is the sole property of the company!			~||;
;||~		Any unauthorized re-usage or transformation of this code is prohibited.			~||;
;||~																						~||;
;||~				© Pantech Solutions Pvt Ltd. | All rights Reserved						~||;
;||~																						~||;
;||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||;
;||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||	Title		: Ex4 Timer Match Code														 ||;
;||	Status		: Completed!																 ||;
;||	Features	: Timer0 Match2 Demonstration using Toggled LEDs 				 			 ||;
;||	Description	: LED      :   P1.16 - P1.23 	    										 ||;
;||				  Timer0 Configured as Match 		    									 ||;
;||				  1000 ms Delay @ 15 MHz PCLK												 ||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
/***********************************************************************************************/

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	Code Begins Here	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

#include <LPC214x.H> 

void T0isr(void)	__irq;


void main(void)
{
	IODIR1		=	0xFF << 16;					// Configure P1.16 - P1.23 as Output Pins					  	
	
	T0MR0		= 14999999;                   	// 1000mSec = 15000.000-1 counts
  	T0MCR		= 3;                          	// Interrupt and Reset on MR0
 	T0TCR 		= 1;                          	// Timer0 Enable

	VICVectAddr4 = (unsigned)T0isr;				// Set the timer ISR vector address
	VICVectCntl4 = 0x00000024;					// Set channel
	VICIntEnable |= 0x00000010;					// Enable the interrupt Timer0

	/* ------ 1000 ms Delay Used for Toggling LED ------- */
	
	while(1); 									//Stay Here...
}

void T0isr (void)	__irq
{
	IOPIN1		=	~(IOPIN1 & 0x00FF0000);		// Toggle LED's for every 1000ms
	T0IR 		|= 0x00000001;					// Clear match 0 interrupt
	VICVectAddr = 0x00000000;					// Dummy write to signal end of interrupt
}
