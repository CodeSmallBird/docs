// ======================================================================================================
//  Copyright (c) 2011 Guangzhou Zhiyuan Electronics Co., Ltd. All rights reserved. 
//  http://www.ecardsys.com
//! @file       fm17xx.c
//! @author     曾本森
//! @date       2012/09/05
//! @version    V2.00
//! @brief      FM1735底层操作函数
//!					- V2.00 曾本森	2012/09/05	在原文件的基础上移植到M0平台，将与耦合芯片（FM17xx）
//!						接口操作的函数移到CDIo.c文件中, 修改Rf_On()和Rf_Off()代码，源代码在多协议处理中存
//!						在问题											
// ======================================================================================================
//2011.08.17修改
//2011.11.29修订 增加mifare卡写入指令的判断

#include "LPC11XX_SPI.h"
#include "M0_PACK_V101_IO.h"
#include "Utility.h"
#include "LPC11xx.h"
#include <string.h>	
#include "fm17xx.h"

unsigned char FM175XX_IRQ;

#define MI_NOTAGERR 0xEE
#define MAXRLEN 64


 unsigned char SPIRead(unsigned char addr);				//SPI读函数
 void SPIWrite(unsigned char add,unsigned char wrdata);	//SPI写函数
 unsigned char SPI_Init(void);		//SPI初始化函数
 unsigned char FM17XX_Init(void);		//1702初始化函数
 unsigned char Write_eeprom(unsigned char lsb,unsigned char msb,unsigned char *buff);
 unsigned char Read_eeprom(unsigned char lsb,unsigned char msb);
 unsigned char Command_Send(unsigned char count,unsigned char *buff,unsigned char Comm);
 void SetTime_delay(unsigned long delaytime);
 unsigned char Read_FIFO(unsigned char *buff);
 void Write_FIFO(unsigned char count,unsigned char *buff);
 unsigned char Clear_FIFO(void);
 void Rf_On(void);
 void Rf_Off(void);
 
 /*********************************************************************************************************
** 函数名称：pcd_Init
** 函数描述：MCU初始化函数、包括SPI和UART的初始化
** 输入参数：无
** 返回值  ：无
*********************************************************************************************************/
void pcd_Init(void)
{
    spi_Init();
    CD_CfgTPD();                                                        /* 配置复位管脚                 */
}

/*********************************************************************************************************
** 函数名称：MCU_TO_PCD_TEST
** 函数描述：MCU与读卡芯片通信测试函数
** 输入参数：无
** 返回值  ：TRUE---测试通过    FALSE----测试失败
** 说明：不同的读卡芯片的选取用于测试的寄存器不一样，需要更具具体芯片而确定目标寄存器
*********************************************************************************************************/
uint8_t MCU_TO_PCD_TEST(void)
{
     unsigned int i;

    FM175X_HardReset();                                                      /* 硬件复位                         */

  

	spi_SetReg(Page_Reg,0x80);	//该寄存器复位值就是0x80
	for (i=0;i<timeout;i++)
	{
		if(spi_GetReg(Page_Reg)==0x80)
			spi_SetReg(Page_Reg,0x00);		//FM1735Q选通SPI接口  select SPI interface
		if(spi_GetReg(Page_Reg)==0x00)
			return TRUE;
	}
	return FALSE;
  
}

/*************************************************************
函数名：	    Read_Reg
功能：	      读寄存器操作
输入参数：	  reg_add，寄存器地址
返回值：	    寄存器数值
*************************************************************/
unsigned char Read_Reg(unsigned char reg_add)
{
unsigned char  reg_value;	   
 reg_value=spi_GetReg(reg_add);
 return reg_value;
}
/*************************************************************
函数名：	    Read_Reg All
功能：	    读64个寄存器操作
输入参数：	无
输出参数		*reg_value，寄存器数值指针
返回值：	    TRUE
************************************************************/
unsigned char Read_Reg_All(unsigned char *reg_value)
{
unsigned char data i;
	for (i=0;i<64;i++)	   
		*(reg_value+i)=spi_GetReg(i);
 return TRUE;
}

