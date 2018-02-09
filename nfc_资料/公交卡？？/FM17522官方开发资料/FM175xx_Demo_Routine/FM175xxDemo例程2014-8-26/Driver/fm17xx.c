// ======================================================================================================
//  Copyright (c) 2011 Guangzhou Zhiyuan Electronics Co., Ltd. All rights reserved. 
//  http://www.ecardsys.com
//! @file       fm17xx.c
//! @author     ����ɭ
//! @date       2012/09/05
//! @version    V2.00
//! @brief      FM1735�ײ��������
//!					- V2.00 ����ɭ	2012/09/05	��ԭ�ļ��Ļ�������ֲ��M0ƽ̨���������оƬ��FM17xx��
//!						�ӿڲ����ĺ����Ƶ�CDIo.c�ļ���, �޸�Rf_On()��Rf_Off()���룬Դ�����ڶ�Э�鴦���д�
//!						������											
// ======================================================================================================
//2011.08.17�޸�
//2011.11.29�޶� ����mifare��д��ָ����ж�

#include "LPC11XX_SPI.h"
#include "M0_PACK_V101_IO.h"
#include "Utility.h"
#include "LPC11xx.h"
#include <string.h>	
#include "fm17xx.h"

unsigned char FM175XX_IRQ;

#define MI_NOTAGERR 0xEE
#define MAXRLEN 64


 unsigned char SPIRead(unsigned char addr);				//SPI������
 void SPIWrite(unsigned char add,unsigned char wrdata);	//SPIд����
 unsigned char SPI_Init(void);		//SPI��ʼ������
 unsigned char FM17XX_Init(void);		//1702��ʼ������
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
** �������ƣ�pcd_Init
** ����������MCU��ʼ������������SPI��UART�ĳ�ʼ��
** �����������
** ����ֵ  ����
*********************************************************************************************************/
void pcd_Init(void)
{
    spi_Init();
    CD_CfgTPD();                                                        /* ���ø�λ�ܽ�                 */
}

/*********************************************************************************************************
** �������ƣ�MCU_TO_PCD_TEST
** ����������MCU�����оƬͨ�Ų��Ժ���
** �����������
** ����ֵ  ��TRUE---����ͨ��    FALSE----����ʧ��
** ˵������ͬ�Ķ���оƬ��ѡȡ���ڲ��ԵļĴ�����һ������Ҫ���߾���оƬ��ȷ��Ŀ��Ĵ���
*********************************************************************************************************/
uint8_t MCU_TO_PCD_TEST(void)
{
     unsigned int i;

    FM175X_HardReset();                                                      /* Ӳ����λ                         */

  

	spi_SetReg(Page_Reg,0x80);	//�üĴ�����λֵ����0x80
	for (i=0;i<timeout;i++)
	{
		if(spi_GetReg(Page_Reg)==0x80)
			spi_SetReg(Page_Reg,0x00);		//FM1735QѡͨSPI�ӿ�  select SPI interface
		if(spi_GetReg(Page_Reg)==0x00)
			return TRUE;
	}
	return FALSE;
  
}

/*************************************************************
��������	    Read_Reg
���ܣ�	      ���Ĵ�������
���������	  reg_add���Ĵ�����ַ
����ֵ��	    �Ĵ�����ֵ
*************************************************************/
unsigned char Read_Reg(unsigned char reg_add)
{
unsigned char  reg_value;	   
 reg_value=spi_GetReg(reg_add);
 return reg_value;
}
/*************************************************************
��������	    Read_Reg All
���ܣ�	    ��64���Ĵ�������
���������	��
�������		*reg_value���Ĵ�����ֵָ��
����ֵ��	    TRUE
************************************************************/
unsigned char Read_Reg_All(unsigned char *reg_value)
{
unsigned char data i;
	for (i=0;i<64;i++)	   
		*(reg_value+i)=spi_GetReg(i);
 return TRUE;
}

