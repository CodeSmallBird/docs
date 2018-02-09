#pragma once
#include "msp430x44x.h"

#include "NFC_variables.h"


//LCD_RS        REG SELECT

#define LCD_COMMAND_REG	     0      //SELECT Command REG
#define LCD_DATA_REG         1      //SELECT Data    REG

//所在行
#define FIRST_LINE           1
#define SECOND_LINE          2

#define LCD_CLEAR_SCREEN    LCDWrite(LCD_COMMAND_REG,0x01) //清屏
#define LCD_GO_HOME         LCDWrite(LCD_COMMAND_REG,0x02) //AC=0，光标、画面回HOME位

//工作方式设置
#define LCD_DISPLAY_DOUBLE_LINE   0x38         //两行显示
#define LCD_DISPLAY_SINGLE_LINE   0x30         //单行显示

//设置显示、光标及闪烁开、关
#define LCD_DISPLAY_ON            0x0C         //显示开
#define LCD_DISPLAY_OFF           0x08         //显示关
#define LCD_CURSOR_ON             0x0A         //光标显示
#define LCD_CURSOR_OFF            0x08         //光标不显示
#define LCD_CURSOR_BLINK_ON       0x09         //光标闪烁
#define LCD_CURSOR_BLINK_OFF      0x08         //光标不闪烁

//光标、画面移动，不影响DDRAM
#define LCD_LEFT_MOVE             0x18         //LCD显示左移一位
#define LCD_RIGHT_MOVE            0x1C         //LCD显示右移一位
#define LCD_CURSOR_LEFT_MOVE      0x10         //光标左移一位
#define LCD_CURSOR_RIGHT_MOVE     0x14         //光标右移一位


//输入方式设置
#define LCD_AC_AUTO_INCREMENT     0x06         //数据读、写操作后，AC自动增一
#define LCD_AC_AUTO_DECREASE      0x04         //数据读、写操作后，AC自动减一
#define LCD_MOVE_ENABLE           0x05         //数据读、写操作，画面平移
#define LCD_MOVE_DISENABLE        0x04         //数据读、写操作，画面不动

void DIN(void)
{
  P2DIR  &=  ~(1<<0);          //P2.0 LCD_D0
  P2DIR  &=  ~(1<<1);          //P2.0 LCD_D1
  P2DIR  &=  ~(1<<2);          //P2.0 LCD_D2
  P2DIR  &=  ~(1<<3);          //P2.0 LCD_D3
  P2DIR  &=  ~(1<<4);          //P2.0 LCD_D4
  P2DIR  &=  ~(1<<5);          //P2.0 LCD_D5
  P2DIR  &=  ~(1<<6);          //P2.0 LCD_D6
  P2DIR  &=  ~(1<<7);          //P2.0 LCD_D7
}
void DOUT(void)
{
  //LCD DATA
  P2DIR  |=  (1<<0);          //P2.0 LCD_D0
  P2DIR  |=  (1<<1);          //P2.0 LCD_D1
  P2DIR  |=  (1<<2);          //P2.0 LCD_D2
  P2DIR  |=  (1<<3);          //P2.0 LCD_D3
  P2DIR  |=  (1<<4);          //P2.0 LCD_D4
  P2DIR  |=  (1<<5);          //P2.0 LCD_D5
  P2DIR  |=  (1<<6);          //P2.0 LCD_D6
  P2DIR  |=  (1<<7);          //P2.0 LCD_D7
}
char Busy_Check(void)
{
  char data;
  //LCD DATA

  DIN();
  
  P1OUT  &= 0xDF;
  
  P1OUT |= 0x80;
  P1OUT |= 0x40;
  
  delay(100);
  data = P2IN;
  delay(100);
  
  DOUT();
  
  return data;
}
void LCDWrite(char style,char data)
{
  style  = (style<<5);
  P1OUT &= 0x7F;
  P1OUT |= style;
  P1OUT &= (0xDF|style);
  P1OUT &= 0xBF;
  delay(50);
  P2OUT  = data;
  delay(50);
  P1OUT |= 0x80;
  delay(50);
  P1OUT &= 0x7F;
  delay(1000);
}
void MS430LCDInit(void)
{
    //LCD CONCROL
    P1DIR  |=  (1<<5);          //P1.5 LCD_RS  
    P1DIR  |=  (1<<6);          //P1.6 LCD_R/W
    P1DIR  |=  (1<<7);          //P1.7 LCD_EN
    
    //LCD DATA
    P2DIR  |=  (1<<0);          //P2.0 LCD_D0
    P2DIR  |=  (1<<1);          //P2.0 LCD_D1
    P2DIR  |=  (1<<2);          //P2.0 LCD_D2
    P2DIR  |=  (1<<3);          //P2.0 LCD_D3
    P2DIR  |=  (1<<4);          //P2.0 LCD_D4
    P2DIR  |=  (1<<5);          //P2.0 LCD_D5
    P2DIR  |=  (1<<6);          //P2.0 LCD_D6
    P2DIR  |=  (1<<7);          //P2.0 LCD_D7
  
}

void LCDPrintData(char data)
{
  char DisplayData;
  DisplayData = ((data & 0xF0)>>4);
  if(DisplayData<=0x09)
  {
    LCDWrite(LCD_DATA_REG,0x30|DisplayData);
  }
  else
    LCDWrite(LCD_DATA_REG,0x40|(DisplayData-0x09));
  
  DisplayData = (data & 0x0F);
  
  if(DisplayData<=0x09)
  {
    LCDWrite(LCD_DATA_REG,0x30|DisplayData);
  }
  else
    LCDWrite(LCD_DATA_REG,0x40|(DisplayData-0x09));
}
void LCDPrintString(char *string)
{
  while(*string != '\0')
  {
    LCDWrite(LCD_DATA_REG,*string);
    string ++;
  }
}
void LCDDisplayString(char *string)
{
  char AC;
  while(*string != '\0')
  {
    AC = Busy_Check();
    if((AC >= 0x10)&&(LineNumber == FIRST_LINE))
    {
      LCDWrite(LCD_COMMAND_REG,(0x80+0x40));
      LineNumber = SECOND_LINE ;
    }
    else if((AC >= 0x50)&&(LineNumber == SECOND_LINE))
    {
      LCDWrite(LCD_COMMAND_REG,(0x80+0x00));
      LineNumber = FIRST_LINE ;
    }
    LCDWrite(LCD_DATA_REG,*string);
    string ++;
  }
}
void LCDDisplay(char data)
{
  char AC;
  LCDPrintData(data);
  AC = Busy_Check();
  if((AC >= 0x10)&&(LineNumber == FIRST_LINE))
  {
    LCDWrite(LCD_COMMAND_REG,(0x80+0x40));
    LineNumber = SECOND_LINE ;
  }
  else if((AC >= 0x50)&&(LineNumber == SECOND_LINE))
  {
    LCDWrite(LCD_COMMAND_REG,(0x80+0x00));
    LineNumber = FIRST_LINE ;
  }
}

void LCDInit(void) //LCD 初始化
{
  LCDWrite(LCD_COMMAND_REG,0x38);
  LCDWrite(LCD_COMMAND_REG,0x01);
  LCDWrite(LCD_COMMAND_REG,0x06);
  LCDWrite(LCD_COMMAND_REG,0x0C);  
  
  LineNumber = FIRST_LINE;
}