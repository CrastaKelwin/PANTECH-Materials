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
;||	Title		: Ex3c_StepperMotor.c														 ||;
;||	Status		: Completed!																 ||;
;||	Features	: Stepper Motor Control through Switches using ADD ON Cards!		 		 ||;
;||	Description	: Stepper Motor Port    :   P1.16 - P1.19									 ||;
;|| 			  Enable Jumper JP8 to 'INT' Mode											 ||;
;||																							 ||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
;||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||;
/***********************************************************************************************/

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	Code Begins Here	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

/***************************************   		Macros	     *************************************************/

#include <lpc21xx.h>
#include <stdio.h>

#define COIL_A 16											// Change here if you want to change the Stepper Motor Port!

/***************************************   	Declarations     *************************************************/
void motor_ccw(void);
void delay(int);

unsigned char STEP[]	=	{0x09, 0x08, 0x0C, 0x04, 0x06, 0x02, 0x03, 0x01};

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	Delay Routine	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> 

void delay(int n)
{
	int i,j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<0x3FF0;j++)
		{;}
	}
}

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	counter-clockwise rotate function	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
void motor_ccw(void)
{
	unsigned int i=0;
	while (STEP[i] != '\0')
	{
		IOSET1 = STEP[i] << COIL_A;
		delay(1);
		IOCLR1 = STEP[i] << COIL_A;
		delay(1);
		i++;
	
	}
}



/*<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<	main function Begins Here	>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>*/
void main(void)
{
	unsigned char i = 0;
	
	PINSEL2	&= 	0xFFFFFFF3;						//Configure P1.16 - P1.31 as GPIO
	IODIR1 = 0x000F0000;						//Configure P1.16 - P1.19 as Output Pins
	
	while(1)									// Loop forever..............
	{
			IOCLR0	=	0xFF << COIL_A;
			motor_ccw();						/* Rotate Stepper Motor counter clockwise */

	 }

}
