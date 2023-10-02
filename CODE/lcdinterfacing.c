/* 
 * File:   LCD Interfacing.c
 * Author: Atul Gaikwad
 * Created on March 25, 2018, 2:32 PM
 */

#include<pic18f4455.h>
#define lcd PORTD                          //----assigning port
#define rs PORTDbits.RD0                   //rs pin for LCD control
#define en PORTDbits.RD1                //en pin for LCD control

//-----Prototype declaration----//
void lcd_cmd(unsigned char x);         //function to send command to lcd
void lcd_dwr(unsigned char x);         //function to send datat to lcd
void lcd_msg(unsigned char *c);     //function to send string of data to lcd
void lcd_init();       // lcd initialization
void lcd_lat();             // function to latch lcd
void delay(unsigned int ms);    //delay function
//------------------------//
void main()
{
    
}
void lcd_lat()
{
 en=1;
 delay(1);
 en=0;
}
void lcd_cmd(unsigned char x)
{
    rs=0;
    lcd&=0x0F;
    
}