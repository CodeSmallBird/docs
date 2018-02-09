// ======================================================================================================
//  Copyright (c) 2011 Guangzhou Zhiyuan Electronics Co., Ltd. All rights reserved. 
//  http://www.zlg.cn
//! @file       M0_PACK_V101_IO.h
//! @author     ZengBenSen
//! @date       2012.09.29
//! @version    V1.00
//! @brief      CK10-GM V1.00 I/O ����.��ֲ��M0 PACK V1.01��ͬʱ����ͨѶ�ڶ���
//!                 - V1.02 2012.12.05  ��Զǿ  ��SAM���ӿڶ��岿�֣��޸���JTAG�����ܽŵļĴ��������¾���
//!                                             ���汾lpc11xx.hͷ�ļ���ͬ�������ǽ��µ�V1.10�汾
//!					- V1.01	2012.11.28	��Զǿ	�޸�CD���ֹܽţ���ֲ��M0 PACK V1.01��ͬʱ����ͨѶ�ڶ���
//!					- V1.00	2012.09.29	����ɭ	CK10-GM V1.00 I/O ����
// ======================================================================================================
#ifndef __MO_PACK_V101_IO_H
#define __M0_PACK_V101_IO_H

#include "LPC11xx.h"

#ifdef __cplusplus
	extern "C" {
#endif

// ============ I/O�ڶ��� ===============================================================================
//! ����P3.2ΪLED_GREEN
#define IO_MASK_LED_GREEN	(1 << 2)															//!< LED_GREEN ���ƽ�����
#define LED_GreenCfg()		LPC_GPIO3->DIR |= IO_MASK_LED_GREEN									// ����LED_GREEN ΪGPIO ���
#define LED_GreenOff()		(LPC_GPIO3->MASKED_ACCESS[IO_MASK_LED_GREEN] = IO_MASK_LED_GREEN)	//!< �ر�LED_GREEN
#define LED_GreenOn()		(LPC_GPIO3->MASKED_ACCESS[IO_MASK_LED_GREEN] = ~IO_MASK_LED_GREEN)	//!< ����LED_GREEN		

//! ����P1.11 ΪLED_RED	  
#define IO_MASK_LED_RED		(1 << 11)															
#define LED_RedCfg()		LPC_GPIO1->DIR |= IO_MASK_LED_RED									//!< ����LED_RED ΪGPIO ���
#define LED_RedOff()		(LPC_GPIO1->MASKED_ACCESS[IO_MASK_LED_RED] = IO_MASK_LED_RED)		//!< �ر�LED_RED
#define LED_RedOn()			(LPC_GPIO1->MASKED_ACCESS[IO_MASK_LED_RED] = ~IO_MASK_LED_RED)		//!< ����LED_RED

// ======================================================================================================
// UART��IICͨѶ�ӿڶ���
//! ����P0.3 Ϊ NINT	  
#define IO_MASK_NINT		(1 << 3)															
#define NINT_Cfg()			LPC_IOCON->PIO0_3 = 0xC0; LPC_GPIO0->DIR |= IO_MASK_NINT	        //!< ����NINT ΪGPIO ���,��������
#define NINT_Set()			(LPC_GPIO0->MASKED_ACCESS[IO_MASK_NINT] = IO_MASK_NINT)		        //!< ����NINT
#define NINT_Clr()			(LPC_GPIO0->MASKED_ACCESS[IO_MASK_NINT] = ~IO_MASK_NINT)	        //!< ���NINT

//! ����P0.4 Ϊ IIC��SCL�ܽţ�ͬʱҲ��UART��SBR�ܽ�
#define IO_MASK_SBR         (1 << 4)
#define UART_CfgSBR()       LPC_IOCON->PIO0_4 = 0xD0; LPC_GPIO0->DIR &= ~IO_MASK_SBR            //!< ����SBRΪGPIO���룬����
//#define UART_SetSBR()       (LPC_GPIO0->MASKED_ACCESS[IO_MASK_SBR] = IO_MASK_SBR)               //!< ����SBR
//#define UART_ClrSBR()       (LPC_GPIO0->MASKED_ACCESS[IO_MASK_SBR] = ~IO_MASK_SBR)              //!< ����SBR
#define UART_GetSBR()       (LPC_GPIO0->MASKED_ACCESS[IO_MASK_SBR])                             //!< ��ȡSBR״̬
//#define UART_GetSBR()       (LPC_GPIO0->DATA & IO_MASK_SBR)

//! ����P0.4ΪSCL  
#define IO_MASK_SCL		(1 << 4)												// CD SPIͨ��SCL������
#define CD_CfgSCL()		(LPC_GPIO0->DIR &= ~IO_MASK_SCL)							// ����SCLΪ����
#define CD_GetSCL()		(LPC_GPIO0->MASKED_ACCESS[IO_MASK_SCL])	// ��ȡCD SPIͨ��SCL��״̬

//! ����P0.5ΪSDA  
#define IO_MASK_SDA		(1 << 5)												// CD SPIͨ��SDA������
#define CD_CfgSDA()		(LPC_GPIO0->DIR &= ~IO_MASK_SDA)							// ����SDAΪ����
#define CD_GetSDA()		(LPC_GPIO0->MASKED_ACCESS[IO_MASK_SDA])	// ��ȡCD SPIͨ��SDA��״̬

//! ����P0.5 Ϊ IIC��SDA�ܽţ�ͬʱҲ��UART��DBR�ܽ�
#define IO_MASK_DBR         (1 << 5)
#define UART_CfgDBR()       LPC_IOCON->PIO0_5 = 0xD0; LPC_GPIO0->DIR &= ~IO_MASK_DBR            //!< ����SBRΪGPIO���룬����
//#define UART_SetDBR()       (LPC_GPIO0->MASKED_ACCESS[IO_MASK_DBR] = IO_MASK_DBR)               //!< ����DBR
//#define UART_ClrDBR()       (LPC_GPIO0->MASKED_ACCESS[IO_MASK_DBR] = ~IO_MASK_DBR)              //!< ����DBR
#define UART_GetDBR()       (LPC_GPIO0->MASKED_ACCESS[IO_MASK_DBR])                             //!< ��ȡDBR״̬

//! ����P1.6 Ϊ UART��RXD�ܽţ�ͬʱҲ��IIC��ADR1�ܽ�
#define IO_MASK_ADR1        (1 << 6)
#define IIC_CfgADR1()      LPC_IOCON->PIO1_6 = 0xD0; LPC_GPIO1->DIR &= ~IO_MASK_ADR1            //!< ����SBRΪGPIO���룬����
//#define IIC_SetADR1()      (LPC_GPIO1->MASKED_ACCESS[IO_MASK_ADR1] = IO_MASK_ADR1)              //!< ����ADR1
//#define IIC_ClrADR1()      (LPC_GPIO1->MASKED_ACCESS[IO_MASK_ADR1] = ~IO_MASK_ADR1)             //!< ����ADR1
#define IIC_GetADR1()      (LPC_GPIO1->MASKED_ACCESS[IO_MASK_ADR1])                             //!< ��ȡADR1״̬

//! ����P1.7 Ϊ UART��TXD�ܽţ�ͬʱҲ��IIC��ADR0�ܽ�
#define IO_MASK_ADR0        (1 << 7)
#define IIC_CfgADR0()      LPC_IOCON->PIO1_7 = 0xD0; LPC_GPIO1->DIR &= ~IO_MASK_ADR0            //!< ����SBRΪGPIO���룬����
//#define IIC_SetADR0()      (LPC_GPIO1->MASKED_ACCESS[IO_MASK_ADR0] = IO_MASK_ADR0)              //!< ����ADR0
//#define IIC_ClrADR0()      (LPC_GPIO1->MASKED_ACCESS[IO_MASK_ADR0] = ~IO_MASK_ADR0)             //!< ����ADR0
#define IIC_GetADR0()      (LPC_GPIO1->MASKED_ACCESS[IO_MASK_ADR0])                             //!< ��ȡADR0״̬

//! ����P0.1 Ϊ 485��CON�ܽţ�ͬʱҲ��IIC��SADR�ܽ�
#define IO_MASK_SADR        (1 << 1)
#define IIC_CfgSADR()      LPC_IOCON->PIO0_1 = 0xD0; LPC_GPIO0->DIR &= ~IO_MASK_SADR            //!< ����SBRΪGPIO���룬����
//#define IIC_SetSADR()      (LPC_GPIO0->MASKED_ACCESS[IO_MASK_SADR] = IO_MASK_SADR)              //!< ����SADR
//#define IIC_ClrSADR()      (LPC_GPIO0->MASKED_ACCESS[IO_MASK_SADR] = ~IO_MASK_SADR)             //!< ����SADR
#define IIC_GetSADR()      (LPC_GPIO0->MASKED_ACCESS[IO_MASK_SADR])                             //!< ��ȡSADR״̬

// ======================================================================================================
// CD �ӿڶ���
//! ����P3.5ΪTPD
#define IO_MASK_TPD			(1 << 5)												//!< CD�������TPD ������
#define CD_CfgTPD()			(LPC_GPIO3->DIR |= IO_MASK_TPD)							//!< ����TPD Ϊ���
#define CD_SetTPD()			(LPC_GPIO3->MASKED_ACCESS[IO_MASK_TPD] = IO_MASK_TPD)	//!< TPD ��λ
#define CD_ClrTPD()			(LPC_GPIO3->MASKED_ACCESS[IO_MASK_TPD] = ~IO_MASK_TPD)	//!< TPD ����

//! ����P0.2ΪNSS	  
#define IO_MASK_NSS			(1 << 2)												//!< CD SPI ͨ��NSS ������
#define CD_CfgNSS()			(LPC_GPIO0->DIR |= IO_MASK_NSS)							//!< ����NSS ΪGPIO ���
#define CD_EnNSS()			(LPC_GPIO0->MASKED_ACCESS[IO_MASK_NSS] = ~IO_MASK_NSS)	//!< SPI ͨ��NSS ʹ�ܣ����㣩
#define CD_DisNSS()			(LPC_GPIO0->MASKED_ACCESS[IO_MASK_NSS] = IO_MASK_NSS)	//!< SPI ͨ��NSS ��ֹ����λ��

//! ����P0.3ΪIRQ
#define CD_IRQn				EINT0_IRQn												//!< CD�жϱ��
#define IO_MASK_IRQ			(1 << 3)												//!< CD�ж�IRQ������
//! ����IRQΪ����, �жϽ�ֹ, �͵�ƽ����
#define CD_CfgIRQ()			LPC_GPIO0->DIR	&= ~IO_MASK_IRQ;\
							LPC_GPIO0->IE   &= ~IO_MASK_IRQ;\
							LPC_GPIO0->IS   |=  IO_MASK_IRQ;\
							LPC_GPIO0->IEV	&= ~IO_MASK_IRQ

                                               
#define CD_EnIRQ()			(LPC_GPIO0->IE	|=  IO_MASK_IRQ)	  					//!< ʹ��IRQ
#define CD_DisIRQ()			(LPC_GPIO0->IE	&= ~IO_MASK_IRQ)	  					//!< ����IRQ
#define CD_GetIRQ()			(LPC_GPIO0->MIS	&= ~IO_MASK_IRQ)	  					//!< ��IRQ
#define CD_ClrIRQ()			(LPC_GPIO0->IC	|=  IO_MASK_IRQ)	  					//1< ��IRQ

// ======================================================================================================
// SAM ���ӿڶ���
//! ����P0.11Ϊ SAM IO
#define SLOT0_IO_MASK		(1ul << 11)
//! IO ����ΪGPIO�����������롢��ֹ�жϡ���Ե�жϡ��½��ش���������жϡ�ʹ��EINT0_IRQn�ж�
#define Slot0_CfgIO()		LPC_IOCON->R_PIO0_11	=  0xD1;	\
							LPC_GPIO0->DIR		&= ~SLOT0_IO_MASK;	\
							LPC_GPIO0->IE		&= ~SLOT0_IO_MASK;	\
							LPC_GPIO0->IS		&= ~SLOT0_IO_MASK;	\
							LPC_GPIO0->IEV		&= ~SLOT0_IO_MASK;	\
							LPC_GPIO0->IC		=  0xFFFFFFFF;		\
							NVIC_EnableIRQ(EINT0_IRQn)
		
#define Slot0_CfgIOIn()		(LPC_GPIO0->DIR &= ~SLOT0_IO_MASK)			// ����SLOT0 IO ΪGPIO����
#define Slot0_CfgIOOut()	(LPC_GPIO0->DIR |= SLOT0_IO_MASK)			// ����SLOT0 IO ΪGPIO���
#define Slot0_SetIO()		(LPC_GPIO0->MASKED_ACCESS[SLOT0_IO_MASK] = SLOT0_IO_MASK)	// IO0��λ
#define Slot0_ClrIO()		(LPC_GPIO0->MASKED_ACCESS[SLOT0_IO_MASK] = ~SLOT0_IO_MASK)	// IO0��0
		
//! ����P1.10 Ϊ SAM CLK
#define SLOT0_CLK_MASK		(1ul << 10)
#define Slot0_CfgCLK()		(LPC_IOCON->PIO1_10 = 0xD2)	// CT16B1_MAT1������
		
//! ����P1.0 Ϊ SAM RST
#define SLOT0_RST_MASK		(1ul << 0)
#define Slot0_CfgRST()		LPC_IOCON->R_PIO1_0	= 0xD1;	LPC_GPIO1->DIR |= SLOT0_RST_MASK	// ����SLOT0 RST ΪGPIO ���
#define Slot0_SetRST()		(LPC_GPIO1->MASKED_ACCESS[SLOT0_RST_MASK] = SLOT0_RST_MASK)		// RST��λ
#define Slot0_ClrRST()		(LPC_GPIO1->MASKED_ACCESS[SLOT0_RST_MASK] = ~SLOT0_RST_MASK)	// RST��0

//! ����P1.1 ΪSAM ����Դ���ƽ�
#define SLOT0_SPC_MASK		(1ul << 1)
#define Slot0_CfgSPC()		LPC_IOCON->R_PIO1_1	= 0xD1; LPC_GPIO1->DIR |= SLOT0_SPC_MASK	// ����SLOT0 SPC ΪGPIO ���
#define Slot0_PowerOn()		(LPC_GPIO1->MASKED_ACCESS[SLOT0_SPC_MASK] = ~SLOT0_SPC_MASK)	// SAM ����Դ��
#define Slot0_PowerOff()	(LPC_GPIO1->MASKED_ACCESS[SLOT0_SPC_MASK] = SLOT0_SPC_MASK)		// SAM ����Դ�ر�

		
//! ��������Ŷ���
//! ����P0.0Ϊ TEST_PIN
#define TEST_PIN_MASK		(1ul << 0)
#define TestPin_Cfg()		LPC_IOCON->RESET_PIO0_0 = 0xD1; LPC_GPIO0->DIR |= TEST_PIN_MASK		
#define TestPin_Set()		(LPC_GPIO0->MASKED_ACCESS[TEST_PIN_MASK] = TEST_PIN_MASK)		// TEST_PIN��λ
#define TestPin_Clr()		(LPC_GPIO0->MASKED_ACCESS[TEST_PIN_MASK] = ~TEST_PIN_MASK)		// TEST_PIN��0
// ======================================================================================================
#ifdef __cplusplus
	}
#endif

#endif				// __IO_DEFINE_H