/*************************************************************
函数名：	    Write_Reg									 
功能：	      写寄存器操作								 

输入参数：	  reg_add，寄存器地址；reg_value，寄存器数值	 
返回值：	    TRUE											 
				      FALSE										 
*************************************************************/
unsigned char Write_Reg(unsigned char reg_add,unsigned char reg_value)
{
	spi_SetReg(reg_add,reg_value);
	return TRUE;
}
/*************************************************************
函数名：	    Read_FIFO									                   
功能：	    读取FIFO  									                   
															                            
输入参数：	length，读取数据长度					                
输出参数：	*fifo_data，数据存放指针		             			
返回值：	    											                      
*************************************************************/
void Read_FIFO(unsigned char length,unsigned char *fifo_data)
{	 
	//SPIRead_Sequence(length,FIFODataReg,fifo_data);
	return;
}
/*************************************************************/
/*函数名：	    Write_FIFO									 */
/*功能：	    写入FIFO  									 */
/*															 */
/*输入参数：	length，读取数据长度；*fifo_data，数据存放指针*/
/*输出参数：												 */
/*返回值：	    											 */
/*															 */
/*************************************************************/
void Write_FIFO(unsigned char length,unsigned char *fifo_data)
{
	//SPIWrite_Sequence(length,FIFODataReg,fifo_data);
	return;
}
/*************************************************************
函数名：	    Clear_FIFO					 
功能：	      清空FIFO  				
															
输入参数：	  无								
输出参数：										
返回值：	    TRUE						
				      FALSE									
************************************************************/
unsigned char Clear_FIFO(void)
{
	unsigned char temp1;
	unsigned char i;
	
	temp1=spi_GetReg(Control_Reg);						//清空FIFO
	temp1=(temp1 | 0x01);
	spi_SetReg(Control_Reg,temp1);
	for(i=0;i<RF_TimeOut;i++)				//检查FIFO是否被清空
	{
		temp1=spi_GetReg(FIFOLength_Reg);
		if(temp1==0)
		return(TRUE);
	}
	return(FALSE);
}
/*************************************************************/
/*函数名：	    Set_BitMask									 */
/*功能：	    置位寄存器操作								 */
/*输入参数：	reg_add，寄存器地址；mask，寄存器置位		 */
/*返回值：	    TRUE											 */
/*				FALSE										 */
/*************************************************************/
unsigned char Set_BitMask(unsigned char reg_add,unsigned char mask)
{
    unsigned char data result;
    result=spi_SetReg(reg_add,Read_Reg(reg_add) | mask);  // set bit mask
	return result;
}
/********************************************
函数名：	    Clear_BitMask
功能：	    清除位寄存器操作
				
输入参数：	reg_add，寄存器地址；mask，寄存器清除位
返回值：	    TRUE
				FALSE
********************************************/
unsigned char Clear_BitMask(unsigned char reg_add,unsigned char mask)
{
    unsigned char data result;
    result=Write_Reg(reg_add,Read_Reg(reg_add) & ~mask);  // clear bit mask
	return result;
}
/*********************************************/
/*函数名：	    Set_RF
功能：	    设置射频输出
				
输入参数：	mode，射频输出模式
				0，关闭输出
        1，仅打开TX2输出
        2，仅打开TX1输出
				3，TX1，TX2打开输出，TX2为反向输出
返回值：	    TRUE
				FALSE
********************************************/

unsigned char Set_Rf(unsigned char mode)
{
unsigned char data result;
if	((Read_Reg(TxControl_Reg)&0x03)==mode)
	return TRUE;
if (mode==0)
	{
	result=Clear_BitMask(TxControl_Reg,0x03); //关闭TX1，TX2输出
	}
if (mode==1)
	{
	result=Clear_BitMask(TxControl_Reg,0x01); //仅打开TX2输出
	}
if (mode==2)
	{
	result=Clear_BitMask(TxControl_Reg,0x02); //仅打开TX1输出
	}
if (mode==3)
	{
	result=Set_BitMask(TxControl_Reg,0x03); //打开TX1，TX2输出
	}
    Delay100us(1000);
return result;
}
/*********************************************
 函数名：	    Pcd_Comm （不利用IRQ管脚的情况）
 功能：	    读卡器通信
				
 输入参数：	Command，通信操作命令；
				pInData，发送数据数组；
				InLenByte，发送数据数组字节长度；
				pOutData，接收数据数组；
				pOutLenBit，接收数据的位长度
 返回值：	    TRUE
				FALSE
 ********************************************/  
