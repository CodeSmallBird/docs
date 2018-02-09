
#include "msp430x44x.h"
#include "NFC_variables.h"

#include "NFC_InitLCD1602.h"

//                0x4400 = Mifare_UltraLight                        /
//                0x0400 = Mifare_One(S50)                          /
//                0x0200 = Mifare_One(S70)                          /
//                0x0800 = Mifare_Pro(X)                            /
//                0x4403 = Mifare_DESFire 
#define OPEN_ANTENNA  WriteReg(0x14,0x83)

char CardTypeARequest(void);
void CardTypeAColl(void);
void CardTypeASelect(void);
void CardTypeARead(void);
void CardTypeAATS(void);
void IDCorrection(void);

char CardTypeA(char speed)
{
  WriteReg(0x18,0x85);
  WriteReg(0x19,0x4D);
  
  WriteReg(0x26,0x79);
  
//  WriteReg(0x23,0x33);
//  WriteReg(0x27,0xFF);
  
  WriteReg(0x28,0x3F);
  WriteReg(0x29,0x03);
//  WriteReg(0x26,0x78);
  WriteReg(0x12,speed);
  WriteReg(0x13,speed);
  WriteReg(0x14,0x83);
  WriteReg(0x15,0x40);          //Type A
  
  if(CardTypeARequest())
  {
    WriteReg(0x14,0x80);
    return FAIL;
  }
  for(int i=0;i<5;i++)
  {
    CardIDTypeA[i] = 0;
  }
  
  WriteReg(0x0a,0x80);
 
  WriteReg(0x09,0x93);          //wirte command
  WriteReg(0x09,0x20);

  WriteReg(0x01,0x0C);          //trans and receive
  WriteReg(0x0D,0x80);          //start send
  delay(1000);
  WriteReg(0x0e,0x20);
  CollFlag  = ReadReg(0x0e);
  
  
  LCDWrite(LCD_COMMAND_REG,(0x80+0x40));
  LCDPrintString("UID: ");
  SpiRxData = ReadReg(0x0a); 
  for(int i=0;i<SpiRxData;i++)
  {
    CardIDTypeA[i] = ReadReg(0x09);
    if(i<4)LCDPrintData(CardIDTypeA[i]);
  }
  WriteReg(0x14,0x80);  
  return SUCCESS;
/*  
  if(!(CollFlag&0x20))
    CardTypeAColl();
  else
  {
    SpiRxData = ReadReg(0x0a); 
    for(int i=0;i<SpiRxData;i++)
      CardIDTypeA[i] = ReadReg(0x09);
  }
*/
//  CardTypeASelect();
//  CardTypeAATS();
//  CardTypeARead();
  
}
char CardTypeARequest(void)         
{
  /*****************REQA************************/
  for(int i=0;i<2;i++)
    TagType[i] = 0;
  WriteReg(0x12,0x00);          //TX CRC
  WriteReg(0x13,0x00);          //RX CRC
  
  WriteReg(0x0a,0x80);
  WriteReg(0x0D,0x07);          //

  WriteReg(0x09,0x26);          //wirte data
  
  SpiRxData = ReadReg(0x0a);
  SpiRxData = ReadReg(0x01);
  WriteReg(0x01,0x0C);          //trans and receive
  WriteReg(0x0D,0x87);          //start send
  delay(1000);
  SpiRxData = ReadReg(0x0a); 
  for(int i=0;i<SpiRxData;i++)
    TagType[i] = ReadReg(0x09);
  
  if((TagType[0] == 0x04) && (TagType[1] == 0x00))
  {
    LCDWrite(LCD_COMMAND_REG,(0x80+0x00));
    LCDPrintString("CardType: S50");
    CardType = _S50;
    
    return SUCCESS;
  }
  else if((TagType[0] == 0x02) && (TagType[1] == 0x00))
  {
    LCDWrite(LCD_COMMAND_REG,(0x80+0x00));
    LCDPrintString("CardType: S70");
    CardType = _S70;
    
    return SUCCESS;
  }
  else if((TagType[0] == 0x44) && (TagType[1] == 0x03))
  {
    LCDWrite(LCD_COMMAND_REG,(0x80+0x00));
    LCDPrintString("CardType:DESFire");
    CardType = _DESFIRE;
    
    return SUCCESS;
  }
  else
    return FAIL;
}
void CardTypeAColl(void)
{
  SpiRxData = ReadReg(0x06); 
  while(!(CollFlag&0x20))
  {
    for(int i=0;i<5;i++)
      CollData[i] = 0;
    
    SpiRxData = ReadReg(0x0a);
    for(int i=0;i<SpiRxData;i++)
      CollData[i] = ReadReg(0x09);
    
    CollPos    = CollFlag&0x1F;
    CollBit    = CollPos&0x07;
    CollByte   = SpiRxData;
    NVB        = CollByte+0x01;
    NVB        = NVB << 4;
    NVB        = NVB + CollBit;
    TxLastBits = CollBit | 0x80;
    
    WriteReg(0x0a,0x80);   
    WriteReg(0x09,0x93);          //wirte data
    WriteReg(0x09,NVB);
    for(int i=0;i<CollByte;i++)
      WriteReg(0x09,CollData[i]);   
    
    WriteReg(0x01,0x0C);          //trans and receive
    WriteReg(0x0D,TxLastBits);          //start send
    delay(3000);
    
    CollFlag = ReadReg(0x0e);
  }
  
  SpiRxData = ReadReg(0x0a); 
  for(int i=0;i<SpiRxData;i++)
    CardIDTypeA[i] = ReadReg(0x09);
  
   WriteReg(0x0a,0x80);
 
  WriteReg(0x09,0x93);          //wirte command
  WriteReg(0x09,0x20);

  WriteReg(0x01,0x0C);          //trans and receive
  WriteReg(0x0D,0x80);          //start send
  delay(1000);
  WriteReg(0x0e,0x20);
  CollFlag  = ReadReg(0x0e);
  
  SpiRxData = ReadReg(0x0a); 
  for(int i=0;i<SpiRxData;i++)
    CardIDTypeA[i] = ReadReg(0x09);
  
//  IDCorrection();

}

