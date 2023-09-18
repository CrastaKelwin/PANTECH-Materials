/* Project name:
     DEMO (Ethernet controller ENC28J60 mini library)
     
 * Description:
        This library is designed to embed tiny servers.
        No Ethernet knowledge is necessary to use it.
        It has a very small footprint.
 
        Features :
                Driver for ENC28J60 Microchip SPI Ethernet controller
                Supports IPV4 protocol
                Does not support fragmented packets
                Replies to :
                        - ARP requests
                        - ICMP echo requests
                        - UDP requests
                        - TCP requests (no stack, no packet reconstruction)
 * Test configuration:
     MCU:             LPC21xx
     Oscillator:      12.0 MHz (cclk = 60.0 MHz, Fcco = 240.0 MHz)
     SW:              KEIL uVision3 v3.50
      
 * BOARD NOTES:
     - Setup PC Ethernet Card "Speed and Duplex" to 10Mb/s Full Duplex
      (P0.7/SSEL0 must be high for SPI to operate)
     - From Command prompt try to ping IP address defined below (deault 192.168.20.60)
     - In web browser type URL http://192.168.1.60
*/

/********************************************************************************/
//  						Hardware settings (ADK Board)
/********************************************************************************/
//
//  CN4(LED)    - Connector (JP8) to LED Output    -->  Port1(P1.16 to P1.23)
//  CN2(Switch) - Connector (JP12) to Switch Input -->  Port1(P1.24 to P1.31)
//
//  SW33 -  DIP Switch
//	==================
//	Turn On - SCK, MISO, MOSI   (SPI Communication)
//
//  SW34 -  DIP Switch
//	==================
//	Turn On - ETHR.CS, ETHR.RST, ETHR.INT, ETHR.WOL (Ethernet ENC28j60)
//
/********************************************************************************/
//			In web browser type URL http://192.168.1.60
/********************************************************************************/

#include <LPC213X.H>
#include "string.h"

#include "ENC28J60.h"
#include "c_ctype.h"
#include "conv.h"
#include "Utility.h"

#define ENC28J60_HALFDUPLEX		0
#define ENC28J60_FULLDUPLEX		1

/************************************************************
 * ROM constant strings
 */
const unsigned char httpHeader[] = "HTTP/1.1 200 OK\nContent-type: ";		// HTTP header
const unsigned char httpMimeTypeHTML[] = "text/html\n\n";					// HTML MIME type
const unsigned char httpMimeTypeScript[] = "text/plain\n\n";				// TEXT MIME type
unsigned char httpMethod[] = "GET /";
/*
 * this HTML page calls the boards to get its status, and builds itself with javascript
 */
const char *indexPage = "<HTML><HEAD></HEAD><BODY>\
<h1> ARM& LPC214x </h1>\
<h2>  WEB SERVER  </h2>\
<a href=/>Reload</a>\
<script src=/s></script>\
<table><tr><td valign=top><table border=2 style=\"font-size:25px ;font-family: terminal ;\">\
<tr><th colspan=3>ADC</th></tr>\
<tr><td>AN0</td><td><script>document.write(AN0)</script></td></tr>\
<tr><td>AN1</td><td><script>document.write(AN1)</script></td></tr>\
</table></td><td><table border=1 style=\"font-size:15px ;font-family: terminal ;\">\
<tr><th colspan=3>P1[31..24]</th></tr>\
<script>\
var str,i;\
str=\"\";\
for(i=0;i<8;i++)\
{str+=\"<tr><td bgcolor=blue>BUTTON #\"+i+\"</td>\";\
if(PORT1_31_24&(1<<i)){str+=\"<td bgcolor=green>ON\";}\
else {str+=\"<td bgcolor=#cccccc>OFF\";}\
str+=\"</td></tr>\";}\
document.write(str) ;\
</script>\
</table></td><td>\
<table border=2 style=\"font-size:15px ;font-family: terminal ;\">\
<tr><th colspan=4>P1[23..16]</th></tr>\
<script>\
var str,i;\
str=\"\";\
for(i=0;i<8;i++)\
{str+=\"<tr><td bgcolor=orange>LED #\"+i+\"</td>\";\
if(PORT1_23_16&(1<<i)){str+=\"<td bgcolor=blue>ON\";}\
else {str+=\"<td bgcolor=#cccccc>OFF\";}\
str+=\"</td><td><a href=/t\"+i+\">Toggle</a></td></tr>\";}\
document.write(str) ;\
</script>\
</table></td></tr></table>\
This is HTTP request #<script>document.write(REQ)</script></BODY></HTML>\
";

