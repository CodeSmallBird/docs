#ifndef UART_H
#define UART_H
#include "Stdint.h"

//串口发送和接收数据帧格式定义
#define    Frame_End      0x03
#define    Maxlength      100

//帧格式定义
#define    DataLength     0                                           //数据帧的长度
#define    Data            1                                           //数据帧数据起始
#define    Cammand         0                                           //命令编号
  

extern volatile   uint8_t          GucRcvNew;                                  /* 串口接收到数据标志        */
extern uint8_t     Send_Buff[Maxlength];                                       //数据发送缓冲
extern uint8_t     Recv_Buff[Maxlength];                                       //数据接收缓冲区         */
extern uint8_t     GulNum;                                                      /* 接收到数据个数           */
extern uint32_t    TotalNum;                                                    /* 接收到数据总个数           */

void uartInit (void);
void uartSendByte (uint8_t ucDat);
void uartSendStr (uint8_t const *pucStr, uint32_t ulNum);

#endif
