/****************************************Copyright (c)****************************************************
**                            Guangzhou ZLGMCU Development Co., LTD
**
**                                 http://www.zlgmcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           RC522.c
** Last modified Date:  2013-03-11
** Last Version:        V1.0
** Descriptions:        
**
**--------------------------------------------------------------------------------------------------------
** Created by:          韩远强
** Created date:        2013-03-11
** Version:             V1.00
** Descriptions:        RC522简单测试程序
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "RC522.h"
#include "..\Chip\LPC11XX_SPI.h"
#include "..\Chip\M0_PACK_V101_IO.h"
#include "..\Utility\Utility.h"
#include "RC522Reg.h"


#define MI_ERR 0xFF
#define MI_OK  0x00
#define MI_NOTAGERR 0xEE
#define MAXRLEN 64

/*********************************************************************************************************
** 函数名称：pcd_Init
** 函数描述：RC522初始化函数
** 输入参数：无
** 返回值  ：无
*********************************************************************************************************/
void pcd_Init(void)
{
    spi_Init();
    CD_CfgTPD();                                                        /* 配置复位管脚                 */
}

/*********************************************************************************************************
** 函数名称：pcd_RST
** 函数描述：RC522复位函数
** 输入参数：无
** 返回值  ：无
*********************************************************************************************************/
void pcd_RST(void)
{
    CD_SetTPD();
    Delay100us(3);
    CD_ClrTPD();                                                        /* 复位管脚拉低                 */
    Delay100us(5);
    CD_SetTPD();                                                        /* 再拉高                       */
    Delay100us(10);
}

/*********************************************************************************************************
** 函数名称：pcd_Close
** 函数描述：RC522关闭数据
** 输入参数：无
** 返回值  ：无
*********************************************************************************************************/
void pcd_Close(void)
{
    CD_ClrTPD();                                                        /* 硬件方式关闭                 */
}

/*********************************************************************************************************
** 函数名称：pcd_WriteReg
** 函数描述：RC522写寄存器函数
** 输入参数：uint8_t ucReg：寄存器地址
**           uint8_t ucValue：寄存器值
** 返回值  ：无
*********************************************************************************************************/
void pcd_WriteReg(uint8_t ucReg, uint8_t ucValue)
{
    spi_SetReg(ucReg, ucValue);
}

/*********************************************************************************************************
** 函数名称：pcd_ReadReg
** 函数描述：RC522读寄存器函数
** 输入参数：uint8_t ucReg：将要读取的寄存器地址
** 返回值  ：寄存器的值
*********************************************************************************************************/
uint8_t pcd_ReadReg(uint8_t ucReg)
{
    return spi_GetReg(ucReg);
}

/*********************************************************************************************************
** 函数名称：pcd_SetBitMask
** 函数描述：RC522置位寄存器相应位
** 输入参数：uint8_t ucReg：将要置位的寄存器地址
**           uint8_t ucMask：置位位为1
** 返回值  ：寄存器的值
*********************************************************************************************************/
void pcd_SetBitMask(uint8_t ucReg, uint8_t ucMask)
{
    uint8_t ucTemp = 0x00;

    ucTemp = pcd_ReadReg(ucReg);
    pcd_WriteReg(ucReg, ucTemp | ucMask); 
}

/*********************************************************************************************************
** 函数名称：pcd_ClearBitMask
** 函数描述：RC522清零寄存器相应位
** 输入参数：uint8_t ucReg：将要置位的寄存器地址
**           uint8_t ucMask：清零位为1
** 返回值  ：寄存器的值
*********************************************************************************************************/
void pcd_ClearBitMask(uint8_t ucReg, uint8_t ucMask)
{
    uint8_t ucTemp = 0x00;

    ucTemp = pcd_ReadReg(ucReg);
    pcd_WriteReg(ucReg, ucTemp & (~ucMask));
}

/*********************************************************************************************************
** 函数名称：pcd_TxOn
** 函数描述：RC522天线驱动开启
** 输入参数：无
** 返回值  ：无
*********************************************************************************************************/
void pcd_TxOn(void)
{
    uint8_t i;

    i = pcd_ReadReg(JREG_TXCONTROL);
    if (!(i & 0x03)) {
        pcd_SetBitMask(JREG_TXCONTROL, 0x03);
    }
}

/*********************************************************************************************************
** 函数名称：pcd_TxOff
** 函数描述：RC522天线驱动关闭
** 输入参数：无
** 返回值  ：无
*********************************************************************************************************/
void pcd_TxOff(void)
{
    pcd_ClearBitMask(JREG_TXCONTROL, 0x03);
}

