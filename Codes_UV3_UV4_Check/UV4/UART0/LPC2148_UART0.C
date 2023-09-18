/***********************************************************************************************
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	Important Note ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||;
;||~																																												~||;
;||~		This part of the software coding is the sole property of the company!								~||;
;||~		Any unauthorized re-usage or transformation of this code is prohibited.							~||;
;||~				  																																							~||;
;||~		Research & Development Wing,																												~||;
;||~		© 2015 Pantech Prolabs India Pvt Ltd.																								~||;
;||~		All rights Reserved																																	~||;
;||~																																												~||;
;||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||;
;||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||	Title		    : UART0 Demonstration						  							 														 ||;
;||	Status		  : Completed!																																 ||;
;||	Features	  : Program to Transmit and Receive Data from LPC2148 to PC				 						 ||;
;||	Description	: UART0 Pins  :   P0.0 - TxD        										 										 ||;					 
;||				  			  					:   P0.1 - RxD	         																			 ||;
;||				  			Connect Serial Cable at CN1(USB to UART) to PC's DB9 Connector.     			 ||;
;||               output show in hyperterminal     																					 ||;
;||    						Set the SW2 UART0 TX and RX become ON																			 ||;
;||    						Set the SW2 UART1 TX and RX become OFF																		 ||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
************************************************************************************************/

#include <LPC214x.H>

#define PCLK 30000000  // PCLK for configuration baudrate

void UART0_Init(unsigned int baudrate);
void UART0_PutC(char c);
void UART0_PutS(char *p);
unsigned int getchar (void);

int main(void)
{
	VPBDIV = 0x02;						//Divide Pclk by two
	UART0_Init(9600);
	UART0_PutS("\f*** ARM7 TYRO V4.0 UART0 Demo ***\n\n\r!!Type Characters to be echoed!!\n\n\r");
	
	while(1)
	{
		UART0_PutC(getchar());					//Echo terminal
	}
}

unsigned int getchar (void) 				/* Read character from Serial Port   */
{                    
  while (!(U0LSR & 0x01));
  return (U0RBR);
}

void UART0_Init(unsigned int baudrate)
{
	  unsigned short BAUD_REG;
    BAUD_REG = PCLK/(16*baudrate);  // Calculate for U0DL value
    PINSEL0 |= 0x00000005;					// Enable rx,tx
	  U0LCR 	 = 0x00000083;					// 8 bit data,1 stop bit,no parity bit
    U0DLL 	 = BAUD_REG & 0xFF;     // U0DL for low byte
    U0DLM	   = (BAUD_REG>>8);       // U0DL for high byte
    U0LCR	   = 0x00000003;			    // DLAB =0
}

void UART0_PutC(char c)
{
	while(!(U0LSR & 0x20)); // Wait until UART0 ready to send character  
	U0THR = c; 							// Send character
}

void UART0_PutS(char *p)
{
	while(*p) 						// Point to character
	{
		UART0_PutC(*p++);   // Send character then point to next character
	}
}