unsigned char Pcd_Comm(	unsigned char Command, 
                 		unsigned char *pInData, 
                 		unsigned char InLenByte,
                 		unsigned char *pOutData, 
                 		unsigned int *pOutLenBit)
{
   uint8_t status  = FALSE;
    uint8_t irqEn   = 0x00;
    uint8_t waitFor = 0x00;
    uint8_t lastBits;
    uint8_t n;
    uint32_t i;
    switch (Command) {

    case MFAuthent:                                                  /* Mifare认证                   */
        irqEn   = 0x12;
        waitFor = 0x10;
    break;
    case Transceive:                                               /* 发送FIFO中的数据到天线，传输后激活接收电路*/
        irqEn   = 0x77;
        waitFor = 0x30;
    break;
    default:
    break;
    }
   
    Write_Reg(ComIEnReg, irqEn | 0x80);
    Clear_BitMask(ComIrqReg, 0x80);
    Write_Reg(CommandReg, Idle);
    Set_BitMask(FIFOLevelReg, 0x80);
    
    for (i=0; i < InLenByte; i++) {
        Write_Reg(FIFODataReg, pInData[i]);
    }
    Write_Reg(CommandReg, Command);

    if (Command == Transceive) {
        Set_BitMask(BitFramingReg, 0x80);
    }

    i = 6000;                                                            /* 根据时钟频率调整，操作M1卡最大等待时间25ms*/

    do {
        n = Read_Reg(ComIrqReg);
        i--;
    } while ((i != 0) && !(n & 0x01) && !(n & waitFor));

    Clear_BitMask(BitFramingReg, 0x80);

    if (i != 0) {
        if(!(Read_Reg(ErrorReg) & 0x1B)) {
            status = TRUE;
            if (n & irqEn & 0x01) {
                status = MI_NOTAGERR;
            }
            if (Command == Transceive) {
                n = Read_Reg(FIFOLevelReg);
                lastBits = Read_Reg(ControlReg) & 0x07;
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
                    pOutData[i] = Read_Reg(FIFODataReg);
                }
            }
        } else {
            status = FALSE;
        }
    }
 	Clear_BitMask(BitFramingReg,0x80);//关闭发送
    return status;
}
 /********************************************
 函数名：	    Pcd_SetTimer
 功能：	    设置接收延时
 输入参数：	delaytime，延时时间（单位为毫秒）
 返回值：	    TRUE
 ********************************************/
 unsigned char Pcd_SetTimer(unsigned long delaytime)//设定超时时间（ms）
{
	unsigned long data TimeReload;
	unsigned int data Prescaler;

	Prescaler=0;
	TimeReload=0;
	while(Prescaler<0xfff)
	{
		TimeReload = ((delaytime*(long)13560)-1)/(Prescaler*2+1);
		
		if( TimeReload<0xffff)
			break;
		Prescaler++;
	}
		TimeReload=TimeReload&0xFFFF;
		Set_BitMask(TModeReg,Prescaler>>8);
		Write_Reg(TPrescalerReg,Prescaler&0xFF);					
		Write_Reg(TReloadMSBReg,TimeReload>>8);
		Write_Reg(TReloadLSBReg,TimeReload&0xFF);
	return TRUE;
}
 /********************************************
 函数名：	    Pcd_ConfigISOType
 功能：	    设置操作协议
 输入参数：	type 0，ISO14443A协议；
 					 1，ISO14443B协议；
 返回值：	    TRUE
 ********************************************/
unsigned char Pcd_ConfigISOType(unsigned char data type)
{
	
   if (type == 0)                     //ISO14443_A
   { 
   		Set_BitMask(ControlReg, 0x10); //ControlReg 0x0C 设置reader模式
		  Set_BitMask(TxAutoReg, 0x40); //TxASKReg 0x15 设置100%ASK有效
	    Write_Reg(TxModeReg, 0x00);  //TxModeReg 0x12 设置TX CRC无效，TX FRAMING =TYPE A
	    Write_Reg(RxModeReg, 0x00); //RxModeReg 0x13 设置RX CRC无效，RX FRAMING =TYPE A
   }
   if (type == 1)                     //ISO14443_B
   	{ 
   		Write_Reg(ControlReg, 0x10); //ControlReg 0x0C 设置reader模式
	    Write_Reg(TxModeReg, 0x83); //TxModeReg 0x12 设置TX CRC有效，TX FRAMING =TYPE B
		  Write_Reg(RxModeReg, 0x83); //RxModeReg 0x13 设置RX CRC有效，RX FRAMING =TYPE B
			Write_Reg(GsNReg, 0xF4); //GsNReg 0x27 设置ON电导
			Write_Reg(GsNOffReg, 0xF4); //GsNOffReg 0x23 设置OFF电导
			Write_Reg(TxAutoReg, 0x00);// TxASKReg 0x15 设置100%ASK无效
	}
   return TRUE;
}
/********************************************
 函数名：	    FM175X_SoftReset
 功能：	    软复位操作
				
 输入参数：	
				

 返回值：	    TRUE
				FALSE
 ********************************************/