void IDCorrection(void)         //·À³åÍ»
{
  char dataID,data;
  char checkflag,checkpos,count_1;
  
  checkpos  = 1<<(8-CollBit);
  for(int i=CollByte;i<5;i++)
  {
    count_1 = 0;
    if(CollBit>1)
      data      = (CardIDTypeA[i-CollByte+1]<<CollBit)|((CardIDTypeA[i-CollByte]>>(9-CollBit)));
    else
      data      =  CardIDTypeA[i-CollByte+1]<<CollBit;

    checkflag = (checkpos & CardIDTypeA[i-CollByte+1])>>(8-CollBit);
    dataID    = data;
    for(int j=0;j<8;j++)
    {
      count_1 += data & 0x01;
      data  >>= 1;
    }
    if(count_1>2)
      count_1 >>= 1;
    if((count_1 & 0x01)==(checkflag))
      dataID  =  dataID | (0x01<<(CollBit-1));
    
    CollData[i] = dataID;
  }
  CollData[CollByte-1] = (CardIDTypeA[0]<<CollBit)|CollData[CollByte-1];
  
  for(int i=0;i<5;i++)
  {
    CardIDTypeA[i] = CollData[i];
  }
}


void CardTypeASelect(void)
{
  /*************************¿¨Ñ¡Ôñ ·¢ËÍÐòÁÐºÅ*******************************/ 
  WriteReg(0x0a,0x80);
  
  WriteReg(0x12,0x80);             //TX CRC
  WriteReg(0x13,0x80);             //TX CRC
  
  WriteReg(0x09,0x93);          //wirte data
  WriteReg(0x09,0x70);
  for(int i=0;i<5;i++)
    WriteReg(0x09,CardIDTypeA[i]);        
  
  WriteReg(0x01,0x0C);          //trans and receive
  WriteReg(0x0D,0x80);          //start send
  delay(1000);
  
  SpiRxData = ReadReg(0x0a); 
  if(SpiRxData == 1)
    SAKData = ReadReg(0x09); 
  
  if(SAKData&0x04)
  {
    WriteReg(0x12,0x00);             //TX CRC
    WriteReg(0x13,0x00);             //TX CRC
  
    WriteReg(0x09,0x95);          //wirte command
    WriteReg(0x09,0x20);

    WriteReg(0x01,0x0C);          //trans and receive
    WriteReg(0x0D,0x80);          //start send
    delay(1000);
    
    SpiRxData = ReadReg(0x0a); 
    for(int i=0;i<SpiRxData;i++)
      CardIDTypeA[5+i] = ReadReg(0x09);
    
    WriteReg(0x0a,0x80);
    WriteReg(0x12,0x80);             //TX CRC
    WriteReg(0x13,0x80);             //TX CRC
    
    WriteReg(0x09,0x95);          //wirte data
    WriteReg(0x09,0x70);
    for(int i=0;i<5;i++)
      WriteReg(0x09,CardID[i+5]);        
    
    WriteReg(0x01,0x0C);          //trans and receive
    WriteReg(0x0D,0x80);          //start send
    delay(1000);

    SpiRxData = ReadReg(0x0a); 
    if(SpiRxData == 1)
      SAKData = ReadReg(0x09); 
    else
      SAKData = 0;
  }   
 
  _NOP();
}

