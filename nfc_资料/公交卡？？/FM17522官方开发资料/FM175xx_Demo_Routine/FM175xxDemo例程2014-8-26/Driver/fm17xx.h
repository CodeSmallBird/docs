// ======================================================================================================
//  Copyright (c) 2011 Guangzhou Zhiyuan Electronics Co., Ltd. All rights reserved. 
//  http://www.ecardsys.com
//! @file       fm17xx.h
//! @author     曾本森
//! @date       2012/09/06
//! @version    V2.00
//! @brief      FM1735底层操作函数
//!					- V2.00 曾本森	2012/09/06	在原文件的基础上移植到M0平台，将与耦合芯片（FM17xx）
//!						接口操作的函数移到CDIo.c文件中											
// ======================================================================================================
#ifndef __FM17xx_H
#define __FM17xx_H

#include "MyType.h"

#ifdef __cplusplus
	extern "C" {
#endif
// ======================================================================================================
//全局变量和全局函数申明
//;==============================================
//;FM17XX发送命令代码
//;==============================================
#define		WriteEE			  	0x01
#define		LoadKeyE2		  	0x0B
#define   	Transmit      		0x1A
#define		Transceive			0x1E
#define		Authent1		  	0x0C
#define		Authent2		  	0x14

#define		Req_Std				0x26
#define		Req_Wakeup			0x52
#define		Anticoll			0x93
#define 	Select				0x93

extern unsigned char SPIRead(unsigned char addr);				//SPI读函数
extern void SPIWrite(unsigned char add,unsigned char wrdata);	//SPI写函数
extern unsigned char SPI_Init(void);		//SPI初始化函数
extern unsigned char FM17XX_Init(void);		//1702初始化函数
extern unsigned char write_eeprom(unsigned char lsb,unsigned char msb,unsigned char *buff);
extern unsigned char read_eeprom(unsigned char lsb,unsigned char msb);
extern unsigned char Command_Send(unsigned char count,unsigned char *buff,unsigned char Comm);
extern void SetTime_delay(unsigned long delaytime);
extern unsigned char Read_FIFO(unsigned char *buff);
extern void Write_FIFO(unsigned char count,unsigned char *buff);
extern unsigned char Clear_FIFO(void);
extern void Rf_On(void);
extern void Rf_Off(void);
									   

//;==============================================
//;FM17XX控制寄存器定义
//;==============================================
#define		Page_Reg		        0x00
#define		Command_Reg		        0x01
#define		FIFO_Reg		        0x02
#define		FIFOLength_Reg		    0x04
#define		SecondaryStatus_Reg  	0x05
#define		InterruptEn_Reg		    0x06
#define		InterruptRq_Reg		    0x07
#define		Control_Reg		        0x09
#define		ErrorFlag_Reg		    0x0A
#define		CollPos_Reg				0x0B
#define		BitFraming_Reg	     	0x0F
#define		TxControl_Reg		0x11
#define		CwConductance_Reg	0x12
#define		ModConductance_Reg	0x13
#define		CoderControl_Reg	0x14	
#define		TypeFraming_Reg		0x17
#define		DecoderControl_Reg	0x1a
#define		RxControl2_Reg		0x1E
#define		ChannelRedundancy_Reg	0x22
#define		CRCPresetLSB_Reg	0x23
#define		CRCPresetMSB_Reg	0x24
#define   	RxWait_Reg          0x21
#define		TimerClock_Reg		0x2A
#define		TimerControl_Reg	0x2B
#define		TimerReload_Reg		0x2C
#define		FIFOLevel_Reg		0x29
#define ERROR		1
#define	OK			0
#define RF_TimeOut 0x0ff
#define timeout	0x3ff

#define ERROR_SUCCESS 				0 	//正确
#define ERROR_NOTAG					1 	//无卡
#define ERROR_CRC					2 	//卡片CRC 校验错误
#define ERROR_EMPTY 					3	//数值溢出错误
#define ERROR_AUTH					4	//验证不成功
#define ERROR_PARITY					5	//卡片奇偶校验错误
#define ERROR_CODE					6	//通讯错误(BCC 校验错) 
#define ERROR_SERNR					8	//卡片序列号错误(anti-collision 错误) 
#define ERROR_SELECT					9	//卡片数据长度字节错误(SELECT 错误) 
#define ERROR_NOTAUTH				10	//卡片没有通过验证
#define ERROR_BITCOUNT				11	//从卡片接收到的位数错误
#define ERROR_BYTECOUNT				12	//从卡片接收到的字节数错误（仅读函数有效）
#define ERROR_REST					13	//调用restore 函数出错
#define ERROR_TRANS					14	//调用transfer 函数出错
#define ERROR_WRITE					15	//调用write 函数出错
#define ERROR_INCR					16	//调用increment 函数出错
#define ERROR_DECR					17	//调用decrement 函数出错
#define ERROR_READ					18	//调用read 函数出错
#define ERROR_LOADKEY				19	//调用LOADKEY 函数出错
#define ERROR_FRAMING				20	//FM1715 帧错误
#define ERROR_RESQ					21	//调用req 函数出错
#define ERROR_SEL					22	//调用sel 函数出错
#define ERROR_ANTICOLL				23	//调用anticoll 函数出错
#define ERROR_INTIVAL				24	//调用初始化函数出错
#define ERROR_READVAL				25	//调用高级读块值函数出错
#define ERROR_DESELECT				26 
#define ERROR_CMD	 				42	//命令错误
#define ERROR_INIT					43 	//初始化失败
#define ERROR_UNKNOWN	 			44	//为找到硬件
#define ERROR_COLLISION	 			45	//位冲突
#define ERROR_FRAME		 			46	//Frame 错误
#define ERROR_ATS_LEN	 			47	//Ats 长度错误

// ======================================================================================================
#ifdef __cplusplus
}
#endif

#endif              // __FM17xx_H