unsigned char  FM175X_SoftReset(void)
  {	
	Write_Reg(CommandReg,SoftReset);//
	return	Set_BitMask(ControlReg,0x10);//17520初始值配置
  }

unsigned char FM175X_HardReset(void)
  {	
	CD_SetTPD();         // 低电平脉冲宽度大于100ns即可
	Delay100us(1);
	CD_ClrTPD();
	Delay100us(1);      // 等待晶振稳定，根据晶振起振情况调整此延时时间
	return TRUE;			
  }
/*********************************************
 函数名：	    FM175X_SoftPowerdown
 功能：	    软件低功耗操作
 输入参数：	
 返回值：	    TRUE，进入低功耗模式；
 				FALSE，退出低功耗模式；
 ********************************************/
unsigned char FM175X_SoftPowerdown(void)
{
	if (Read_Reg(CommandReg)&0x10)
		{
		Clear_BitMask(CommandReg,0x10);//退出低功耗模式
		return FALSE;
		}
	else
		Set_BitMask(CommandReg,0x10);//进入低功耗模式
		return TRUE;
}
/*********************************************
 函数名：	    FM175X_HardPowerdown
 功能：	    硬件低功耗操作
 输入参数：	
 返回值：	    TRUE，进入低功耗模式；
 				FALSE，退出低功耗模式；
*********************************************/
unsigned char FM175X_HardPowerdown(void)
{	
	//NPD=~NPD;
	//if(NPD==1)
		return TRUE; //进入低功耗模式
//	else
		//return FALSE;//退出低功耗模式
}
/********************************************
 函数名：	    Read_Ext_Reg
 功能：	    读取扩展寄存器
 输入参数：	reg_add，寄存器地址
 返回值：	    寄存器数值
 ********************************************/
unsigned char Read_Ext_Reg(unsigned char reg_add)
{
 	Write_Reg(0x0F,0x80+reg_add);
 	return Read_Reg(0x0F);
}
 /********************************************
 函数名：	    Write_Ext_Reg
 功能：	    写入扩展寄存器
 输入参数：	reg_add，寄存器地址；reg_value，寄存器数值
 返回值：	    TRUE
				FALSE
 ********************************************/
unsigned char Write_Ext_Reg(unsigned char reg_add,unsigned char reg_value)
{
	Write_Reg(0x0F,0x40+reg_add);
	return (Write_Reg(0x0F,0xC0+reg_value));
}
//////////////////////////////////////////////////////////////////////////////////////////
/*****************************************************************************************
*****************************************************************************************
*****************************************************************************************
*****************************************************************************************
*****************************************************************************************/
//////////////////////////////////////////////////////////////////////////////////////////


// ======================================================================================================
//! @brief			读寄存器
//! @param[in]		addr		-- 要读出FM1702SL内的寄存器地址[0x00~0x3f]	reg_address
//! @return		寄存器的值
// ======================================================================================================
unsigned char SPIRead(unsigned char addr)
{  
	return CD_GetReg(addr);
}
//=======================================================
//SPI写入程序	  SPI write function
//入口: 
//add:	要写到FM1702SL内的寄存器地址[0x00~0x3f]	  reg_address
//wrdata:   要写入的数据						  reg_data
//=======================================================
void SPIWrite(unsigned char add,unsigned char wrdata)
{
	CD_SetReg(add, wrdata);
}

// ======================================================================================================
//! @brief			SPI初始化函数				  SPI initial function
//! @return			操作结果
//!						- OK		-- 初始化成功
//!						- ERROR		-- 初始化失败
// ======================================================================================================
uint8_t SPI_Init(void)
{
	unsigned int i;

	SPIWrite(Page_Reg,0x80);	//该寄存器复位值就是0x80
	for (i=0;i<timeout;i++)
	{
		if(SPIRead(Page_Reg)==0x80)
			SPIWrite(Page_Reg,0x00);		//FM1735Q选通SPI接口  select SPI interface
		if(SPIRead(Page_Reg)==0x00)
			return OK;
	}
	return ERROR;
}

