/* 
 * File:   System.h
 * Author: Siva
 *
 * Created on May 12, 2019, 4:41 PM
 */

#include <pic18f46k22.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#define Internal_RC_Oscillator_Frequency_64MHz 0b111
#define Internal_RC_Oscillator_Frequency_8MHz 0b110
#define Internal_RC_Oscillator_Frequency_4MHz 0b101
#define Internal_RC_Oscillator_Frequency_2MHz 0b100
#define Internal_RC_Oscillator_Frequency_1MHz 0b011
#define Frequency_Multiplier_4xPLL_for_HFINTOSC_Enable 1
#define Global_Interrupt_Enable 1
#define Peripheral_Interrupt_Enable 1
#define EUSART2_Receive_Interrupt_Priority 1
#define EUSART2_Receive_Interrupt_Enable 1
#define EUSART1_Receive_Interrupt_Priority 1
#define EUSART1_Receive_Interrupt_Enable 1
#define GSM_PKEY_ASSIGN TRISB3
#define MODEM_ASSIGN TRISA2
#define RELAY_ASSIGN TRISC4
#define INT0_ASSIGN TRISB0
#define GSM_PKEY LATB3
#define MODEM LATA2
#define RELAY LATC4
#define Enable 1
#define Disable 0
#define Primary_System_Clock_Select 1
#define INT0_External_Interrupt_Enable 1
#define MAXIMUM_RECEIVE 255

unsigned char receiver_count=0,string[MAXIMUM_RECEIVE],string1[MAXIMUM_RECEIVE];
char Reset_count=0,Server_TMR_Bit;
char IMEI[17],flag=1,SIM_Name[20],MOB_No[15];
bool INT0IF_flag=0,delay_bit,Server_sent,receiver_flag=0; 
int Timer_count=0,Timer_count_flag=0,Server_TMR;
extern bool receiver_check();
extern void MSG_check();
extern void GPRS_Connect();

//extern void __delay_ms(unsigned int h);

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
       OSCCONbits.IRCF      =   Internal_RC_Oscillator_Frequency_8MHz;
       OSCCONbits.SCS       =   0b10;//Primary_System_Clock_Select;
       OSCTUNEbits.PLLEN    =   Disable; 
       INTCONbits.GIE       =   Global_Interrupt_Enable;
       INTCONbits.PEIE      =   Peripheral_Interrupt_Enable;
      //  TRISB = 0xFF;
       TRISBbits.TRISB0     =   Enable;
     //  ANSELBbits.ANSB0     =   Disable;
//       IPR3bits.RC2IP       =   EUSART2_Receive_Interrupt_Priority;
//        RC2IE       =   Enable;
      // IPR1bits.RC1IP       =   EUSART1_Receive_Interrupt_Priority;
       PIE1bits.RC1IE       =   EUSART1_Receive_Interrupt_Enable;
       INTCONbits.INT0IE    =   Enable;
    //   INTCONbits.INT0IF    =   Enable;
    //   IPR1bits.RC1IP       =   Enable;
      // PIE1bits.RC1IE       =   Enable;
      
       INTCON2bits.INTEDG0  =   Disable;
       
       ANSELB=0x00;
       ANSELA=0x00;
       MODEM_ASSIGN =0;
       RELAY_ASSIGN =0;
       MODEM=0;
       RELAY=0;
       PIE5bits.TMR4IE=1;
       T4CONbits.T4CKPS0=1;
       TMR4=249;                // Timer 4 Interrupt @500us
       INTEDG0=0;
      // ANSELB=ANSELA=ANSELC=ANSELD=0x00;

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

/*********************************************************************
* Function: void Modem_install()
*
* Overview: Modem installation of the GSM Modem Enable and Install 
  the Instructions.
*
* PreCondition: none
*
* Input: None
*
* Output:None
*
********************************************************************/
bool SIM_Provider_Name()
{
  UART1PutString("AT+CSPN?\r\n",10); // Get Service Provider Name
 __delay_ms(200);flag=0;
 for(int i=0;string[i]!='\0';i++)
 {
     if(string[i]=='"')
     {
         i++;
         
         for(flag=0;string[i]!='"';flag++)
         {
         SIM_Name[flag]=string[i];
        // UART2_PutChar(SIM_Name[flag]);
         i++;
         }
         memset(string,0,255);__delay_ms(20);
         return 1;
     }
    
 }
 return 0;
}



