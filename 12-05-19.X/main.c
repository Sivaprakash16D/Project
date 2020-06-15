/*
 * File:   main.c
 * Author: Siva
 *
 * Created on May 12, 2019, 4:23 PM
 */


#include "App.h"


/*******************************************************************************
 * InternalEEROM ADDRESS  : DETAILS
 * 0 - 4                  : Master Mobile No
 * 15                     : Master Mobile No Enable Or None
 * 16-18                  : Server delay time
 * 19                     : Server delay time set bit
 *******************************************************************************/





void main(void) {
    
   SYS_Initialize ();
   UART1_Init(9600);
   UART2_Init(9600);
   Modem_install();
  
//    RELAY=MODEM=1;
//    __delay_ms(2000);
//   RELAY=MODEM=0; 
    while(1)
    {
      MSG_check(); 
//         if(INT0IF_flag)
//    {
//    INT0IF_flag=0;
//      MODEM=~MODEM;
//       
//     
//       
//    }
        // else
//         {
//          RELAY=MODEM=0; 
//         }
//     
    }
    
    
}