//////////////////////////////////////////////////////
//EEPROM读取函数				   EEPROM read
//出口:
//addressbyte	lsb: lower byte,msb:  uper byte
//
//OK    :初始化成功
//ERROR :初始化失败
/////////////////////////////////////////////////////
unsigned char Read_eeprom(unsigned char lsb,unsigned char msb)
{

	SetTime_delay(10);
	temp[0]=lsb;
	temp[1]=msb;
	temp[2]=16;
	Command_Send(3,temp,0x03);
	return(rec_len);
}
//////////////////////////////////////////////////////
//EEPROM写入函数
//addressbyte	lsb: lower byte,msb:  uper byte
//*buff data to write
//OK    :初始化成功
//ERROR :初始化失败
/////////////////////////////////////////////////////
unsigned char Write_eeprom(unsigned char lsb,unsigned char msb,unsigned char *buff)
{	unsigned char i,temp1;
	SPIWrite(Command_Reg,0x00);
	temp[0]=lsb;
	temp[1]=msb;
	for(i=0;i<16;i++)
	temp[i+2]=*(buff+i);
	Clear_FIFO();
	Write_FIFO(18,temp);	
	SPIWrite(Command_Reg,0x01);	  //写EEPROM指令

	temp1=SPIRead(SecondaryStatus_Reg);	 //读取0x05寄存器，判断bit6 E2-ready位
	if(temp1==0x60)
	return(0);
	delay_100us(1000);
 	temp1=SPIRead(SecondaryStatus_Reg);	 //读取0x05寄存器，判断bit6 E2-ready位
	if(temp1==0x60)
	return(0);
	else
	return(1);
}

////////////////////////////////////////////////////
//1702初始化
//
////////////////////////////////////////////////////
unsigned char FM17XX_Init()
{
	CD_IoInit();										// FM17xx 硬件接口初始化
	CD_HardRst();									// FM17xx 硬件复位

	return OK;
}
/*******************************************************************************************************/
/*名称: Clear_FIFO                                                                     	               */
/*功能: 该函数实现清空FM17XX中FIFO的数据                                                               */                           	             
/*												  													   */											
/*输入:                                                                                                */
/*      N/A                                                                                            */
/*                                                                                                     */
/*输出:                                                                                                */
/*	OK: FIFO被清空                                                 		        				       */
/*	ERROR: FIFO未被清空  	                                       								       */
/*******************************************************************************************************/
unsigned char Clear_FIFO(void)
{
	unsigned char temp1;
	unsigned char i;
	
	temp1=SPIRead(Control_Reg);						//清空FIFO
	temp1=(temp1 | 0x01);
	SPIWrite(Control_Reg,temp1);
	for(i=0;i<RF_TimeOut;i++)				//检查FIFO是否被清空
	{
		temp1=SPIRead(FIFOLength_Reg);
		if(temp1==0)
		return(OK);
	}
	return(ERROR);
}

/********************************************************************************************************/
/*名称: Write_FIFO                                                                     	               	*/
/*功能: 该函数实现向FM17XX的FIFO中写入数据   		                                                     */
/*																								       	*/	
/*输入:                                                                                                	*/
/*      count, 待写入字节的长度                                                                        	*/
/*		buff, 指向待写入数据的指针                                 	                                  	*/
/*                                                                                                     	*/
/*输出:                                                                                                	*/
/*	N/A                                                 		 			              		       	*/
/********************************************************************************************************/
void Write_FIFO(unsigned char count,unsigned char *buff)
{
// 	unsigned char i;
// 	for(i=0;i<count;i++)
// 	SPIWrite(FIFO_Reg,*(buff+i));
	CD_WriteFIFO(buff, count);
}


/********************************************************************************************************/
/*名称: Read_FIFO                                                                     	               	*/
/*功能: 该函数实现从FM1735Q的FIFO中读出x bytes数据                                                       	*/                           	             
/*												 												       */		
/*输入:                                                                                                	*/
/*       buff, 指向读出数据的指针                                                                      	*/
/*                                                                                                     	*/
/*输出:                                                                                                	*/
/*	 N/A                                              			   		                                */
/********************************************************************************************************/
unsigned char Read_FIFO(unsigned char  *buff)
{
// 	unsigned char temp1;
// 	unsigned char i;
// 	temp1=SPIRead(FIFOLength_Reg);
// 	for(i=0;i<temp1;i++)
//  	*(buff+i)=SPIRead(FIFO_Reg);
// 	return temp1;
	uint8_t nBytes;

	nBytes = CD_GetReg(FIFOLength_Reg);
	CD_ReadFIFO(buff, nBytes);
	return nBytes;
 }

