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
;||	Title		: Serial.c 																	 ||;
;||	Status		: Completed          														 ||;
;||	Features	: Design of Alarm Clock using RTC and Buzzer	               				 ||;
;||	Description	: On Chip RTC is used & I2C is used for Internal Communications    		     ||;
;||				  To make jp1 to enable position   											 ||;
;||	OutPut		: Visualize Output on UART1 of LPC2148 @ 9600 Baud Rate						 ||;
;||																							 ||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
/***********************************************************************************************/
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	   DIRECTIVES      	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

#include <LPC213x.h>
#include <stdio.h>


#include "UART.h"
#include "RTC.h"
extern char Key = 0;
#define BUZZ	7									//Buzzer Connected to P0.30

unsigned char Flag=0;
/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	Code Begins Here	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<   Delay SubRoutine   	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

void DelayMs(long ms)  // delay 1 ms per count @ CCLK 60 MHz
{
	long i,j;
	for (i = 0; i < ms; i++ )
	for (j = 0; j < 6659; j++ );
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
	

	PINSEL0	|=	0x50000;
	U1LCR	=	0x83;
	U1DLL	=	97; 
	U1DLM	=	0x00;
	U1LCR	=	0x03;
		
	U1IER	=	3;								//Enable UART1 Recieve Interrupt

	IO0DIR	|= 	(1<<BUZZ);							//Configure P0.30 as O/p (Buzzer)

	PREINT = 0x00000392;     // Set RTC Prescaler for PCLK 30 MHz
    PREFRAC = 0x00004380;

	U1THR	=	0x1B;
	printf("[2J\0");						// Clear Screen

	CCR		=	0x11;
	AMR	=	0xF8;


	VICVectAddr13 = (unsigned)RTC_ISR;
	VICVectCntl13 = 0x20 | 13;
	VICIntEnable |= (1<<13);

	VICVectAddr7	=	(unsigned long)UART1_ISR;
	VICVectCntl7	=	0x20 | 7;
	VICIntEnable	=	(1 << 7);

	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
	printf("  ARM LPC2138 RTC Demo\n\r--------------------\n\n\n");
	printf("> Press * to Set Time\n");
	printf("> Press ! to Set Alarm\n");
	printf("> Press $ to Snooze Alarm 5 Minutes\n");
	printf("> Press . to Stop Alarm\n");
	printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n");
	
	while(1)
	{
	//	printf("CTC : %d\t",CTC);
		printf(">>  TIME: %02d:%02d:%02d            \r",HOUR,MIN,SEC); // Display time format hh:mm:ss 
	
    	DelayMs(100);									// Delay for display

		if (Key == '*')
		{
			Key	=	0;
			printf(">>    Set Time:   ");
			RTC_Setup(&HOUR);
			printf(":");
			RTC_Setup (&MIN);
			printf (":");
			RTC_Setup (&SEC);
			
			U1THR	=	0x1B; 						//Escape
			printf("[2J\0");						// Clear Screen
									
		}

		if (Key == '!')
		{
			Key = 0;
   			printf(">>\tSet Alarm: ");
			RTC_Setup(&ALHOUR);
			printf(":");
			RTC_Setup(&ALMIN);
			printf(":");
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
			printf("\nALARM SNOOZED!\n");	
		}

		if (Key == '.' && Flag == 1)
		{
			Key		=	0;
			Flag	= 	0;
			printf("\nALARM STOPPED...!\n");	
		}

		if (Flag == 1)
		{
			Alarm();
			Wait();
			Alarm();
		}

		 DelayMs(100);  // Delay for display
		
	}
}


 void UART1_ISR(void) __irq
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

	VICVectAddr	=	7;								//Acknowledge Interrupt
}



void RTC_ISR (void) __irq
{
	if (ILR & 0x01)
	{
		ILR	=	0x01;	
	}
	if (ILR & 0x02)
	{
		Flag = 1;
		printf("\nALARM\n");
		ILR = 0x02;	
		
	}
	
	VICVectAddr	=	0;								//Ack Interrupt
}

