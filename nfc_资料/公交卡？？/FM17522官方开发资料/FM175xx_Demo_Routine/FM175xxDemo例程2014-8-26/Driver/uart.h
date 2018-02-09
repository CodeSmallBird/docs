#ifndef UART_H
#define UART_H
#include "Stdint.h"

//���ڷ��ͺͽ�������֡��ʽ����
#define    Frame_End      0x03
#define    Maxlength      100

//֡��ʽ����
#define    DataLength     0                                           //����֡�ĳ���
#define    Data            1                                           //����֡������ʼ
#define    Cammand         0                                           //������
  

extern volatile   uint8_t          GucRcvNew;                                  /* ���ڽ��յ����ݱ�־        */
extern uint8_t     Send_Buff[Maxlength];                                       //���ݷ��ͻ���
extern uint8_t     Recv_Buff[Maxlength];                                       //���ݽ��ջ�����         */
extern uint8_t     GulNum;                                                      /* ���յ����ݸ���           */
extern uint32_t    TotalNum;                                                    /* ���յ������ܸ���           */

void uartInit (void);
void uartSendByte (uint8_t ucDat);
void uartSendStr (uint8_t const *pucStr, uint32_t ulNum);

#endif