/*************************************************************
��������	    Write_Reg									 
���ܣ�	      д�Ĵ�������								 

���������	  reg_add���Ĵ�����ַ��reg_value���Ĵ�����ֵ	 
����ֵ��	    TRUE											 
				      FALSE										 
*************************************************************/
unsigned char Write_Reg(unsigned char reg_add,unsigned char reg_value)
{
	spi_SetReg(reg_add,reg_value);
	return TRUE;
}
/*************************************************************
��������	    Read_FIFO									                   
���ܣ�	    ��ȡFIFO  									                   
															                            
���������	length����ȡ���ݳ���					                
���������	*fifo_data�����ݴ��ָ��		             			
����ֵ��	    											                      
*************************************************************/
void Read_FIFO(unsigned char length,unsigned char *fifo_data)
{	 
	//SPIRead_Sequence(length,FIFODataReg,fifo_data);
	return;
}
/*************************************************************/
/*��������	    Write_FIFO									 */
/*���ܣ�	    д��FIFO  									 */
/*															 */
/*���������	length����ȡ���ݳ��ȣ�*fifo_data�����ݴ��ָ��*/
/*���������												 */
/*����ֵ��	    											 */
/*															 */
/*************************************************************/
void Write_FIFO(unsigned char length,unsigned char *fifo_data)
{
	//SPIWrite_Sequence(length,FIFODataReg,fifo_data);
	return;
}
/*************************************************************
��������	    Clear_FIFO					 
���ܣ�	      ���FIFO  				
															
���������	  ��								
���������										
����ֵ��	    TRUE						
				      FALSE									
************************************************************/
unsigned char Clear_FIFO(void)
{
	unsigned char temp1;
	unsigned char i;
	
	temp1=spi_GetReg(Control_Reg);						//���FIFO
	temp1=(temp1 | 0x01);
	spi_SetReg(Control_Reg,temp1);
	for(i=0;i<RF_TimeOut;i++)				//���FIFO�Ƿ����
	{
		temp1=spi_GetReg(FIFOLength_Reg);
		if(temp1==0)
		return(TRUE);
	}
	return(FALSE);
}
/*************************************************************/
/*��������	    Set_BitMask									 */
/*���ܣ�	    ��λ�Ĵ�������								 */
/*���������	reg_add���Ĵ�����ַ��mask���Ĵ�����λ		 */
/*����ֵ��	    TRUE											 */
/*				FALSE										 */
/*************************************************************/
unsigned char Set_BitMask(unsigned char reg_add,unsigned char mask)
{
    unsigned char data result;
    result=spi_SetReg(reg_add,Read_Reg(reg_add) | mask);  // set bit mask
	return result;
}
/********************************************
��������	    Clear_BitMask
���ܣ�	    ���λ�Ĵ�������
				
���������	reg_add���Ĵ�����ַ��mask���Ĵ������λ
����ֵ��	    TRUE
				FALSE
********************************************/
unsigned char Clear_BitMask(unsigned char reg_add,unsigned char mask)
{
    unsigned char data result;
    result=Write_Reg(reg_add,Read_Reg(reg_add) & ~mask);  // clear bit mask
	return result;
}
/*********************************************/
/*��������	    Set_RF
���ܣ�	    ������Ƶ���
				
���������	mode����Ƶ���ģʽ
				0���ر����
        1������TX2���
        2������TX1���
				3��TX1��TX2�������TX2Ϊ�������
����ֵ��	    TRUE
				FALSE
********************************************/

unsigned char Set_Rf(unsigned char mode)
{
unsigned char data result;
if	((Read_Reg(TxControl_Reg)&0x03)==mode)
	return TRUE;
if (mode==0)
	{
	result=Clear_BitMask(TxControl_Reg,0x03); //�ر�TX1��TX2���
	}
if (mode==1)
	{
	result=Clear_BitMask(TxControl_Reg,0x01); //����TX2���
	}
if (mode==2)
	{
	result=Clear_BitMask(TxControl_Reg,0x02); //����TX1���
	}
if (mode==3)
	{
	result=Set_BitMask(TxControl_Reg,0x03); //��TX1��TX2���
	}
    Delay100us(1000);
return result;
}
/*********************************************
 ��������	    Pcd_Comm ��������IRQ�ܽŵ������
 ���ܣ�	    ������ͨ��
				
 ���������	Command��ͨ�Ų������
				pInData�������������飻
				InLenByte���������������ֽڳ��ȣ�
				pOutData�������������飻
				pOutLenBit���������ݵ�λ����
 ����ֵ��	    TRUE
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

    case MFAuthent:                                                  /* Mifare��֤                   */
        irqEn   = 0x12;
        waitFor = 0x10;
    break;
    case Transceive:                                               /* ����FIFO�е����ݵ����ߣ�����󼤻���յ�·*/
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

    i = 6000;                                                            /* ����ʱ��Ƶ�ʵ���������M1�����ȴ�ʱ��25ms*/

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
 	Clear_BitMask(BitFramingReg,0x80);//�رշ���
    return status;
}
 /********************************************
 ��������	    Pcd_SetTimer
 ���ܣ�	    ���ý�����ʱ
 ���������	delaytime����ʱʱ�䣨��λΪ���룩
 ����ֵ��	    TRUE
 ********************************************/
 unsigned char Pcd_SetTimer(unsigned long delaytime)//�趨��ʱʱ�䣨ms��
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
 ��������	    Pcd_ConfigISOType
 ���ܣ�	    ���ò���Э��
 ���������	type 0��ISO14443AЭ�飻
 					 1��ISO14443BЭ�飻
 ����ֵ��	    TRUE
 ********************************************/
