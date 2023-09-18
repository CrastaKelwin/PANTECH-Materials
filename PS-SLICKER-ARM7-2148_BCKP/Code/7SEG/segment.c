/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
^######################################################################^
^##																	 ##^
^##    This Software Coding is the sole property of the company!	 ##^
^##    Any unauthorized re-usage of this code for commercial		 ##^
^##	   purpose is forbidden!...										 ##^
^##    The removal of the object files may cause the code to crack!	 ##^
^##												  					 ##^
^##			© Pantech Solutions | All rights Reserved.				 ##^
^##																	 ##^
^######################################################################^
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Title 		: 	Seven Segment Demo Program
Status		: 	Open to all...
Features	: 	Displays as a counter from 0 to 9999

Company		:	© Pantech Solutions | All rights Reserved.

Description	:	The Seven Segment Data port are user configurable....
				The coding is established for a generic approach to any 
				ports.
				
				It is advised to configure the PINSEL and IODIR corresponding to 
				the OutPut ports........

Important	:   P0.10 to P0.13 are Hardware Configured Control Pins..
				It is strictly advised not to change the Control Pins..
--------------------------------------------------------------------------*/

#include <stdio.h>
#include <LPC213x.H>

#include "7SEG.H"

unsigned int thou,hun,ten,single;
unsigned int x;

void DelayMs(unsigned int count) 	
{  
						
									
    unsigned int i,j;		       	
    for(i=0;i<count;i++)
	{
		for(j=0;j<3000;j++);
	}
}
int main(void)
{

	PINSEL0 = 0;
	PINSEL1 = 0;
	PINSEL2 &= 0x0000000C;
	IODIR0 |= 0x0F << 10 ;					// P0.10 - P0.13 in-built Control Lines
   	IODIR1 |= 0xFF << 16;					//P1.16 - P1.23 are configured as Outputs
	

	while(1)
	{
		if(x == 300)   
		{
			x=0;
			single++;
			if(single>9)
			{
				single=0;
				ten++;	
				if(ten>9)
				{
					ten=0;
					hun++;	
					if(hun>9)
					{
						hun=0;
						thou++;	
						if(thou>9)
							thou=0;
					}
				}
			}
		}
		x++;
/*****************Function Call to the Utility File************************/

		Segment_Disp(&IOPIN1/*Segment Data Port*/, 16/*Data LSB D0*/, 	  //(Port1.16 - P1.23) Data lines
					 thou, hun, ten, single);		
	
	}
}
