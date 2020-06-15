/* 
 * File:   App.h
 * Author: Siva
 *
 * Created on May 12, 2019, 4:26 PM
 */

#ifndef APP_H
#define	APP_H

#ifdef	__cplusplus
extern "C" {
#endif
    
#define _XTAL_FREQ 8000000
#include "htc.h"
#include "Config.h"
#include "In_EEPROM.h"
#include "UART1.h"
#include "UART2.h"
#include "System.h"
#include "ISR.h"
//
//void UART_STRING(char UART_Select,char *texts, int Bit_count)
//{
//    if(UART_Select == 1)
//    {
//    UART1_PutString(*texts, Bit_count);
//    }
//     if(UART_Select == 2)
//    {
//    UART1_PutString(*texts, Bit_count);
//    }
//}

#ifdef	__cplusplus
}
#endif

#endif	/* APP_H */

