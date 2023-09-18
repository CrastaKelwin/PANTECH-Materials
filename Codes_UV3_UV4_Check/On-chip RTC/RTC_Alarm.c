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
;||	Title		: RTC.c 																	 ||;
;||	Status		: On progress...															 ||;
;||	Features	: Design of Alarm Clock using RTC and Buzzer	               				 ||;
;||	Description	:                                      										 ||;
;||				                               		     									 ||;
;||																							 ||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
/***********************************************************************************************/
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	   DIRECTIVES      	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

#include <LPC213x.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#include "UART.h"

#define BUZZ	7									//Buzzer Connected to P0.7

void UART1_ISR(void)__irq;
void RTC_ISR (void)__irq;

unsigned char Flag=0;
char Key;
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	Code Begins Here	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   Delay SubRoutine   	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void DelayMs(long ms)  // delay 1 ms per count @ CCLK 60 MHz
{
	long i,j;
	for (i = 0; i < ms; i++ )
	for (j = 0; j < 6659; j++ );
}

void Initialize(void)
{
	PLLCFG	=	0x24;			// M : 4  | P = 2  -> Fosc = 12MHz & CCLK = 60MHz
	PLLFEED	=	0xAA;			// Feed Process
	PLLFEED	=	0x55;

	PLLCON	=	0x01;
	PLLFEED	=	0xAA;			// Feed Process
	PLLFEED	=	0x55;

	while (!(PLLSTAT & 0x400));	//Wait untill PLL is Locked!

	PLLCON	=	0x03;			//Connect PLL as the Clock Source for Microcontroller
	PLLFEED	=	0xAA;			// Feed Process
	PLLFEED	=	0x55;

	MAMCR	=	0x02;			//Memory Accerleration Module Fully Enabled
	MAMTIM	=	0x04;			//MAM fetch cycles are 4 CCLKs in duration

	VPBDIV	=	0x02;			//Divide Clock for PCLK = 30MHz

}


void RTC_Setup(char *Buff)
{

	unsigned char TimE;
	char i=0;

	for(i=0;i<2;i++)
	{
		while(!isdigit(Key));					//Wait till Key = 0 to 9
		if (i==0)
		{
			TimE	=	10 * atoi( &Key );
		}
		if (i==1)
		{
			TimE	+=	atoi( &Key );
		}
		putchar(Key);
		Key 	=	0;

	}

	*Buff	=	TimE;							//Load Setup New Value
}

void Delay()
{
	unsigned int i,j;
	for(i=0;i<50;i++)
		for(j=0;j<700;j++);
}

void Wait()
{
	Delay();Delay();Delay();
	Delay();Delay();Delay();
	Delay();Delay(); Delay();
}

void Alarm(void)
{
	IOSET0 = 1 << BUZZ;
	Wait();Wait();
	IOCLR0 = 1 << BUZZ;
	Wait();
}

//void Clean(void)
//{
//	unsigned char i;
//	//for(i=0;i<250;i++)
//		printf("[2M");
//}
void main(void)
{
	
	Initialize();
	
	UART1_Init(9600/*Baud Rate*/);
		
	U1IER	=	3;								//Enable UART1 Recieve Interrupt
	//PINSEL0	|=	(1 << 18);						//Select Pin as UART1

	IO0DIR	|= 	(1<<7);							//Configure P0.7 as O/p (Buzzer)

	VICVectAddr0	=	(unsigned)UART1_ISR;
	VICVectCntl0	=	0x20 | 7;
	VICIntEnable	|=	(1 << 7);

	VICVectAddr2	=	(unsigned)RTC_ISR;
	VICVectCntl2	=	0x20 | 13;
	VICIntEnable	|=	(1 << 13);

	AMR		=	0xFF;							//Mask all valued except hh:mm:ss for alarm comparision	
	PREINT = 0x00000392;     // Set RTC Prescaler for PCLK 30 MHz
    PREFRAC = 0x00004380;
//	printf("[2J\0");						// Clear Screen

	CCR		=	0x01;
	//CIIR	=	0x01;

	UART1_PutS("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	UART1_PutS("  ARM LPC2138 RTC Demo\n\r--------------------\n\n\n");
	UART1_PutS("> Press * to Set Time\n");
	UART1_PutS("> Press ! to Set Alarm\n");
	UART1_PutS("> Press $ to Snooze Alarm 5 Minutes\n");
	UART1_PutS("> Press . to Stop Alarm\n");
	UART1_PutS("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
	
	while(1)
	{
		printf("CTC : %d\t",CTC);
		printf(">>  TIME: %02d:%02d:%02d            \r",HOUR,MIN,SEC); // Display time format hh:mm:ss 
	
    	DelayMs(100);									// Delay for display

		if (Key == '*')
		{
			Key	=	0;
			UART1_PutS(">>    Set Time:   ");
			RTC_Setup(&HOUR);
			UART1_PutC(':');
			RTC_Setup (&MIN);
			UART1_PutC (':');
			RTC_Setup (&SEC);
			//printf("\r\tTIME: %02d:%02d:%02d       \r",HOUR,MIN,SEC); 	// Display time format hh:mm:ss 
			//printf("^[2J");
			

			U1THR	=	0x1B; 						//Escape
			UART1_PutS("[2J\0");						// Clear Screen
									
		}

		if (Key == '!')
		{
			AMR	=	0xF8;
			Key = 0;
   			UART1_PutS(">>\tSet Alarm: ");
			RTC_Setup(&ALHOUR);
			UART1_PutC(':');
			RTC_Setup(&ALMIN);
			UART1_PutC(':');
			RTC_Setup(&ALSEC);

		}

		if (Key == '$' && Flag == 1)
		{
			if (MIN >= 55)
			{
				ALHOUR	=	HOUR + 1;
				ALMIN	=	5	-	(60 - MIN);
			}
			else
			{
				ALMIN	=	MIN + 5;
			}
			Key 	= 	0;
			Flag	=	0;		
		}

		if (Key == '.')
		{
			Key		=	0;
			Flag	= 	0;
		}

		if (Flag == 1)
		{
			Alarm();
			Wait();
			Alarm();
		}

	}
}



void UART1_ISR(void)__irq
{
	char Msg;

	if(((Msg = U1IIR) & 0x01) == 0)					//Check Flag Status of Recieve Interrupt
	{
		switch	(Msg & 0x0E)						//Filter Msg
		{
			case 0x04:	while (!(U1LSR & 0x20));	//Recieve Data 
					   	Key = U1RBR;

			case 0x02:	break;						//THRE Interrupt

			default	 :	break;
		}
	}

	VICVectAddr	=	0;								//Acknowledge Interrupt
}



void RTC_ISR (void)__irq
{
	if ((ILR & 0x02) == 0x02)
	{
		Flag = 1;
		UART1_PutS("\nALARM\n");
		ILR = 0x02;									//Clear Current Interrupt
	}
	
	VICVectAddr	=	0;								//Ack Interrupt
}