/***********************************
 * RAM variables
 */
unsigned char myMacAddr[6] = {0x00, 0x14, 0xA5, 0x76, 0x19, 0x3f};	// my MAC address
unsigned char myIpAddr[4] = {192, 168, 1, 60};						// my IP address
unsigned char getRequest[15];										// HTTP request buffer
unsigned char dyna[31];												// buffer for dynamic response
unsigned long httpCounter = 0;										// counter of HTTP requests




/*******************************************
 * functions
 */
 
/*
 * put the constant string pointed to by s to the ENC transmit buffer
 */
unsigned long putConstString(const char *s)
{
	unsigned long ctr;
        
	ctr = 0;
	while(*s)
	{
		ENC28J60_putByte(*s++);
		ctr++;
	}
	return(ctr);
}
        
/*
 * put the string pointed to by s to the ENC transmit buffer
 */
unsigned long putString(char *s)
{
	unsigned long ctr;

	ctr = 0;
	while(*s)
	{
		ENC28J60_putByte(*s++);
		ctr++;
	}
	return(ctr);
}


/*
 * this function is called by the library
 * the user accesses to the HTTP request by successive calls to ENC28J60_getByte()
 * the user puts data in the transmit buffer by successive calls to ENC28J60_putByte()
 * the function must return the length in bytes of the HTTP reply, or 0 if nothing to transmit
 *
 * if you don't need to reply to HTTP requests,
 * just define this function with a return(0) as single statement
 *
 */
unsigned long ENC28J60_userTCP(unsigned char *remoteHost, unsigned long remotePort, unsigned long localPort, unsigned long reqLength)
{
	unsigned long
		len,						// my reply length
		i,							// general purpose integer
		bitMask;				// for bit mask

	// for parameters that are not used, skip compiler warnings "unreferenced parameter"
	i = (unsigned long) remoteHost;
	i = remotePort;
	i = reqLength;

	len = 0;
	bitMask = 0;

	if (localPort != 80)			// I listen only to web request on port 80
		return(0) ;
                
	// get 10 first bytes only of the request, the rest does not matter here
	for (i = 0; i < 10; i++)
		getRequest[i] = ENC28J60_getByte();
	getRequest[i] = 0;

	if (memcmp(getRequest, httpMethod, 5))						// only GET method is supported here
		return(0);

	httpCounter++;												// one more request done

	if (getRequest[5] == 's')									// if request path name starts with s, store dynamic data in transmit buffer
	{
		// the text string replied by this request can be interpreted as javascript statements by browsers
		len = putConstString(httpHeader);						// HTTP header
		len += putConstString(httpMimeTypeScript);			// with text MIME type

		// add AN0 value to reply
		IntToStr(0, dyna);
		len += putConstString("var AN0=");
		len += putString(dyna);
		len += putConstString(";");

		// add AN1 value to reply
		IntToStr(0, dyna);
		len += putConstString("var AN1=");
		len += putString(dyna);
		len += putConstString(";");

		// add PORT1[31..24] value (buttons) to reply
		len += putConstString("var PORT1_31_24=");
		IntToStr(((IOPIN1 >> 24) & 0xFF), dyna);
		len += putString(dyna);
		len += putConstString(";");
                        
		// add PORT1[23..16] value (LEDs) to reply
		len += putConstString("var PORT1_23_16=");
		IntToStr(((IOPIN1 >> 16) & 0xFF), dyna);
		len += putString(dyna);
		len += putConstString(";");

		// add HTTP requests counter to reply
		IntToStr(httpCounter, dyna);
		len += putConstString("var REQ=");
		len += putString(dyna);
		len += putConstString(";");
	}
	else if (getRequest[5] == 't')								// if request path name starts with t, toggle LED bit number that comes after
	{
		if (isdigit(getRequest[6]))								// if 0 <= bit number <= 9, bits 8 & 9 does not exist but does not matter
		{
			bitMask = getRequest[6] - '0';						// convert ASCII to integer
			bitMask = 1 << (bitMask + 16);						// create bit mask
			if ((IOPIN1 & bitMask) != 0)						// Toggled LED
				IOCLR1 |= bitMask;
			else
				IOSET1 |= bitMask;
		}
	}

	if (len == 0)													// what do to by default
	{
		len =  putConstString(httpHeader);							// HTTP header
		len += putConstString(httpMimeTypeHTML);					// with HTML MIME type
		len += putConstString(indexPage);							// HTML page
	}

	return (len);					// return to the library with the number of bytes to transmit
}
        
