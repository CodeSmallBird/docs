// ======================================================================================================
//  Copyright (c) 2011 Guangzhou Zhiyuan Electronics Co., Ltd. All rights reserved. 
//  http://www.zlg.cn
//! @file       M0_PACK_V101_IO.h
//! @author     ZengBenSen
//! @date       2012.09.29
//! @version    V1.00
//! @brief      CK10-GM V1.00 I/O 定义.移植到M0 PACK V1.01，同时增加通讯口定义
//!                 - V1.02 2012.12.05  韩远强  在SAM卡接口定义部分，修改了JTAG几个管脚的寄存器名，新旧两
//!                                             个版本lpc11xx.h头文件不同，这里是较新的V1.10版本
//!					- V1.01	2012.11.28	韩远强	修改CD部分管脚，移植到M0 PACK V1.01，同时增加通讯口定义
//!					- V1.00	2012.09.29	曾本森	CK10-GM V1.00 I/O 定义
// ======================================================================================================
#ifndef __MO_PACK_V101_IO_H
#define __M0_PACK_V101_IO_H

#include "LPC11xx.h"

#ifdef __cplusplus
	extern "C" {
#endif

// ============ I/O口定义 ===============================================================================
//! 定义P3.2为LED_GREEN
#define IO_MASK_LED_GREEN	(1 << 2)															//!< LED_GREEN 控制脚掩码
#define LED_GreenCfg()		LPC_GPIO3->DIR |= IO_MASK_LED_GREEN									// 配置LED_GREEN 为GPIO 输出
#define LED_GreenOff()		(LPC_GPIO3->MASKED_ACCESS[IO_MASK_LED_GREEN] = IO_MASK_LED_GREEN)	//!< 关闭LED_GREEN
#define LED_GreenOn()		(LPC_GPIO3->MASKED_ACCESS[IO_MASK_LED_GREEN] = ~IO_MASK_LED_GREEN)	//!< 点亮LED_GREEN		

//! 定义P1.11 为LED_RED	  
#define IO_MASK_LED_RED		(1 << 11)															
#define LED_RedCfg()		LPC_GPIO1->DIR |= IO_MASK_LED_RED									//!< 配置LED_RED 为GPIO 输出
#define LED_RedOff()		(LPC_GPIO1->MASKED_ACCESS[IO_MASK_LED_RED] = IO_MASK_LED_RED)		//!< 关闭LED_RED
#define LED_RedOn()			(LPC_GPIO1->MASKED_ACCESS[IO_MASK_LED_RED] = ~IO_MASK_LED_RED)		//!< 点亮LED_RED

// ======================================================================================================
// UART及IIC通讯接口定义
//! 定义P0.3 为 NINT	  
#define IO_MASK_NINT		(1 << 3)															
#define NINT_Cfg()			LPC_IOCON->PIO0_3 = 0xC0; LPC_GPIO0->DIR |= IO_MASK_NINT	        //!< 配置NINT 为GPIO 输出,无上下拉
#define NINT_Set()			(LPC_GPIO0->MASKED_ACCESS[IO_MASK_NINT] = IO_MASK_NINT)		        //!< 设置NINT
#define NINT_Clr()			(LPC_GPIO0->MASKED_ACCESS[IO_MASK_NINT] = ~IO_MASK_NINT)	        //!< 清除NINT

//! 定义P0.4 为 IIC的SCL管脚，同时也是UART的SBR管脚
#define IO_MASK_SBR         (1 << 4)
#define UART_CfgSBR()       LPC_IOCON->PIO0_4 = 0xD0; LPC_GPIO0->DIR &= ~IO_MASK_SBR            //!< 配置SBR为GPIO输入，上拉
//#define UART_SetSBR()       (LPC_GPIO0->MASKED_ACCESS[IO_MASK_SBR] = IO_MASK_SBR)               //!< 拉高SBR
//#define UART_ClrSBR()       (LPC_GPIO0->MASKED_ACCESS[IO_MASK_SBR] = ~IO_MASK_SBR)              //!< 拉低SBR
#define UART_GetSBR()       (LPC_GPIO0->MASKED_ACCESS[IO_MASK_SBR])                             //!< 获取SBR状态
//#define UART_GetSBR()       (LPC_GPIO0->DATA & IO_MASK_SBR)

//! 定义P0.4为SCL  
#define IO_MASK_SCL		(1 << 4)												// CD SPI通信SCL脚掩码
#define CD_CfgSCL()		(LPC_GPIO0->DIR &= ~IO_MASK_SCL)							// 配置SCL为输入
#define CD_GetSCL()		(LPC_GPIO0->MASKED_ACCESS[IO_MASK_SCL])	// 获取CD SPI通信SCL的状态

//! 定义P0.5为SDA  
#define IO_MASK_SDA		(1 << 5)												// CD SPI通信SDA脚掩码
#define CD_CfgSDA()		(LPC_GPIO0->DIR &= ~IO_MASK_SDA)							// 配置SDA为输入
#define CD_GetSDA()		(LPC_GPIO0->MASKED_ACCESS[IO_MASK_SDA])	// 获取CD SPI通信SDA的状态

//! 定义P0.5 为 IIC的SDA管脚，同时也是UART的DBR管脚
#define IO_MASK_DBR         (1 << 5)
#define UART_CfgDBR()       LPC_IOCON->PIO0_5 = 0xD0; LPC_GPIO0->DIR &= ~IO_MASK_DBR            //!< 配置SBR为GPIO输入，上拉
//#define UART_SetDBR()       (LPC_GPIO0->MASKED_ACCESS[IO_MASK_DBR] = IO_MASK_DBR)               //!< 拉高DBR
//#define UART_ClrDBR()       (LPC_GPIO0->MASKED_ACCESS[IO_MASK_DBR] = ~IO_MASK_DBR)              //!< 拉低DBR
#define UART_GetDBR()       (LPC_GPIO0->MASKED_ACCESS[IO_MASK_DBR])                             //!< 获取DBR状态

//! 定义P1.6 为 UART的RXD管脚，同时也是IIC的ADR1管脚
#define IO_MASK_ADR1        (1 << 6)
#define IIC_CfgADR1()      LPC_IOCON->PIO1_6 = 0xD0; LPC_GPIO1->DIR &= ~IO_MASK_ADR1            //!< 配置SBR为GPIO输入，上拉
//#define IIC_SetADR1()      (LPC_GPIO1->MASKED_ACCESS[IO_MASK_ADR1] = IO_MASK_ADR1)              //!< 拉高ADR1
//#define IIC_ClrADR1()      (LPC_GPIO1->MASKED_ACCESS[IO_MASK_ADR1] = ~IO_MASK_ADR1)             //!< 拉低ADR1
#define IIC_GetADR1()      (LPC_GPIO1->MASKED_ACCESS[IO_MASK_ADR1])                             //!< 获取ADR1状态

//! 定义P1.7 为 UART的TXD管脚，同时也是IIC的ADR0管脚
#define IO_MASK_ADR0        (1 << 7)
#define IIC_CfgADR0()      LPC_IOCON->PIO1_7 = 0xD0; LPC_GPIO1->DIR &= ~IO_MASK_ADR0            //!< 配置SBR为GPIO输入，上拉
//#define IIC_SetADR0()      (LPC_GPIO1->MASKED_ACCESS[IO_MASK_ADR0] = IO_MASK_ADR0)              //!< 拉高ADR0
//#define IIC_ClrADR0()      (LPC_GPIO1->MASKED_ACCESS[IO_MASK_ADR0] = ~IO_MASK_ADR0)             //!< 拉低ADR0
#define IIC_GetADR0()      (LPC_GPIO1->MASKED_ACCESS[IO_MASK_ADR0])                             //!< 获取ADR0状态

//! 定义P0.1 为 485的CON管脚，同时也是IIC的SADR管脚
#define IO_MASK_SADR        (1 << 1)
#define IIC_CfgSADR()      LPC_IOCON->PIO0_1 = 0xD0; LPC_GPIO0->DIR &= ~IO_MASK_SADR            //!< 配置SBR为GPIO输入，上拉
//#define IIC_SetSADR()      (LPC_GPIO0->MASKED_ACCESS[IO_MASK_SADR] = IO_MASK_SADR)              //!< 拉高SADR
//#define IIC_ClrSADR()      (LPC_GPIO0->MASKED_ACCESS[IO_MASK_SADR] = ~IO_MASK_SADR)             //!< 拉低SADR
#define IIC_GetSADR()      (LPC_GPIO0->MASKED_ACCESS[IO_MASK_SADR])                             //!< 获取SADR状态

// ======================================================================================================
// CD 接口定义
//! 定义P3.5为TPD
#define IO_MASK_TPD			(1 << 5)												//!< CD掉电控制TPD 脚掩码
#define CD_CfgTPD()			(LPC_GPIO3->DIR |= IO_MASK_TPD)							//!< 配置TPD 为输出
#define CD_SetTPD()			(LPC_GPIO3->MASKED_ACCESS[IO_MASK_TPD] = IO_MASK_TPD)	//!< TPD 置位
#define CD_ClrTPD()			(LPC_GPIO3->MASKED_ACCESS[IO_MASK_TPD] = ~IO_MASK_TPD)	//!< TPD 清零

//! 定义P0.2为NSS	  
#define IO_MASK_NSS			(1 << 2)												//!< CD SPI 通信NSS 脚掩码
#define CD_CfgNSS()			(LPC_GPIO0->DIR |= IO_MASK_NSS)							//!< 配置NSS 为GPIO 输出
#define CD_EnNSS()			(LPC_GPIO0->MASKED_ACCESS[IO_MASK_NSS] = ~IO_MASK_NSS)	//!< SPI 通信NSS 使能（清零）
#define CD_DisNSS()			(LPC_GPIO0->MASKED_ACCESS[IO_MASK_NSS] = IO_MASK_NSS)	//!< SPI 通信NSS 禁止（置位）

//! 定义P0.3为IRQ
#define CD_IRQn				EINT0_IRQn												//!< CD中断编号
#define IO_MASK_IRQ			(1 << 3)												//!< CD中断IRQ脚掩码
//! 配置IRQ为输入, 中断禁止, 低电平触发
#define CD_CfgIRQ()			LPC_GPIO0->DIR	&= ~IO_MASK_IRQ;\
							LPC_GPIO0->IE   &= ~IO_MASK_IRQ;\
							LPC_GPIO0->IS   |=  IO_MASK_IRQ;\
							LPC_GPIO0->IEV	&= ~IO_MASK_IRQ

                                               
#define CD_EnIRQ()			(LPC_GPIO0->IE	|=  IO_MASK_IRQ)	  					//!< 使能IRQ
#define CD_DisIRQ()			(LPC_GPIO0->IE	&= ~IO_MASK_IRQ)	  					//!< 禁能IRQ
#define CD_GetIRQ()			(LPC_GPIO0->MIS	&= ~IO_MASK_IRQ)	  					//!< 读IRQ
#define CD_ClrIRQ()			(LPC_GPIO0->IC	|=  IO_MASK_IRQ)	  					//1< 清IRQ

// ======================================================================================================
// SAM 卡接口定义
//! 定义P0.11为 SAM IO
#define SLOT0_IO_MASK		(1ul << 11)
//! IO 配置为GPIO、上拉、输入、禁止中断、边缘中断、下降沿触发、清除中断、使能EINT0_IRQn中断
#define Slot0_CfgIO()		LPC_IOCON->R_PIO0_11	=  0xD1;	\
							LPC_GPIO0->DIR		&= ~SLOT0_IO_MASK;	\
							LPC_GPIO0->IE		&= ~SLOT0_IO_MASK;	\
							LPC_GPIO0->IS		&= ~SLOT0_IO_MASK;	\
							LPC_GPIO0->IEV		&= ~SLOT0_IO_MASK;	\
							LPC_GPIO0->IC		=  0xFFFFFFFF;		\
							NVIC_EnableIRQ(EINT0_IRQn)
		
#define Slot0_CfgIOIn()		(LPC_GPIO0->DIR &= ~SLOT0_IO_MASK)			// 配置SLOT0 IO 为GPIO输入
#define Slot0_CfgIOOut()	(LPC_GPIO0->DIR |= SLOT0_IO_MASK)			// 配置SLOT0 IO 为GPIO输出
#define Slot0_SetIO()		(LPC_GPIO0->MASKED_ACCESS[SLOT0_IO_MASK] = SLOT0_IO_MASK)	// IO0置位
#define Slot0_ClrIO()		(LPC_GPIO0->MASKED_ACCESS[SLOT0_IO_MASK] = ~SLOT0_IO_MASK)	// IO0清0
		
//! 定义P1.10 为 SAM CLK
#define SLOT0_CLK_MASK		(1ul << 10)
#define Slot0_CfgCLK()		(LPC_IOCON->PIO1_10 = 0xD2)	// CT16B1_MAT1，上拉
		
//! 定义P1.0 为 SAM RST
#define SLOT0_RST_MASK		(1ul << 0)
#define Slot0_CfgRST()		LPC_IOCON->R_PIO1_0	= 0xD1;	LPC_GPIO1->DIR |= SLOT0_RST_MASK	// 配置SLOT0 RST 为GPIO 输出
#define Slot0_SetRST()		(LPC_GPIO1->MASKED_ACCESS[SLOT0_RST_MASK] = SLOT0_RST_MASK)		// RST置位
#define Slot0_ClrRST()		(LPC_GPIO1->MASKED_ACCESS[SLOT0_RST_MASK] = ~SLOT0_RST_MASK)	// RST清0

//! 定义P1.1 为SAM 卡电源控制脚
#define SLOT0_SPC_MASK		(1ul << 1)
#define Slot0_CfgSPC()		LPC_IOCON->R_PIO1_1	= 0xD1; LPC_GPIO1->DIR |= SLOT0_SPC_MASK	// 配置SLOT0 SPC 为GPIO 输出
#define Slot0_PowerOn()		(LPC_GPIO1->MASKED_ACCESS[SLOT0_SPC_MASK] = ~SLOT0_SPC_MASK)	// SAM 卡电源打开
#define Slot0_PowerOff()	(LPC_GPIO1->MASKED_ACCESS[SLOT0_SPC_MASK] = SLOT0_SPC_MASK)		// SAM 卡电源关闭

		
//! 测试输出脚定义
//! 定义P0.0为 TEST_PIN
#define TEST_PIN_MASK		(1ul << 0)
#define TestPin_Cfg()		LPC_IOCON->RESET_PIO0_0 = 0xD1; LPC_GPIO0->DIR |= TEST_PIN_MASK		
#define TestPin_Set()		(LPC_GPIO0->MASKED_ACCESS[TEST_PIN_MASK] = TEST_PIN_MASK)		// TEST_PIN置位
#define TestPin_Clr()		(LPC_GPIO0->MASKED_ACCESS[TEST_PIN_MASK] = ~TEST_PIN_MASK)		// TEST_PIN清0
// ======================================================================================================
#ifdef __cplusplus
	}
#endif

#endif				// __IO_DEFINE_H