void CardTypeARead(void)
{
  /*************************ÑéÖ¤ÃÜÔ¿*******************************/        
  for(int i=0;i<16;i++)
  {
    CardDataTypeA[i] = 0;
  }
  WriteReg(0x0a,0x80);
//       WriteReg(0x0D,0x07);          //
  WriteReg(0x12,0x80);             //TX CRC
  WriteReg(0x13,0x80);             //TX CRC
  
  WriteReg(0x09,0x60);          //wirte data
  WriteReg(0x09,0x03);
  for(int i=0;i<6;i++)
    WriteReg(0x09,0xff);      
  
  for(int i=0;i<4;i++)
    WriteReg(0x09,CardIDTypeA[i]);
  
  SpiRxData = ReadReg(0x0a);
  SpiRxData = ReadReg(0x01);
  
  WriteReg(0x01,0x0e);          //MFAuthent
  
  delay(3000);
  ProtocolErr = ReadReg(0x06); 
  MFCrypto    = ReadReg(0x08); 
  
  _NOP();
  /******************¶Á¿é******************/       
  for(int i=0;i<16;i++)
  {
    CardDataTypeA[i] = 0;
  }
   WriteReg(0x0a,0x80);
  
  WriteReg(0x12,0x80);             //TX CRC
  WriteReg(0x13,0x80);             //TX CRC
  
  WriteReg(0x09,0x30);          //wirte data
  WriteReg(0x09,0x03);    
  
  SpiRxData = ReadReg(0x0a);
  SpiRxData = ReadReg(0x01);
  WriteReg(0x01,0x0C);          //trans and receive
  WriteReg(0x0D,0x80);          //start send
  delay(3000);
  WriteReg(0x0D,0x00);          //stop send
  SpiRxData = ReadReg(0x0a); 
  for(int i=0;i<SpiRxData;i++)
  {
    CardDataTypeA[i] = ReadReg(0x09);
  }   
  ProtocolErr = ReadReg(0x06); 
  MFCrypto    = ReadReg(0x08); 
  _NOP();
}

void CardTypeAATS(void)
{
  WriteReg(0x0a,0x80);
  
  WriteReg(0x12,0x80);             //TX CRC
  WriteReg(0x13,0x80);             //TX CRC
  
  WriteReg(0x09,0xE0);          //wirte data
  WriteReg(0x09,0x50);    
  
  SpiRxData = ReadReg(0x0a);
  SpiRxData = ReadReg(0x01);
  WriteReg(0x01,0x0C);          //trans and receive
  WriteReg(0x0D,0x80);          //start send
  delay(1000);
  SpiRxData = ReadReg(0x0a); 
  for(int i=0;i<SpiRxData;i++)
  {
    CardDataTypeA[i] = ReadReg(0x09);
  }      
  _NOP();
}





