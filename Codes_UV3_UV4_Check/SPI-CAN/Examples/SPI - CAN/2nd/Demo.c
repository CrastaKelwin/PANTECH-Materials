/*
 * Project name:
     DEMO (CAN Network demonstration with CAN onboard module)
     2nd Board demo
 * Description:
      The Can_1st node initiates the communication with the Can_2nd node by
      sending some data to its address. The 2nd node responds by sending back
      the data. 
 * Test configuration:
     MCU:             LPC2148
     Dev.Board:       Primer
     Oscillator:      12.0 MHz (cclk = 60.0 MHz, Fcco = 240.0 MHz)
     SW:              KEIL uVision3 v3.50
 * BOARD NOTES:
     - Turn on CAN-CS, CAN-RST & CAN-INT located on SW2 DIP switch
     - Turn on MOSI, MISO & SCK located on SW4 DIP switch
     - Connect CANs with 2-wire cable (L-L, H-H)
     - The indicator of good communication in this example is,
       that the _1st node always displays EVEN data on PORTF P1[31..24],
       whilst the _2nd node always displays ODD data on PORTF P1[31..24].
*/


#include <LPC214X.H>

#include "CAN_SPIsw.h"
#include "CAN_SPIconst.h"
#include "SPIsw.h"
#include "Utility.h"

void DelayMs(unsigned int ms);

unsigned char
	aa, aa1, aa2,
	len,
	zr,
	cont,
	oldstate,
	data[8];

unsigned long
	id;



int main (void)
{
	PINSEL0 = 0;
	PINSEL1 = 0;
	PINSEL2 &= 0x0000000C;


	//--- prepare port PORT1[23..16] as Initialization progress indicator
	IOCLR1 |= ((unsigned long)0xFF << 16);			// PORT1[23..16] = 0
	IODIR1 |= ((unsigned long)0xFF << 16);			// PORT1[23..16] output
	//--- prepare port PORT1[31..24] as Data Value indicator
	IOCLR1 |= ((unsigned long)0xFF << 24);			// PORT1[31..24] = 0
	IODIR1 |= ((unsigned long)0xFF << 24);			// PORT1[31..24] output

	//--- initialize SPI (mode 0,0)
	IODIR0 |= ((unsigned long)1 << 11);					// P0.11 output
	IODIR0 |= ((unsigned long)1 << 10);					// P0.10 output
	IOSET0 |= ((unsigned long)1 << 11);					// P0.11 = 1			// RESET line
	IOCLR0 |= ((unsigned long)1 << 10);					// P0.10 = 0			// CS line

	SPI_init(&IOPIN0, 10/*CS*/, 5/*MISO*/, 6/*MOSI*/, 4/*SCK*/, 0/*CPOL*/, 0/*CPHA*/);

	//--- configure CAN_SPI module
	aa = 0;
	aa1 = 0;
	aa2 = 0;
	aa =	CAN_CONFIG_SAMPLE_THRICE &						// form value to be used
				CAN_CONFIG_PHSEG2_PRG_ON &						// with CANSPIInitialize
				CAN_CONFIG_STD_MSG &
				CAN_CONFIG_DBL_BUFFER_ON &
				CAN_CONFIG_VALID_XTD_MSG;
	aa1 =	CAN_TX_PRIORITY_0 &										// form value to be used
				CAN_TX_XTD_FRAME &										// with CANSPIWrite
				CAN_TX_NO_RTR_FRAME;
	IOSET0 |= (1 << 10);												// set CS line for CAN module high (inactive)
	id = -1;																		// id temporarily used for SetMask operation
	CANSPIInitialize(1,1,3,3,1,aa);							// initialize external CAN module
	IOCLR1 |= (0xFF << 16);											// PORT1[23..16] = 3;
	IOSET1 |= (0x03 << 16);

	CANSPISetOperationMode(CAN_MODE_CONFIG,0xFF);		// set CONFIGURATION mode
	IOCLR1 |= (0xFF << 16);											// PORT1[23..16] = 4;
	IOSET1 |= (0x04 << 16);

	CANSPISetMask(CAN_MASK_B1,id,CAN_CONFIG_XTD_MSG);						// set all mask1 bits to ones
	CANSPISetMask(CAN_MASK_B2,id,CAN_CONFIG_XTD_MSG);						// set all mask2 bits to ones
	CANSPISetFilter(CAN_FILTER_B2_F4,12111,CAN_CONFIG_XTD_MSG);			// set id of filter B1_F1 to 12111
	CANSPISetOperationMode(CAN_MODE_NORMAL,0xFF);								// set NORMAL mode
	IOCLR1 |= (0xFF << 16);											// PORT1[23..16] = 5;
	IOSET1 |= (0x05 << 16);

	data[0] = 9;
	data[1] = 0x81;
	id = 3;
	IOCLR1 |= (0xFF << 16);											// PORT1[23..16] = 255;
	IOSET1 |= (0xFF << 16);


	while (1)
	{
		zr = CANSPIRead(&id , data , &len, &aa2);
		if ((id == 12111) && zr)									// respond only to data from node No.12111
		{
			IOCLR1 |= (0xFF << 16);									// output data at PORT1[23..16]
			IOSET1 |= ((unsigned long)data[0] << 16) & 0x00FF0000;
			data[0]++ ;

			id = 3;														// id contains CAN network ID of this node
			CANSPIWrite(id, data,8,aa1);								// send incremented data back
			DelayMs(1000);
		}
	}
}

 void DelayMs(unsigned int ms)                     //Delay at 30Mhz frquency
{
long i,j;
for (i = 0; i < ms; i++ )
for (j = 0; j < 6659; j++ );
}

