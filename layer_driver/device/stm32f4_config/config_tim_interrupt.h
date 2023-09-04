#ifndef CONFIG_TIM_INTERRUPT_H
#define CONFIG_TIM_INTERRUPT_H

#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "system_stm32f4xx.h"

#include "config_port.h"
#include "name_converter.h"

int Init_TIM_interrupt(TIM_TypeDef * TIMx,int frequency);

#endif//CONFIG_TIM_INTERRUPT_H
