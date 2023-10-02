/* 
 * File:   amit.c
 * Author: Atul
 *
 * Created on March 25, 2018, 3:42 PM
 */

 
#include<pic18f4455.h>
#define RS PORTDbits.RD2;
#define EN PORTDbits.RD3;
#define RED PORTDbits.RD7;
#define GREEN PORTDbits.RD6;
#define LCDDATA PORTB;

void Rxmsg(void);
void lcdinit(void);
void lcdData(unsigned char l);
void lcdcmd(unsigned char k);
void DelayMs(unsigned int count); 
void sucessRx(void);
void unknown(void);
void display(unsigned char s,t);

void welcome(void);

void main()
{
	unsigned char i=0;
	unsigned int j=0;
	unsigned char c[13];
	TRISB = 0;
	TRISD = 0;
	OSCCON = 0X72;
	lcd_init();
	TRISC7=1;
	SPBRG=12;
	RCSTA=0X90;
	RED=1;
	GREEN=0; 
	lcd_init();
	welcome();
	while(1)
	{
		back:
			for(i=0;i<15;i++)
			{
				c[i]=0Xff;
			}
			while(RCIF==0);
			for(i=0;i<15;i++)
			{
				j=0;
				while(RCIF==0)
				{
					if(j>=1000)
					if(RCSTAbits.OERR)
					{
						CREN=0;
						NOP();
						CREN=1;
					}
					goto timeout;
					j++;
				}
				c[i]=RCREG;
				RCIF=0;
			}
			timeout:
				for(i=0;i<15;i++)
				{
					if(c[i]=='1' && c[i+1]=='E' && c[i+2]=='0' && c[i+3]=='0' && c[i+4]=='7' && c[i+5]=='C' && c[i+6]=='A' && c[i+7]=='0' && c[i+8]=='3' && c[i+9]=='C')
					{
						sucessRx();
						DelayMs(1000);
						RED=0;
						GREEN=1;
						DelayMs(1000);
						GREEN=0;
						lcdinit();
						DelayMs(100);
						welcome();
						
						goto back;
					}
				}
				unknown();
				DelayMs(1000);
				RED=1;
				GREEN=0;
				DelayMs(1000);
				RED=0;
				DelayMs(1000);
				lcdinit();
				DelayMs(100);
				welcome();
	}
}
void sucessRx()
{
unsigned int i=0;
unsigned char c[]="ACCESS GRANTED  ";

lcdcmd(0x01);
DelayMs(10);
lcdcmd(0x80);
DelayMs(10);

while(c[i]!='\0')
{
lcdData(c[i]);
i++;
}
}

void unknown(void)
{
unsigned int i=0;
unsigned char c[]="ACCESS DENIED";

lcdcmd(0x01);
DelayMs(10);
lcdcmd(0x80);
DelayMs(10);

while(c[i]!='\0')
{
lcdData(c[i]);
i++;
}
}

//---------------------------------------
// Lcd initialization subroutine
//---------------------------------------
void lcdinit(void)
{
lcdcmd(0x38);
DelayMs(250);
lcdcmd(0x0E);
DelayMs(250);
lcdcmd(0x01);
DelayMs(250);
lcdcmd(0x06);
DelayMs(250);
lcdcmd(0x80);
DelayMs(250);
}

//---------------------------------------
// Lcd data display
//---------------------------------------
void lcdData(unsigned char l)
{
LCDDATA=l;
RS=1;
EN=1;
DelayMs(1);
EN=0;
return;
}

//---------------------------------------
// Lcd command
//---------------------------------------
void lcdcmd(unsigned char k)
{
LCDDATA=k;
RS=0;
EN=1;
DelayMs(1);
EN=0;
return;
}			   

//---------------------------------------
// Delay mS function
//---------------------------------------
void DelayMs(unsigned int count) 
{  // mSec Delay 11.0592 Mhz 
    unsigned int i;		      // Keil v7.5a 
    while(count) {
        i = 115; 			 // 115	exact value
		while(i>0) 
		i--;
        count--;
    }
}

void welcome(void)
{
unsigned int i=0;
unsigned char c[]="WELCOME TO AISSMS";	 
unsigned char d[]="SECURITY SYSTEM";

lcdcmd(0x01);
DelayMs(10);
lcdcmd(0x80);
DelayMs(10);

while(c[i]!='\0')
{
lcdData(c[i]);
i++;
}

lcdcmd(0xc0);

i=0;
while(d[i]!='\0')
{
lcdData(d[i]);
i++;
}
}	