void Pcd_ISOType(uint8_t ucType)
{
    
}

/*********************************************************************************************************
** 函数名称：pcd_Commun
** 函数描述：读写芯片与卡片通信函数
** 输入参数：uint8_t Command：RC522命令字
**           uint8_t *pInData：通过RC522发送到卡片的数据
**           uint8_t InLenByte：发送数据的字节长度
**           uint8_t *pOutData：接收到的卡片返回数据
**           uint8_t *pOutLenBit：接收数据的位长度
** 返回值  ：成功返回MI_OK
*********************************************************************************************************/
uint8_t pcd_Commun( uint8_t   Command,
                    uint8_t  *pInData,
                    uint8_t   InLenByte,
                    uint8_t  *pOutData, 
                    uint32_t *pOutLenBit)
{
    uint8_t status  = MI_ERR;
    uint8_t irqEn   = 0x00;
    uint8_t waitFor = 0x00;
    uint8_t lastBits;
    uint8_t n;
    uint32_t i;
    switch (Command) {

    case JCMD_AUTHENT:                                                  /* Mifare认证                   */
        irqEn   = 0x12;
        waitFor = 0x10;
    break;
    case JCMD_TRANSCEIVE:                                               /* 发送FIFO中的数据到天线，传输后激活接收电路*/
        irqEn   = 0x77;
        waitFor = 0x30;
    break;
    default:
    break;
    }
   
    pcd_WriteReg(JREG_COMMIEN, irqEn | 0x80);
    pcd_ClearBitMask(JREG_COMMIRQ, 0x80);
    pcd_WriteReg(JREG_COMMAND, JCMD_IDLE);
    pcd_SetBitMask(JREG_FIFOLEVEL, 0x80);
    
    for (i=0; i < InLenByte; i++) {
        pcd_WriteReg(JREG_FIFODATA, pInData[i]);
    }
    pcd_WriteReg(JREG_COMMAND, Command);

    if (Command == JCMD_TRANSCEIVE) {
        pcd_SetBitMask(JREG_BITFRAMING, 0x80);
    }

    i = 6000;                                                            /* 根据时钟频率调整，操作M1卡最大等待时间25ms*/

    do {
        n = pcd_ReadReg(JREG_COMMIRQ);
        i--;
    } while ((i != 0) && !(n & 0x01) && !(n & waitFor));

    pcd_ClearBitMask(JREG_BITFRAMING, 0x80);

    if (i != 0) {
        if(!(pcd_ReadReg(JREG_ERROR) & 0x1B)) {
            status = MI_OK;
            if (n & irqEn & 0x01) {
                status = MI_NOTAGERR;
            }
            if (Command == JCMD_TRANSCEIVE) {
                n = pcd_ReadReg(JREG_FIFOLEVEL);
                lastBits = pcd_ReadReg(JREG_CONTROL) & 0x07;
                if (lastBits) {
                    *pOutLenBit = (n - 1) * 8 + lastBits;
                } else {
                    *pOutLenBit = n * 8;
                }
                if (n == 0) {
                    n = 1;
                }
                if (n > MAXRLEN) {
                    n = MAXRLEN;
                }
                for (i = 0; i < n; i++) {
                    pOutData[i] = pcd_ReadReg(JREG_FIFODATA);
                }
            }
        } else {
            status = MI_ERR;
        }
    }

   pcd_SetBitMask(JREG_CONTROL, 0x80);                                  /* stop timer now               */
   pcd_WriteReg(JREG_COMMAND, JCMD_IDLE);

   return status;
}

/*********************************************************************************************************
** 函数名称：pcd_Request
** 函数描述：卡片请求（寻卡）
** 输入参数：uint8_t req_code：寻卡方式
**                  0x52 = 寻感应区内所有符合14443A标准的卡
**                  0x26 = 寻未进入休眠状态的卡
** 输出参数：uint8_t 8pTagType：卡片类型代码
**                  0x4400 = Mifare_UltraLight
**                  0x0400 = Mifare_One(S50)
**                  0x0200 = Mifare_One(S70)
**                  0x0800 = Mifare_Pro(X)
**                  0x4403 = Mifare_DESFire
** 返回值  ：成功返回MI_OK
*********************************************************************************************************/
uint8_t pcd_Request(uint8_t req_code, uint8_t *pTagType)
{
    uint8_t  status;
    uint32_t unLen;
    uint8_t  ucComMF522Buf[MAXRLEN];
    
    pcd_ClearBitMask(JREG_STATUS2, 0x08);
    pcd_WriteReg(JREG_BITFRAMING, 0x07);
    pcd_SetBitMask(JREG_TXCONTROL, 0x03);
    
    ucComMF522Buf[0] = req_code;
    
    status = pcd_Commun(JCMD_TRANSCEIVE, ucComMF522Buf, 1, ucComMF522Buf, &unLen);
    
    if ((status == MI_OK) && (unLen == 0x10)) {
        *pTagType       = ucComMF522Buf[0];
        *(pTagType + 1) = ucComMF522Buf[1];
    } else {
        status = MI_ERR;
    }

    return status;
}

