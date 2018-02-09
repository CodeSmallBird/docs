#include "LPC11XX_SPI.h"
#include "M0_PACK_V101_IO.h"

void spi_Init(void)
{
    LPC_SYSCON->PRESETCTRL |= 0x01;             // 禁止SPI0复位

    LPC_IOCON->SCK_LOC = 0x02;                  // 将SCK0定位于P0.6/SCK0
    LPC_IOCON->PIO0_6 = 0xd2;                   // 配置P0.6为SCK0,上拉
    LPC_IOCON->PIO0_8 = 0xd1;                   // 配置P0.8为MISO0,上拉
    LPC_IOCON->PIO0_9 = 0xd1;                   // 配置P0.9为MOSI0,上拉
    LPC_IOCON->PIO0_2 = 0xd0;                   // 配置P0.2为GPIO(NSS),上拉

    CD_CfgNSS();

    LPC_SSP0->CR0 = 0x02c7;                     // 8位数据传输,SPI,SCR = 3
    LPC_SSP0->CPSR = 2;                         // Fspi = Fmain/(CPSR*(SCR+1)) = Fmain/8
    LPC_SSP0->CR1 = 0x02;                       // SPI使能
}

void spi_SetReg(uint8_t ucRegAddr, uint8_t ucRegVal)
{
    CD_EnNSS();
    LPC_SSP0->DR = ucRegAddr<<1;
    LPC_SSP0->DR = ucRegVal;
    while (!(LPC_SSP0->SR & 0x04));             // 等待接收FIFO不为空
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
    while (!(LPC_SSP0->SR & 0x04));             // 等待接收FIFO不为空
    ucRegVal = LPC_SSP0->DR;
    while (!(LPC_SSP0->SR & 0x04));
    ucRegVal = LPC_SSP0->DR;
    CD_DisNSS();

    return ucRegVal;
}



