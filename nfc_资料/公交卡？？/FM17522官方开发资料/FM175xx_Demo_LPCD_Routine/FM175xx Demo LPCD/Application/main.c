/*************************************************************/
//2014.06.03�޸İ�
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
unsigned char   DefualtKey[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};      //Ĭ�Ͽ�Ƭ��֤��Կ
unsigned char   DefualtData[16]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16}; 
//Ĭ��д�뿨Ƭ������
 unsigned char Event_Flag             = FALSE;		    //�Ƿ����¼�Ҫ����
 unsigned char Event_FM175XX_IRQ      = FALSE;			//FM175XX IRQ�Ƿ�����IRQ�������ظ��ж�����
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

		CD_ClrIRQ();       //����ж�
		
		Event_Flag = TRUE;
		Event_FM175XX_IRQ = TRUE;
		temp = spi_GetReg(FIFOLevelReg);
		if(LpcdFunc == 1)		 		//LTS 20140319 ֻ���漰��LPCDģʽ�������ж�
		//ԭ��ֻҪ�����жϾͻ���NRSTPDΪ0��1�Ķ�����Ŀǰ�ķ����ǲ�����	
		//�����ԭ�����������Ӱ�죬��Ҫ�޸�
		{
			NRSTPD_CTRL(1);			  //NRST
		}
		Delay100us(200);				  //��ʱ2ms���þ����ȶ�
	
  
} */


/*****************************************************************************
** �����ܣ�Demo��ͨ�����ڽ���������ݲ�ͬ��������ִ�в�ͬ�Ĳ���
** ������ܣ�         0x01�� Demo��᷵�ظ����̵İ汾�������Ϣ
**                    0x02�� Demo��ִ�м���TypeA����������ʱ�п�Ƭ�����߸������ͨ�����ڴ�ӡ��Ƭ��ID��
**                    0x03�� TypeA��Ƭ��Կ��֤����Ƭ����������ͨ����֤����򿪿�Ƭ�Ķ�дȨ��
**                    0x04�� ��TypeA��Ƭ04�����ݣ���ȡ�ɹ����ͨ�����ڴ�ӡ������16�ֽ�����(16���Ƹ�ʽ)
**                    0x05�� ��TypeA��Ƭ04��д��16�ֽ����ݣ�д�����DefualtData�����е����ݣ����ҷ���ִ�н��
**                    0x06�� ��TypeB�����Demo���յ���������󣬻�Ѱ��TypeB���͵Ŀ����ȡ��
**                           ��ID�Ų�ͨ�����ڷ���
**                    0x07�� ִ��ѭ��Ѱ�����򣬽����ģʽ��Demo���ֱ��Ѱ�ҿ�Ƭ�����п�Ƭ��������
**                           Demo���ͨͨLED��˸ָʾ��ʾ��⵽��Ƭ(TypeA������TypeB��������)
** ע�⣺����ѭ��Ѱ��ģʽ��Demo��Ͳ��ܽ��������ˣ�Ҫ�뷵��ǰ�������ģʽ������Ҫ�����ϵ�
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
	  SystemInit();                     //MCUϵͳ��ʼ��
    LED_RedCfg();                     //ָʾ�ƿ�������
	  LED_GreenCfg();
	  CD_CfgIRQ1();
	  uartInit ();
	  //CD_CfgIRQ();     //IRQ�ж����ã��½����ж�
    pcd_Init();                       //����оƬ��ʼ��
		uartInit ();                      //MCU���ڳ�ʼ��
	  CD_CfgSCL();
	  statues = MCU_TO_PCD_TEST();			//MCU�����оƬSPIͨ�Ų��ԣ����������˼Ĵ�����д����  
	//CD_EnIRQ();                     //ʹ��IRQ�ж�
	//	NVIC_EnableIRQ(EINT0_IRQn);
	if(statues == TRUE){                //����ͨ�����LED��һ��
    LED_RedOn();
    Delay100us(10000);
    LED_RedOff();
		
		//FM175XX�ӿ�����
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
	//LPCD��ʼ��
	//----------------------------------------------------
	FM175XX_Initial();
	LpcdParamInit();									//LPCD������ʼ��
	LpcdRegisterInit();       							//LPCD�Ĵ�����ʼ��
	//LpcdAuxSelect(OFF);									//����AUX�۲�ͨ��
	LpcdInitCalibra(&CalibraFlag);						//LPCD��ʼ������

	if (CalibraFlag == TRUE)							//������̳ɹ���������
	{
		LED_GreenOn();
		Delay100us(10000);
		LED_GreenOff();
		
		NRSTPD_CTRL(0);									//����LPCDģʽ
		Event_Flag = FALSE;								//
		Event_FM175XX_IRQ = FALSE;
	//	CD_EnIRQ();                     //ʹ��IRQ�ж�
		//NVIC_EnableIRQ(EINT3_IRQn);
	} 
	else
	{
		LED_GreenOn();                     //ʧ�������˸һ��
    //Delay100us(10000);
   // LED_GreenOff();
		//OLED_ShowString(64,STING_SIZE*2,"LpcdCalFail");  //ROW 2
		//OLED_Refresh_Gram();
	}
	
	//----------------------------------------------------------
	//LPCD���Ѵ���
	//----------------------------------------------------------	
	while(1)
	{		
		if(CD_GetIRQ1()==0){
			
			Event_Flag = TRUE;
		Event_FM175XX_IRQ = TRUE;
		temp = spi_GetReg(FIFOLevelReg);
		if(LpcdFunc == 1)		 		//LTS 20140319 ֻ���漰��LPCDģʽ�������ж�
		//ԭ��ֻҪ�����жϾͻ���NRSTPDΪ0��1�Ķ�����Ŀǰ�ķ����ǲ�����	
		//�����ԭ�����������Ӱ�죬��Ҫ�޸�
		{
			NRSTPD_CTRL(1);			  //NRST
		}
		Delay100us(200);				  //��ʱ2ms���þ����ȶ�
	}
		
		if (Event_FM175XX_IRQ==TRUE)
		{
				Event_FM175XX_IRQ = FALSE;
				//LPCD��֧(��Ҫ���ϵ��жϴ�������ȥ)
				if(LpcdFunc == 1)												//֧��LPCD����
				{
					Delay100us(20);
					GetReg_Ext(JREG_LPCD_IRQ,&lpcd_irq);
					//��ʱ���ѣ��Զ���У
					if ((lpcd_irq & JBIT_AUTO_WUP_IRQ) == JBIT_AUTO_WUP_IRQ )
					{
						//LED_GreenOff();
					
						LpcdAutoWakeUp_IRQHandler();
						
					}	
					//��Ƭ����
					if ((lpcd_irq & JBIT_CARD_IN_IRQ) == JBIT_CARD_IN_IRQ )	   //��ʾ��⵽��Ƭ
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






