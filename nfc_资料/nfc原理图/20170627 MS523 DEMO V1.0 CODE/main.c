#include "msp430x44x.h"

#include "NFC_InitSPI.h"
#include "NFC_InitLCD1602.h"

#include "NFC_TypeA.c"
#include "NFC_TypeB.c"

#define  KEY_FLAG    (P4IN&0x01)

void main( void )
{
  // Stop watchdog timer to prevent time out reset
  WDTCTL = WDTPW + WDTHOLD;
  
  P4DIR  &=  0xFE;     //KEY
  
  MS430SPIInit();
  MS430LCDInit();
  delay(1000);
  LCDInit();
  
  //Timer A inital
  TACTL  = TASSEL0 +TACLR ; //选择ACLK ，清TAR
  CCTL0  = CCIE;           // CCR0中断允许
  CCR0   = TIMEINTERVAL;
  TACTL |= MC1;           //启动定时器A（连续模式）
  _EINT();                //使能GIE
 
  while(1)
  {
    if(!KEY_FLAG)
    {
      while(!KEY_FLAG);
      LCD_CLEAR_SCREEN;
      
      Result = CardTypeA(SPEED_106); //检测是否有A卡
      
      if(Result == FAIL)           //检测是否有B卡
        Result = CardTypeB(); 
      
      if(Result == FAIL)
      {
        LCDPrintString("NO CARD IN RF...");
        CardType = _NOCARD;
      }
      
    } 
  }
}
