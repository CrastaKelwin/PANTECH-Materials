#include "config.h"
#include "stdlib.h"

#define	TaskStkLengh	64						//Define the Task0 stack length
 
OS_STK	TaskStk0 [TaskStkLengh];		//Define the Task0 stack
OS_STK	TaskStk1 [TaskStkLengh];		//Define the Task0 stack

void 	Task0(void *pdata);			//Task0
void 	Task1(void *pdata);			//Task1

int main (void)
{
	IO1DIR=0xf0ff0000;
	IO1SET=1<<30;
	IO1CLR=0x00ff0000;

	OSInit();																										
	OSTaskCreate (Task0,(void *)0, &TaskStk0[TaskStkLengh - 1], 2);		
	OSTaskCreate (Task1,(void *)0, &TaskStk1[TaskStkLengh - 1], 3);		
	
	OSStart();
	return 0;															
}


/*******************************************************************************************************
Task0
********************************************************************************************************/

void Task0	(void *pdata)								//LED P1.16 Blinks at 1S
{
	unsigned int a=0;
	pdata = pdata;
	TargetInit ();
	while (1)
	{
		if(a%2==1) 
		IO1SET=0x00010000;
		else
		IO1CLR=0x00010000;
		a++;
		OSTimeDly(1000);
	}
}

void Task1	(void *pdata)							//LED P1.11 Blinks at 100ms
{
	unsigned int b=0;
	while (1)
	{
		if(b%2==0)
		IO1SET=0x00020000;
		else
		IO1CLR=0x00020000;
		b++;
		OSTimeDly(100);
	}
}    

/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
