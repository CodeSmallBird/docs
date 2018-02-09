#include "LPC11XX_SPI.h"
#include "M0_PACK_V101_IO.h"

void spi_Init(void)
{
    LPC_SYSCON->PRESETCTRL |= 0x01;             // ��ֹSPI0��λ

    LPC_IOCON->SCK_LOC = 0x02;                  // ��SCK0��λ��P0.6/SCK0
    LPC_IOCON->PIO0_6 = 0xd2;                   // ����P0.6ΪSCK0,����
    LPC_IOCON->PIO0_8 = 0xd1;                   // ����P0.8ΪMISO0,����
    LPC_IOCON->PIO0_9 = 0xd1;                   // ����P0.9ΪMOSI0,����
    LPC_IOCON->PIO0_2 = 0xd0;                   // ����P0.2ΪGPIO(NSS),����

    CD_CfgNSS();

    LPC_SSP0->CR0 = 0x02c7;                     // 8λ���ݴ���,SPI,SCR = 3
    LPC_SSP0->CPSR = 2;                         // Fspi = Fmain/(CPSR*(SCR+1)) = Fmain/8
    LPC_SSP0->CR1 = 0x02;                       // SPIʹ��
}

void spi_SetReg(uint8_t ucRegAddr, uint8_t ucRegVal)
{
    CD_EnNSS();
    LPC_SSP0->DR = ucRegAddr<<1;
    LPC_SSP0->DR = ucRegVal;
    while (!(LPC_SSP0->SR & 0x04));             // �ȴ�����FIFO��Ϊ��
    ucRegVal = LPC_SSP0->DR;
    while (!(LPC_SSP0->SR & 0x04));
    ucRegVal = LPC_SSP0->DR;
    CD_DisNSS();
}

uint8_t spi_GetReg(uint8_t ucRegAddr)
{
    unsigned char ucRegVal;

    CD_EnNSS();
    LPC_SSP0->DR = 0x80|(ucRegAddr<<1);
    LPC_SSP0->DR = 0;
    while (!(LPC_SSP0->SR & 0x04));             // �ȴ�����FIFO��Ϊ��
    ucRegVal = LPC_SSP0->DR;
    while (!(LPC_SSP0->SR & 0x04));
    ucRegVal = LPC_SSP0->DR;
    CD_DisNSS();

    return ucRegVal;
}



