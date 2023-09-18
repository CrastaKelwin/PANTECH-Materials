/*
    DEMO (Interfacing the DS1820 temperature sensor)

  	 Description:
	 ------------
     This code demonstrates one-wire communication with temperature sensor
     DS18x20 connected to P0.26 / p0.29 pin.
     After reset, ARM reads temperature from the sensor and prints it on the LCD.
     The display format of the temperature is '#x.xxxx°C'. To obtain correct
     results, the 18x20's temperature resolution has to be adjusted (constant
     TEMP_RESOLUTION)

 * Test configuration:
     MCU:             LPC2138
     Oscillator:      12.0 MHz (cclk = 60.0 MHz, Fcco = 240.0 MHz)
*/

/********************************************************************************/
//  						Hardware settings (ADK Board)
/********************************************************************************/
//
//  CN6(LCD)    - Connector (JP13) for LCD Data Lines    -->  Port1(P0.16 to P0.23)

//
//  SW31 -  DIP Switch(for LCD Control Lines)
//	=========================================
//	Turn On - RS, RW, EN (LCD control Lines)
//
//  J5 - LCD/GLCD Select Jumper
//	===========================
//	Put Jumper to  - 'CHAR' legend selected //
//
//  JP12 - DS18s20
//	==================
//	Put Jumper to  - P0.29 legend selected 
//
/********************************************************************************/


#include <LPC213X.H>

#include "OneWire.h"
#include "LCD8.h"
#include "Utility.h"



const unsigned char LCD_Message[16] = {"ARM Digital Temp"};

unsigned char DSMAG; 
char DSSIGN; 

unsigned char TEMPnum[16];
int Temperature;
unsigned char Xpos, temp1;



/* Read temperature from DS1820 */
unsigned char DS1820_read (unsigned char *DSM, char *DSS)
{
	// Get current temperature reading
	if (ow_reset() == 0x01)
	{
		return (0x01);							// Return ERROR, part not present
	}
	ow_write (0xCC);							// DS1820 ROM Command, SKIP PROM
	ow_write (0xBE);		 					// DS1820 Function Command, READ SCRATCH PAD

	*DSM = ow_read(); 						// Get MAGNITUDE
	*DSS = ow_read();							// Get SIGN (FF = -ve, 00 = +ve)


	// Start NEW Conversion
	if (ow_reset() == 0x01)
	{
		return (0x01);							// Return ERROR, part not present
	}
	ow_write (0xCC);							// DS1820 ROM Command, SKIP PROM
	ow_write (0x44);		 					// DS1820 Function Command, START CONVERSION

	// If DSSIGN not 0 or 255 then return ERROR
	if ((*DSS != 0x00) && (*DSS != 0xFF))
		return (0x01);

	return (0x00);
}



int main (void)
{
	PINSEL0 = 0;
	PINSEL1 = 0;
	PINSEL2 &= 0x0000000C;

	LCD8_init (&IOPIN0/*CTRL Port*/, &IOPIN0/*DATA Port*/, 12/*RS*/, 13/*RW*/, 14/*E*/, 16/*D0*/);

	ow_init (&IOPIN0, 29/*DQ*/);   //DS18s20 Sensor output

	LCD8_sendstr (0,0, LCD_Message);

	// Read and display temperature every second in infinite loop...
	while (1)
	{
		Xpos = 0;

		if (DS1820_read (&DSMAG, &DSSIGN) == 0x00)
		{
			Temperature = DSMAG / 2;
			if (DSSIGN == 0xFF)
				Temperature = -Temperature;

			Xpos += int2str (TEMPnum, Temperature);
			TEMPnum[Xpos++] = '.';
			if ((DSMAG % 2) == 0)
				TEMPnum[Xpos++] = '0';
			else
				TEMPnum[Xpos++] = '5';

			TEMPnum[Xpos++] = 0xDF;
			TEMPnum[Xpos++] = 'C';

			TEMPnum[Xpos] = 0;

			LCD8_sendstr (0,1, TEMPnum);
		}
		else
		{
			LCD8_sendstr (0,1, "Error   ");
		}

		// Delay 1s
		delay_Nx10cyc (5999999);
	}
}