/*********************************************************************************************************
** 函数名称：pcd_ISOType
** 函数描述：修改PCD的模式
** 输入参数：uint8_t ucType：模式，RC522只支持A型卡，参数在这里无意义，可以为任意值
** 返回值  ：无
*********************************************************************************************************/
void pcd_ISOType(uint8_t ucType)
{
    // 使用RC522
    ucType = ucType;                                // 只有TypeA模式
    pcd_WriteReg(JREG_TXASK, 0x40);                 // 强制100%ASK
    pcd_WriteReg(JREG_CONTROL, 0x10);               // 主模式
    pcd_WriteReg(JREG_TXMODE, 0x00);                // TypeA baud 106kbps
    pcd_WriteReg(JREG_RXMODE, 0x08);                // TypeA baud 106kbps

    pcd_WriteReg(JREG_DEMOD, 0x4D);
    pcd_WriteReg(JREG_CWGSP, 0x3F);                 // 发送最大射频场
    pcd_WriteReg(JREG_RXTHRESHOLD, 0x84);           // MinLevel = 8; CollLevel = 4
    pcd_WriteReg(JREG_RFCFG, 0x48);                 // RxGain = 6
}

/*********************************************************************************************************
** 函数名称：pcd_Config
** 函数描述：PCD配置
** 输入参数：uint8_t ucType：模式，RC522只支持A型卡，参数在这里无意义，可以为任意值
** 返回值  ：无
*********************************************************************************************************/
uint8_t pcd_Config(uint8_t ucType)
{
    volatile uint8_t ucRegVal;

    pcd_RST();                                                      /* 硬件复位                         */

    pcd_ISOType(ucType);

    pcd_WriteReg(JREG_GSN, 0xF0 | 0x04);            // CWGsN = 0xF; ModGsN = 0x4
    ucRegVal = pcd_ReadReg(JREG_GSN);
    if(ucRegVal != 0xF4)                        // 验证接口正确
        return MI_ERR;

    // 预分频器产生302us的时钟
    // 发送结束后定时器自动启动，接收到第1位数据后自动停止
    pcd_WriteReg(JREG_TPRESCALER, 0x7FF & 0xFF);
    pcd_WriteReg(JREG_TMODE, JBIT_TAUTO | ((0x7FF >> 8) & JMASK_TPRESCALER_HI));                        

//    SetTimeOut(RIC_DELAY5MS);                    // 产生5ms的定时
    pcd_WriteReg(JREG_TRELOADLO, ((uint8_t)(17 & 0xFF)));
    pcd_WriteReg(JREG_TRELOADHI, ((uint8_t)((17 >> 8) & 0xFF)));
    
    // 激活RF场
//    pcd_SetBitMask(JREG_TXCONTROL, JBIT_TX2RFEN | JBIT_TX1RFEN);

    pcd_SetBitMask(JREG_TXCONTROL, JBIT_TX1RFEN);
    // 启动定时器
    pcd_SetBitMask(JREG_CONTROL, JBIT_TSTARTNOW);
    // 等待RF场稳定
    do {
        ucRegVal = pcd_ReadReg(JREG_COMMIRQ);
    }while(!(ucRegVal & JBIT_TIMERI));

    pcd_WriteReg(JREG_COMMIRQ, JBIT_TIMERI);        // 清除定时器中断标志
    pcd_WriteReg(JREG_COMMAND, JCMD_IDLE);
    pcd_WriteReg(JREG_ANALOGTEST, 0xCD);
    
    pcd_WriteReg(JREG_TXSEL, 0x17);
    
    pcd_WriteReg(JREG_FIFOLEVEL, JBIT_FLUSHBUFFER);    // 清空FIFO
    pcd_WriteReg(JREG_WATERLEVEL, 16);    // 使用中断模式，需要将警戒线修改
    pcd_WriteReg(JREG_COMMIRQ, 0x7F);                // 清除所有的中断标志
    pcd_WriteReg(JREG_DIVIRQ, 0x7F);

//    g_ucTxConMask = 0x03;

    return MI_OK;
}

