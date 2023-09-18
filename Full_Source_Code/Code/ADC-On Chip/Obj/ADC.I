
#line 1 "ADC.c" 
















 
 
  
#line 1 "C:\KEIL\ARM\INC\PHILIPS\LPC213X.H" 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
#line 19 "ADC.c" 
 
  
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
 
 
 
#line 20 "ADC.c" 
 
 
 
 
 
 
 
 void serial_init(void)
 {
  (*((volatile unsigned long *) 0xE002C000)) = 0x00000005;            
  (*((volatile unsigned char *) 0xE000C00C)) = 0x83;                    
  (*((volatile unsigned char *) 0xE000C000)) = 195;                      
  (*((volatile unsigned char *) 0xE000C00C)) = 0x03;                    
 }
 
 
 
 
 
 void delay(int n)
 {
 int i,j;
 for(i=0;i<n;i++)
 {
 for(j=0;j<0x5000;j++)
 {;}
 }
 }	 
 
 
 
 
 
 
 void main(void)
 {
 
 unsigned long val;
  (*((volatile unsigned long *) 0xE002C000)) =	0X00000005;		 
  (*((volatile unsigned long *) 0xE002C004)) =	0X01000000;		 
  (*((volatile unsigned char *) 0xE01FC100)) 	= 	0x02;			 
  (*((volatile unsigned long *) 0xE0034000))	=	0x00270602;		 
  (*((volatile unsigned long *) 0xE0034000))	|=	0X01000000;		 
  (*((volatile unsigned long *) 0xE0028008))  = 	0x0FFF7030;
 
 serial_init();				 
 
 
 while(1)
 {
 do
 {
 val = (*((volatile unsigned long *) 0xE0034004));                   	 
 }     
 while ((val & 0x80000000) == 0);  	 
 val = ((val >> 6) & 0x03FF);
 delay(1);
 printf("value : %d\n",val);
 delay(1);
 }
 }
 
 
