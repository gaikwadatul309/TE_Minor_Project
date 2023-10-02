#include<p18f4520.h>

#include<delays.h>
#include<usart.h>


#pragma config WDT = OFF,FCMEN=ON,IESO = OFF,MCLRE = OFF, PBADEN = OFF,LVP = OFF,BOREN = SBORDIS,BORV = 3,LPT1OSC = OFF
#pragma config WDTPS = 32768,CP0 = OFF,CP1 = OFF,CP2 = OFF,CP3 = OFF
#pragma config OSC = INTIO67
#pragma config PWRT = ON

#define IN4 PORTAbits.RA3
#define IN3 PORTAbits.RA4
#define EN2 PORTAbits.RA5

#define rs PORTDbits.RD5
#define rw PORTDbits.RD6
#define en PORTDbits.RD7

#define LED_G PORTCbits.RC2
#define LED_R PORTCbits.RC3

void InterruptHandlerHigh(void);
void uart_init();
void intrrupt_init();

unsigned char rf1[]="450041134057"; // 450041134058

unsigned char rfid_data[13],rfid_flag=0,cnt=0;

/*********************************************************************
*			Interrupt routine										 *
*********************************************************************/
#pragma code InterruptVectorHigh = 0x08
void dInterruptVectorHigh (void)
{
  _asm
    goto InterruptHandlerHigh                //jump to interrupt routine
  _endasm
}


#pragma interrupt InterruptHandlerHigh

void InterruptHandlerHigh(void)
{
	unsigned char recv_data;
	
	if(PIR1bits.RCIF)  				  //serial interrupt // do we have uart rx interrupt request?
	{
		PIR1bits.RCIF=0;
	 	recv_data=RCREG;
	 	rfid_data[cnt]= recv_data;
	 	cnt++;
	 	if(cnt==12)
	 	{
	 		rfid_data[cnt]='\0';
	 		cnt=0;
	 		rfid_flag=1;
	 	}
	}
} 

	

/*********************************************************************
*				Delay        										 *
*********************************************************************/

void delays(unsigned char x)
{
		Delay100TCYx(44);
}
void delay_ms(unsigned int del)
{
	unsigned int i;
	for(i=0;i<del;i++)
	{
		Delay1KTCYx(2);//2
	}
}

/******************************************************************
   LCD function
******************************************************************/
void lcd_data(unsigned char value, unsigned char cmd)
{
	PORTB = value;
	rs=cmd;
	rw=0;
	en=1;
	delays(100);
	en=0;
}

void lcd_init(void)
{
	lcd_data(0x038,0);delays(100);
	lcd_data(0x0e,0);delays(100);
	lcd_data(0x0c,0);delays(100);
	lcd_data(0x06,0);delays(100);
	lcd_data(0x01,0);delays(100);
	lcd_data(0x80,0);delays(100);	
}
void lcdprint(unsigned rom char *str)  //Function to send string data to LCD.
	{
    while(*str)
    {
        lcd_data(*str,1);
        str++;
    }
	}

void digit_disp(unsigned int p)
 {
	char t[4],i=0;
	for(i=0;i<4;i++)
	{
		t[i]=48+(p%10);
		p=p/10;
	}
	for(i=3;i>=0;i--)
	{	
		lcd_data(t[i],1);Delay10TCYx(200);
	}
}
/*********************************************************************
*				Uart Initialization							 *
**********************************************************************/
void uart_init(void)
{
	TRISCbits.TRISC7 = 1;
	TRISCbits.TRISC6 = 0;
	OpenUSART (USART_TX_INT_OFF & USART_RX_INT_ON & USART_ASYNCH_MODE &   USART_EIGHT_BIT & 	USART_CONT_RX & USART_BRGH_LOW,12);    //9600 baud rate com 5
																															//for 4800 25 for 9600 12
	Delay1KTCYx(200);
}
/*********************************************************************
*				Intrruppt Initialization 							 *
**********************************************************************/
// INT0-INT1-receiver
void intrrupt_init(void)
{
	INTCON=0b11000000;  // GIEL=GIEH=INT0IE=1
	INTCON2=0b00000000;    
	INTCON3=0x00;
	PIR1=0b00000000;  
	PIR2=0x00;
	PIE1=0b00000000;  
	PIE2=0x00;
	IPR1=0b00000000;   // TMR1IP=1;
	IPR2=0x00;
	PIE1bits.RCIE=1;     // receive interrupt enable bit
	IPR1bits.RCIP=1;    // EUSART Receive Interrupt Priority bit made High
	
	IPR1bits.TXIP=0;
	RCON=0x9f;     // interrupt prioroty level bit
}


/***********************************************************************
	Motor forward and reversed
*********************************************************************/
void motor_forward()
{
	EN2=1;
	IN3=1;
	delay_ms(500);
	IN3=0;
	EN2=0;delay_ms(100);
	
}
void motor_reversed()
{
	EN2=1;
	IN4=1;
	delay_ms(500);
	IN4=0;
	EN2=0;delay_ms(100);
}

unsigned char compare(unsigned char a[], unsigned char b[], unsigned char len)
{
    unsigned char i,res=0;
    for(i=0;i<len;i++)
    {
        if(a[i] != b[i])
        {
            res = a[i] - b[i];
            break;
        }
    }
    return res;
}
/**********************************************************************
	Main Function
**********************************************************************/
	
void main()
{
	
	unsigned char vh1=0;
	unsigned char i;
	ADCON1=0b00001111;
	TRISB=0b00000000;
	TRISC=0b00000000;
	TRISA=0b00000000;  
	TRISD=0x00;
	TRISE=0x00;
	
	PORTE=0X00;
	PORTD=0x00;
	PORTA=0b00000011;
	PORTB=0b00000101;
	PORTC=0b00001100;
	
	OSCCONbits.IRCF0=1;
  	OSCCONbits.IRCF1=1;      //8 mhz   
  	OSCCONbits.IRCF2=1;
  	OSCCONbits.SCS1=1;      //  Internal osc
  	OSCCONbits.SCS0=0;
  	while(OSCCONbits.IOFS==0);  // osc stable
  	lcd_init();
  	lcdprint("PLEASE");
  	lcd_data(0xc0,0);
  	lcdprint("SHOW YOUR ID"); delay_ms(500);
  	uart_init();
  	intrrupt_init();
  	while(1)
  	{
  		if(rfid_flag==1)
  		{
	  		rfid_flag=0;
  			vh1= compare(rf1,rfid_data,12);		
  			if(vh1==0)
  			{
  				lcd_data(0x01,0);delays(10);
	  			lcd_data(0x80,0);delays(10);
	  			lcdprint("Authorized  ");
	  			LED_G=0;delay_ms(100);
	  			motor_forward();
	  			delay_ms(500);
	  			motor_reversed();delay_ms(500);
	  			LED_G=1;delay_ms(100);
	  			
  			}
  			if(vh1)
  			{
	  			LED_G=0;delay_ms(100);
  				lcd_data(0x01,0);delays(10);
	  			lcd_data(0x80,0);delays(10);
	  			lcdprint("Unauthorized");delays(500);
	  			
	  			LED_G=1;delay_ms(100);
  			}
  		}		  	
  	}
}  	
  	
  	