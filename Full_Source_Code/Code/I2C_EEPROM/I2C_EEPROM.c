/***********************************************************************************************
;|||||||||||||||||||||||||||||||||||||||||||||||;
;|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	Important Note ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|;
;|~																							~|;
;|~		This part of the software coding is the sole property of the company!				~|;
;|~		Any unauthorized re-usage or transformation of this code is prohibited.				~|;
;|~																							~|;
;|~				© Pantech Solutions Pvt Ltd.  All rights Reserved							~|;
;|~																							~|;
;|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|;
;|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|;
;|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;|	Title		: I2C_EEPROM_Interface.c													 |;
;|	Status		: Completed...																 |;
;|	Features	: LPC21xx I2C							 									 |;
;|	Description	: I2C Used    	:  	P0.2 and P0.3 (I2C0 of ARM7TDMI)						 |;
;|				  Oscillator	:	12MHz                       		         			 |;
;|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
***********************************************************************************************/

/************************************   Directives   *******************************************/

#include <LPC214x.h>
#include <stdio.h>
#include <string.h>
#include "UART.h"

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/************************************    Macros     *******************************************/
#define SW23		1<<24
#define SW24		1<<25
#define SW25		1<<26

#define MAX		10
#define	AA		2
#define	SI		3
#define	STO		4
#define	STA		5
#define	I2EN	6
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   Declarations  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void I2C_ISR(void)__irq;

void Wait (unsigned int);
void I2C_Init (void);

int I2C_Start (unsigned int Slave_Addr);


char ReLoad[MAX]	=	{0x00/*Address Low Bits*/,0x00/*Address Low Bits*/,'A','R','M','7','2','1','x','x'};
char Buff[MAX]		=	{0x00/*Address Low Bits*/,0x00/*Address Low Bits*/,'A','R','M','7','2','1','x','x'};

unsigned char Rec[MAX]	=	{"NO-DATA!"};
unsigned char index		=	0;
unsigned char flag = 0, ii, Ready=0;
unsigned char Erase = 0;

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void Delay(void)
{
	unsigned int i,j;
	
	for(i=0;i<150;i++)
		for(j=0;j<900;j++);
}

void Wait (unsigned int Delay)
{
	while(Delay--);
}

void I2C_Init (void)
{
	PINSEL0	=	0x00000055;			// P0.3 - SDA0 and P0.2	- SCL0 
	I2C0SCLH	=	150;			// 50%duty cycle .....   
	I2C0SCLL	=	150;			// I2C Frequency ->100 KHz for PCLK = 30MHz 
	I2C0CONSET	=	1 << I2EN;		// Enable I2C 0 


}