unsigned char Pcd_ConfigISOType(unsigned char data type)
{
	
   if (type == 0)                     //ISO14443_A
   { 
   		Set_BitMask(ControlReg, 0x10); //ControlReg 0x0C ����readerģʽ
		  Set_BitMask(TxAutoReg, 0x40); //TxASKReg 0x15 ����100%ASK��Ч
	    Write_Reg(TxModeReg, 0x00);  //TxModeReg 0x12 ����TX CRC��Ч��TX FRAMING =TYPE A
	    Write_Reg(RxModeReg, 0x00); //RxModeReg 0x13 ����RX CRC��Ч��RX FRAMING =TYPE A
   }
   if (type == 1)                     //ISO14443_B
   	{ 
   		Write_Reg(ControlReg, 0x10); //ControlReg 0x0C ����readerģʽ
	    Write_Reg(TxModeReg, 0x83); //TxModeReg 0x12 ����TX CRC��Ч��TX FRAMING =TYPE B
		  Write_Reg(RxModeReg, 0x83); //RxModeReg 0x13 ����RX CRC��Ч��RX FRAMING =TYPE B
			Write_Reg(GsNReg, 0xF4); //GsNReg 0x27 ����ON�絼
			Write_Reg(GsNOffReg, 0xF4); //GsNOffReg 0x23 ����OFF�絼
			Write_Reg(TxAutoReg, 0x00);// TxASKReg 0x15 ����100%ASK��Ч
	}
   return TRUE;
}
/********************************************
 ��������	    FM175X_SoftReset
 ���ܣ�	    ��λ����
				
 ���������	
				

 ����ֵ��	    TRUE
				FALSE
 ********************************************/

unsigned char  FM175X_SoftReset(void)
  {	
	Write_Reg(CommandReg,SoftReset);//
	return	Set_BitMask(ControlReg,0x10);//17520��ʼֵ����
  }

unsigned char FM175X_HardReset(void)
  {	
	CD_SetTPD();         // �͵�ƽ�����ȴ���100ns����
	Delay100us(1);
	CD_ClrTPD();
	Delay100us(1);      // �ȴ������ȶ������ݾ������������������ʱʱ��
	return TRUE;			
  }
/*********************************************
 ��������	    FM175X_SoftPowerdown
 ���ܣ�	    ����͹��Ĳ���
 ���������	
 ����ֵ��	    TRUE������͹���ģʽ��
 				FALSE���˳��͹���ģʽ��
 ********************************************/
unsigned char FM175X_SoftPowerdown(void)
{
	if (Read_Reg(CommandReg)&0x10)
		{
		Clear_BitMask(CommandReg,0x10);//�˳��͹���ģʽ
		return FALSE;
		}
	else
		Set_BitMask(CommandReg,0x10);//����͹���ģʽ
		return TRUE;
}
/*********************************************
 ��������	    FM175X_HardPowerdown
 ���ܣ�	    Ӳ���͹��Ĳ���
 ���������	
 ����ֵ��	    TRUE������͹���ģʽ��
 				FALSE���˳��͹���ģʽ��
*********************************************/
unsigned char FM175X_HardPowerdown(void)
{	
	//NPD=~NPD;
	//if(NPD==1)
		return TRUE; //����͹���ģʽ
//	else
		//return FALSE;//�˳��͹���ģʽ
}
/********************************************
 ��������	    Read_Ext_Reg
 ���ܣ�	    ��ȡ��չ�Ĵ���
 ���������	reg_add���Ĵ�����ַ
 ����ֵ��	    �Ĵ�����ֵ
 ********************************************/
