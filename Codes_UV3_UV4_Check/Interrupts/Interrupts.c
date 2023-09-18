/***********************************************************************************************
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
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
;||	Title		: Ex5 External Interrupt Study Code											 ||;
;||	Status		: Completed!																 ||;
;||	Features	: External Interrupts are processed and the number of press is 				 ||;
;||				  displayed in UART0...														 ||;
;||	Description	: Interrupt Pins    :   INT1 : P0.14 | INT2 : P0.15							 ||;
;||				  UART0 Pins		:   TxD  : P0.0  | RxD 	: P0.1							 ||;
;||				  UART0 Configured for 9600 bps @ 15 MHz PCLK								 ||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
/***********************************************************************************************/

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	Code Begins Here	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

#include <lpc214x.h>
#include <stdio.h>


int volatile EINT1 	=	0;
int volatile EINT2	=	0;

void ExtInt_Serve1(void)__irq;
void ExtInt_Serve2(void)__irq;

/*----------------------------------<   INT2 Initialization  >------------------------------*/
void ExtInt_Init2(void)
{
	EXTMODE  |= 4;										//Edge sensitive mode on EINT2
	EXTPOLAR = 0;										//Falling Edge Sensitive
	PINSEL0  |= 0x80000000;								//Enable EINT2 on P0.15
	VICVectCntl1 = 0x20 | 16;							//Use VIC1 for EINT2 ; 16 is index of EINT2
	VICVectAddr1 = (unsigned long) ExtInt_Serve2;		//Set Interrupt Vec Addr in VIC1
	VICIntEnable |= 1<<16;								//Enable EINT2
}

/*----------------------------------<   INT1 Initialization  >------------------------------*/
void ExtInt_Init1(void)
{
	EXTMODE  |= 2;										//Edge sensitive mode on EINT1
	EXTPOLAR = 0;										//Falling Edge Sensitive
	PINSEL0  |= 0x20000000;								//Enable EINT2 on P0.14
	VICVectCntl0 = 0x20 | 15;							//Use VIC0 for EINT2 ; 15 is index of EINT1
	VICVectAddr0 = (unsigned long) ExtInt_Serve1;		//Set Interrupt Vec Addr in VIC0
	VICIntEnable |= 1<<15;								//Enable EINT1
}

/*----------------------------------<   Serial Initialization  >------------------------------*/
void Serial_Init(void)
{
	PINSEL0 |=	0X00000005;					//Enable Txd0 and Rxd0
	U0LCR 	=	0x00000083;	  				//8-bit data, no parity, 1-stop bit 
	U0DLL	=	0x00000061;					//XTAL = 12MHz (pclk = 60 MHz, VPB = 15MHz(pclk=cclk/4)
	U0LCR	=	0x00000003;					//DLAB = 0;
}
/*------------------------------------<   Delay Function  >----------------------------------*/
 void DelayMs(unsigned int count) 	{  
						
    unsigned int i,j;		       	
    for(i=0;i<count;i++)
	{
		for(j=0;j<1000;j++);
	}
}

/*------------------------------------<   Main Function  >----------------------------------*/

void main(void)
{
	Serial_Init();
	ExtInt_Init1();							//Initialize Interrupt 1
	ExtInt_Init2();							//Initialize Interrupt 2
	
	putchar(0x0C);
	printf ("***************** External Interrupt Demo ***************************\n\n\r");
	DelayMs(100);
	printf (">>> Press Interrupt Keys SW5(INT1) and SW6(INT2) for Output... \n\n\n\r");
	DelayMs(100);
		
	while(1)
	{
		DelayMs(500);
		printf("INT1 Generated : %d  |  INT2 Generated : %d \r", EINT1, EINT2);
		DelayMs(500);	
	}
	
}

/*----------------------------------<   Interrupt Sub-Routine  >------------------------------*/

void ExtInt_Serve1(void)__irq
{
	++EINT1;
	EXTINT |= 2;
	VICVectAddr = 1;
}

void ExtInt_Serve2(void)__irq
{
	++EINT2;
	EXTINT |= 4;
	VICVectAddr = 0;
}