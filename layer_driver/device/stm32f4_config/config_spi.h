#ifndef CONFIG_SPI_H
#define CONFIG_SPI_H

#include "cmsis_lib/include/stm32f4xx_spi.h"
#include "stm32f4xx.h"
#include "config_port.h"

void Init_spi(SPI_TypeDef *use_spi,GPIO_TypeDef *GPIOxClk, uint16_t clkPin,GPIO_TypeDef *GPIOxMosi, uint16_t mosiPin,GPIO_TypeDef *GPIOxMiso, uint16_t misoPin,int mode,int direction);

#endif//CONFIG_SPI_H
