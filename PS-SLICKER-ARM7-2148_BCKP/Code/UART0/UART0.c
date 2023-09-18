/***********************************************************************************************
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	Important Note ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||;
;||~																						~||;
;||~		This part of the software coding is the sole property of the company!			~||;
;||~		Any unauthorized re-usage or transformation of this code is prohibited.			~||;
;||~				  																		`||;
;||~		Research & Development Wing,													~||;
;||~		© 2009 Pantech Solutions Pvt Ltd.												~||;
;||~		All rights Reserved																~||;
;||~																						~||;
;||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||;
;||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||	Title		: Ex13 UART0 Demonstration													 ||;
;||	Status		: Completed!																 ||;
;||	Features	: UART0 @ 9600bps, 8-bit, No Parity											 ||;
;||	Description	: UART0 Pins  :   P0.0 - TxD        										 ||;
;||				  			  :   P0.1 - RxD	         									 ||;
;||				  Clock		  :	  12MHz x 5	= 60MHz (by default)							 ||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
/***********************************************************************************************/

#define CR     0x0D
#include <LPC21xx.H>
 
void init_serial (void);
int putchar (int ch);
int getchar (void);
unsigned char test;

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	Code Begins Here	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

int main(void)
{
	char *Ptr = "*** UART0 Demo ***\n\n\rType Characters to be echoed!!\n\n\r";

	VPBDIV = 0x02;						//Divide Pclk by two
	init_serial();

	while(1)
	{
		while (*Ptr)
		{
			putchar(*Ptr++);
		}
					
		putchar(getchar());					//Echo terminal
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<< Serial Initialization >>>>>>>>>>>>>>>>>>>>>>>>>>>>

void init_serial (void)			  /* Initialize Serial Interface       */
{               	   
  PINSEL0 	= 0x00000005;         /* Enable RxD0 and TxD0              */ 
  U0LCR 	= 0x00000083;         /* 8 bits, no Parity, 1 Stop bit     */
  U0DLL 	= 0x000000C3;         /* 9600 Baud Rate @ 30MHz VPB Clock  */
  U0LCR 	= 0x00000003;         /* DLAB = 0                          */
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<< Putchar Function  >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

int putchar (int ch) 			  /* Write character to Serial Port    */
{                  		

  if (ch == '\n')  {
    while (!(U0LSR & 0x20));
    U0THR = CR;              	   /* output CR */
  }
  while (!(U0LSR & 0x20));
  return (U0THR = ch);
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<< Getchar Function >>>>>>>>>>>>>>>>>>>>>>>>>>>>>

int getchar (void) 				/* Read character from Serial Port   */
{                    

  while (!(U0LSR & 0x01));
  return (U0RBR);

}
