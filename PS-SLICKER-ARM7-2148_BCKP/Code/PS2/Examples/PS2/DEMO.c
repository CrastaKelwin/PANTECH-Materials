/*
 * Project name:
     DEMO (Simple demonstration of the PS2 Library functions)
 * Description:
     This code demonstrates communication with ps/2 keyboard.
     Scan codes that represent key are send to PC via UART-0.
 * Test configuration:
     MCU:             LPC2148
     Dev.Board:       ARM7 SLICKER
     Oscillator:      12.0 MHz (cclk = 60.0 MHz, Fcco = 240.0 MHz)
     Ext. Modules:    - PS/2 Keyboard
     SW:              KEIL uVision3 v3.34
 * NOTES:
     - Be sure to turn ON the switches for RS232-0 to connect UART lines
       to RS232-0 output (Switch SW3)
     - Start UART terminal adjusted to 38400 bps, 8 bit, no parity
*/

/********************************************************************************/
//  						Hardware settings (ADK Board)
/********************************************************************************/
//
//  PS2-(1) - P1.16 (CLK), P1.17 (DATA)  
//
//  SW32 -  DIP Switch(for LCD Control Lines)
//	=========================================
//	Turn On - DATA1, CLK1 (PS2(1)) (P1.16 & P1.17)
//	Turn On - DATA0, CLK0 (PS2(2))
//
//  connect serial cable to P1(DB9 connector) to PC's serial port
//  
//	Output in Hyperterminal Window - Set Baudrate to 38400bps 
//
/********************************************************************************/
#include <LPC213X.H>
#include <stdio.h>

#include "ps2.h"
#include "Serial.h"
#include "Utility.h"


unsigned char
	raw_data[16],
	raw_pos;

unsigned long
	n;


int main (void)
{

	PINSEL0 = 0;
	PINSEL1 = 0;
	PINSEL2 &= 0x0000000C;


	UART0_init(38400/*bps*/, 15000/*kHz*/, length_8_bit, stop_bit_1, parity_disable, parity_odd);
	U0THR = 'A';

	// Init PS/2 Keyboard
	Ps2_Init(&IOPIN0, 27/*CLK*/, 25/*DATA*/);

	// Delay 100ms, Wait for keyboard to finish
	delay_Nx10cyc(599999);
	// Set Keyboard Delay and Typematic Rate
	Ps2_Adjust_Rate(TR_Delay_750ms, TR_Rate_10);
	delay_Nx10cyc(599999);

	delay_Nx10cyc(599999);
	do
	{
		Ps2_Data_Read(&raw_pos, raw_data);

		if (raw_pos != 0)
		{
			for (n=0; n<raw_pos; n++)
			{
				UART0_WriteNum ("0x", raw_data[n]);
				UART0_sendchar (' ');
				UART0_sendstring ("\r");
			}
//			UART0_sendstring ("\r");
		}
	}	while (1);

}
