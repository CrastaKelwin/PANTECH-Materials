#include <lpc214x.h>
#include <stdio.h>

#define PS2_DATA			((IO1PIN & 0x00080000)>>19)
#define PS2_CLK				((IO1PIN & 0x00040000)>>18)
#define PCLK 					30000000  		// PCLK for configuration baudrate

void UART0_Init(unsigned int baudrate);
void UART0_PutC(char c);
void UART0_PutS(char *p);

void delay(int);
unsigned char Scan_Data(void);

unsigned char Key[]={"0F FFFFF FFFFt`  as cQ1   ZSAW2  CXDE43   VFTR5  NBHGY6   MJU78  ,KIO09  ./L;P-   ' [=\\"};
unsigned char KeyVal,KeyVal2,i=0,Value;

int main()
{
		IO1DIR	  &= ~0x000C0000;				// P1.21 &P1.22 set as input direction
		IO1SET		|=  0x000C0000;				// P1.21 &P1.22 set as input direction
		VPBDIV = 0x02;						//Divide Pclk by two
		UART0_Init(9600);
		UART0_PutS("\n\r PS2 KEYBOARD 2 \n\n\r");		
	
	while(1)
	{
		IO1SET		|=  0x000C0000;
		while(PS2_CLK==1);
		
		KeyVal=Scan_Data();
		if(KeyVal>85 || KeyVal<20 || KeyVal==0x0e)
		{
				if(KeyVal==0x0e)
				Value = KeyVal;
				if(KeyVal==0x5d||KeyVal==0x5b)
				{
					if(KeyVal==0x5b)		Value = KeyVal;
					if(KeyVal==0x5d)		Value = KeyVal;
				}
		}
		else
		{
				Value = Key[KeyVal];
				UART0_PutC(Value);
		}
		delay(5);
	}
}

unsigned char Scan_Data()
{
		unsigned char Data=0,temp;
		while(PS2_CLK==0);					// waiting for start bit CLK low
		for(i=0;i<10;i++)
		{
		while(PS2_CLK);							// waiting for data bit CLK HIGH
		temp=(PS2_DATA?1:0);
		temp<<=i;
		Data|=temp;									// storing the each received bit in Data
		while(PS2_CLK==0);					// waiting for data bit CLK low
		}
		delay(5);
		return(Data);
}

void delay(int n)														// Delay function
{
	unsigned int i,j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<1500;j++)
		{;}
	}
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