bool receiver_check()
{
    __delay_ms(20);
    Reset_count++;
   // MSG_check();
    for(int i=0;i<255;i++)//string[i]!='\0';i++)
    {
       if(string[i]=='O'&&string[i+1]=='K')
       {
            memset(string,0,255);
           Reset_count=0;
           
           return 1;
       }
       
      
    }
    if(delay_bit)__delay_ms(100);
 //   MSG_check();    
    
    if(Reset_count==15)Reset();         
    
    return 0;
}

char SIM_Ready_Check_bit_count=0;


bool SIM_Ready_Check()
{
for(int i=0;string[i]<254;i++)//!='\0';i++)
    {
    __delay_ms(1);
   // UART1PutChar(string[i]);
    if(string[i]=='C'&&string[i+1]=='a')
       {
           
        memset(string,0,255);
           return 1;
          
       }
     
    }

SIM_Ready_Check_bit_count++; 

return 0;
}

void Modem_install()
{
   //  __delay_ms(1000);
        MODEM=1;
 while(!SIM_Ready_Check())
    {
      
        UART1PutString("AT\n\r",4);
        __delay_ms(3000);
      //  UART1PutString(string,254);
        if(SIM_Ready_Check_bit_count>10)
        {
        SIM_Ready_Check_bit_count=0;
        UART1PutString("Restart Modem\n\r",15);
       
        MODEM=0;
        __delay_ms(2000);
        MODEM=1;
     
        
        UART1PutString("AT\n\r",4);
        __delay_ms(3000);
        }
    }
//         __delay_ms(3000);
//  memset(string,0,255);
// RC1IF=0;
// UART1PutString("AT+GSN\r\n",8);  // Identification IMEI to save temporary array
// while(!RC1IF);flag=0;
// memset(IMEI,string+9,15);
// 
// UART1PutString("\r\n",2);
//  UART1PutString(IMEI,15);
//  UART1PutString("\r\n",2);
// 
 UART1PutString("AT+CMGF=1\r\n",11); // SMS Text Format only
 while(!receiver_check()){__delay_ms(2000);UART1PutString("AT+CMGF=1\r\n",11);}
// UART1PutString("AT+CSCA=\"+919442099997\",145\r\n",29); // SMS Text Format only
// while(!receiver_check()){__delay_ms(2000);UART1PutString("AT+CSCA=\"+919442099997\",145\r\n",29);}
//  __delay_ms(1000); memset(string,0,255);SIM_Provider_Name(); 
 UART1PutString("AT+CNMI=2,1,0,0,0\r\n",19); // SMS Text Format only
 while(!receiver_check()){__delay_ms(2000);UART1PutString("AT+CNMI=2,1,0,0,0\r\n",19);}
// UART1PutString("AT+CMGDA=\"DEL ALL\"\r\n",20); // Delete all messages
// while(!receiver_check()){__delay_ms(2000);UART1PutString("AT+CMGDA=\"DEL ALL\"\r\n",20);}
 UART1PutString("AT+CSCS=\"IRA\"\r\n",15);  //International reference alphabet
 while(!receiver_check()){__delay_ms(2000);UART1PutString("AT+CSCS=\"IRA\"\r\n",15);}
 UART1PutString("AT+CFGRI=0\r\n",12);  //URC =OFF
 while(!receiver_check()){__delay_ms(2000);UART1PutString("AT+CFGRI=0\r\n",12);}
 UART1PutString("AT+CLIP=1\r\n",11);  //Enable Calling Line Identification Presentation 
 while(!receiver_check()){__delay_ms(2000);UART1PutString("AT+CLIP=1\r\n",11);}
 UART1PutString("AT+CUSD=1\r\n",11);  //Enable Unstructured Supplementary Service Data 
 while(!receiver_check()){__delay_ms(2000);UART1PutString("AT+CUSD=1\r\n",11);}
 UART1PutString("AT+CLTS=0\r\n",11);  //disable Get Local Timestamp 
 while(!receiver_check()){__delay_ms(2000);UART1PutString("AT+CLTS=0\r\n",11);}
 UART1PutString("AT+CREG?\r\n",10);  //Network Register 
 while(!receiver_check()){__delay_ms(2000);UART1PutString("AT+CREG?\r\n",10);}
 UART1PutString("AT+CBC\r\n",9);  //Network Register 
 while(!receiver_check()){__delay_ms(2000);UART1PutString("AT+CBC\r\n",9);}
 UART1PutString("AT+CSQ\r\n",8);  //Signal Quality Report
 while(!receiver_check()){__delay_ms(2000);UART1PutString("AT+CSQ\r\n",8);}
 UART1PutString("AT+CGATT?\r\n",11);  //Attach GPRS
 while(!receiver_check()){__delay_ms(2000);UART1PutString("AT+CGATT?\r\n",11);}
 UART1PutString("AT+CIPCSGP=1,\"bsnllive\",\"\",\"\"\r\n",31);  //Set GPRS Connection mode
 while(!receiver_check()){__delay_ms(2000);UART1PutString("AT+CIPCSGP=1,\"bsnllive\",\"\",\"\"\r\n",31);} 
 UART1PutString("AT+CSTT=\"bsnllive\",\"\",\"\"\r\n",26);  //Set GPRS Connection mode
 while(!receiver_check()){__delay_ms(2000);UART1PutString("AT+CSTT=\"bsnllive\",\"\",\"\"\r\n",26);}
 
}



