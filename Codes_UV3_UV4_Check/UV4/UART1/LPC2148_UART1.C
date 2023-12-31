/***********************************************************************************************
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~	Important Note ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||;
;||~																																												~||;
;||~		This part of the software coding is the sole property of the company!								~||;
;||~		Any unauthorized re-usage or transformation of this code is prohibited.							~||;
;||~				  																																							~||;
;||~		Research & Development Wing,																												~||;
;||~		� 2015 Pantech Prolabs India Pvt Ltd.																								~||;
;||~		All rights Reserved																																	~||;
;||~																																												~||;
;||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||;
;||~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||	Title		    : UART1 Demonstration						  							 														 ||;
;||	Status		  : Completed!																																 ||;
;||	Features	  : Program to Transmit and Receive Data from LPC2148 to PC				 						 ||;
;||	Description	: UART0 Pins  :   P0.8 - TxD        										 										 ||;					 
;||				  			  					:   P0.9 - RxD	         																			 ||;
;||				  			Connect Serial Cable at CN1(USB to UART) to PC's DB9 Connector.     			 ||;
;||               output show in hyperterminal     																					 ||;
;||    						Set the SW2 UART1 TX and RX become ON																			 ||;
;||    						Set the SW2 UART0 TX and RX become OFF																		 ||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
************************************************************************************************/

#include <LPC214x.H>

#define PCLK 30000000  // PCLK for configuration baudrate

void UART1_Init(unsigned int baudrate);
void UART1_PutC(char c);
void UART1_PutS(char *p);
unsigned int getchar (void);

int main(void)
{
	VPBDIV = 0x02;						//Divide Pclk by two
	UART1_Init(9600);
	UART1_PutS("\f*** ARM7 TYRO V4.0 UART1 Demo ***\n\n\r!!Type Characters to be echoed!!\n\n\r");
	
	while(1)
	{
		UART1_PutC(getchar());					//Echo terminal
	}
}

unsigned int getchar (void) 				/* Read character from Serial Port   */
{                    
  while (!(U1LSR & 0x01));
  return (U1RBR);
}

void UART1_Init(unsigned int baudrate)
{
	  unsigned short BAUD_REG;
    BAUD_REG = PCLK/(16*baudrate);  // Calculate for U0DL value
    PINSEL0 |= (1<<16 | 1<<18);			// Enable rx,tx
	  U1LCR 	 = 0x00000083;					// 8 bit data,1 stop bit,no parity bit
    U1DLL 	 = BAUD_REG & 0xFF;     // U0DL for low byte
    U1DLM	   = (BAUD_REG>>8);       // U0DL for high byte
    U1LCR	   = 0x00000003;			    // DLAB =0
}

void UART1_PutC(char c)
{
	while(!(U1LSR & 0x20)); // Wait until UART0 ready to send character  
	U1THR = c; 							// Send character
}

void UART1_PutS(char *p)
{
	while(*p) 						// Point to character
	{
		UART1_PutC(*p++);   // Send character then point to next character
	}
}
