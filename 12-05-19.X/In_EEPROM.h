/* 
 * File:   In_EEPROM.h
 * Author: Siva
 *
 * Created on May 29, 2019, 7:14 AM
 */

void EEPROM_PutChar(int eepromAddress, char eepromData)
{
    unsigned char gie_Status;
    if(eepromAddress>255)
    {
    EEADRH=eepromAddress>>8;
    }
    while(WR);            // check the WR bit to see if a previous Write operation is in progress
    EEADR=eepromAddress;  // Write the address to EEADR.
    EEDATA=eepromData;    // load the 8-bit data value to be written in the EEDATA register.
    WREN=1;               // Set the WREN bit to enable eeprom operation.
    gie_Status = GIE;     // Copy the current Interrupt state
    GIE = 0;              // Disable the interrupts
    EECON2=0x55;          // Execute the special instruction sequence
    EECON2=0xaa;          // Refer the datasheet for more info
    WR=1;                 // Set the WR bit to trigger the eeprom write operation.
    GIE = gie_Status;     // Restore the interrupts
    WREN=0;               // Disable the EepromWrite
}
/*********************************************************************
 * Function: void EEPROM_WriteString(unsigned int address,unsigned char *data)
 *
 * Overview: Puts a string on the EEPROM.  Unsupported characters will be
*           discarded.May block or throw away characters is EEPROM is not ready
*           or buffer space is not available.
 *
 * PreCondition: already initialized via LCD_Initialize()
 *
 * Input: None
 *
 * Output: None
 *
 ********************************************************************/
void EEPROM_WriteString(int address,char *data)
{
    /*Write Operation for String
    
    unsigned char i;
  for(i=0;data[i]!='\0';i++)
  {
      EEPROM_PutChar(address,*data);
    address++;
        *data++;
}*/
  while(*data!=0)
    {
         EEPROM_PutChar(address,*data);
        address++;
        *data++;
         
    }      
}/*********************************************************************
 * Function: static void LCD_CarriageReturn(void)
 *
 * Overview: Handles a carriage return
 *
 * PreCondition: already initialized via LCD_Initialize()
 *
 * Input: None
 *
 * Output: None
 *
 ********************************************************************/

char EEPROM_ReadChar(unsigned char eepromAddress)
{
    while(RD || WR);           // check the WR&RD bit to see if a RD/WR is in progress
    EEADR=eepromAddress;       // Write the address to EEADR.
    EEPGD = 0;                 //Selecting EEPROM Data Memory
    RD = 1;                    // Set the RD bit to trigger the eeprom read operation.
    return(EEDATA);            // Return the data read form eeprom.

}

