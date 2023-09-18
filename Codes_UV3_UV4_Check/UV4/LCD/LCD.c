#include <LPC214x.H>

#define RS	0x0001000
#define RW	0x0002000
#define EN	0x0004000

#define D0	0x0010000
#define D1	0x0020000
#define D2	0x0040000
#define D3	0x0080000
#define D4	0x0100000
#define D5	0x0200000
#define D6	0x0400000
#define D7	0x0800000

void lcd_cmd  (char);
void lcd_data (char);
void lcd_Init (void);
void lcd_display (void);
void lcd_Convert(char);
void delay(int);

char cmd[8] = {0x33,0x32,0x38,0x0C,0x06,0x01};				
char msg[] = {" ARM7 TYRO VER4 "};								
char msg1[]= {"  > LCD DEMO <  "};						

int  main(void)
{	
	PINSEL0=0;
	PINSEL2=0x04;
	IODIR0	=	0x000F000;	
	IODIR1	=	0x0FF0000;		
	delay(300);
	lcd_Init();							
							
	while(1)
	{
		lcd_cmd(0x01);
		lcd_display();
		delay(300);
	}
}

void delay(int n)
{
	unsigned int i,j;
	for(i=0;i<n;i++)
	{
		for(j=0;j<12000;j++)
		{;}
	}
}

void lcd_Init(void)					// LCD Initialize
{
	unsigned int i;
	for(i=0;i<6;i++)
	{	
		lcd_cmd(cmd[i]);
		delay(10); 
	}
}

void lcd_cmd(char data)						// LCD Command Mode
{
	IOCLR0	= RS | RW;		
	lcd_Convert(data);
}

void lcd_data (char data)					// LCD Data Mode
{
	IOSET0	= RS;			
	IOCLR0	= RW;	
	lcd_Convert(data);
	
}

void lcd_display (void)
{
	unsigned int i;
	
	lcd_cmd(0x80);			
	delay(10); 					// Display First Line

	for(i=0;msg[i]!='\0';i++)
	{
		lcd_data(msg[i]);			delay(40);
	}

	lcd_cmd(0xC0);			
	delay(10); 					// Display Second Line

	for(i=0;msg1[i]!='\0';i++)
	{
		lcd_data(msg1[i]);		delay(40); 
	}
}

void lcd_Convert(char c)
{
	if(c & 0x80) IOSET1 = D7; else IOCLR1 = D7;	
	if(c & 0x40) IOSET1 = D6; else IOCLR1 = D6;
	if(c & 0x20) IOSET1 = D5; else IOCLR1 = D5;
	if(c & 0x10) IOSET1 = D4; else IOCLR1 = D4;						
	if(c & 0x08) IOSET1 = D3; else IOCLR1 = D3;
	if(c & 0x04) IOSET1 = D2; else IOCLR1 = D2;
	if(c & 0x02) IOSET1 = D1; else IOCLR1 = D1;
	if(c & 0x01) IOSET1 = D0; else IOCLR1 = D0;

	IOSET0	= EN;			
	delay(5);			
	IOCLR0	= EN;		 																			
}
