/*************************************************************/
//2014.06.03修改版
/*===========================================================*/

#include "LPC11xx.h"
#include "M0_PACK_V101_IO.h"
#include "fm175xx.h"
#include "type_a.h"
#include "type_b.h"
#include "Utility.h"
#include "mifare_card.h"
#include "Uart.h"
#include "string.h"

unsigned char BCC(unsigned char *p);
unsigned char DataDeal(unsigned char *theData);
void FormCal(unsigned char *getdata);
void Uart_Send(void);
unsigned char   system_info[] = "FM17520 READER IC LPC1114/301 DEMO V1.0 2014/07/16";
unsigned char   DefualtKey[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};      //默认卡片验证密钥
unsigned char   DefualtData[16]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16}; 
//默认写入卡片的数据

void Uart_Send(void)
{
	//memcpy(Send_Buff+Send_Buff[DataLength],"\r\n",sizeof("\r\n"));
	
	uartSendStr(Send_Buff+Data, Send_Buff[DataLength]);//+sizeof("\r\n")
}

/*****************************************************************************
** 程序功能：Demo板通过串口接收命令，根据不同的命令编号执行不同的操作----波特率固定19200
** 例程功能：通过SDA引脚的电平进入两种不同模式：  SDA=1-----模式1（卡片自动检测模式）---绿灯常亮
**     此模式下，卡片会一直检测卡片，当有卡片靠近天线时，会读取卡片的ID号并通过串口打印ID号
**                                                SDA=0-----模式2（串口命令控制模式，介绍如下）
** 命令介绍：         0x01： Demo板会返回改例程的版本及相关信息
**                    0x02： Demo板执行激活TypeA卡命令，如果此时有卡片在天线附件则会通过串口打印卡片的ID号
**                    0x03： TypeA卡片密钥认证，卡片被激活后可以通过认证步骤打开卡片的读写权限
**                    0x04： 读TypeA卡片04块数据，读取成功后会通过串口打印读到的16字节数据(16进制格式)
**                    0x05： 向TypeA卡片04块写入16字节数据，写入的是DefualtData数据中的数据，并且返回执行结果
**                    0x06： 读TypeB卡命令，Demo板收到这条命令后，会寻找TypeB类型的卡则读取卡
**                           得ID号并通过串口返回
**                    0x07： 执行循环寻卡程序，进入此模式后，Demo板会直接寻找卡片，当有卡片靠近天线
**                           Demo板会通通LED闪烁指示表示检测到卡片(TypeA卡或者TypeB卡都可以)
** 注意：进入循环寻卡模式后，Demo板就不能接受命令了，要想返回前面的命令模式，则需要重新上电
**
*****************************************************************************/
int main(void)
{
	volatile unsigned char statues;
	unsigned char Card_Type;            //1:TypeA ;  2:TypeB
	unsigned int Rec_len;               //卡片返回数据长度
	  SystemInit();                     //MCU系统初始化
    LED_RedCfg();                     //指示灯控制设置
	  LED_GreenCfg();
    pcd_Init();                       //读卡芯片初始化
		uartInit ();                      //MCU串口初始化
	  statues = MCU_TO_PCD_TEST();			//MCU与读卡芯片SPI通信测试，这里面做了寄存器读写测试  
	if(statues == TRUE){                //测试通过则LED闪一次
    LED_RedOn();
    Delay100us(10000);
    LED_RedOff();
	  CD_CfgSDA();
		
	while(1)
	{
	if(!CD_GetSDA()){                                                 //如果SDA为低电平则进入串口命令控制模式   
		
		LED_GreenOff();                                                 //绿灯灭
		if (GucRcvNew == 1) {                                           // 串口接收到命令            
				GucRcvNew = 0;                                              // 清除串口接收标志                 
				 
		switch(Recv_Buff[Cammand])
		{
			case 0x01://版本确认
				Send_Buff[DataLength]=sizeof(system_info);
				memcpy(Send_Buff+Data,system_info,Send_Buff[DataLength]);
				break;
			
			case 0x02://激活卡（寻卡，防冲突，选卡）
				statues=TypeA_CardActive(picc_atqa,picc_uid,picc_sak);
				if(statues == TRUE){
				memcpy(Send_Buff+Data,picc_uid,4);  //如果是4字节ID号卡则为4，如果是7字节ID号卡则为10
				Send_Buff[DataLength]=4;	}
				else{
					memcpy(Send_Buff+Data,"ERR!",sizeof("ERR!"));
				Send_Buff[DataLength]=sizeof("ERR!");
				}
				break;
			
			case 0x03://Mifare 认证
				
				statues=Mifare_Auth(0x00,0x01,DefualtKey,picc_uid);	//card authentication
				if(statues == TRUE){
					memcpy(Send_Buff+Data,"Auth Ok!",sizeof("Auth Ok!"));
					Send_Buff[DataLength]=sizeof("Auth Ok!");
				}else{
					memcpy(Send_Buff+Data,"ERR!",sizeof("ERR!"));
				Send_Buff[DataLength]=sizeof("ERR!");
				}
				break;

			case 0x04://Mifare 读块
				statues=Mifare_Blockread(0x04,Send_Buff+Data);	//bolck read function(读04块的数据)
			if(statues==1)
				Send_Buff[DataLength]=16;
			else {
				memcpy(Send_Buff+Data,"ERR!",sizeof("ERR!"));
				Send_Buff[DataLength]=sizeof("ERR!");
			}
				break;

			case 0x05://Mifare 写块

				statues=Mifare_Blockwrite(0x04,DefualtData);	 //block write function
			if(statues == TRUE){
				memcpy(Send_Buff+Data,"Write Data Ok!",sizeof("Write Data Ok!"));
				Send_Buff[DataLength]=sizeof("Write Data Ok!");
			}else{
				memcpy(Send_Buff+Data,"ERR!",sizeof("ERR!"));
				Send_Buff[DataLength]=sizeof("ERR!");
			}
				break;

			case 0x06://FM175xx TYPE B 卡激活并获取ID号
				FM175X_SoftReset();	
				Rec_len = 0;
				Pcd_ConfigISOType(1);             //设置TypeB
			  Set_Rf(3);                        //turn on radio
				statues=TypeB_WUP(&Rec_len,Send_Buff+Data,PUPI);//寻卡 Standard	 send request command Standard mode
				if(Rec_len == 0x60)
					{
						statues=TypeB_Select(PUPI,&Rec_len,Send_Buff+Data);
						if(statues == TRUE)
						{
							statues=TypeB_GetUID(&Rec_len,Send_Buff+Data);
							if(statues == TRUE)
							{
								Send_Buff[DataLength]=Rec_len/8;	
							}else
							{
								memcpy(Send_Buff+Data,"GetUidERR!",sizeof("GetUidERR!"));
								Send_Buff[DataLength]=sizeof("GetUidERR!");
							}
						}else
						{
								memcpy(Send_Buff+Data,"Select ERR!",sizeof("Select ERR!"));
								Send_Buff[DataLength]=sizeof("Select ERR!");
						}
				}else
				{
							memcpy(Send_Buff+Data,"WUP ERR!",sizeof("WUP ERR!"));
							Send_Buff[DataLength]=sizeof("WUP ERR!");
				}
				break;
				
				case 0x07://FM175xx 进入自动检测卡片程序，当检测到有卡时LED闪烁指示，直到卡片移走
								memcpy(Send_Buff+Data,"Card Detect Mode!",sizeof("Card Detect Mode!"));
								Send_Buff[DataLength]=sizeof("Card Detect Mode!");
								Uart_Send();
					while(1)   //死循环，会一直执行寻卡操作，要返跳出只能复位Demo板(重新上电)
					{
						
						statues=TypeA_CardActive(picc_atqa,picc_uid,picc_sak);
						if(statues == TRUE){
								LED_RedOn();
								Delay100us(1000);
								LED_RedOff();
								Delay100us(1000);
							}	
						Set_Rf(0);   //turn off radio		
							
					}

			default:
				memcpy(Send_Buff+Data,"Code ERR!",sizeof("Code ERR!"));
				Send_Buff[DataLength]=sizeof("Code ERR!");
				break;
				}
			Uart_Send();              //串口返回命令执行结果信息
		 } 
	 }else{                       //如果SDA为高电平则执行自动检测卡片模式
		 LED_GreenOn();             //绿灯常亮
		 Card_Type = 1;
		 statues=TypeA_CardActive(picc_atqa,picc_uid,picc_sak);
		 if(statues != TRUE){
			 Card_Type = 2;
			 FM175X_SoftReset();
				Rec_len = 0;
				Pcd_ConfigISOType(1);             //设置TypeB
			  Set_Rf(3);   //turn on radio
				statues=TypeB_WUP(&Rec_len,Send_Buff+Data,PUPI);//寻卡 Standard	 send request command Standard mode
				if(statues == TRUE)statues=TypeB_Select(PUPI,&Rec_len,Send_Buff+Data);				 
		 }
				if(statues == TRUE){
						LED_RedOn();
						Delay100us(1000);
						LED_RedOff();
						Delay100us(1000);
					if(Card_Type == 1)memcpy(Send_Buff+Data,picc_uid,4);  //如果是4字节ID号卡则为4，如果是7字节ID号卡则为10
				  else   memcpy(Send_Buff+Data,PUPI,4);
					Send_Buff[DataLength]=4;	
					  Uart_Send();              //串口返回命令执行结果信息
				}					
		Set_Rf(0);   //turn off radio	
	 }
 }
 }
}