void MSG_check()
{
     if(INT0IF_flag)
    {
        
 
        __delay_ms(1000);
        char i,flag=0,mob[11];
        for(i=0;string[i]!='\0';i++)
       {
            if(string[i]=='R'&&string[i+1]=='I')
            {
                __delay_ms(1000);
               UART1PutString("ATH\n\r",5);__delay_ms(2000);
               memset(string,' ',255);
            }
            if(string[i]=='C'&&string[i+1]=='M'&&string[i+2]=='T'&&string[i+3]=='I')
            {
                 __delay_ms(200);
                  
                 
                   UART1PutString("AT+CMGF=1\n\r",11);__delay_ms(20);
                 clear_Buffer();
                   UART1PutString("AT+CMGR=1\r\n",11);__delay_ms(50);
                for(int a=0;a<255;a++)
                            {
                                string1[a] = string[a];
                                __delay_us(10);
                            }
                
                UART1PutString(string1,255);
                 char temp[10] = {0};
                 for(int a=0;a<255;a++)
                            {                             
                                       sprintf(temp,"%03d:",a);
                                        UART1PutString(temp,4);
                                      UART1PutString(string1[a],1);
                                      UART1PutString("\n\r",2);
                                      __delay_ms(10);  
                               
                            }
                   
                 for(int i=0;i<MAXIMUM_RECEIVE;i++)//74
                 {
//                     sprintf(temp,"%03d:",i);
//                     sprintf(temp+4,string[i],1);
//                     sprintf(temp+5,"\n\r",2);
//                     UART1PutString(temp,7);
//                  } 
                     if(string[i]=='M'&&string[i+1]=='R'&&string[i+2]=='+')
                     {
                          
                        EEADR=11;
                        RD = 1;
                        while(TX2IF==0);
                        if(EEDATA==0xFF||EEDATA==0)
                        {
                            EEPROM_PutChar(15,1); 
                            flag=0;
                            for(int i=33;string[i]!='"';i++)
                                {
                                  EEPROM_PutChar(flag,string[i]);
                                  flag++;
                                }
                            clear_Buffer();
                              UART1PutString("AT+CMGDA=\"DEL ALL\"\r\n",22);
                            __delay_ms(2000);

                              UART1PutString("AT+CMGS=\"",11);
                            for(int a=0;a<13;a++)
                                {
                                    EEADR=a;
                                    RD = 1;
                                    while(TX2IF==0);
                                    TXREG2   = EEDATA;
                                    __delay_ms(20);
                                }
                              UART1PutString("\"\r\n",3);
                            __delay_ms(1000);
                              UART1PutString("ARC-LIGHT",9);
                            UART2_PutChar('\n');
                             
                           
                              UART1PutString("Master No:",10);
                            for(int a=0;a<13;a++)
                                {
                                    EEADR=a;
                                    RD = 1;
                                    while(TX2IF==0);
                                    TXREG2   = EEDATA;
                                    
                                }
                              UART1PutString(" add Successfully\n",18);
                              UART1PutString("IMEI ID:",8);
                            for(int a=0;a<15;a++)
                            UART2_PutChar(IMEI[a]);
                              UART2_PutChar('\n');
                              UART1PutString("SIM:",4);
                            for(int a=0;SIM_Name[a]!='\0';a++)
                           UART2_PutChar(SIM_Name[a]);
                            UART2_PutChar('\n');
                             __delay_ms(500);
                            UART2_PutChar(0x1A);
                            UART2_PutChar('\n');
                            __delay_ms(2000);
                            clear_Buffer();
                         }
                        
                     }
                    
                 }UART2_PutChar('\n');
                
           for(int b=0;b<13;b++)
                            {
                                EEADR=b;
                                RD = 1;
                                while(TX2IF==0);
                                MOB_No[b]= EEDATA;
                                __delay_ms(20);
                            }
                 flag=0;   
                if(MOB_No[flag+3]==string[36]&&MOB_No[flag+4]==string[37]&&MOB_No[flag+5]==string[38]&&MOB_No[flag+6]==string[39]&&MOB_No[flag+7]==string[40]&&MOB_No[flag+8]==string[41]&&MOB_No[flag+9]==string[42]&&MOB_No[flag+10]==string[43]&&MOB_No[flag+11]==string[44]&&MOB_No[flag+12]==string[45])
                {
                for(int i=0;string[i]!='\0';i++)//74
                     {
                         if(string[i]=='?')
                         {
                            clear_Buffer();
                              UART1PutString("AT+CMGDA=\"DEL ALL\"\r\n",22);
                            __delay_ms(2000);

                              UART1PutString("AT+CMGS=\"",9);
                            for(int b=0;b<13;b++)
                                {
                                    EEADR=b;
                                    RD = 1;
                                    while(TX2IF==0);
                                    TXREG2   = EEDATA;
                                    __delay_ms(20);
                                }
                              UART1PutString("\"\r\n",3);
                            __delay_ms(100);
                              UART1PutString("ARC-LIGHT",9);
                            
                            UART2_PutChar('\n');
                              UART1PutString("Text a States\n",14);
                              UART1PutString("Light 1 :",10);
                            if(LATA0)  UART1PutString("ON",2);
                            if(!LATA0)  UART1PutString("OFF",3);
                            UART2_PutChar('\n');
                            
                              UART1PutString("IMEI ID:",8);
                            for(int a=0;a<15;a++)
                            UART2_PutChar(IMEI[a]);UART2_PutChar('\n');
                              UART1PutString("SIM:",4);
                            for(int b=0;SIM_Name[b]!='\0';b++)
                            {
                                
                                UART2_PutChar(SIM_Name[b]);
                            }
                            
                            UART2_PutChar('\n');
                             __delay_ms(200);
                            UART2_PutChar(0x1A);
                            __delay_ms(2000);
                            clear_Buffer();
                         }
                         if(string[i]=='D'&&string[i+1]=='e'&&string[i+2]=='l'&&string[i+3]=='a'&&string[i+4]=='y')
                         {
                             EEPROM_PutChar(16,string[i+6]-'0');
                             EEPROM_PutChar(17,string[i+7]-'0');
                             EEPROM_PutChar(18,string[i+8]-'0');
                              EEPROM_PutChar(19,1);
                             clear_Buffer();
                             EEADR=16;
                             RD = 1;
                             Server_TMR += EEDATA*100;
                             EEADR=17;
                             RD = 1;
                             Server_TMR += EEDATA*10;
                             EEADR=18;
                             RD = 1;
                             Server_TMR += EEDATA ;
                                  UART1PutString("AT+CMGDA=\"DEL ALL\"\r\n",20);
                                __delay_ms(2000);
                                  UART1PutString("AT+CMGS=\"",9);
                                for(int b=0;b<13;b++)
                                    {
                                        EEADR=b;
                                        RD = 1;
                                        while(TX2IF==0);
                                        TXREG2   = EEDATA;
                                        
                                    }
                                  UART1PutString("\"\r\n",3);
                                __delay_ms(2000);
                                  UART1PutString("ARC-LIGHT",9);
                                
                                UART2_PutChar('\n');
                                  UART1PutString(" Server Delay Time Set :\r\n",30);
                                
                                for(int b=16;b<19;b++)
                                    {
                                        EEADR=b;
                                        RD = 1;
                                        while(TX2IF==0);
                                        TXREG2   = EEDATA+'0';                                       
                                    }
                                UART2_PutChar('\n');
                                  UART1PutString("IMEI ID:",8);
                            for(int a=0;a<15;a++)
                            UART2_PutChar(IMEI[a]);
                                UART2_PutChar('\n');
                                  UART1PutString("SIM:",4);
                                for(int b=0;SIM_Name[b]!='\0';b++)
                                {
                                    __delay_ms(10);
                                    UART2_PutChar(SIM_Name[b]);
                                }
                                UART2_PutChar('\n');
                                 __delay_ms(200);
                                UART2_PutChar(0x1A);
                                UART2_PutChar('\n');
                                 __delay_ms(2000);
                         }
                          if(string[i]=='O'&&string[i+1]=='N'&&string[i+2]=='1')
                         {
                             
                             clear_Buffer();
                           //  RELAY1=1;
                                  UART1PutString("AT+CMGDA=\"DEL ALL\"\r\n",20);
                                __delay_ms(2000);
                                  UART1PutString("AT+CMGS=\"",9);
                                for(int b=0;b<13;b++)
                                    {
                                        EEADR=b;
                                        RD = 1;
                                        while(TX2IF==0);
                                        TXREG2   = EEDATA;
                                        
                                    }
                                  UART1PutString("\"\r\n",3);
                                __delay_ms(2000);
                                  UART1PutString("ARC-LIGHT",9);
                                UART2_PutChar('\n');
                                  UART1PutString(" LIGHT1 'ON'\r\n",14);
                                
                                  UART1PutString("IMEI ID:",8);
                                for(int b=0;b<15;b++)
                                UART2_PutChar(IMEI[b]);
                                  UART1PutString("SIM:",4);
                                for(int b=0;SIM_Name[b]!='\0';b++)
                                {
                                    
                                    UART2_PutChar(SIM_Name[b]);
                                }
                                UART2_PutChar('\n');
                                 __delay_ms(200);
                                UART2_PutChar(0x1A);
                                UART2_PutChar('\n');
                                 __delay_ms(2000);
                         }
                         
                          if(string[i]=='O'&&string[i+1]=='F'&&string[i+2]=='F'&&string[i+3]=='1')
                         {
                             
                             clear_Buffer();
    //                         RELAY1=0;
                                  UART1PutString("AT+CMGDA=\"DEL ALL\"\r\n",20);
                                __delay_ms(2000);
                                  UART1PutString("AT+CMGS=\"",9);
                                for(int b=0;b<13;b++)
                                    {
                                        EEADR=b;
                                        RD = 1;
                                        while(TX2IF==0);
                                        TXREG2   = EEDATA;
                                        
                                    }
                                  UART1PutString("\"\r\n",3);
                                __delay_ms(2000);
                                  UART1PutString("ARC-LIGHT",9);
                                UART2_PutChar('\n');
                                  UART1PutString(" LIGHT1 'OFF'\r\n",13);
                                
                                  UART1PutString("IMEI ID:",8);
                                for(int b=0;b<15;b++)
                                UART2_PutChar(IMEI[b]);
                                  UART1PutString("SIM:",4);
                                for(int b=0;SIM_Name[b]!='\0';b++)
                                {
                                    
                                    UART2_PutChar(SIM_Name[b]);
                                }
                                UART2_PutChar('\n');
                                 __delay_ms(200);
                                UART2_PutChar(0x1A);
                                UART2_PutChar('\n');
                                 __delay_ms(2000);
                         }
                          
                         if(string[i]=='M'&&string[i+1]=='R'&&string[i+2]=='-')
                            {
                               EEADR=11;
                               RD = 1;
                               while(TX2IF==0);
                               
                                   EEPROM_PutChar(15,0); 
                                   flag=0;
                                   for(int a=0;a<13;a++)
                                       {
                                         EEPROM_PutChar(flag,0);
                                         flag++;
                                       }
                                   clear_Buffer();
                                   
                                   
                                

                            }
                    
                 
                      }
                }
                  UART1PutString("AT+CMGDA=\"DEL ALL\"\r\n",20);//Delete All SMS
                __delay_ms(2000);
            }
        }
     INT0IF_flag=0;
    }

}




extern void GPRS_Connect();




