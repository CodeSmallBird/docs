// ======================================================================================================
//  Copyright (c) 2011 Guangzhou Zhiyuan Electronics Co., Ltd. All rights reserved. 
//  http://www.ecardsys.com
//! @file       fm17xx.h
//! @author     ����ɭ
//! @date       2012/09/06
//! @version    V2.00
//! @brief      FM1735�ײ��������
//!					- V2.00 ����ɭ	2012/09/06	��ԭ�ļ��Ļ�������ֲ��M0ƽ̨���������оƬ��FM17xx��
//!						�ӿڲ����ĺ����Ƶ�CDIo.c�ļ���											
// ======================================================================================================
#ifndef __FM17xx_H
#define __FM17xx_H

#include "MyType.h"

#ifdef __cplusplus
	extern "C" {
#endif
// ======================================================================================================
//ȫ�ֱ�����ȫ�ֺ�������
//;==============================================
//;FM17XX�����������
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

extern unsigned char SPIRead(unsigned char addr);				//SPI������
extern void SPIWrite(unsigned char add,unsigned char wrdata);	//SPIд����
extern unsigned char SPI_Init(void);		//SPI��ʼ������
extern unsigned char FM17XX_Init(void);		//1702��ʼ������
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
//;FM17XX���ƼĴ�������
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

#define ERROR_SUCCESS 				0 	//��ȷ
#define ERROR_NOTAG					1 	//�޿�
#define ERROR_CRC					2 	//��ƬCRC У�����
#define ERROR_EMPTY 					3	//��ֵ�������
#define ERROR_AUTH					4	//��֤���ɹ�
#define ERROR_PARITY					5	//��Ƭ��żУ�����
#define ERROR_CODE					6	//ͨѶ����(BCC У���) 
#define ERROR_SERNR					8	//��Ƭ���кŴ���(anti-collision ����) 
#define ERROR_SELECT					9	//��Ƭ���ݳ����ֽڴ���(SELECT ����) 
#define ERROR_NOTAUTH				10	//��Ƭû��ͨ����֤
#define ERROR_BITCOUNT				11	//�ӿ�Ƭ���յ���λ������
#define ERROR_BYTECOUNT				12	//�ӿ�Ƭ���յ����ֽ������󣨽���������Ч��
#define ERROR_REST					13	//����restore ��������
#define ERROR_TRANS					14	//����transfer ��������
#define ERROR_WRITE					15	//����write ��������
#define ERROR_INCR					16	//����increment ��������
#define ERROR_DECR					17	//����decrement ��������
#define ERROR_READ					18	//����read ��������
#define ERROR_LOADKEY				19	//����LOADKEY ��������
#define ERROR_FRAMING				20	//FM1715 ֡����
#define ERROR_RESQ					21	//����req ��������
#define ERROR_SEL					22	//����sel ��������
#define ERROR_ANTICOLL				23	//����anticoll ��������
#define ERROR_INTIVAL				24	//���ó�ʼ����������
#define ERROR_READVAL				25	//���ø߼�����ֵ��������
#define ERROR_DESELECT				26 
#define ERROR_CMD	 				42	//�������
#define ERROR_INIT					43 	//��ʼ��ʧ��
#define ERROR_UNKNOWN	 			44	//Ϊ�ҵ�Ӳ��
#define ERROR_COLLISION	 			45	//λ��ͻ
#define ERROR_FRAME		 			46	//Frame ����
#define ERROR_ATS_LEN	 			47	//Ats ���ȴ���

// ======================================================================================================
#ifdef __cplusplus
}
#endif

#endif              // __FM17xx_H
