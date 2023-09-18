/***********************************************************************************************
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
;||	Title		: Keypad.c 																	 ||;
;||	Status		: Completed																	 ||;
;||	Features	: Key Press Detection and UART Display			               				 ||;
;||	Description	: KeyPad Port    :   P1.24 - P1.31     										 ||;
;||				  On-chip UART   :   P0.0 - Tx | P0.1 - Rx 									 ||;
;||																							 ||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
/***********************************************************************************************/
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	   DIRECTIVES      	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

#include <LPC214x.h>
#include <stdio.h>

#define CR		0x0D
#define D0		24
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	   Declarations    	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void Delay(void);

unsigned char Row_Data, Col_Data;
unsigned char M,N;
unsigned char Msg[4][4] = 	{ '0','1','2','3',
							  '4','5','6','7',
							  '8','9','A','B',
							  'C','D','E','F'
							};
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	Code Begins Here	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   Delay SubRoutine   	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void Delay(void)
{
	unsigned int i,j;
	for(i=0;i<350;i++)
		for(j=0;j<1234;j++);
}

void KeyScan ()
{

	Delay();
	/*^^^^^^^^^^^^^^^^^^^^^ Scanning of Rows ^^^^^^^^^^^^^^^^^^^^^^^^^*/

	IODIR1 = (0x0F << D0);		// Configuring Rows as Input && Colum as OutPut	 (P1.16 - P1.23)
	IOPIN1 = (0xF0 << D0);		// Push Column Values to LOW so as to get ROW value

	while (((IOPIN1>>D0)&0x00F0) == 0xF0);
	
	M = IOPIN1 >> D0;

	if (M == 0xE0)
	{
		Row_Data = 0; 
	}
	else if (M == 0xD0)
	{
		Row_Data = 1; 
	}
	else if (M == 0xB0)
	{
		Row_Data = 2; 
	}
	else if (M == 0x70)
	{
		Row_Data = 3; 
	}
	else
		Row_Data = 4; 

	Delay();
	Delay();
			
	/*^^^^^^^^^^^^^^^^^^^ Scanning of Column ^^^^^^^^^^^^^^^^^^^^^^^^^*/
	IOPIN1	=	0x0F << D0;
	IODIR1	= 	(0xF0 << D0);		// Configure Column as Input and Rows as OutPut	(P1.16 - P1.23)
	
	IOPIN1	= 	(0x0F << D0);		// Push LOW to Rows to get the Column value of Key Press

	while (((IOPIN1>>D0)&0x000F) == 0x0F);

	N = (IOPIN1 >> D0);

	if (N == 0x0E)
	{
		Col_Data = 0; 
	}
	else if (N == 0x0D)
	{
		Col_Data = 1; 
	}
	else if (N == 0x0B)
	{
		Col_Data = 2; 
	}
	else if (N == 0x07)
	{
		Col_Data = 3; 
	}
	else
		Col_Data = 4; 

	Delay();
	IOPIN1	=	0xF0 << D0;
	Delay();
	
}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<     Main Function    	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void main(void)
{
	
	U0LCR	=	0x83;  				//8-Bit, 1 Stop Bit, No Parity
	U0DLL	=	0x61;				//9600 baud rate @ 15MHz Clock
	U0LCR	=	0x03;

	PINSEL0	|=	0x05;				//Pin Select for configuring P0.0 and P0.1 as UART0
   	U0THR	=	0x0C;				//Clear the Hyperterminal

	while (1)
	{
		Delay();
		
		KeyScan();					/* Call KeyScan to Scan Row & Column  */

		if (Row_Data < 4 && Col_Data < 4)
		{
			U0THR 	=	Msg[Row_Data][Col_Data];
			Delay();
			U0THR	=	'\n';
			Delay();
			U0THR 	=	'\r';

		}
	}
}