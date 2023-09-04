#ifndef CONFIG_GPIO_INTERRUPT_H
#define CONFIG_GPIO_INTERRUPT_H

#include "stm32f4xx.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_conf.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "misc.h"

void Init_gpio_interrupt(GPIO_TypeDef *port,uint16_t pin,EXTITrigger_TypeDef trigger);
void Deinit_gpio_interrupt(GPIO_TypeDef *port,uint16_t pin);
#endif