/********************************************************************************************************/
/*名称: SetTime_delay                                                                     	               	*/
/*功能: 设置FM17XX定时值                                                      	*/                           	             
/*												 												       */		
/*输入: delaytime(ms)                                                                                               	*/
/*                                                                            	*/
/*                                                                                                     	*/
/*输出:                                                                                                	*/
/* N/A                                              			   		                                */
/********************************************************************************************************/
void SetTime_delay(unsigned long delaytime)
{
	unsigned long temp1,TimeLoad;
	unsigned char Prescalar;
	temp1=1;
	Prescalar=0;
	TimeLoad=0;
while(1)
	{
		temp1=temp1*2;
		TimeLoad = (delaytime*13560)/temp1 ;
		Prescalar++;	
		if( TimeLoad<256 )
			break;
		if(Prescalar>21)
			break;
	
	}
	if(Prescalar<21)
	{
	SPIWrite(TimerClock_Reg,Prescalar);					
	SPIWrite(TimerReload_Reg,TimeLoad);
	}
	else
	{
	SPIWrite(TimerClock_Reg,21);	//默认值为最大延时时间				
//	SPIWrite(TimerReload_Reg,256);	//默认值为最大延时时间
	SPIWrite(TimerReload_Reg,255);	//默认值为最大延时时间
	}
 }

/********************************************************************************************************/
/*名称: Command_Send                                                                     	        */
/*功能: 该函数实现向RC531发送命令集的功能                                                              	*/                           	             
/*												     													*/
/*输入:                                                                                                	*/
/*       count, 待发送命令集的长度                                                                     	*/
/*		 buff, 指向待发送数据的指针                                                                    	*/
/*       Comm_Set, 命令码                                                                              	*/
/*																								       	*/
/*输出:                                                                                                	*/
/*	OK: 命令被正确执行                         			                        		               	*/
/*	ERROR: 命令执行错误  	                  				                     				        */
/********************************************************************************************************/
unsigned char Command_Send(unsigned char count,unsigned char * buff,unsigned char Comm_Set)
{
	
	unsigned char ret,temp2,temp3;
	rec_len=0;

	SPIWrite(InterruptRq_Reg,0x3f);					//清除中断标识
	SPIWrite(InterruptEn_Reg,0xbf);					//中断使能
	SPIWrite(TimerControl_Reg,0x02);				//数据发送结束后，定时器开始计时
	SPIWrite(Command_Reg,0x00);
	Clear_FIFO();
	Write_FIFO(count,buff);					
	SPIWrite(Command_Reg,Comm_Set);					//命令执行，向FM1735Q写指令的步骤是先把具体命令写入FIFO，
	                                        //再再指令寄存器中写入命令代码
	while(1)				//检查中断标识
	{			
		temp2=SPIRead(InterruptRq_Reg);
		if(temp2&0x04)		//IdelIRq
		{
		ret=ERROR_SUCCESS;
	  	break;
		}
		
	//	if(temp2&0x10)		//TxIRq
	//	{
	//	temp1=ERROR_SUCCESS;
	//  	break;
	//	}
		if(temp2&0x08)		//RxIRq
		{
		ret=ERROR_SUCCESS;
	  	break;
		}
		if(temp2&0x20)		//TimerIRq  定时器时间用尽
		{
		ret=ERROR_NOTAG;		
		break;
		}
//		if(temp2&0x02)		//FIFO溢出标志
//		{
		
//		temp3=Read_FIFO(temp+rec_len);	//读出FIFO内容
//		rec_len=rec_len+ temp3;
//		SPIWrite(InterruptRq_Reg,0x02);	
//		break;
//		}
	}

	   if (ret==ERROR_SUCCESS)
	   {
	   	temp3=Read_FIFO(temp+rec_len); //读出FIFO内容
		rec_len=rec_len+ temp3;
		}
	   return(ret);	
}


void Rf_On()
{

//	SPIWrite(TxControl_Reg,0x5B); //FM1722 为双端天线
	SPIWrite(TxControl_Reg, SPIRead(TxControl_Reg) | 0x03);  
	//0x58|0x03=0x5B,与直接写的区别   直接写0x5B之后就只能读A卡，不能读B卡
	delay_100us(500); //等待天线载波信号稳定

}

void Rf_Off()
{
	SPIWrite(TxControl_Reg, SPIRead(TxControl_Reg) & ~0x03);
}

