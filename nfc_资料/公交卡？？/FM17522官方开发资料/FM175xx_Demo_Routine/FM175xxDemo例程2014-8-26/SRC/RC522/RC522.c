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
** Created by:          ��Զǿ
** Created date:        2013-03-11
** Version:             V1.00
** Descriptions:        RC522�򵥲��Գ���
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
** �������ƣ�pcd_Init
** ����������RC522��ʼ������
** �����������
** ����ֵ  ����
*********************************************************************************************************/
void pcd_Init(void)
{
    spi_Init();
    CD_CfgTPD();                                                        /* ���ø�λ�ܽ�                 */
}

/*********************************************************************************************************
** �������ƣ�pcd_RST
** ����������RC522��λ����
** �����������
** ����ֵ  ����
*********************************************************************************************************/
void pcd_RST(void)
{
    CD_SetTPD();
    Delay100us(3);
    CD_ClrTPD();                                                        /* ��λ�ܽ�����                 */
    Delay100us(5);
    CD_SetTPD();                                                        /* ������                       */
    Delay100us(10);
}

/*********************************************************************************************************
** �������ƣ�pcd_Close
** ����������RC522�ر�����
** �����������
** ����ֵ  ����
*********************************************************************************************************/
void pcd_Close(void)
{
    CD_ClrTPD();                                                        /* Ӳ����ʽ�ر�                 */
}

/*********************************************************************************************************
** �������ƣ�pcd_WriteReg
** ����������RC522д�Ĵ�������
** ���������uint8_t ucReg���Ĵ�����ַ
**           uint8_t ucValue���Ĵ���ֵ
** ����ֵ  ����
*********************************************************************************************************/
void pcd_WriteReg(uint8_t ucReg, uint8_t ucValue)
{
    spi_SetReg(ucReg, ucValue);
}

/*********************************************************************************************************
** �������ƣ�pcd_ReadReg
** ����������RC522���Ĵ�������
** ���������uint8_t ucReg����Ҫ��ȡ�ļĴ�����ַ
** ����ֵ  ���Ĵ�����ֵ
*********************************************************************************************************/
uint8_t pcd_ReadReg(uint8_t ucReg)
{
    return spi_GetReg(ucReg);
}

/*********************************************************************************************************
** �������ƣ�pcd_SetBitMask
** ����������RC522��λ�Ĵ�����Ӧλ
** ���������uint8_t ucReg����Ҫ��λ�ļĴ�����ַ
**           uint8_t ucMask����λλΪ1
** ����ֵ  ���Ĵ�����ֵ
*********************************************************************************************************/
void pcd_SetBitMask(uint8_t ucReg, uint8_t ucMask)
{
    uint8_t ucTemp = 0x00;

    ucTemp = pcd_ReadReg(ucReg);
    pcd_WriteReg(ucReg, ucTemp | ucMask); 
}

/*********************************************************************************************************
** �������ƣ�pcd_ClearBitMask
** ����������RC522����Ĵ�����Ӧλ
** ���������uint8_t ucReg����Ҫ��λ�ļĴ�����ַ
**           uint8_t ucMask������λΪ1
** ����ֵ  ���Ĵ�����ֵ
*********************************************************************************************************/
void pcd_ClearBitMask(uint8_t ucReg, uint8_t ucMask)
{
    uint8_t ucTemp = 0x00;

    ucTemp = pcd_ReadReg(ucReg);
    pcd_WriteReg(ucReg, ucTemp & (~ucMask));
}

/*********************************************************************************************************
** �������ƣ�pcd_TxOn
** ����������RC522������������
** �����������
** ����ֵ  ����
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
** �������ƣ�pcd_TxOff
** ����������RC522���������ر�
** �����������
** ����ֵ  ����
*********************************************************************************************************/
void pcd_TxOff(void)
{
    pcd_ClearBitMask(JREG_TXCONTROL, 0x03);
}

void Pcd_ISOType(uint8_t ucType)
{
    
}

