/* 
 * File:   UART2.h
 * Author: Siva
 *
 * Created on May 12, 2019, 5:59 PM
 */

#ifndef UART2_H
#define	UART2_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdbool.h>
#define _XTAL_FREQ 8000000
//(359) illegal conversion between pointer types
//#pragma warning disable 359   // Suppress warning for Illegal Pointer conversion during string handling
//#pragma warning disable 361
//#pragma warning disable 520   //is never called
/*********************************************************************
* Function: bool UART2_Init(const long int baudrate
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
bool UART2_Init(const long int baudrate)
{
	int  x;
	x = ((_XTAL_FREQ / baudrate)/64)-1;
	ANSELD=0x00;
	TRISD7 = 1; // Enable the Receiver Pin
    TRISD6 = 0; // Enable the Transmite pin
    SPBRGH2 = x>>8;
    SPBRG2=x;   // Generate the Baud Rate
    TXSTA2bits.SYNC=0;// Asynchronous
    TXSTA2bits.BRGH=0;// for high baud_rate
    RCSTA2bits.SPEN=1;// Enable serial port pins
    BAUDCON1bits.BRG16=0;  //16-bit Baud Rate Generator is used (SPBRGHx:SPBRGx)
    RCSTA2bits.CREN2=1; // Continuous Receive Enable bit
    TXSTA2bits.TXEN =1; // enable transmission
    BAUDCON2bits.DTRXP=0;// Receive data (RXx) is not inverted (active-high
    BAUDCON2bits.WUE=1;// Receiver is operating normally
    BAUDCON2bits.RCIDL=0;// Start bit has been detected and the receiver is active
    TXSTA2bits.TX9=0;// 8-bit reception selected
    RCSTA2bits.RX9=0; // 8-bit reception mode selected
    RCSTA2bits.SREN= 0; // Single Receive Enable bit 
    RCSTA2bits.ADDEN=0; //Address Detect Enable bit
    TXSTA2bits.TX9D =0; // Ninth bit of Transmit Data
    TXSTA2bits.SENDB = 0; // Send Break Character bit
    
   return 1;
}

char UART2_TX_Empty()
{
  return TXSTA1bits.TRMT;
}

char UART2_Data_Ready()
{
   return RC2IF;
}

/*********************************************************************
* Function: void UART2_PutChar(char data);
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
void UART2_PutChar(char data)
{
  while(TX2IF==0);
    
  TXREG1 = data;
}
/*********************************************************************
* Function: void UART2_PutString(char *text)
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
void UART2_PutString(char *text, int lenth)
{
  int i;
  for(i=0;i <= lenth;i++)
	  UART2_PutChar(text[i]);
}




#ifdef	__cplusplus
}
#endif

#endif	/* UART2_H */

