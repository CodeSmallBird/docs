/****************************************Copyright (c)****************************************************
**                            Guangzhou ZLGMCU Development Co., LTD
**
**                                 http://www.zlgmcu.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:           RC522.h
** Last modified Date:  2013-03-11
** Last Version:        V1.0
** Descriptions:        
**
**--------------------------------------------------------------------------------------------------------
** Created by:          º«Ô¶Ç¿
** Created date:        2013-03-11
** Version:             V1.00
** Descriptions:        RC522¼òµ¥²âÊÔ³ÌÐò
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#ifndef __RC522_H
#define __RC522_H

#include "..\Utility\Utility.h"

extern void pcd_Init(void);
extern void pcd_RST(void);
extern void pcd_Close(void);
extern void pcd_WriteReg(uint8_t ucReg, uint8_t ucValue);
extern uint8_t pcd_ReadReg(uint8_t ucReg);
extern void pcd_SetBitMask(uint8_t ucReg, uint8_t ucMask);
extern void pcd_ClearBitMask(uint8_t ucReg, uint8_t ucMask);
extern void pcd_TxOn(void);
extern void pcd_TxOff(void);
extern uint8_t pcd_Commun( uint8_t   Command,
                    uint8_t  *pInData,
                    uint8_t   InLenByte,
                    uint8_t  *pOutData, 
                    uint32_t *pOutLenBit);
extern uint8_t pcd_Request(uint8_t req_code, uint8_t *pTagType);
extern void pcd_ISOType(uint8_t ucType);
extern uint8_t pcd_Config(uint8_t ucType);


#endif
