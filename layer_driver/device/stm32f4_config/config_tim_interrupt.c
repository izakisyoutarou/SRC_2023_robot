#include "config_tim_interrupt.h"

#ifdef PRINTF_AVAILABLE
#include <stdio.h>
#endif
#define PERIOD_LIMIT						65535	//設定できるperiodの最大値
#define PERIOD_UNDER_LIMIT			100		//設定するperiodの最小値
#define PRESCALER_LIMIT					65535	//設定できるプリスケーラの最大値
#define PRESCALER_UNDER_LIMIT		1			//設定できるプリスケーラの最小値
#define FREQUENCY_UNDER_LIMIT	1			//設定できる周波数の最小値
#define RCC_TIMPRE			((MY_RCC->DCKCFGR>>24)&1)			//RCCの分周
#define MY_RCC                ((TIM_INTERRUPT_RCC_TypeDef *) RCC_BASE)		//永谷が設定したRCCのアドレス

//RCCのアドレスの構造体
typedef struct
{
  __IO uint32_t CR;            /*!< RCC clock control register,                                  Address offset: 0x00 */
  __IO uint32_t PLLCFGR;       /*!< RCC PLL configuration register,                              Address offset: 0x04 */
  __IO uint32_t CFGR;          /*!< RCC clock configuration register,                            Address offset: 0x08 */
  __IO uint32_t CIR;           /*!< RCC clock interrupt register,                                Address offset: 0x0C */
  __IO uint32_t AHB1RSTR;      /*!< RCC AHB1 peripheral reset register,                          Address offset: 0x10 */
  __IO uint32_t AHB2RSTR;      /*!< RCC AHB2 peripheral reset register,                          Address offset: 0x14 */
  __IO uint32_t AHB3RSTR;      /*!< RCC AHB3 peripheral reset register,                          Address offset: 0x18 */
  uint32_t      RESERVED0;     /*!< Reserved, 0x1C                                                                    */
  __IO uint32_t APB1RSTR;      /*!< RCC APB1 peripheral reset register,                          Address offset: 0x20 */
  __IO uint32_t APB2RSTR;      /*!< RCC APB2 peripheral reset register,                          Address offset: 0x24 */
  uint32_t      RESERVED1[2];  /*!< Reserved, 0x28-0x2C                                                               */
  __IO uint32_t AHB1ENR;       /*!< RCC AHB1 peripheral clock register,                          Address offset: 0x30 */
  __IO uint32_t AHB2ENR;       /*!< RCC AHB2 peripheral clock register,                          Address offset: 0x34 */
  __IO uint32_t AHB3ENR;       /*!< RCC AHB3 peripheral clock register,                          Address offset: 0x38 */
  uint32_t      RESERVED2;     /*!< Reserved, 0x3C                                                                    */
  __IO uint32_t APB1ENR;       /*!< RCC APB1 peripheral clock enable register,                   Address offset: 0x40 */
  __IO uint32_t APB2ENR;       /*!< RCC APB2 peripheral clock enable register,                   Address offset: 0x44 */
  uint32_t      RESERVED3[2];  /*!< Reserved, 0x48-0x4C                                                               */
  __IO uint32_t AHB1LPENR;     /*!< RCC AHB1 peripheral clock enable in low power mode register, Address offset: 0x50 */
  __IO uint32_t AHB2LPENR;     /*!< RCC AHB2 peripheral clock enable in low power mode register, Address offset: 0x54 */
  __IO uint32_t AHB3LPENR;     /*!< RCC AHB3 peripheral clock enable in low power mode register, Address offset: 0x58 */
  uint32_t      RESERVED4;     /*!< Reserved, 0x5C                                                                    */
  __IO uint32_t APB1LPENR;     /*!< RCC APB1 peripheral clock enable in low power mode register, Address offset: 0x60 */
  __IO uint32_t APB2LPENR;     /*!< RCC APB2 peripheral clock enable in low power mode register, Address offset: 0x64 */
  uint32_t      RESERVED5[2];  /*!< Reserved, 0x68-0x6C                                                               */
  __IO uint32_t BDCR;          /*!< RCC Backup domain control register,                          Address offset: 0x70 */
  __IO uint32_t CSR;           /*!< RCC clock control & status register,                         Address offset: 0x74 */
  uint32_t      RESERVED6[2];  /*!< Reserved, 0x78-0x7C                                                               */
  __IO uint32_t SSCGR;         /*!< RCC spread spectrum clock generation register,               Address offset: 0x80 */
  __IO uint32_t PLLI2SCFGR;    /*!< RCC PLLI2S configuration register,                           Address offset: 0x84 */
  __IO uint32_t PLLSAICFGR;    /*!< 6.3.24 RCC PLL configuration register,                       Address offset: 0x88 */
  __IO uint32_t DCKCFGR;	   /*!< 6.3.25 RCC Dedicated Clock Configuration Register,           Address offset: 0x8c */
} TIM_INTERRUPT_RCC_TypeDef;