unsigned char Read_Ext_Reg(unsigned char reg_add)
{
 	Write_Reg(0x0F,0x80+reg_add);
 	return Read_Reg(0x0F);
}
 /********************************************
 ��������	    Write_Ext_Reg
 ���ܣ�	    д����չ�Ĵ���
 ���������	reg_add���Ĵ�����ַ��reg_value���Ĵ�����ֵ
 ����ֵ��	    TRUE
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
//! @brief			���Ĵ���
//! @param[in]		addr		-- Ҫ����FM1702SL�ڵļĴ�����ַ[0x00~0x3f]	reg_address
//! @return		�Ĵ�����ֵ
// ======================================================================================================
unsigned char SPIRead(unsigned char addr)
{  
	return CD_GetReg(addr);
}
//=======================================================
//SPIд�����	  SPI write function
//���: 
//add:	Ҫд��FM1702SL�ڵļĴ�����ַ[0x00~0x3f]	  reg_address
//wrdata:   Ҫд�������						  reg_data
//=======================================================
void SPIWrite(unsigned char add,unsigned char wrdata)
{
	CD_SetReg(add, wrdata);
}

// ======================================================================================================
//! @brief			SPI��ʼ������				  SPI initial function
//! @return			�������
//!						- OK		-- ��ʼ���ɹ�
//!						- ERROR		-- ��ʼ��ʧ��
// ======================================================================================================
uint8_t SPI_Init(void)
{
	unsigned int i;

	SPIWrite(Page_Reg,0x80);	//�üĴ�����λֵ����0x80
	for (i=0;i<timeout;i++)
	{
		if(SPIRead(Page_Reg)==0x80)
			SPIWrite(Page_Reg,0x00);		//FM1735QѡͨSPI�ӿ�  select SPI interface
		if(SPIRead(Page_Reg)==0x00)
			return OK;
	}
	return ERROR;
}

//////////////////////////////////////////////////////
//EEPROM��ȡ����				   EEPROM read
//����:
//addressbyte	lsb: lower byte,msb:  uper byte
//
//OK    :��ʼ���ɹ�
//ERROR :��ʼ��ʧ��
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
//EEPROMд�뺯��
//addressbyte	lsb: lower byte,msb:  uper byte
//*buff data to write
//OK    :��ʼ���ɹ�
//ERROR :��ʼ��ʧ��
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
	SPIWrite(Command_Reg,0x01);	  //дEEPROMָ��

	temp1=SPIRead(SecondaryStatus_Reg);	 //��ȡ0x05�Ĵ������ж�bit6 E2-readyλ
	if(temp1==0x60)
	return(0);
	delay_100us(1000);
 	temp1=SPIRead(SecondaryStatus_Reg);	 //��ȡ0x05�Ĵ������ж�bit6 E2-readyλ
	if(temp1==0x60)
	return(0);
	else
	return(1);
}

////////////////////////////////////////////////////
//1702��ʼ��
//
////////////////////////////////////////////////////
unsigned char FM17XX_Init()
{
	CD_IoInit();										// FM17xx Ӳ���ӿڳ�ʼ��
	CD_HardRst();									// FM17xx Ӳ����λ

	return OK;
}
/*******************************************************************************************************/
/*����: Clear_FIFO                                                                     	               */
/*����: �ú���ʵ�����FM17XX��FIFO������                                                               */                           	             
/*												  													   */											
/*����:                                                                                                */
/*      N/A                                                                                            */
/*                                                                                                     */
/*���:                                                                                                */
/*	OK: FIFO�����                                                 		        				       */
/*	ERROR: FIFOδ�����  	                                       								       */
/*******************************************************************************************************/
unsigned char Clear_FIFO(void)
{
	unsigned char temp1;
	unsigned char i;
	
	temp1=SPIRead(Control_Reg);						//���FIFO
	temp1=(temp1 | 0x01);
	SPIWrite(Control_Reg,temp1);
	for(i=0;i<RF_TimeOut;i++)				//���FIFO�Ƿ����
	{
		temp1=SPIRead(FIFOLength_Reg);
		if(temp1==0)
		return(OK);
	}
	return(ERROR);
}

