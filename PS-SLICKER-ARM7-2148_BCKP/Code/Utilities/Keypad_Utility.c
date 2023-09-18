/*
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
^######################################################################^
^##																	 ##^
^## Important : Not to be Disclosed to the Clients..				 ##^
^## This is transferred into as an object File and...				 ##^
^## is included only for the Company's Development Purpose...		 ##^
^##												  					 ##^
^##		© Pantech Solutions | All rights Reserved.					 ##^
^##																	 ##^
^######################################################################^
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Title 		: 	KEYPAD Utility File.C
Status		: 	Hidden From the project..
Company		:	© Pantech Solutions | All rights Reserved.

Description	:	The Keypad ports are user configurable....
				The coding is established for a generic approach to any ports
				*** P0.0 and P0.1 are not advised to use since UART is used ***

-----------------------------------------------------------------------------------*/

#include <LPC214x.h>

extern void Delay();
unsigned char M,N;



/************************* Delay Sub_Routin *******************************/

void DelayMs()
{
	unsigned int i,j;
	for(i=0;i<300;i++)
		for(j=0;j<2000;j++);
}



/************************ Key Scan Funtion ********************************/

void KeyScan (unsigned long *KeysPort, unsigned char D0, unsigned char *R, unsigned char *C)
{

	Delay();
	/*^^^^^^^^^^^^^^^^^^^^^ Scanning of Rows ^^^^^^^^^^^^^^^^^^^^^^^^^*/

	*(KeysPort+2) = (0x0F << D0);		// Configuring Rows as Input && Colum as OutPut
	*(KeysPort) = (0xF0 << D0);			// Push Column Values to LOW so as to get ROW value

	while ((((*KeysPort)>>D0)&0x00F0) == 0xF0);
	
	M = (*KeysPort) >> D0;

	if (M == 0xE0)
	{
		*R = 0; 
	}
	else if (M == 0xD0)
	{
		*R = 1; 
	}
	else if (M == 0xB0)
	{
		*R = 2; 
	}
	else if (M == 0x70)
	{
		*R = 3; 
	}
	else
		*R = 4; 

	Delay();
	Delay();
			
	/*^^^^^^^^^^^^^^^^^^^ Scanning of Column ^^^^^^^^^^^^^^^^^^^^^^^^^*/
	*KeysPort	=	0x0F << D0;
	*(KeysPort+2) = (0xF0 << D0);		// Configure Column as Input and Rows as OutPut
	
	*(KeysPort) = (0x0F << D0);			// Push LOW to Rows to get the Column value of Key Press

	while ((((*KeysPort)>>D0)&0x000F) == 0x0F);

	N = ((*KeysPort) >> D0);

	if (N == 0x0E)
	{
		*C = 0; 
	}
	else if (N == 0x0D)
	{
		*C = 1; 
	}
	else if (N == 0x0B)
	{
		*C = 2; 
	}
	else if (N == 0x07)
	{
		*C = 3; 
	}
	else
		*C = 4; 

	DelayMs();
	*KeysPort	=	0xF0 << D0;
	DelayMs();
	
}