/*
 * this function is called by the library
 * the user accesses to the UDP request by successive calls to ENC28J60_getByte()
 * the user puts data in the transmit buffer by successive calls to ENC28J60_putByte()
 * the function must return the length in bytes of the UDP reply, or 0 if nothing to transmit
 *
 * if you don't need to reply to UDP requests,
 * just define this function with a return(0) as single statement
 *
 */
unsigned long ENC28J60_userUDP(unsigned char *remoteHost, unsigned long remotePort, unsigned long destPort, unsigned long reqLength)
{
	unsigned long
		len;						// my reply length
	unsigned char
		*ptr;						// pointer to the dynamic buffer

	// reply is made of the remote host IP address in human readable format
	ByteToStr(remoteHost[0], dyna);				// first IP address byte
	dyna[3] = '.';
	ByteToStr(remoteHost[1], dyna + 4);		// second
	dyna[7] = '.';
	ByteToStr(remoteHost[2], dyna + 8);		// third
	dyna[11] = '.';
	ByteToStr(remoteHost[3], dyna + 12);	// fourth

	dyna[15] = ':';												// add separator

	// then remote host port number
	IntToStr(remotePort, dyna + 16);
	dyna[22] = '[';
	IntToStr(destPort, dyna + 23);
	dyna[29] = ']';
	dyna[30] = 0;

	// the total length of the request is the length of the dynamic string plus the text of the request
	len = 30 + reqLength;

	// puts the dynamic string into the transmit buffer
	ptr = dyna;
	while (*ptr)
		ENC28J60_putByte(*ptr++);

	// then puts the request string converted into upper char into the transmit buffer
	while (reqLength--)
		ENC28J60_putByte(toupper(ENC28J60_getByte()));

	return (len);		// back to the library with the length of the UDP reply
}




/*
 * main entry
 */
int main (void)
{
	PINSEL0 = 0;
	PINSEL1 = 0;
	PINSEL2 &= 0x0000000C;
	delay_Nx10cyc(599999);			// Delay 0,1s

	IODIR1 &= 0x00FFFFFF;				// Set P1[31..24] as inputs for buttons
	IODIR1 |= 0x00FF0000;				// Set P1[23..16] as outputs for LED
	IOCLR1 |= 0x00FF0000;				// Turn off LEDs
        
	// starts ENC28J60 with : RST bit on IOPIN0.13, CS bit on IOPIN0.12, my MAC & IP address, full duplex
	ENC28J60_Init(&IOPIN0, 13, &IOPIN0, 12, myMacAddr, myIpAddr, ENC28J60_FULLDUPLEX) ;

	while (1)
	{
		ENC28J60_doPacket();		// process incoming Ethernet packets
		/*
		* add your stuff here if needed
		* ENC28J60_doPacket() must be called as often as possible
		* otherwise packets could be lost
		*/
	}
}
