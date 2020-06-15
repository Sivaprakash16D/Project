/* 
 * File:   ISR.h
 * Author: Siva
 *
 * Created on May 29, 2019, 7:09 AM
 */
void __interrupt () ISR(void)
{
    if(RC1IF)  //Receiver Interrupt
       {
           unsigned char c;
           c=RCREG1bits.RC1REG;   
           string[receiver_count]=c;
           receiver_count++;
           if(receiver_count==254)receiver_count=0; 
           // RELAY =~RELAY;
           RC1IF=0;
          
       }
    if(INTCONbits.INT0IF) //External(GSM Modem) Interrupt
        {
           
//            if(INTCON2bits.INTEDG0 == 1)
//            //interrupt is triggered on falling edge.
//            INTCON2bits.INTEDG0=0;
//             else
//            //interrupt is triggered on faising edge.
//            INTCON2bits.INTEDG0=1;
            INT0IF_flag=1;
           INTCONbits.INT0IF=0;
        }
    if(TMR4IF) // Timer 4 Interrupt @500us
    {
        TMR2=249;
         Timer_count++;
        if(Timer_count>=2000)
        {
            Timer_count_flag++;
            Timer_count=0;
            // RELAY=~RELAY;
            
        }
        if(Timer_count_flag>=Server_TMR)
        {
            Server_sent=1;
            Timer_count_flag=0;
        }
       
        TMR4IF=0;
    }
     if(RC2IF)  //Receiver Interrupt
       {
//           unsigned char c;
//           c=RCREG1bits.RC1REG;   
//           string[receiver_count]=c;
//           receiver_count++;
//           if(receiver_count==254)receiver_count=0; 
//           // RELAY =~RELAY;
           RC2IF=0;
          
       }
}

