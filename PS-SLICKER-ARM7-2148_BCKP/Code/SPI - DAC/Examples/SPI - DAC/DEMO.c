/*
 * Project name:
     DEMO (Driving on-board MCP4921 Digital-to-Analog converter)
 * Description:
     This is a sample program which demonstrates the use of the Microchip's
     MCP4921 12-bit D/A converter with ARM mcu's. This device accepts digital
     input (number from 0..4095) and transforms it to the output voltage,
     ranging from 0..Vref. In this example the D/A communicates with ARM 
     through the software SPI. The reference voltage is adjustable between
     Vcc and 4,096V. On reset the DAC is being set to the mid-range.
 * Test configuration:
     MCU:             LPC2148
     Oscillator:      12.0 MHz (cclk = 60.0 MHz, Fcco = 240.0 MHz)
     SW:              KEIL uVision3 v3.50
 * MCU CARD NOTES:
 * BOARD NOTES:
     - Turn on MOSI, MISO & SCK located on SW33 DIP switch
     - Turn on DAC-CS located on SW34 DIP switch
*/


#include <LPC214X.H>

#include "SPIsw.h"
#include "Utility.h"



unsigned long
	DACval,
	DACreg;



int main (void)
{

	PINSEL0 = 0;
	PINSEL1 = 0;
	PINSEL2 &= 0x0000000C;

	SPI_init (&IOPIN0, 29/*CS*/, 5/*MISO*/, 6/*MOSI*/, 4/*SCK*/, 0/*CPOL*/, 0/*CPHA*/);

	// Set output voltage
	DACval = 2047;										// Range [0..4095]
	DACreg = DACval | 0x7000;							// A#/B=0, BUF=1, GA#=1, SHDN#=1

	SPI_enable ();
	SPI_char ((DACreg >> 8) & 0x00FF);		// Write DAC registers
	SPI_char (DACreg & 0x00FF);
	SPI_disable ();

	// Endless loop
	while (1);
}
