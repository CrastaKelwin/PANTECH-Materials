#include<lpc214x.h>


unsigned char x=0, n=1;
unsigned char thou=0,hun=0,ten=0,single=0;
unsigned char a[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

void hex2dec(int);
void send_seg(unsigned char,unsigned char,unsigned char,unsigned char);
void DelayMs(unsigned int);

int main(void)
{
	VPBDIV=2;
	PINSEL0=0;
	PINSEL1=0;
	PINSEL2=0x4;
	IODIR0 = 0xFF00;
	IODIR1 = 0xFF0000;
	while(1)
	{
		if(x == 100)   
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
		send_seg(thou,hun,ten,single);		
	}
}

void send_seg(unsigned char thou,unsigned char hun,unsigned char ten,unsigned char single)
{
	if(n==1)
	{
		IOPIN0=0x2000;
		IOPIN1=(a[single])<<16;
		n=2;
		DelayMs(5);
	}
	else if(n==2)
		 {
			IOPIN0=0x1000;
			IOPIN1=(a[ten])<<16;
			n=3;
			DelayMs(5);
		 }
		 else if(n==3)
			{
		 		IOPIN0=0x0800;
				IOPIN1=(a[hun])<<16;
				n=0;
				DelayMs(5);
			}
		else
		{
			IOPIN0=0x0400;
			IOPIN1=(a[thou])<<16;
			n=1;
			DelayMs(5);
		}
}


void DelayMs(unsigned int ms)                     //Delay at 30Mhz frquency
{
	long i,j;
	for (i = 0; i < ms; i++ )
	for (j = 0; j < 6659; j++ );
}

