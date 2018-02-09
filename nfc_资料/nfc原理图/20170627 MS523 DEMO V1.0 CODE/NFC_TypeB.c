
#include "msp430x44x.h"
#include "NFC_variables.h"

#include "NFC_InitLCD1602.h"

void CardTyoeBInit(void);
char REQB(void);
void ATTRIB(void);
void read_2nd_generation_ID_card(void);

char CardTypeB(void)
{
  
  CardTyoeBInit();
  //    WriteReg(0x26,0x78);          //ASK

  for(int i=0;i<16;i++)
  {
    CarfATQB[i] = 0;
    UID[i]      = 0;
  }   
  if(REQB())
  {
   WriteReg(0x14,0x80);
   return FAIL;
  }
  if(CardType == _2nd_generation_ID)
  {
    read_2nd_generation_ID_card();  
  }
  else if(CardType == _Type_B)
  {
    LCDWrite(LCD_COMMAND_REG,(0x80+0x40));
    LCDPrintString("UID: ");
    for(int i=0;i<4;i++)
    {
      LCDPrintData(CarfATQB[i+1]);
    }
    DisplayStatus = DISPLAY_CARDUID;
  }

    return SUCCESS;
/**************************END***************************/
}

void CardTyoeBInit(void)
{
    WriteReg(0x14,0x83);
    WriteReg(0x15,0x00);          /* forces a 14% ASK modulation      */
//    WriteReg(0x29,0x0e);        /* 20170627    */
//    WriteReg(0x28,0x3F);        /* MS52X SPI   */
    
    WriteReg(0x29,0x06);          /* 2016-06-27   */
    WriteReg(0x28,0x3F);          /* MS523_DEMO_V2.0   */
    
    WriteReg(0x12,0x83);          //
    WriteReg(0x13,0x83);          //
}

/***************************REAB*********************/ 
char REQB(void)
{
    WriteReg(0x09,0x05);          // wirte command
    WriteReg(0x09,0x00);          //
    WriteReg(0x09,0x08);          //
    
    WriteReg(0x01,0x0C);          //trans and receive
    WriteReg(0x0D,0x80);          //start send
    delay(1000);
    
    SpiRxData = ReadReg(0x0a); 
    if(SpiRxData  == 0)
      return FAIL;
    for(int i=0;i<SpiRxData;i++)
    {
      CarfATQB[i] = ReadReg(0x09);
    }
    if((CarfATQB[1] == 0 ) && (CarfATQB[2] == 0 ) && (CarfATQB[3] == 0 ) && (CarfATQB[4] == 0 ))
    {
      LCDWrite(LCD_COMMAND_REG,0x01);  
      LCDWrite(LCD_COMMAND_REG,(0x80+0x00));LineNumber = FIRST_LINE ;
      
      LCDPrintString("CardType:");
      LCDWrite(LCD_COMMAND_REG,(0x80+0x40));LineNumber = SECOND_LINE ;
      
      LCDPrintString("2ndGeneration ID");
      
      CardType      = _2nd_generation_ID;
      DisplayStatus = DISPLAY_CARDTYPE;
    }
    else
    {
      LCDPrintString("Type B");
      CardType = _Type_B;
    }
      
    _NOP();
    return SUCCESS;
}
/**************************ATTRIB***************************/
void ATTRIB(void)
{
    WriteReg(0x12,0x83);          //TX CRC B
    WriteReg(0x13,0x83);          //RX CRC B
    
    WriteReg(0x09,0x1D);          //wirte data
    for(int i=0;i<4;i++)
      WriteReg(0x09,CarfATQB[i+1]);
    WriteReg(0x09,0x00);  
    WriteReg(0x09,0x00);
    WriteReg(0x09,0x01); 
    WriteReg(0x09,0x08); 
    SpiRxData = ReadReg(0x0a);
    
    WriteReg(0x01,0x0C);          //trans and receive
    WriteReg(0x0D,0x80);          //start send
    delay(3000);
    WriteReg(0x0D,0x00);          //stop send
    SpiRxData = ReadReg(0x0a); 
    for(int i=0;i<SpiRxData;i++)
      CardCID   = ReadReg(0x09);
    _NOP();
}

void read_2nd_generation_ID_card(void)
{
    WriteReg(0x12,0x83);          //TX CRC B
    WriteReg(0x13,0x83);          //RX CRC B
    
    WriteReg(0x09,0x1D);          //wirte data
    WriteReg(0x09,0x00);
    WriteReg(0x09,0x00);
    WriteReg(0x09,0x00);
    WriteReg(0x09,0x00);  
    WriteReg(0x09,0x00);
    WriteReg(0x09,0x08); 
    WriteReg(0x09,0x01); 
    WriteReg(0x09,0x08); 
    SpiRxData = ReadReg(0x0a);  
    WriteReg(0x01,0x0C);          //trans and receive
    WriteReg(0x0D,0x80);          //start send
    delay(3000);
    WriteReg(0x0D,0x00);          //stop send
    SpiRxData = ReadReg(0x0a); 
    for(int i=0;i<SpiRxData;i++)
      CardCID   = ReadReg(0x09);
    _NOP();
    
    WriteReg(0x09,0x00);          //wirte data
    WriteReg(0x09,0x36);
    WriteReg(0x09,0x00);
    WriteReg(0x09,0x00);
    WriteReg(0x09,0x08);
    SpiRxData = ReadReg(0x0a);  
    
    WriteReg(0x01,0x0C);          //trans and receive
    WriteReg(0x0D,0x80);          //start send
    delay(3000);
    WriteReg(0x0D,0x00);          //stop send
    SpiRxData = ReadReg(0x0a); 
    for(int i=0;i<SpiRxData;i++)
      UID[i]   = ReadReg(0x09);
}

#pragma vector = TIMERA0_VECTOR
__interrupt void timeAContinue(void)
{
//  CCR0 += TIMEINTERVAL;
  TACTL &= ~MC0; //暂停定时器

  if((CardType == _2nd_generation_ID) && (DisplayStatus == DISPLAY_CARDUID))
  {
    LCDWrite(LCD_COMMAND_REG,0x01);  
    LCDWrite(LCD_COMMAND_REG,(0x80+0x00));LineNumber = FIRST_LINE ;
    
    LCDPrintString("CardType:");
    LCDWrite(LCD_COMMAND_REG,(0x80+0x40));LineNumber = SECOND_LINE ;
    
    LCDPrintString("2ndGeneration ID");
    DisplayStatus = DISPLAY_CARDTYPE;
  }
  else if((CardType == _2nd_generation_ID) && (DisplayStatus == DISPLAY_CARDTYPE))
  {
    LCDWrite(LCD_COMMAND_REG,0x01);  
    LCDWrite(LCD_COMMAND_REG,(0x80+0x00));LineNumber = FIRST_LINE ;
    LCDPrintString("UID: ");
    for(int i=0;i<10;i++)
    {
      LCDDisplay(UID[i]);
    }
    DisplayStatus = DISPLAY_CARDUID;
  }

  TACTL |= MC1; //重新启动
}