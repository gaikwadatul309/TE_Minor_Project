#define _XTAL_FREQ 8000000

#define RS RB2
#define EN RB3
#define D4 RB4
#define D5 RB5
#define D6 RB6
#define D7 RB7

#include <xc.h>
#include "lcd.h";
#include "uart.h";
#include <pic16f877a.h>

// BEGIN CONFIG
#pragma config FOSC = HS // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF // Watchdog Timer Enable bit (WDT enabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF // Flash Program Memory Code Protection bit (Code protection off)
//END CONFIG

void main()
{
  char i,rfid[13] = "123456781212";

  TRISB = 0x00;
  
  Lcd_Init();                        // Initialize LCD
  Lcd_Clear();                       // Clear display
  Lcd_Set_Cursor(1,1);
  Lcd_Write_String("RFID Tag Reader");     // Write text in first row

  UART_Init(9600);

  rfid[12] = '\0';

  while(1)
  {
     if(UART_Data_Ready())
     {
       for(i=0;i<12;)
       {
         if(UART_Data_Ready())
         {
            rfid[i] = UART_Read();
            i++;
         }
       }

       if((rfid[0] ^ rfid[2] ^ rfid[4] ^ rfid[6] ^ rfid[8] == rfid[10]) && (rfid[1] ^ rfid[3] ^ rfid[5] ^ rfid[7] ^ rfid[9] == rfid[11]))
       {
           Lcd_Set_Cursor(2,1);
           Lcd_Write_String(rfid);
       }
       else
       {
         Lcd_Set_Cursor(1,1);
         Lcd_Write_String("Error           ");
       }
     }
  }
}