/***********************************************************************************************
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	Important Note ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||;
;||~																						~||;
;||~		This part of the software coding is the sole property of the company!			~||;
;||~		Any unauthorized re-usage or transformation of this code is prohibited.			~||;
;||~																						~||;
;||~				� Pantech Solutions Pvt Ltd. | All rights Reserved						~||;
;||~																						~||;
;||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||;
;||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||																							 ||;
;||	Title		: Relay_BUZ_Toggle.c														 ||;
;||	Status		: Completed!																 ||;
;||	Features	: Port pin is toggled to trun On and OFF the Relay | Buzzer					 ||;
;||	Description	: OUTPUTS (Relay1 || Relay2 | Buzzer)										 ||;
:||				  P1.20	-	RELAY 1									 						 ||;
;||				  P1.21	-	RELAY 2									 						 ||;
;||				  Oscillator	 :	12MHz                       		         			 ||;
;||																							 ||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
/***********************************************************************************************/

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	   DIRECTIVES      	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include <lpc21xx.h>
#include <stdio.h>



#define RLY1	20	//RLY1 (P1.20)
#define RLY2	21	//RLY2 (P1.21)
#define BUZ		22	//BUZ  (P1.22)


void Delay()
{
	unsigned int i,j;
	for(i=0;i<4000;i++)
		for(j=0;j<900;j++);
}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	 Main Code Begins 	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void main(void)
{

	PINSEL2	=	0x00;

	IODIR1 = 1 << RLY1;		//Configure P1.20 Output
	IODIR1 |= 1 << RLY2;	//Configure P1.21 Output
	IODIR1 |= 1 << BUZ;		//Configure P1.22 Output


	while(1)				//loop forever
	{

			IOSET1 	= 	1 << RLY1;
			Delay();
			IOCLR1	=	1 << RLY1;
			Delay();
			IOSET1	=	1 << RLY2;
			Delay();
			IOCLR1	=	1 << RLY2;
			Delay();
			IOSET1	=	1 << BUZ;
			Delay();
			IOCLR1	=	1 << BUZ;
			Delay();
	}

}