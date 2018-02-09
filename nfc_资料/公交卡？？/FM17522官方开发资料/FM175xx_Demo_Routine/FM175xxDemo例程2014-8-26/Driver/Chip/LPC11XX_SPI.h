#ifndef __LPC11XX_SPI_H
#define __LPC11XX_SPI_H

#include "..\Utility\Utility.h"

extern void spi_Init(void);
extern uint8_t spi_SetReg(uint8_t ucRegAddr, uint8_t ucRegVal);
extern uint8_t spi_GetReg(uint8_t ucRegAddr);


#endif
