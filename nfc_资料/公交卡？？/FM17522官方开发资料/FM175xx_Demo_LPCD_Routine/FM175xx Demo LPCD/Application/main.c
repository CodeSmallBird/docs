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
#include "LPC11XX_SPI.h"

#include "lpcd_reg.h"
#include "lpcd_config.h"
#include "lpcd_config_user.h"
#include "lpcd_regctrl.h"
#include "lpcd_api.h"

unsigned char BCC(unsigned char *p);
unsigned char DataDeal(unsigned char *theData);
void FormCal(unsigned char *getdata);
void Uart_Send(void);
unsigned char   system_info[] = "FM17520 READER IC LPC1114/301 DEMO V1.0 2014/07/16";
unsigned char   DefualtKey[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};      //默认卡片验证密钥
unsigned char   DefualtData[16]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16}; 
//默认写入卡片的数据
 unsigned char Event_Flag             = FALSE;		    //是否有事件要处理
 unsigned char Event_FM175XX_IRQ      = FALSE;			//FM175XX IRQ是否被置起，IRQ的上升沿该中断起来
 unsigned char LpcdFunc = 1;

void Uart_Send(void)
{
	//memcpy(Send_Buff+Send_Buff[DataLength],"\r\n",sizeof("\r\n"));
	
	uartSendStr(Send_Buff+Data, Send_Buff[DataLength]);//+sizeof("\r\n")
}

/*********************************************************************************************************
** Function name:       PIOINT3_IRQHandler
** Descriptions:        ???(????)
**     ????:       ?P3.0???KEY1??,P2.7???BEEP??
**     ?    ?:       ??KEY1,BEEP????
** input parameters:    ?
** output parameters:   ?
** Returned value:      ?
*********************************************************************************************************/
/*void PIOINT0_IRQHandler(void)
{
    
	
	unsigned char temp;

		CD_ClrIRQ();       //清除中断
		
		Event_Flag = TRUE;
		Event_FM175XX_IRQ = TRUE;
		temp = spi_GetReg(FIFOLevelReg);
		if(LpcdFunc == 1)		 		//LTS 20140319 只由涉及到LPCD模式才启动中断
		//原来只要产生中断就会有NRSTPD为0到1的动作，目前改法还是不严密	
		//这个对原来程序可能有影响，需要修改
		{
			NRSTPD_CTRL(1);			  //NRST
		}
		Delay100us(200);				  //延时2ms，让晶振稳定
	
  
} */


/*****************************************************************************
** 程序功能：Demo板通过串口接收命令，根据不同的命令编号执行不同的操作
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
	 //unsigned int Rec_len;
		unsigned char lpcd_irq;
  	unsigned char CalibraFlag;
	  volatile unsigned char temp;
	//unsigned char Extdata[10],i;
	  SystemInit();                     //MCU系统初始化
    LED_RedCfg();                     //指示灯控制设置
	  LED_GreenCfg();
	  CD_CfgIRQ1();
	  uartInit ();
	  //CD_CfgIRQ();     //IRQ中断设置，下降沿中断
    pcd_Init();                       //读卡芯片初始化
		uartInit ();                      //MCU串口初始化
	  CD_CfgSCL();
	  statues = MCU_TO_PCD_TEST();			//MCU与读卡芯片SPI通信测试，这里面做了寄存器读写测试  
	//CD_EnIRQ();                     //使能IRQ中断
	//	NVIC_EnableIRQ(EINT0_IRQn);
	if(statues == TRUE){                //测试通过则红LED闪一次
    LED_RedOn();
    Delay100us(10000);
    LED_RedOff();
		
		//FM175XX接口配置
	//---------------------------------------------------
	
	spi_SetReg(SerialSpeedReg,0x3A);
	Delay100us(10);
		
//		memset(Extdata,0,10);

// 		for(i=1;i<10;i++){
// 		statues= GetReg_Ext(i, Extdata+i-1);
// 		//statues= SetReg_Ext(0x0C, 0x80);
// 		//statues= GetReg_Ext(0x0C, &temp);
// 		}
		
	//---------------------------------------------------
	

	//---------------------------------------------------
	
	
	//----------------------------------------------------
	//LPCD初始化
	//----------------------------------------------------
	FM175XX_Initial();
	LpcdParamInit();									//LPCD参数初始化
	LpcdRegisterInit();       							//LPCD寄存器初始化
	//LpcdAuxSelect(OFF);									//开启AUX观测通道
	LpcdInitCalibra(&CalibraFlag);						//LPCD初始化调教

	if (CalibraFlag == TRUE)							//如果调教成功，则亮灯
	{
		LED_GreenOn();
		Delay100us(10000);
		LED_GreenOff();
		
		NRSTPD_CTRL(0);									//进入LPCD模式
		Event_Flag = FALSE;								//
		Event_FM175XX_IRQ = FALSE;
	//	CD_EnIRQ();                     //使能IRQ中断
		//NVIC_EnableIRQ(EINT3_IRQn);
	} 
	else
	{
		LED_GreenOn();                     //失败则灯闪烁一次
    //Delay100us(10000);
   // LED_GreenOff();
		//OLED_ShowString(64,STING_SIZE*2,"LpcdCalFail");  //ROW 2
		//OLED_Refresh_Gram();
	}
	
	//----------------------------------------------------------
	//LPCD唤醒处理
	//----------------------------------------------------------	
	while(1)
	{		
		if(CD_GetIRQ1()==0){
			
			Event_Flag = TRUE;
		Event_FM175XX_IRQ = TRUE;
		temp = spi_GetReg(FIFOLevelReg);
		if(LpcdFunc == 1)		 		//LTS 20140319 只由涉及到LPCD模式才启动中断
		//原来只要产生中断就会有NRSTPD为0到1的动作，目前改法还是不严密	
		//这个对原来程序可能有影响，需要修改
		{
			NRSTPD_CTRL(1);			  //NRST
		}
		Delay100us(200);				  //延时2ms，让晶振稳定
	}
		
		if (Event_FM175XX_IRQ==TRUE)
		{
				Event_FM175XX_IRQ = FALSE;
				//LPCD分支(需要整合到中断处理函数中去)
				if(LpcdFunc == 1)												//支持LPCD功能
				{
					Delay100us(20);
					GetReg_Ext(JREG_LPCD_IRQ,&lpcd_irq);
					//定时唤醒，自动调校
					if ((lpcd_irq & JBIT_AUTO_WUP_IRQ) == JBIT_AUTO_WUP_IRQ )
					{
						//LED_GreenOff();
					
						LpcdAutoWakeUp_IRQHandler();
						
					}	
					//卡片进场
					if ((lpcd_irq & JBIT_CARD_IN_IRQ) == JBIT_CARD_IN_IRQ )	   //表示检测到卡片
					{
						LED_RedOn();
						Delay100us(10000);
						LED_RedOff();
						LpcdCardIn_IRQHandler();	
					}

				}
			}
		}
 }
}