/******************************************************************************
*	タイトル ： タイマー割り込み初期設定
*	  関数名 ： Init_PWM
*	  戻り値 ： int型 0:設定できた 1:設定できない
*	   引数1 ： TIM_TypeDef *型 TIMx  TIMx TIMのポインタ
*	   引数2 ： GPIO_TypeDef型 *GPIOx  GPIOx GPIOのポインタ
*	   引数3 ： uint16_t型 pin  GPIO_Pin_x PINの設定
*	   引数4 ： int型 frequency  PWM周波数[Hz](整数)
*	  作成者 ： 永谷 智貴
*	  作成日 ： 2016/1/15
******************************************************************************/
int Init_TIM_interrupt(TIM_TypeDef * TIMx,int frequency)//エラーがあれば1、なければ0をreturnする
{
	long TIM_clock=0;
	int prescaler=0;
	int period=0;
	int calc_retry_flag=1;
	//float error_ratio=0;

//	unsigned short i = 0;
//	Pin_t	pin_state;//

	//システムクロックをRCC_Clocksで取得
	SystemCoreClockUpdate();
	RCC_ClocksTypeDef RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);

	//TIMのクロックの取得
	if((TIM2<=TIMx&&TIMx<=TIM7)||(TIM12<=TIMx&&TIMx<=TIM14)){
		TIM_clock=RCC_Clocks.PCLK1_Frequency*((RCC_TIMPRE+1)*2);	//PCLK1のTIMプリスケーラ倍したらTIM2-7,12-14のクロックが出てくる
	}else{
		TIM_clock=RCC_Clocks.PCLK2_Frequency*((RCC_TIMPRE+1)*2);	//PCLK2のTIMプリスケーラ倍したら上のやつ以外のクロックが出てくる
	}
#ifdef PRINTF_AVAILABLE
	printf("Init_PWM() start.\nTIM_clock:%d,\n",TIM_clock);
#endif


	//上下の設定可能な周波数の中に納まっているか確認
	if(frequency<FREQUENCY_UNDER_LIMIT || frequency>TIM_clock/PRESCALER_UNDER_LIMIT/PERIOD_UNDER_LIMIT)
	{
#ifdef PRINTF_AVAILABLE
		printf("Error. Frequency value out of range. '%d' - '%d' Requested frequency '%d'\n",FREQUENCY_UNDER_LIMIT,TIM_clock/PRESCALER_UNDER_LIMIT/PERIOD_UNDER_LIMIT,frequency);
#endif
		return 1;													//おかしければエラー返して終了
	}

	//prescaler,periodを計算
	while(calc_retry_flag)											//periodが制限内の最大になるまでprescalerを上げているだけ。計算でも出せるけど、見た目だけはこっちのほうがきれい。
	{
		prescaler++;
		period=TIM_clock/prescaler/frequency;
		if(period<=PERIOD_LIMIT) calc_retry_flag=0;
		if(prescaler>=PRESCALER_LIMIT){								//prescalerが上の制限を超しちゃったらエラーを履くけどそうはならない。
#ifdef PRINTF_AVAILABLE
			printf("Error. Prescaler value out of range. '%d'-'%d' \n",PRESCALER_UNDER_LIMIT,PRESCALER_LIMIT);
#endif
			return 1;												//おかしければエラー返して終了
		}
	}
//	error_ratio=fabs(((float)TIM_clock/prescaler/period-(float)frequency)/(float)frequency)*100;//周波数の誤差をパーセントで計算。1%以内には納まる。
	frequency=TIM_clock/prescaler/period;							//設定した数値から算出される周波数　大体同じ。
#ifdef PRINTF_AVAILABLE
	printf("Result: \n period:%d,\n prescaler:%d,\n frequency:%d,\n\n",period,prescaler,frequency);
#endif


	//RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE); //タイマ有効化
	RCC_PeriphClock_TIM(TIMx);
	NVIC_Configuration(Tim_irqn_select(TIMx));

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	// 72MHz -> 18MHz -> 1kHz -> 2Hz ?
	TIM_TimeBaseStructure.TIM_Period = period-1;
	TIM_TimeBaseStructure.TIM_Prescaler = prescaler-1;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;

	// 設定を反映
	TIM_TimeBaseInit( TIMx, &TIM_TimeBaseStructure);

	// 割り込み許可...割り込み要因のORを引数に入れる
	TIM_ITConfig( TIMx, TIM_IT_Update, ENABLE);

	// タイマ2起動
	TIM_Cmd( TIMx, ENABLE);

	/*

	//ここから普通のPWM設定
	//設定に使用する構造体の宣言
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;					//TIM設定用構造体宣言

	//クロック供給
	RCC_PeriphClock_TIM(TIMx);//TIMクロック供給
	//クロック供給とGPIO設定

	//TIM設定
	TIM_TimeBaseStructure.TIM_Period = period-1;					//計算したperiod-1
	TIM_TimeBaseStructure.TIM_Prescaler = prescaler-1;				//計算したprescaler-1
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;			//特に意味は無い
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//カウンターモードアップ設定
	TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;				//高機能タイマー用　基本0
	TIM_TimeBaseInit(TIMx,&TIM_TimeBaseStructure);					//設定書き込み

	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE );

	TIM_Cmd(TIMx,ENABLE);//タイマー有効化

	NVIC_Configuration(Tim_irqn_select(TIMx));
*/
	return 0;
}