int I2C_Start (unsigned int Slave_Addr)
{
	I2C0CONCLR	=	1 << STO;
	I2C0CONSET	=	1 << AA;

	I2C0CONSET	=	1 << STA;

	return 0;
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   Main Function  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


int main()
{
	unsigned int i;
	VPBDIV	=	0x02;
	PINSEL0	=	0x00000055;			// P0.3 - SDA0 and P0.2	- SCL0 
	PINSEL2	=	0xFFFFFFF3;

	IO1DIR	=	0x00 << SW23;

	UART0_Init (9600);

	VICIntSelect	= 0<<9;
	VICVectCntl0 	= 0x020 | 9 ;
	VICVectAddr0 	= (unsigned long)I2C_ISR; 	//Select Vector Slot for I2C0
	VICIntEnable 	= 1<<9;

	
	/*	Before the master transmitter mode
		can be entered, the I2CONSET register must be initialized
	*/


	I2C_Init();
    //UART0_PutS ("[2J\0");										// Clear Screen

	UART0_PutS ("********* ARM Slicker LPc2148 I2C EEPROM Demo **********\n\n\r");
	UART0_PutS (">>> Connect I2C Add-On Card to J11 on ARM Tyro Board .. \n\n\r");
	UART0_PutS ("[~] Turn SW23 ON to Write default data to EEPROM! \n\r");
	UART0_PutS ("[~] Turn SW24 ON to Read and Display data from EEPROM! \n\r");
	UART0_PutS ("[~] Turn SW25 ON to Erase data from EEPROM \n\r");
	

	while (1)
	{
		if ((IOPIN1 & SW23) == 0)					/*...To Load the Default Data to the EEPROM ...*/
		{
			ii = 0;
			while (ii < MAX)  						//Re-load Default Data to Buff
			{
				Buff [ii]	=	ReLoad [ii];
				ii++;
			}
			Wait (5000);			
			flag	=	'W';
			I2C_Start (0x70);
			Erase = 0;
			for (i=0;i<30;i++)	Wait(10000);
	
			I2C0CONCLR	=	1 << SI;
			while (!(IOPIN1 & SW23));
			Wait (5000);Wait (5000);
		}	
		else if ((IOPIN1 & SW24) == 0)				/*..To Read the Data Stored in the EEPROM...*/
		{
			flag 	=	'R';
			I2C_Start (0x70);
	
			for (i=0;i<30;i++)	Wait(10000);
	
			I2C0CONCLR	=	1 << SI;

			while (Ready == 'F');							//Ready = FALSE

			if (Ready == 'T')						//Check if Ready = TRUE
			{
				ii=0;
				UART0_PutS ("\n\r The Read Data are: \t");
				while (ii<MAX)
				{
					//U0THR	=	'\n';
					Wait (1000);
					U0THR	=	Rec[ii];
					Wait (1000);
					ii++;
				}
				UART0_PutC ('\n');
				Wait (1000);
				Ready 	=	'F';
			}
			while (!(IOPIN1 & SW24));
			Wait (5000);Wait (5000);Wait (5000);Wait (5000);
		}

		if ((IOPIN1 & SW25) == 0) 						/*To Erase the Content in EEPROM*/
		{
			ii = 2;
			while (ii < MAX)
			{
				Buff[ii]	=	0xFF; 					//Load 0xFF to EEPROM
				ii++;
			}
			flag	=	'W';
			Erase = 1;
			I2C_Start (0x70);
	
			for (i=0;i<30;i++)	Wait(10000);
	
			I2C0CONCLR	=	1 << SI;	

			while (!(IOPIN1 & SW25));
			Wait (5000);Wait (5000);Wait (5000);Wait (5000);
		}
	}		 

}

void I2C_ISR(void) __irq
{
	if (I2C0CONSET & 0x08)
	{
		

		switch (I2C0STAT)
		{
			case (0x08)	:/*... A START condition has been Transmitted...*/	

							if (flag == 'W')
							{
								I2C0CONCLR	=	1 << STO;
								I2C0CONCLR	=	1 << STA;	//Clear START Bit
								I2C0DAT		=	0xA0;		//Slave Addr + W  1010 p2 p1 p0 w
								I2C0CONCLR	=	1 << SI;
								
							}
							else if (flag == 'R')
							{
								//I2C0CONCLR	=	1 << STO;
								I2C0DAT		=	0xA0;		//Slave Addr + R  1010 p2 p1 p0 r	
								I2C_Start (0xA1);
								I2C0CONCLR	=	1 << SI;
								
							}
							index	=	0;
							break;
	
			case (0x10)	:	/*...A repeated START condition has been Transmitted...*/

							I2C0CONCLR	=	1 << STA;	
							if (flag == 'W')
								I2C0DAT	=	0xA0;		//Slave Addr + W  1010 p2 p1 p0 w
							else if (flag == 'R')
							{
								I2C0CONCLR	=	1 << STA;
								I2C0CONCLR	=	1 << STO;	
								I2C0DAT		=	0xA1;		//Slave Addr + R  1010 p2 p1 p0 r	
								I2C0CONCLR	=	1 << SI;
								index = 0;
							}
							
							break;
	
			case (0x18)	:	/*....SLA+W has been transmitted; ACK has been received....*/

							I2C0CONCLR	=	0x20;		//Clear START Bit
							I2C0CONCLR	=	0x38;		//Clear all flags (STA, STO, SI)
							I2C0DAT		=	Buff[index];
							index++;
							
							break;
	
			case (0x20)	:	/*...SLA+W has been transmitted; NOT ACK has been received....*/

							I2C0CONCLR	=	0x20;		//Clear START Bit
							I2C0CONCLR	=	0x38;		//Clear all flags (STA, STO, SI)
							I2C0DAT		=	Buff[index];
							index++;						
							break;
	
			case (0x28)	:	/*...Data byte in I2DAT has been transmitted; ACK has been received....*/

							if (index < MAX)   				// Place all the contents of Buff[] in I2C0DAT
							{
								if (flag == 'W')
								{
									I2C0DAT		=	Buff[index];
									I2C0CONCLR	=	0x20;		//Clear START Bit
									I2C0CONCLR	=	0x38;		//Clear all flags (STA, STO, SI)
								
								}
								else if ((index > 1) && flag == 'R')	//Generate a Repeated Start to jump to MST/REC Mode
								{
									I2C0CONCLR	=	0x18;
									I2C0CONSET	=	1 << STA;
									
								}
								else							// Send Upper Nibble of Adress to Read the Data... 
								{
									I2C0DAT		=	Buff[index];
									I2C0CONCLR	=	0x20;		//Clear START Bit
									I2C0CONCLR	=	0x38;		//Clear all flags (STA, STO, SI)
									
								}
										
								index++;
							}
							else								//Stop Transmission to EEPROM.... Data Count Reset..
							{
								index = 0;
								flag = 'R';
								I2C0CONSET	=	1 << STO;		//Send STOP Bit
								if (Erase == 1)
								{
									UART0_PutS ("\n\r Memory Erase Successfull.....!\n");
								}
								else
								{
									UART0_PutS ("\n\r Data Successfully Written on Memory!\n");
								}
								
								I2C0CONCLR	=	1 << STA;
								I2C0CONCLR	=	1 << SI;
							}
							break;
	
			case (0x30)	:	/*... Data byte in I2DAT has been transmitted; NOT ACK has been received ...*/

							I2C0CONCLR	=	0x20;		//Clear START Bit
							if (index < MAX)
							{
								if (flag == 'W')
								{
									I2C0DAT		=	Buff[index];
								
								}
															
								index++;
							}
							else
							{
								index = 0;
								flag = 0;
								I2C0CONSET	=	0x10;		//Send STOP Bit
								I2C0CONCLR	=	1 << SI;
								
							}
							break;
	
			case (0x38)	:	/*... Arbitration lost in SLA+R/W or Data bytes ....*/

							I2C0CONSET	=	0x20;
							break;

			case (0x40)	:	/*... SLA+R has been transmitted; ACK has been received.*/

							I2C0CONSET	=	1 << AA;
							I2C0CONCLR	=	1 << STA;
							I2C0CONCLR	=	1 << STO;
							I2C0CONCLR	=	1 << SI;
							break;

			case (0x48)	:	/*...SLA+R has been transmitted; NOT ACK has been received ...*/

							I2C0CONSET	=	1 << STA;		//Repeated Start
							break;

			case (0x50)	:	/*... Data byte has been received; ACK has been returned ....*/

							I2C0CONSET	=	1 << AA;	
							if (index < MAX)
							{
								Rec [index]	=	I2C0DAT;
								index++;
							
							}
							else
							{
								I2C0CONSET	=	1 << STO;
								I2C0CONCLR	=	1 << SI;
								index = 0;
								Ready	=	'T';			//Make Ready TRUE after Reading is over!
							}
							break;

			case (0x58)	:	/*... Data byte has been received; NOT ACK has been returned ....*/
			
							Rec [index]	=	I2C0DAT;
							I2C0CONCLR	=	1 << STA;
							I2C0CONCLR	=	1 << STO;
							break;
							
		}
	}
	I2C0CONCLR	=	1 << SI;
	VICVectAddr	=	0x00;
}


