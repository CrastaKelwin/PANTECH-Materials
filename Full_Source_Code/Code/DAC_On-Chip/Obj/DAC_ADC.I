
#line 1 "DAC_ADC.c" 

















 
  
#line 1 "C:\KEIL\ARM\INC\PHILIPS\LPC213X.H" 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 19 "DAC_ADC.c" 
 
  
#line 1 "C:\KEIL\ARM\INC\STDIO.H" 






 
 
 
 
 
 
#line 13 "C:\KEIL\ARM\INC\STDIO.H" 
 
 
#line 15 "C:\KEIL\ARM\INC\STDIO.H" 
 
 
 
 
 
 
 
 
 
 
 
 typedef unsigned int size_t;
 
 
 extern int  _getkey  (void);
 extern int  getchar  (void);
 extern int  ungetchar(int  c);
 extern int  putchar  (int  c);
 extern int  printf   (const char *fmt, ...); 
 extern char *gets    (char *s, unsigned int n);
 extern int  scanf    (const char *fmt, ...);
 extern int  vprintf  (const char *fmt, char *parm);
 extern int  puts     (const char *s);
 extern int  sprintf  (char *buf, const char *fmt, ...);
 extern int  vsprintf (char *buf, const char *fmt, char *parm);
 extern int  sscanf   (const char *buf, const char *fmt, ...);
 
 
#line 43 "C:\KEIL\ARM\INC\STDIO.H" 
 
 
#line 45 "C:\KEIL\ARM\INC\STDIO.H" 
 
 
 
#line 20 "DAC_ADC.c" 
 
 
 void DelayMs(unsigned int);
 
 
 
 
 void main(void)
 {
 unsigned long val;
 
  (*((volatile unsigned long *) 0xE002C000)) = 0x00050000;
  (*((volatile unsigned long *) 0xE002C004)) =	0X00480000;
  (*((volatile unsigned char *) 0xE01FC100)) 	= 	0x02;						 
 
  (*((volatile unsigned char *) 0xE001000C)) = 0x83;						
  (*((volatile unsigned char *) 0xE0010000)) = 195;
  (*((volatile unsigned char *) 0xE001000C)) = 0x03;
 
  (*((volatile unsigned long *) 0xE0034000))	=	0x00270401;
  (*((volatile unsigned long *) 0xE0034000))	|=	0X01000000;
  (*((volatile unsigned long *) 0xE0028008))  = 	0x0FFF7030;
 printf("******************************************************\n");
 printf("ADC I/P Adjust Pot(   ) and Measure P0.27 for DAC O/P \n");
 printf("******************************************************\n");
 
 while(1)
 {
 do
 {
 val = (*((volatile unsigned long *) 0xE0034004));                   	 
 }     
 while ((val & 0x80000000) == 0);   
 val = ((val >> 6) & 0x03FF)<<16;   
 DelayMs(20);
 printf("ADC I/P: %d \r",val>>16);
 val = val>>16; 
 val = val<<6;
  (*((volatile unsigned long *) 0xE006C000)) = (val & 0x0000FFC0);
 DelayMs(50);
 }
 }
 
 
 void DelayMs(unsigned int count) 	
 {  
 
 unsigned int i,j;		       		 
 for(i=0;i<count;i++)
 {
 for(j=0;j<1000;j++);
 }
 }
 
