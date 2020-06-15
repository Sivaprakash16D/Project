#include "System.h"

/*********************************************************************
* Function: void clear_Buffer()
*
* Overview: Receiving Intruppt storage array should be clear by loop.
*
* PreCondition: none
*
* Input: None
*
* Output:None
*
********************************************************************/
void clear_Buffer()
{
    unsigned char clear=0;
    for(clear=0;clear<255;clear++)
    {
        string[clear]=0;
        receiver_count=0; 
    }
}
/*********************************************************************
* Function: void SYS_Initialize ()
*
* Overview: System installation of the system oscillation speed and enable 
  Globel Interrupt and periperale interrupt.
*
* PreCondition: none
*
* Input: None
*
* Output:None
*
********************************************************************/
void SYS_Initialize ()
{
  
  
 OSCCONbits.IRCF = Internal_RC_Oscillator_Frequency_8MHz;

 OSCCONbits.SCS =  0b10;//Primary_System_Clock_Select;
 OSCTUNEbits.PLLEN = 0;
 INTCONbits.GIE=Global_Interrupt_Enable;
 INTCONbits.PEIE=Peripheral_Interrupt_Enable;
 IPR3bits.RC2IP=EUSART2_Receive_Interrupt_Priority;
 PIE3bits.RC2IE=EUSART2_Receive_Interrupt_Enable;
 INTCONbits.INT0IE=INT0_External_Interrupt_Enable;
 ANSELA=0x00;
 MODEM_ASSIGN =0;
 RELAY_ASSIGN =0;
 MODEM=0;
 RELAY=0;
 PIE5bits.TMR4IE=1;
 T4CONbits.T4CKPS0=1;
 TMR4=249;                // Timer 4 Interrupt @500us
 INTEDG0=0;
 //ANSELB=ANSELA=ANSELC=ANSELD=0x00;

 EEADR=19;
 RD = 1;
 Server_TMR_Bit   = EEDATA;
  if(Server_TMR_Bit==0xFF)Server_TMR=120;
 if(Server_TMR_Bit==1)
 { 
     
           EEADR=16;
           RD = 1;
           Server_TMR += EEDATA*100;
           EEADR=17;
           RD = 1;
           Server_TMR += EEDATA*10;
           EEADR=18;
           RD = 1;
           Server_TMR += EEDATA ;
          
 
 }
 T4CONbits.TMR4ON=1; 
}

