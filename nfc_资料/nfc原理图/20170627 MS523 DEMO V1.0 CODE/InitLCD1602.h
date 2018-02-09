#pragma once
#include "msp430x44x.h"

#include "NFC_variables.h"


//LCD_RS        REG SELECT

#define LCD_COMMAND_REG	     0      //SELECT Command REG
#define LCD_DATA_REG         1      //SELECT Data    REG



#define LCD_CLEAR_SCREEN	  0x01      // ����
#define LCD_GO_HOME               0x02         //AC=0����ꡢ�����HOMEλ


//������ʽ����
#define LCD_DISPLAY_DOUBLE_LINE   0x38         //������ʾ
#define LCD_DISPLAY_SINGLE_LINE   0x30         //������ʾ

//������ʾ����꼰��˸������
#define LCD_DISPLAY_ON            0x0C         //��ʾ��
#define LCD_DISPLAY_OFF           0x08         //��ʾ��
#define LCD_CURSOR_ON             0x0A         //�����ʾ
#define LCD_CURSOR_OFF            0x08         //��겻��ʾ
#define LCD_CURSOR_BLINK_ON       0x09         //�����˸
#define LCD_CURSOR_BLINK_OFF      0x08         //��겻��˸

//��ꡢ�����ƶ�����Ӱ��DDRAM
#define LCD_LEFT_MOVE             0x18         //LCD��ʾ����һλ
#define LCD_RIGHT_MOVE            0x1C         //LCD��ʾ����һλ
#define LCD_CURSOR_LEFT_MOVE      0x10         //�������һλ
#define LCD_CURSOR_RIGHT_MOVE     0x14         //�������һλ


//���뷽ʽ����
#define LCD_AC_AUTO_INCREMENT     0x06         //���ݶ���д������AC�Զ���һ
#define LCD_AC_AUTO_DECREASE      0x04         //���ݶ���д������AC�Զ���һ
#define LCD_MOVE_ENABLE           0x05         //���ݶ���д����������ƽ��
#define LCD_MOVE_DISENABLE        0x04         //���ݶ���д���������治��

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
  delay(5000);
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
void LCDInit(void)
{
  LCDWrite(LCD_COMMAND_REG,0x38);
  LCDWrite(LCD_COMMAND_REG,0x01);
  LCDWrite(LCD_COMMAND_REG,0x06);
  LCDWrite(LCD_COMMAND_REG,0x0C);  
}
void LCDPrint(char data)
{
  
}
