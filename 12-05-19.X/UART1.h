/* 
 * File:   UART1.h
 * Author: Siva
 *
 * Created on May 12, 2019, 5:38 PM
 */

#include <stdbool.h>
#define _XTAL_FREQ 8000000
//(359) illegal conversion between pointer types
#pragma warning disable 359   // Suppress warning for Illegal Pointer conversion during string handling
#pragma warning disable 361
#pragma warning disable 520   //is never called
/*********************************************************************
* Function: bool UART_Init(const long int baudrate
*
* Overview: Initializes the UART2.  Can take several hundred
*           milliseconds.
*
* PreCondition: none
*
* Input: None
*
* Output: true if successful, false otherwise
*
********************************************************************/
bool UART1_Init(const long int baudrate)
{
	int  x;
	x = ((_XTAL_FREQ / baudrate)/64)-1;
	ANSELC=0x00;
	TRISC7 = 1; // Enable the Receiver Pin
    TRISC6 = 0; // Enable the Transmite pin
    SPBRGH1 = x>>8;
    SPBRG1=x;   // Generate the Baud Rate
    TXSTA1bits.SYNC=0;// Asynchronous
    TXSTA1bits.BRGH=0;// for high baud_rate
    RCSTA1bits.SPEN=1;// Enable serial port pins
    BAUDCON1bits.BRG16=0;  //16-bit Baud Rate Generator is used (SPBRGHx:SPBRGx)
    RCSTA1bits.CREN=1; // Continuous Receive Enable bit
    TXSTA1bits.TXEN =1; // enable transmission
    BAUDCON1bits.DTRXP=0;// Receive data (RXx) is not inverted (active-high
    BAUDCON1bits.WUE=1;// Receiver is operating normally
    BAUDCON1bits.RCIDL=1;// Start bit has been detected and the receiver is active
    TXSTA1bits.TX9=0;// 8-bit reception selected
    RCSTA1bits.RX9=0; // 8-bit reception mode selected
    RCSTA1bits.SREN= 0; // Single Receive Enable bit 
    RCSTA1bits.ADDEN=0; //Address Detect Enable bit
    TXSTA1bits.TX9D =0; // Ninth bit of Transmit Data
    TXSTA1bits.SENDB = 0; // Send Break Character bit
    // SPEN enabled; RX9 8-bit; CREN enabled; ADDEN disabled; SREN disabled; 
    // TX9 8-bit; TX9D 0; SENDB sync_break_complete; TXEN enabled; SYNC asynchronous; BRGH hi_speed; CSRC slave_mode; 
   
    
   return 1;
}

char UART1_TX_Empty()
{
  return TXSTA1bits.TRMT;
}

char UART1_Data_Ready()
{
   return RC1IF;
}

/*********************************************************************
* Function: void UART_PutChar(char data);
*
* Overview: Puts a character on the UART.  Unsupported characters will be
*           discarded.  May block or throw away characters is UART is not ready
*           or buffer space is not available.
*
* PreCondition: None
*
* Input: char - character to print
*
* Output: None
*
********************************************************************/
void UART1PutChar(char data)
{
  while(TX1IF==0);
    
  TXREG1 = data;
  
}
/*********************************************************************
* Function: void UART_PutString(char *text)
*
* Overview: Puts a string on the UART.  Unsupported characters will be
*           discarded.  May block or throw away characters is UART is not ready
*           or buffer space is not available.  
*
* PreCondition: None
*
* Input: char* - string to print
*        
* Output: None
*
********************************************************************/
void UART1PutString(char *text, int lenth)
{
  int i;
  for(i=0;i < lenth;i++)
      
	 UART1PutChar(text[i]);
}


