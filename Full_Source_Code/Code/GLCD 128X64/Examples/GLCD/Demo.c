/* * Project name:
     DEMO (Demonstration of the GLCD library routines)
 * Copyright
     (c)Pantech Solutions, 2007.
 * Description:
      This is a simple demonstration of the GLCD library routines:
     - Init and Clear (pattern fill)
     - Image display
 * Test configuration:
     MCU:             LPC2148
     Ext. Modules:    GLCD 128x64 dots

 * GLCD CONNECTIONS

     - CN6 - T0 - JP8 (P1.16 to P1.23 datalines)

 * BOARD NOTES:
     - Place GLCD in position.
     - Move jumper J5 in down position for GLCD Contrast control.
     - Adjust GLCD contrast with r18.
*/


#include <LPC213X.H>

#include "Glcd.h"
#include "bmp_maska.h"
#include "Utility.h"


unsigned char
	ii;

char
	*someText;




int main (void)
{
	PINSEL0 = 0;
	PINSEL1 = 0;
	PINSEL2 &= 0x0000000C;


	Glcd_Init(&IOPIN0,	// Control lines Port
						&IOPIN1,	// Data Lines Port
						10,				// CS1#
						11,				// CS2#
						12,				// RS
						13,				// R/W#
						15,				// RST
						14,				// EN
						16);			// D0 data line position	 


	while (1)
	{
		Glcd_Fill(0x00);

		Glcd_Image( maska_bmp );
		delay_Nx10cyc (11999999);				// Delay 2s

		Glcd_Fill(0x00);

		Glcd_Line(120,1, 5,60, 1);
		Glcd_Line(12,42, 5,60, 1);
		delay_Nx10cyc (5999999);				// Delay 1s

		Glcd_Rectangle(12,20, 93,57, 1);
		delay_Nx10cyc (5999999);				// Delay 1s

		Glcd_Line(120,12, 12,60, 1);
		delay_Nx10cyc (5999999);				// Delay 1s

		Glcd_H_Line(5,15, 6, 1);
		Glcd_Line(0,12, 120,60, 1);
		Glcd_V_Line(7,63, 127, 1);
		delay_Nx10cyc (5999999);				// Delay 1s

		for (ii = 1; ii <= 10; ii++)
			Glcd_Circle(63,32, 3*ii, 1);
		delay_Nx10cyc (5999999);				// Delay 1s

		Glcd_Box(12,20, 70,57, 2);
		delay_Nx10cyc (5999999);				// Delay 1s

		someText = "BIG:ONE";
		Glcd_Write_Text(someText, 5,3, 2);
		delay_Nx10cyc (5999999);				// Delay 1s

		someText = "SMALL:NOT:SMALLER";
		Glcd_Write_Text(someText, 20,5, 1);
		delay_Nx10cyc (5999999);				// Delay 1s	 */
	}
}


