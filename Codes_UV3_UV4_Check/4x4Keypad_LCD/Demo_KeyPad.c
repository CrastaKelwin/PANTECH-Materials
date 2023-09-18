/*
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
^######################################################################^
^##																	 ##^
^## This Software Coding is the sole property of the company!		 ##^
^## Any unauthorized re-usage of this code is prohibitted..			 ##^
^## The removal of the object files may cause the code to crack!	 ##^
^##												  					 ##^
^##		© Pantech Solutions | All rights Reserved.					 ##^
^##																	 ##^
^######################################################################^
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Title 		: 	KEYPAD Display Program.C
Status		: 	Open to all...
Features	: 	Displays the Key Press in both LCD and UART

Company		:	© Pantech Solutions | All rights Reserved.

Description	:	The Keypad ports are user configurable....
				The coding is established for a generic approach to any ports
				*** P0.0 and P0.1 are not advised to use since UART is used ***

				It is also adviced to use distinct port for both LCD and KeyPad
-----------------------------------------------------------------------------------*/

#include <LPC213X.H>

#include "ADBKEYPAD.H"
#include "LCD8.H"

extern void Delay();

unsigned char Row_Data, Col_Data;
unsigned char Pos = 0x80;
unsigned char Msg[4][4] = 	{ '0','1','2','3',
							  '4','5','6','7',
							  '8','9','A','B',
							  'C','D','E','F'
							};
/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ Delay Sub_Routine ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

void Delay(void)
{
	unsigned int i,j;
	for(i=0;i<35;i++)
		for(j=0;j<1234;j++);
}

/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^ Main Function ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^*/

void main()
{

	U0LCR = 0x83;			//Serial Port Configuration to 9600 bps @ 12 MHz
	U0DLL = 0x61;
	U0LCR = 0x03;

	PINSEL0 |= 0x05;		//Pin Select for UART 0 and remaining GPIO
	PINSEL1 = 0;
	
	/* ^^^^^^^^^^^^^^^ LCD Config Function ^^^^^^^^^^^^^^^^^^^^*/

   	LCD_Config (&IOPIN0 /*LCD Data Port*/, 16 /* Data LSB D0*/);

	while(1)				//Loop From Here....
	{
		Delay();
		Delay();

		KeyScan(&IOPIN1/*KeyPad Port*/,16/*Starting Data Pin D0*/,
			    &Row_Data/*Addr of Row*/,&Col_Data/*Addr of Col*/);

		

		if (Row_Data < 4 && Col_Data < 4)
		{
			U0THR = Msg[Row_Data][Col_Data];
			Delay();
			Delay();
			U0THR = '\n';
			if (Pos == 0x80)
			{
				lcd_cmd (&IOPIN0 /*LCD Data Port*/,16 /*Data LSB D0*/, 
						 0x01 /*LCD Command*/);
			}
			if (Pos == 0x90)
			{
				Pos = 0xC0;
			}
			if (Pos > 0xCF)
			{
				Pos = 0x80;
			}
			lcd_cmd (&IOPIN0/*LCD Data Port*/,16/*Data LSB D0*/,
					 Pos++/*Cursor Position*/);
			Delay();
			lcd_data (&IOPIN0/*LCD Data Port*/,16/*Data LSB D0*/,
					  Msg[Row_Data][Col_Data]/*Data to be Displayed in LCD*/);
		}
	}
}
