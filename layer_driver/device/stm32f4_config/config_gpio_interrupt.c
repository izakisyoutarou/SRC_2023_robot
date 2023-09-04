#include "config_gpio_interrupt.h"
#include "name_converter.h"
//trigger
//EXTI_Trigger_Rising = 0x08,
//EXTI_Trigger_Falling = 0x0C,
//EXTI_Trigger_Rising_Falling = 0x10

void Init_gpio_interrupt(GPIO_TypeDef *port,uint16_t pin,EXTITrigger_TypeDef trigger) {
	/* Set variables used */
	//GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	//モジュールストップの解除
	RCC_PeriphClock_GPIO(port);

	/* Enable clock for SYSCFG */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	/* Set pin as input */
	/*GPIO_StructInit(&GPIO_InitStructure);						//初期化用構造体にパラメータをセットしていくため、いったん初期値に戻す
	GPIO_InitStructure.GPIO_Pin = pin;							//設定するピンを指定する
	GPIO_InitStructure.GPIO_Mode = mode;						//ピンのモード設定
	GPIO_InitStructure.GPIO_OType = otype;						//出力タイプ設定
	GPIO_InitStructure.GPIO_PuPd = pupd;						//出力ポートをプッシュプルまたはオープンドレインに設定
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//GPIOのスピードを100MHz（最高速）に設定する
	GPIO_Init(port, &GPIO_InitStructure);*/

	/* Tell system that you will use PD0 for EXTI_Line0 */
	SYSCFG_EXTILineConfig(Exti_port_select_source(port),Exti_pin_select_source(pin));
	/*ex) PD0 is connected to EXTI_Line0 */
	EXTI_InitStruct.EXTI_Line=ETIT_line_select(pin);

	/* Enable interrupt */
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	/* Interrupt mode */
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	/* Triggers on rising and falling edge */
	EXTI_InitStruct.EXTI_Trigger = trigger;
	/* Add to EXTI */
	EXTI_Init(&EXTI_InitStruct);

	/* Add IRQ vector to NVIC */
	/* PD0 is connected to EXTI_Line0, which has EXTI0_IRQn vector */

	NVIC_InitStruct.NVIC_IRQChannel = ETIT_irqn_select(pin);

	/* Set priority */
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0x00;
	/* Set sub priority */
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0x00;
	/* Enable interrupt */
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	/* Add to NVIC */
	NVIC_Init(&NVIC_InitStruct);
	return;
}

void Deinit_gpio_interrupt(GPIO_TypeDef *port,uint16_t pin){
	EXTI_InitTypeDef EXTI_InitStruct;
	/* Tell system that you will use PD0 for EXTI_Line0 */
	//SYSCFG_EXTILineConfig(Exti_port_select_source(port),Exti_pin_select_source(pin));
	/*ex) PD0 is connected to EXTI_Line0 */
	EXTI_InitStruct.EXTI_Line=ETIT_line_select(pin);

	/* Enable interrupt */
	EXTI_InitStruct.EXTI_LineCmd = DISABLE;
	/* Interrupt mode */
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	/* Triggers on rising and falling edge */
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	/* Add to EXTI */
	EXTI_Init(&EXTI_InitStruct);
	return;
}

//ex)handler
#if 0
/* Set interrupt handlers */
/* Handle PD0 interrupt */
extern "C" void EXTI0_IRQHandler(void){
	/* Make sure that interrupt flag is set */
	if (EXTI_GetITStatus(EXTI_Line0) != RESET) {
		/* Do your stuff when PD0 is changed */

		printinit();
		/* Clear interrupt flag */
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}

/* Handle PB12 interrupt */
extern "C" void EXTI15_10_IRQHandler(void) {
	/* Make sure that interrupt flag is set */
	if (EXTI_GetITStatus(EXTI_Line12) != RESET) {
		/* Do your stuff when PB12 is changed */
		printinit();
		/* Clear interrupt flag */
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
}
#endif


