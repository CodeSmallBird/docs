//MICRO DEFIN

#pragma once
#include "msp430x44x.h"

#include "NFC_variables.h"

#define CS_0     P3OUT &= ~(1<<1)
#define CS_1     P3OUT |=  (1<<1)
#define UCLK_0   P3OUT &= ~(1<<2)
#define UCLK_1   P3OUT |=  (1<<2)
#define SIMO_0   P3OUT &= ~(1<<3)
#define SIMO_1   P3OUT |=  (1<<3)

#define SOMI    ((P3IN & BIT4)>>4)


void MS430SPIInit(void)
{
    //SPI 1
    P3DIR  |=  (1<<1);          //P3.1 CS   
    P3DIR  |=  (1<<2);          //P3.2 SCK
    P3DIR  |=  (1<<3);          //P3.3 MOSI
    P3DIR  &= ~(1<<4);          //P3.4 MISO
    
    CS_1;
    UCLK_0;
    SIMO_0;
    
}
//SPI 1
char ReadReg(char Address)
{
    char RegData;
    CS_0;
    UCLK_0;
    delay(5);
    
    Address = (Address << 1)|0x80;
    for(int i=0;i<8;i++)                     //write address
    {
      if((Address&0x80)==0x80)
        SIMO_1;
      else
        SIMO_0;
      UCLK_1;
      Address <<= 1;
      UCLK_0;
    }
    
    for(int i=0;i<8;i++)                    //read data
    {
      UCLK_1;
      RegData <<=1;
      RegData |= SOMI;
      UCLK_0;
    }
    delay(5);
    CS_1;
    
    return RegData;
}

void WriteReg(char Address,char Data)
{
    
    CS_0;
    UCLK_0;
    delay(5);
    
    Address  =  Address << 1 ;
    Address  = (Address&(~0x80));

    for(int i=0;i<8;i++)                     //write address
    {
      if((Address&0x80)==0x80)
        SIMO_1;
      else
        SIMO_0;
      UCLK_1;
      Address <<= 1;
      UCLK_0;
    }
    
    for(int i=0;i<8;i++)                     //write data
    {
      if((Data&0x80)==0x80)
        SIMO_1;
      else
        SIMO_0;
      UCLK_1;
      Data <<= 1;
      UCLK_0;
    }
    
    delay(5);
    CS_1;
    
}