/********************************************************************************************************/
/*����: Write_FIFO                                                                     	               	*/
/*����: �ú���ʵ����FM17XX��FIFO��д������   		                                                     */
/*																								       	*/	
/*����:                                                                                                	*/
/*      count, ��д���ֽڵĳ���                                                                        	*/
/*		buff, ָ���д�����ݵ�ָ��                                 	                                  	*/
/*                                                                                                     	*/
/*���:                                                                                                	*/
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
/*����: Read_FIFO                                                                     	               	*/
/*����: �ú���ʵ�ִ�FM1735Q��FIFO�ж���x bytes����                                                       	*/                           	             
/*												 												       */		
/*����:                                                                                                	*/
/*       buff, ָ��������ݵ�ָ��                                                                      	*/
/*                                                                                                     	*/
/*���:                                                                                                	*/
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
/*����: SetTime_delay                                                                     	               	*/
/*����: ����FM17XX��ʱֵ                                                      	*/                           	             
/*												 												       */		
/*����: delaytime(ms)                                                                                               	*/
/*                                                                            	*/
/*                                                                                                     	*/
/*���:                                                                                                	*/
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
	SPIWrite(TimerClock_Reg,21);	//Ĭ��ֵΪ�����ʱʱ��				
//	SPIWrite(TimerReload_Reg,256);	//Ĭ��ֵΪ�����ʱʱ��
	SPIWrite(TimerReload_Reg,255);	//Ĭ��ֵΪ�����ʱʱ��
	}
 }

/********************************************************************************************************/
/*����: Command_Send                                                                     	        */
/*����: �ú���ʵ����RC531��������Ĺ���                                                              	*/                           	             
/*												     													*/
/*����:                                                                                                	*/
/*       count, ����������ĳ���                                                                     	*/
/*		 buff, ָ����������ݵ�ָ��                                                                    	*/
/*       Comm_Set, ������                                                                              	*/
/*																								       	*/
/*���:                                                                                                	*/
/*	OK: �����ȷִ��                         			                        		               	*/
/*	ERROR: ����ִ�д���  	                  				                     				        */
/********************************************************************************************************/
unsigned char Command_Send(unsigned char count,unsigned char * buff,unsigned char Comm_Set)
{
	
	unsigned char ret,temp2,temp3;
	rec_len=0;

	SPIWrite(InterruptRq_Reg,0x3f);					//����жϱ�ʶ
	SPIWrite(InterruptEn_Reg,0xbf);					//�ж�ʹ��
	SPIWrite(TimerControl_Reg,0x02);				//���ݷ��ͽ����󣬶�ʱ����ʼ��ʱ
	SPIWrite(Command_Reg,0x00);
	Clear_FIFO();
	Write_FIFO(count,buff);					
	SPIWrite(Command_Reg,Comm_Set);					//����ִ�У���FM1735Qдָ��Ĳ������ȰѾ�������д��FIFO��
	                                        //����ָ��Ĵ�����д���������
	while(1)				//����жϱ�ʶ
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
		if(temp2&0x20)		//TimerIRq  ��ʱ��ʱ���þ�
		{
		ret=ERROR_NOTAG;		
		break;
		}
//		if(temp2&0x02)		//FIFO�����־
//		{
		
//		temp3=Read_FIFO(temp+rec_len);	//����FIFO����
//		rec_len=rec_len+ temp3;
//		SPIWrite(InterruptRq_Reg,0x02);	
//		break;
//		}
	}

	   if (ret==ERROR_SUCCESS)
	   {
	   	temp3=Read_FIFO(temp+rec_len); //����FIFO����
		rec_len=rec_len+ temp3;
		}
	   return(ret);	
}


void Rf_On()
{

//	SPIWrite(TxControl_Reg,0x5B); //FM1722 Ϊ˫������
	SPIWrite(TxControl_Reg, SPIRead(TxControl_Reg) | 0x03);  
	//0x58|0x03=0x5B,��ֱ��д������   ֱ��д0x5B֮���ֻ�ܶ�A�������ܶ�B��
	delay_100us(500); //�ȴ������ز��ź��ȶ�

}

void Rf_Off()
{
	SPIWrite(TxControl_Reg, SPIRead(TxControl_Reg) & ~0x03);
}

