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

unsigned char BCC(unsigned char *p);
unsigned char DataDeal(unsigned char *theData);
void FormCal(unsigned char *getdata);
void Uart_Send(void);
unsigned char   system_info[] = "FM17520 READER IC LPC1114/301 DEMO V1.0 2014/07/16";
unsigned char   DefualtKey[6]={0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};      //Ĭ�Ͽ�Ƭ��֤��Կ
unsigned char   DefualtData[16]={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13,0x14,0x15,0x16}; 
//Ĭ��д�뿨Ƭ������

void Uart_Send(void)
{
	//memcpy(Send_Buff+Send_Buff[DataLength],"\r\n",sizeof("\r\n"));
	
	uartSendStr(Send_Buff+Data, Send_Buff[DataLength]);//+sizeof("\r\n")
}

/*****************************************************************************
** �����ܣ�Demo��ͨ�����ڽ���������ݲ�ͬ��������ִ�в�ͬ�Ĳ���----�����ʹ̶�19200
** ���̹��ܣ�ͨ��SDA���ŵĵ�ƽ�������ֲ�ͬģʽ��  SDA=1-----ģʽ1����Ƭ�Զ����ģʽ��---�̵Ƴ���
**     ��ģʽ�£���Ƭ��һֱ��⿨Ƭ�����п�Ƭ��������ʱ�����ȡ��Ƭ��ID�Ų�ͨ�����ڴ�ӡID��
**                                                SDA=0-----ģʽ2�������������ģʽ���������£�
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
	unsigned char Card_Type;            //1:TypeA ;  2:TypeB
	unsigned int Rec_len;               //��Ƭ�������ݳ���
	  SystemInit();                     //MCUϵͳ��ʼ��
    LED_RedCfg();                     //ָʾ�ƿ�������
	  LED_GreenCfg();
    pcd_Init();                       //����оƬ��ʼ��
		uartInit ();                      //MCU���ڳ�ʼ��
	  statues = MCU_TO_PCD_TEST();			//MCU�����оƬSPIͨ�Ų��ԣ����������˼Ĵ�����д����  
	if(statues == TRUE){                //����ͨ����LED��һ��
    LED_RedOn();
    Delay100us(10000);
    LED_RedOff();
	  CD_CfgSDA();
		
	while(1)
	{
	if(!CD_GetSDA()){                                                 //���SDAΪ�͵�ƽ����봮���������ģʽ   
		
		LED_GreenOff();                                                 //�̵���
		if (GucRcvNew == 1) {                                           // ���ڽ��յ�����            
				GucRcvNew = 0;                                              // ������ڽ��ձ�־                 
				 
		switch(Recv_Buff[Cammand])
		{
			case 0x01://�汾ȷ��
				Send_Buff[DataLength]=sizeof(system_info);
				memcpy(Send_Buff+Data,system_info,Send_Buff[DataLength]);
				break;
			
			case 0x02://�����Ѱ��������ͻ��ѡ����
				statues=TypeA_CardActive(picc_atqa,picc_uid,picc_sak);
				if(statues == TRUE){
				memcpy(Send_Buff+Data,picc_uid,4);  //�����4�ֽ�ID�ſ���Ϊ4�������7�ֽ�ID�ſ���Ϊ10
				Send_Buff[DataLength]=4;	}
				else{
					memcpy(Send_Buff+Data,"ERR!",sizeof("ERR!"));
				Send_Buff[DataLength]=sizeof("ERR!");
				}
				break;
			
			case 0x03://Mifare ��֤
				
				statues=Mifare_Auth(0x00,0x01,DefualtKey,picc_uid);	//card authentication
				if(statues == TRUE){
					memcpy(Send_Buff+Data,"Auth Ok!",sizeof("Auth Ok!"));
					Send_Buff[DataLength]=sizeof("Auth Ok!");
				}else{
					memcpy(Send_Buff+Data,"ERR!",sizeof("ERR!"));
				Send_Buff[DataLength]=sizeof("ERR!");
				}
				break;

			case 0x04://Mifare ����
				statues=Mifare_Blockread(0x04,Send_Buff+Data);	//bolck read function(��04�������)
			if(statues==1)
				Send_Buff[DataLength]=16;
			else {
				memcpy(Send_Buff+Data,"ERR!",sizeof("ERR!"));
				Send_Buff[DataLength]=sizeof("ERR!");
			}
				break;

			case 0x05://Mifare д��

				statues=Mifare_Blockwrite(0x04,DefualtData);	 //block write function
			if(statues == TRUE){
				memcpy(Send_Buff+Data,"Write Data Ok!",sizeof("Write Data Ok!"));
				Send_Buff[DataLength]=sizeof("Write Data Ok!");
			}else{
				memcpy(Send_Buff+Data,"ERR!",sizeof("ERR!"));
				Send_Buff[DataLength]=sizeof("ERR!");
			}
				break;

			case 0x06://FM175xx TYPE B �������ȡID��
				FM175X_SoftReset();	
				Rec_len = 0;
				Pcd_ConfigISOType(1);             //����TypeB
			  Set_Rf(3);                        //turn on radio
				statues=TypeB_WUP(&Rec_len,Send_Buff+Data,PUPI);//Ѱ�� Standard	 send request command Standard mode
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
				
				case 0x07://FM175xx �����Զ���⿨Ƭ���򣬵���⵽�п�ʱLED��˸ָʾ��ֱ����Ƭ����
								memcpy(Send_Buff+Data,"Card Detect Mode!",sizeof("Card Detect Mode!"));
								Send_Buff[DataLength]=sizeof("Card Detect Mode!");
								Uart_Send();
					while(1)   //��ѭ������һֱִ��Ѱ��������Ҫ������ֻ�ܸ�λDemo��(�����ϵ�)
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
			Uart_Send();              //���ڷ�������ִ�н����Ϣ
		 } 
	 }else{                       //���SDAΪ�ߵ�ƽ��ִ���Զ���⿨Ƭģʽ
		 LED_GreenOn();             //�̵Ƴ���
		 Card_Type = 1;
		 statues=TypeA_CardActive(picc_atqa,picc_uid,picc_sak);
		 if(statues != TRUE){
			 Card_Type = 2;
			 FM175X_SoftReset();
				Rec_len = 0;
				Pcd_ConfigISOType(1);             //����TypeB
			  Set_Rf(3);   //turn on radio
				statues=TypeB_WUP(&Rec_len,Send_Buff+Data,PUPI);//Ѱ�� Standard	 send request command Standard mode
				if(statues == TRUE)statues=TypeB_Select(PUPI,&Rec_len,Send_Buff+Data);				 
		 }
				if(statues == TRUE){
						LED_RedOn();
						Delay100us(1000);
						LED_RedOff();
						Delay100us(1000);
					if(Card_Type == 1)memcpy(Send_Buff+Data,picc_uid,4);  //�����4�ֽ�ID�ſ���Ϊ4�������7�ֽ�ID�ſ���Ϊ10
				  else   memcpy(Send_Buff+Data,PUPI,4);
					Send_Buff[DataLength]=4;	
					  Uart_Send();              //���ڷ�������ִ�н����Ϣ
				}					
		Set_Rf(0);   //turn off radio	
	 }
 }
 }
}