/*********************************************************************************************************
** �������ƣ�pcd_Commun
** ������������доƬ�뿨Ƭͨ�ź���
** ���������uint8_t Command��RC522������
**           uint8_t *pInData��ͨ��RC522���͵���Ƭ������
**           uint8_t InLenByte���������ݵ��ֽڳ���
**           uint8_t *pOutData�����յ��Ŀ�Ƭ��������
**           uint8_t *pOutLenBit���������ݵ�λ����
** ����ֵ  ���ɹ�����MI_OK
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

    case JCMD_AUTHENT:                                                  /* Mifare��֤                   */
        irqEn   = 0x12;
        waitFor = 0x10;
    break;
    case JCMD_TRANSCEIVE:                                               /* ����FIFO�е����ݵ����ߣ�����󼤻���յ�·*/
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

    i = 6000;                                                            /* ����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms*/

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
** �������ƣ�pcd_Request
** ������������Ƭ����Ѱ����
** ���������uint8_t req_code��Ѱ����ʽ
**                  0x52 = Ѱ��Ӧ�������з���14443A��׼�Ŀ�
**                  0x26 = Ѱδ��������״̬�Ŀ�
** ���������uint8_t 8pTagType����Ƭ���ʹ���
**                  0x4400 = Mifare_UltraLight
**                  0x0400 = Mifare_One(S50)
**                  0x0200 = Mifare_One(S70)
**                  0x0800 = Mifare_Pro(X)
**                  0x4403 = Mifare_DESFire
** ����ֵ  ���ɹ�����MI_OK
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
** �������ƣ�pcd_ISOType
** �����������޸�PCD��ģʽ
** ���������uint8_t ucType��ģʽ��RC522ֻ֧��A�Ϳ������������������壬����Ϊ����ֵ
** ����ֵ  ����
*********************************************************************************************************/
void pcd_ISOType(uint8_t ucType)
{
    // ʹ��RC522
    ucType = ucType;                                // ֻ��TypeAģʽ
    pcd_WriteReg(JREG_TXASK, 0x40);                 // ǿ��100%ASK
    pcd_WriteReg(JREG_CONTROL, 0x10);               // ��ģʽ
    pcd_WriteReg(JREG_TXMODE, 0x00);                // TypeA baud 106kbps
    pcd_WriteReg(JREG_RXMODE, 0x08);                // TypeA baud 106kbps

    pcd_WriteReg(JREG_DEMOD, 0x4D);
    pcd_WriteReg(JREG_CWGSP, 0x3F);                 // ���������Ƶ��
    pcd_WriteReg(JREG_RXTHRESHOLD, 0x84);           // MinLevel = 8; CollLevel = 4
    pcd_WriteReg(JREG_RFCFG, 0x48);                 // RxGain = 6
}

/*********************************************************************************************************
** �������ƣ�pcd_Config
** ����������PCD����
** ���������uint8_t ucType��ģʽ��RC522ֻ֧��A�Ϳ������������������壬����Ϊ����ֵ
** ����ֵ  ����
*********************************************************************************************************/
uint8_t pcd_Config(uint8_t ucType)
{
    volatile uint8_t ucRegVal;

    pcd_RST();                                                      /* Ӳ����λ                         */

    pcd_ISOType(ucType);

    pcd_WriteReg(JREG_GSN, 0xF0 | 0x04);            // CWGsN = 0xF; ModGsN = 0x4
    ucRegVal = pcd_ReadReg(JREG_GSN);
    if(ucRegVal != 0xF4)                        // ��֤�ӿ���ȷ
        return MI_ERR;

    // Ԥ��Ƶ������302us��ʱ��
    // ���ͽ�����ʱ���Զ����������յ���1λ���ݺ��Զ�ֹͣ
    pcd_WriteReg(JREG_TPRESCALER, 0x7FF & 0xFF);
    pcd_WriteReg(JREG_TMODE, JBIT_TAUTO | ((0x7FF >> 8) & JMASK_TPRESCALER_HI));                        

//    SetTimeOut(RIC_DELAY5MS);                    // ����5ms�Ķ�ʱ
    pcd_WriteReg(JREG_TRELOADLO, ((uint8_t)(17 & 0xFF)));
    pcd_WriteReg(JREG_TRELOADHI, ((uint8_t)((17 >> 8) & 0xFF)));
    
    // ����RF��
//    pcd_SetBitMask(JREG_TXCONTROL, JBIT_TX2RFEN | JBIT_TX1RFEN);

    pcd_SetBitMask(JREG_TXCONTROL, JBIT_TX1RFEN);
    // ������ʱ��
    pcd_SetBitMask(JREG_CONTROL, JBIT_TSTARTNOW);
    // �ȴ�RF���ȶ�
    do {
        ucRegVal = pcd_ReadReg(JREG_COMMIRQ);
    }while(!(ucRegVal & JBIT_TIMERI));

    pcd_WriteReg(JREG_COMMIRQ, JBIT_TIMERI);        // �����ʱ���жϱ�־
    pcd_WriteReg(JREG_COMMAND, JCMD_IDLE);
    pcd_WriteReg(JREG_ANALOGTEST, 0xCD);
    
    pcd_WriteReg(JREG_TXSEL, 0x17);
    
    pcd_WriteReg(JREG_FIFOLEVEL, JBIT_FLUSHBUFFER);    // ���FIFO
    pcd_WriteReg(JREG_WATERLEVEL, 16);    // ʹ���ж�ģʽ����Ҫ���������޸�
    pcd_WriteReg(JREG_COMMIRQ, 0x7F);                // ������е��жϱ�־
    pcd_WriteReg(JREG_DIVIRQ, 0x7F);

//    g_ucTxConMask = 0x03;

    return MI_OK;
}

