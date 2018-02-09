/****************************************Copyright (c)****************************************************
**                            Guangzhou ZLGMCU Development Co., LTD
**
**                                 http://www.zlgmcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           main.c
** Last modified Date:  2010-02-04
** Last Version:        V1.0
** Descriptions:        The main() function example template
**
*********************************************************************************************************/
#include "LPC11xx.h"                                                    /* LPC11xx?????            */
#include "uart.h"
#include "mytype.h"
/*********************************************************************************************************
   
*********************************************************************************************************/
#define    UART_BPS       19200                                        /* ???????               */

/*********************************************************************************************************
  ????
*********************************************************************************************************/
volatile    uint8_t          GucRcvNew;                                  /* 串口接收到数据标志         */
uint8_t     Send_Buff[Maxlength];                                       //数据发送缓冲
uint8_t     Recv_Buff[Maxlength];                                       //数据接收缓冲区
uint8_t     GulNum;                                                      /* 接收到数据个数           */


/*********************************************************************************************************
** Function name:        uartInit
** Descriptions:        串口初始化程序
** input parameters:    无
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uartInit (void)
{
    uint16_t usFdiv;
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);                             /* ??IOCON??               */
    LPC_IOCON->PIO1_6 |= 0x01;                                          /* 将P1.6 1.7配置为RXD和TXD    */
    LPC_IOCON->PIO1_7 |= 0x01;

    LPC_SYSCON->SYSAHBCLKCTRL |= (1<<12);                               /* ??UART??????         */
    LPC_SYSCON->UARTCLKDIV       = 0x01;                                /* UART????                 */

    LPC_UART->LCR  = 0x83;                                              /* ???????               */
    usFdiv = (SystemFrequency/LPC_SYSCON->UARTCLKDIV/16)/UART_BPS;      /* ?????                   */
    LPC_UART->DLM  = usFdiv / 256;
    LPC_UART->DLL  = usFdiv % 256;
    LPC_UART->LCR  = 0x03;                                              /* ?????                   */
    LPC_UART->FCR  = 0x87;                                              /* ??FIFO,??8??????  */
    
    NVIC_EnableIRQ(UART_IRQn);                                          /* ??UART??,??????   */
    NVIC_SetPriority(UART_IRQn, 1);

    LPC_UART->IER  = 0x01;                                              /* 使能串口接收中断*/
    
}

/*********************************************************************************************************
** Function name:       uartSendByte
** Descriptions:        串口发单字节数据函数
** input parameters:    ucDat:   要发送的数据
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uartSendByte (uint8_t ucDat)
{
    LPC_UART->THR = ucDat;                                              /*  赋值给发送buff   */
    while ((LPC_UART->LSR & 0x40) == 0);                                /*  等待数据发送完毕        */
}

/*********************************************************************************************************
** Function name:       uartSendStr
** Descriptions:        串口发送多字节数据
** input parameters:    pucStr:  要发送的数据首地址
**                      ulNum:   要发送给的数据长度
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void uartSendStr (uint8_t const *pucStr, uint32_t ulNum)
{
    uint32_t i;
    for (i = 0; i < ulNum; i++){                                        /* 循环发送n字节数据           */
        uartSendByte (*pucStr++);
    }
		
}


/*********************************************************************************************************
* Function Name:        UART_IRQHandler
* Description:          串口中断程序
* Input:                无
* Output:               无
* Return:               无
*********************************************************************************************************/
void UART_IRQHandler (void)
{
    GulNum = 0;

    while ((LPC_UART->IIR & 0x01) == 0){                                /*  是否有中断挂起          */
        switch (LPC_UART->IIR & 0x0E){                                  /*  ??????                */
        
            case 0x04:                                                  /*  ??????                */
                GucRcvNew = 1;                                          /*  ????????            */
                for (GulNum = 0; GulNum < 8; GulNum++){                 /*  ????8???             */
                Recv_Buff[GulNum++] = LPC_UART->RBR;
                }
                break;
            
            case 0x0C:                                                  /*  数据接收超时中断                */
                GucRcvNew = 1;
                while ((LPC_UART->LSR & 0x01) == 0x01){                 /*  ??????????        */
                    Recv_Buff[GulNum++] = LPC_UART->RBR;
                }
                break;
                
            default:
                break;
        }
    }
}

/*********************************************************************************************************
  End Of File
*********************************************************************************************************/
