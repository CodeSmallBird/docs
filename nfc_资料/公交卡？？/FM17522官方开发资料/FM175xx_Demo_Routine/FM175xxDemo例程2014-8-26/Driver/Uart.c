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
volatile    uint8_t          GucRcvNew;                                  /* ���ڽ��յ����ݱ�־         */
uint8_t     Send_Buff[Maxlength];                                       //���ݷ��ͻ���
uint8_t     Recv_Buff[Maxlength];                                       //���ݽ��ջ�����
uint8_t     GulNum;                                                      /* ���յ����ݸ���           */


/*********************************************************************************************************
** Function name:        uartInit
** Descriptions:        ���ڳ�ʼ������
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void uartInit (void)
{
    uint16_t usFdiv;
    LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 16);                             /* ??IOCON??               */
    LPC_IOCON->PIO1_6 |= 0x01;                                          /* ��P1.6 1.7����ΪRXD��TXD    */
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

    LPC_UART->IER  = 0x01;                                              /* ʹ�ܴ��ڽ����ж�*/
    
}

/*********************************************************************************************************
** Function name:       uartSendByte
** Descriptions:        ���ڷ����ֽ����ݺ���
** input parameters:    ucDat:   Ҫ���͵�����
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void uartSendByte (uint8_t ucDat)
{
    LPC_UART->THR = ucDat;                                              /*  ��ֵ������buff   */
    while ((LPC_UART->LSR & 0x40) == 0);                                /*  �ȴ����ݷ������        */
}

/*********************************************************************************************************
** Function name:       uartSendStr
** Descriptions:        ���ڷ��Ͷ��ֽ�����
** input parameters:    pucStr:  Ҫ���͵������׵�ַ
**                      ulNum:   Ҫ���͸������ݳ���
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
void uartSendStr (uint8_t const *pucStr, uint32_t ulNum)
{
    uint32_t i;
    for (i = 0; i < ulNum; i++){                                        /* ѭ������n�ֽ�����           */
        uartSendByte (*pucStr++);
    }
		
}


/*********************************************************************************************************
* Function Name:        UART_IRQHandler
* Description:          �����жϳ���
* Input:                ��
* Output:               ��
* Return:               ��
*********************************************************************************************************/
void UART_IRQHandler (void)
{
    GulNum = 0;

    while ((LPC_UART->IIR & 0x01) == 0){                                /*  �Ƿ����жϹ���          */
        switch (LPC_UART->IIR & 0x0E){                                  /*  ??????                */
        
            case 0x04:                                                  /*  ??????                */
                GucRcvNew = 1;                                          /*  ????????            */
                for (GulNum = 0; GulNum < 8; GulNum++){                 /*  ????8???             */
                Recv_Buff[GulNum++] = LPC_UART->RBR;
                }
                break;
            
            case 0x0C:                                                  /*  ���ݽ��ճ�ʱ�ж�                */
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
