#pragma once

char Result,CardType;

//LCD display
char LineNumber;

//MS52X FIFO data read
char SpiRxData,FIFOLevel;

//type A parameter
char CollPos,CollByte,CollBit,CollFlag,CollData[5],Correction;
char NVB,TxLastBits;
char CardIDTypeA[10],CardDataTypeA[16];
char CardType;

char CardData[16],CardID[15],SAKData,TagType[2];
char MFCrypto,ProtocolErr;

//type B parameter
char CarfATQB[16],CardCID,UID[16];;

#define SPEED_106 0x00
#define SPEED_212 0x10
#define SPEED_424 0x20
#define SPEED_848 0x30
#define SPEED_1696 0x40
#define SPEED_3392 0x50

#define TIMEINTERVAL 1500

#define _NOCARD            0
#define _S50               1
#define _S70               2
#define _DESFIRE           3
#define _Type_B            4
#define _2nd_generation_ID 5

char DisplayStatus;
#define DISPLAY_CARDTYPE   0
#define DISPLAY_CARDUID    1

#define SUCCESS  0
#define FAIL     1

void delay(int x)
{
  for(int i=0;i<=x;i++);
}
void delay_ms(int x)
{
  for(int i=0;i<=x;i++)
    for(int j=0;j<=1000;j++);
}