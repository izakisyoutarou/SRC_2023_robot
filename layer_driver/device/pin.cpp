#include "stm32f4xx.h"
#include "stm32f4xx_conf.h"

/*
チンパンジーでもわかる！
ichigoplus基板ドライバの作り方！
何をしたい？
１、ピンの配置を変えたい　→15行目へ
２、新しいピンを追加したい　→　ｎ行目へ
３、ピンを消したい　→　ｎ行目へ
４、基板の名前が変わった　→　ｎ行目へ
１、ピンの配置を変えたい
　１、デジタルピンかアナログピンの場合
　　例)#define D0_PIN		  GPIOE,GPIO_Pin_4
　　D0_PINはD0という名前のクラスのピンの名前を記したdefineです。
　　もしD1ならD1_PINとなります。
　　後ろのGPIOE,GPIO_Pin_4はピンの名前で、データシートのPC4を指します
　　例えばPA0を指定したければここにGPIOA,GPIO_Pin_0を入力してください。
　　アナログピンであればA0_PINみたいな名前になっているはずです。
　　これもA0という名前のクラスのピンの名前を記したdefineです。
　　もしA1ならA1_PINとなります。
　　後ろのGPIOE,GPIO_Pin_4はデータシートのPC4を指します
　　例えばPA0を指定したければここにGPIOA,GPIO_Pin_0を入力してください。
　２，エンコーダの場合
　　例)
　　#define ENC0TIM TIM5
　　#define ENC0		GPIOA,GPIO_Pin_0 | GPIO_Pin_1
　　ENC0TIMはEnc0という名前のクラスが使うタイマーを記したdefineです。
　　もしEnc1ならENC1TIMとなります。
　　後ろのTIM5はタイマーの名前で、データシートのTIM5を指します。
　　例えばTIM4を指定したければここにTIM4を入力してください。
　　ENC0はEnc0という名前のクラスが使うピンを記したdefineです。
　　もしEnc1ならENC1となります。
　　後ろのGPIOA,GPIO_Pin_0 | GPIO_Pin_1は使うピンの名前で、データシートのPA0,PA1を指します。
　　例えば、PE1,PE2を指したければGPIOE, GPIO_Pin_0 | GPIO_Pin_1 と入力してください。
　３，PWMの場合
　　例)
　　#define PWM0		    GPIOB,GPIO_Pin_14
　　#define PWM0SET			TIM12,PWM0
　　#define PWM0TIMCH		TIM12,1
　　PWM0はPwm0という名前のクラスが使うピンを記したdefineです。
　　デジタルと同じです。
　　PWM0SETはPwm0という名前のクラスが使うタイマーを記したdefineです。
　　もしPwm1ならPWM1SETとなります。
　　後ろのTIM12は使うタイマーの名前で、データシートのTIM12を指します。
　　PWM0は上で解説したdefineです。もしPWM1SETならPWM1としてください。
　　PWM0TIMCH　はタイマーとチャンネルを記したdefineです。
　　もしPwm1ならPWM1TIMCHとなります。
　　後ろの TIM12,1は TIM12のCH1を指します。　もしTIM8のCH3を指したければTIM8,3と入力してください。
　４，CANの場合
　　例)
　　#define CAN0_PORT GPIOB,GPIO_Pin_9,GPIOB,GPIO_Pin_8
　　#define CAN0_NUMBER 1
　　CAN0_PORTはCan0という名前のクラスが使うピンを記したdefineです。
　　デジタルと同じです。
　　CAN0_NUMBERはCANモジュールの番号を指します。CAN1なら1を、CAN2なら2を入れて下さい
　５，シリアルの場合
　　例)
　　#define SERIAL0TxRx GPIOB,GPIO_Pin_10,GPIOB,GPIO_Pin_11
　　#define SERIAL0CH USART3
　　SERIAL0TxRxはSerial0という名前のクラスが使うピンを記したdefineです。
　　デジタルと同じです
　　SERIAL0CHはSerial0という名前のクラスが使うシリアルモジュールを記したdefineです。
　　USART3ならUSART3を、UART6ならUSART6を入れて下さい
　  シリアルでは割り込みハンドラの名前も変更する必要があります。
　　extern "C" void USART6_IRQHandler(void)のUSART6の部分を利用するモジュールの名前に置き換えてください。
２，新しいピンを追加したい
　先にpin.hppを読んで下さい
　読んできました？
　既に書かれているものと同じ種類のクラスを見つけてコピーします。
　A0とかPwm0とかにはここからここまでがクラスが書いてあるところという印があります。そこからそこまでをコピーしてください。SerialだけはSerial1を使って下さい。
　それが終わったらコピーして貼り付けた部分のクラス名やdefineを全部新たなクラス名やdefineに書き直してください。全部です。一文字も間違えないで下さい。
３、ピンを消したい
　消したいクラスをまるごと消して下さい。
　クラス名::関数名　という名前で関数が宣言されています。同じクラス名のものを全部消してしまいましよう。
　pin.hppのほうも見て下さい。
４、基板の名前が変わった
　char* board_name()という関数があります。この中に基板の名前を書いてください。
 */

extern "C" {
#include "config_usart.h"
#include "config_encoder.h"
#include "config_systick.h"
#include "config_usart.h"
#include "config_pwm.h"
#include "config_port.h"
#include "config_adc.h"
#include "config_can.h"
#include "config_usb.h"
#include "config_i2c.h"
#include "config_iwdg.h"
#include "config_spi.h"
#include "config_tim_interrupt.h"
}

#include "pin.hpp"
using namespace rp_lib;

char* board_name() {
	return (char*)"STM32F4 SRC2022 Auto Main";
}

//--- ENCODER
#define ENC0TIM TIM5
#define ENC0    GPIOA,GPIO_Pin_0, GPIOA,GPIO_Pin_1

#define ENC1TIM TIM4
#define ENC1    GPIOB,GPIO_Pin_6, GPIOB,GPIO_Pin_7

#define ENC2TIM TIM2
#define ENC2	GPIOA,GPIO_Pin_5, GPIOB,GPIO_Pin_3

#define ENC3TIM TIM3
#define ENC3    GPIOA,GPIO_Pin_6, GPIOA,GPIO_Pin_7

//Motor
#define MD0R        GPIOB,GPIO_Pin_14
#define MD0RSET     TIM12,MD0R
#define MD0RTIMCH   TIM12,1
#define MD0L        GPIOB,GPIO_Pin_15
#define MD0LSET     TIM12,MD0L
#define MD0LTIMCH   TIM12,2

#define MD1R        GPIOA,GPIO_Pin_2
#define MD1RSET     TIM9,MD1R
#define MD1RTIMCH   TIM9,1
#define MD1L        GPIOA,GPIO_Pin_3
#define MD1LSET     TIM9,MD1L
#define MD1LTIMCH   TIM9,2

#define MD2R        GPIOC,GPIO_Pin_6
#define MD2RSET     TIM8,MD2R
#define MD2RTIMCH   TIM8,1
#define MD2L        GPIOC,GPIO_Pin_7
#define MD2LSET     TIM8,MD2L
#define MD2LTIMCH   TIM8,2

#define MD3R        GPIOC,GPIO_Pin_8
#define MD3RSET     TIM8,MD3R
#define MD3RTIMCH   TIM8,3
#define MD3L        GPIOC,GPIO_Pin_9
#define MD3LSET     TIM8,MD3L
#define MD3LTIMCH   TIM8,4

#define MD4R        GPIOA,GPIO_Pin_8
#define MD4RSET     TIM1,MD4R
#define MD4RTIMCH   TIM1,1
#define MD4L        GPIOA,GPIO_Pin_9
#define MD4LSET     TIM1,MD4L
#define MD4LTIMCH   TIM1,2

#define MD5R        GPIOA,GPIO_Pin_10
#define MD5RSET     TIM1,MD4R
#define MD5RTIMCH   TIM1,3
#define MD5L        GPIOA,GPIO_Pin_11
#define MD5LSET     TIM1,MD4L
#define MD5LTIMCH   TIM1,4


//Photo
#define Photo0_PIN      GPIOC,GPIO_Pin_0
#define Photo1_PIN      GPIOC,GPIO_Pin_1
#define Photo2_PIN      GPIOC,GPIO_Pin_2
#define Photo3_PIN      GPIOA,GPIO_Pin_4


//LED
#define LED0_PIN      GPIOD,GPIO_Pin_3
#define LED1_PIN      GPIOD,GPIO_Pin_4
#define LED2_PIN      GPIOD,GPIO_Pin_5
#define LED3_PIN      GPIOD,GPIO_Pin_6
#define LED4_PIN      GPIOD,GPIO_Pin_7
#define LED5_PIN      GPIOB,GPIO_Pin_4
#define LED6_PIN      GPIOB,GPIO_Pin_5



/*Valve*/
#define Valve0_PIN      GPIOE,GPIO_Pin_4
#define Valve1_PIN      GPIOE,GPIO_Pin_3
#define Valve2_PIN      GPIOE,GPIO_Pin_2


/*CAN*/
#define CAN0_PORT GPIOB,GPIO_Pin_13,GPIOB,GPIO_Pin_12
#define CAN0_NUMBER 1

/*UART*/

#define SERIAL0TxRx GPIOB,GPIO_Pin_10,GPIOB,GPIO_Pin_11
#define SERIAL0CH USART3

#define SERIAL1TxRx GPIOC,GPIO_Pin_10,GPIOC,GPIO_Pin_11
#define SERIAL1CH UART4

#define SERIAL2TxRx GPIOC,GPIO_Pin_12,GPIOD,GPIO_Pin_2
#define SERIAL2CH UART5

/*I2C*/
#define I2C_PORT_SCL_SDA	GPIOB,GPIO_Pin_8,GPIOB,GPIO_Pin_9
#define I2CMODE I2C1

bool DigitalInterrupt0::handlerMap[16];
DigitalInterruptHandler *DigitalInterrupt0::digitalInterruptHander[16];

// Led0 start

// Pwm0 start
int LED0::_digitalWrite(){
	return GPIO_ReadOutputDataBit(LED0_PIN);
}

void LED0::_digitalWrite(int value){
	if (value == 0)GPIO_ResetBits(LED0_PIN);
	else GPIO_SetBits(LED0_PIN);
}

int LED0::_digitalRead(){
	return GPIO_ReadInputDataBit(LED0_PIN);
}

int LED0::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, LED0_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int LED0::_setupDigitalOut(){
	mode = MODE_DIGITAL;
	Init_port(GPIO_Mode_OUT, LED0_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int LED0::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, LED0_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
	return 0;
}

int LED0::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, LED0_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 0;
}

int LED0::_setupAnalogIn(){
	mode = MODE_ANALOG;
	Init_ADC1(LED0_PIN);
	return 0;
}

float LED0::_analogRead(){
	return (get_ADC1_value(LED0_PIN) / analogResolution());
}
//--- A0 end
//--- A1 start
int LED1::_digitalWrite(){
	return GPIO_ReadOutputDataBit(LED1_PIN);
}

void LED1::_digitalWrite(int value){
	if (value == 0)GPIO_ResetBits(LED1_PIN);
	else GPIO_SetBits(LED1_PIN);
}

int LED1::_digitalRead(){
	return GPIO_ReadInputDataBit(LED1_PIN);
}

int LED1::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, LED1_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int LED1::_setupDigitalOut(){
	mode = MODE_DIGITAL;
	Init_port(GPIO_Mode_OUT, LED1_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int LED1::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, LED1_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
	return 0;
}

int LED1::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, LED1_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 0;
}

int LED1::_setupAnalogIn(){
	mode = MODE_ANALOG;
	Init_ADC1(LED1_PIN);
	return 0;
}

float LED1::_analogRead(){
	return (get_ADC1_value(LED1_PIN) / analogResolution());
}
//--- A1 end
//--- A2 start
int LED2::_digitalWrite(){
	return GPIO_ReadOutputDataBit(LED2_PIN);
}

void LED2::_digitalWrite(int value){
	if (value == 0)GPIO_ResetBits(LED2_PIN);
	else GPIO_SetBits(LED2_PIN);
}

int LED2::_digitalRead(){
	return GPIO_ReadInputDataBit(LED2_PIN);
}

int LED2::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, LED2_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int LED2::_setupDigitalOut(){
	mode = MODE_DIGITAL;
	Init_port(GPIO_Mode_OUT, LED2_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int LED2::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, LED2_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
	return 0;
}

int LED2::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, LED2_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 0;
}

int LED2::_setupAnalogIn(){
	mode = MODE_ANALOG;
	Init_ADC1(LED2_PIN);
	return 0;
}

float LED2::_analogRead(){
	return (get_ADC1_value(LED2_PIN) / analogResolution());
}
//--- A2 end
//--- A3 start
int LED3::_digitalWrite(){
	return GPIO_ReadOutputDataBit(LED3_PIN);
}

void LED3::_digitalWrite(int value){
	if (value == 0)GPIO_ResetBits(LED3_PIN);
	else GPIO_SetBits(LED3_PIN);
}

int LED3::_digitalRead(){
	return GPIO_ReadInputDataBit(LED3_PIN);
}

int LED3::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, LED3_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int LED3::_setupDigitalOut(){
	mode = MODE_DIGITAL;
	Init_port(GPIO_Mode_OUT, LED3_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int LED3::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, LED3_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
	return 0;
}

int LED3::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, LED3_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 0;
}

int LED3::_setupAnalogIn(){
	mode = MODE_ANALOG;
	Init_ADC1(LED3_PIN);
	return 0;
}

float LED3::_analogRead(){
	return (get_ADC1_value(LED3_PIN) / analogResolution());
}
//--- A3 end
//--- A4 start
int LED4::_digitalWrite(){
	return GPIO_ReadOutputDataBit(LED4_PIN);
}

void LED4::_digitalWrite(int value){
	if (value == 0)GPIO_ResetBits(LED4_PIN);
	else GPIO_SetBits(LED4_PIN);
}

int LED4::_digitalRead(){
	return GPIO_ReadInputDataBit(LED4_PIN);
}

int LED4::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, LED4_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int LED4::_setupDigitalOut(){
	mode = MODE_DIGITAL;
	Init_port(GPIO_Mode_OUT, LED4_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int LED4::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, LED4_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
	return 0;
}

int LED4::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, LED4_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 0;
}

int LED4::_setupAnalogIn(){
	mode = MODE_ANALOG;
	Init_ADC1(LED4_PIN);
	return 0;
}

float LED4::_analogRead(){
	return (get_ADC1_value(LED4_PIN) / analogResolution());
}
//--- A4 end
//--- A5 start
int LED5::_digitalWrite(){
	return GPIO_ReadOutputDataBit(LED5_PIN);
}

void LED5::_digitalWrite(int value){
	if (value == 0)GPIO_ResetBits(LED5_PIN);
	else GPIO_SetBits(LED5_PIN);
}

int LED5::_digitalRead(){
	return GPIO_ReadInputDataBit(LED5_PIN);
}

int LED5::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, LED5_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int LED5::_setupDigitalOut(){
	mode = MODE_DIGITAL;
	Init_port(GPIO_Mode_OUT, LED5_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int LED5::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, LED5_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
	return 0;
}

int LED5::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, LED5_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 0;
}

int LED5::_setupAnalogIn(){
	mode = MODE_ANALOG;
	Init_ADC1(LED5_PIN);
	return 0;
}

float LED5::_analogRead(){
	return (get_ADC1_value(LED5_PIN) / analogResolution());
}
//--- A5 end
//--- A6 start
int LED6::_digitalWrite(){
	return GPIO_ReadOutputDataBit(LED6_PIN);
}

void LED6::_digitalWrite(int value){
	if (value == 0)GPIO_ResetBits(LED6_PIN);
	else GPIO_SetBits(LED6_PIN);
}

int LED6::_digitalRead(){
	return GPIO_ReadInputDataBit(LED6_PIN);
}

int LED6::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, LED6_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int LED6::_setupDigitalOut(){
	mode = MODE_DIGITAL;
	Init_port(GPIO_Mode_OUT, LED6_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int LED6::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, LED6_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
	return 0;
}

int LED6::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, LED6_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 0;
}

int LED6::_setupAnalogIn(){
	mode = MODE_ANALOG;
	Init_ADC1(LED6_PIN);
	return 0;
}

float LED6::_analogRead(){
	return (get_ADC1_value(LED6_PIN) / analogResolution());
}
//--- A6 end
//--- Photo0 start
Photo0::Photo0(){
	line=Pin_source(Photo0_PIN);
}
int Photo0::_digitalWrite(){
	return GPIO_ReadOutputDataBit(Photo0_PIN);
}

void Photo0::_digitalWrite(int value){
	if (value == 0) {
		GPIO_ResetBits(Photo0_PIN);
	} else {
		GPIO_SetBits(Photo0_PIN);
	}
}

int Photo0::_digitalRead(){
	return GPIO_ReadInputDataBit(Photo0_PIN);
}

int Photo0::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, Photo0_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Photo0::_setupDigitalOut(){
	Init_port(GPIO_Mode_OUT, Photo0_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Photo0::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, Photo0_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
	return 0;
}

int Photo0::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, Photo0_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 0;
}
//--- Photo0 end

//--- Photo1 start
Photo1::Photo1(){
	line=Pin_source(Photo1_PIN);
}

int Photo1::_digitalWrite(){
	return GPIO_ReadOutputDataBit(Photo1_PIN);
}

void Photo1::_digitalWrite(int value){
	if (value == 0)GPIO_ResetBits(Photo1_PIN);
	else GPIO_SetBits(Photo1_PIN);
}

int Photo1::_digitalRead(){
	return GPIO_ReadInputDataBit(Photo1_PIN);
}

int Photo1::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, Photo1_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Photo1::_setupDigitalOut(){
	Init_port(GPIO_Mode_OUT, Photo1_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Photo1::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, Photo1_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
	return 0;
}

int Photo1::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, Photo1_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 0;
}
//--- Photo1 end

//--- Photo2 start
Photo2::Photo2(){
	line=Pin_source(Photo2_PIN);
}
int Photo2::_digitalWrite(){
	return GPIO_ReadOutputDataBit(Photo2_PIN);
}

void Photo2::_digitalWrite(int value){
	if (value == 0) {
		GPIO_ResetBits(Photo2_PIN);
	} else {
		GPIO_SetBits(Photo2_PIN);
	}
}

int Photo2::_digitalRead(){
	return GPIO_ReadInputDataBit(Photo2_PIN);
}

int Photo2::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, Photo2_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Photo2::_setupDigitalOut(){
	Init_port(GPIO_Mode_OUT, Photo2_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Photo2::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, Photo2_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
	return 0;
}

int Photo2::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, Photo2_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 0;
}
//--- Photo2 end

//--- Photo3 start
Photo3::Photo3(){
	line=Pin_source(Photo3_PIN);
}
int Photo3::_digitalWrite(){
	return GPIO_ReadOutputDataBit(Photo3_PIN);
}

void Photo3::_digitalWrite(int value){
	if (value == 0) {
		GPIO_ResetBits(Photo3_PIN);
	} else {
		GPIO_SetBits(Photo3_PIN);
	}
}

int Photo3::_digitalRead(){
	return GPIO_ReadInputDataBit(Photo3_PIN);
}

int Photo3::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, Photo3_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Photo3::_setupDigitalOut(){
	Init_port(GPIO_Mode_OUT, Photo3_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Photo3::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, Photo3_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
	return 0;
}

int Photo3::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, Photo3_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 0;
}
//--- Photo3 end
//--- Valve0 start
Valve0::Valve0(){
	line=Pin_source(Valve0_PIN);
}
int Valve0::_digitalWrite(){
	return GPIO_ReadOutputDataBit(Valve0_PIN);
}

void Valve0::_digitalWrite(int value){
	if (value == 0) {
		GPIO_ResetBits(Valve0_PIN);
	} else {
		GPIO_SetBits(Valve0_PIN);
	}
}

int Valve0::_digitalRead(){
	return GPIO_ReadInputDataBit(Valve0_PIN);
}

int Valve0::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, Valve0_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Valve0::_setupDigitalOut(){
	Init_port(GPIO_Mode_OUT, Valve0_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Valve0::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, Valve0_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
	return 0;
}

int Valve0::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, Valve0_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 0;
}
//--- Valve0 end
//--- Valve1 start
Valve1::Valve1(){
	line=Pin_source(Valve1_PIN);
}
int Valve1::_digitalWrite(){
	return GPIO_ReadOutputDataBit(Valve1_PIN);
}

void Valve1::_digitalWrite(int value){
	if (value == 0) {
		GPIO_ResetBits(Valve1_PIN);
	} else {
		GPIO_SetBits(Valve1_PIN);
	}
}

int Valve1::_digitalRead(){
	return GPIO_ReadInputDataBit(Valve1_PIN);
}

int Valve1::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, Valve1_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Valve1::_setupDigitalOut(){
	Init_port(GPIO_Mode_OUT, Valve1_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Valve1::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, Valve1_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
	return 0;
}

int Valve1::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, Valve1_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 0;
}
//--- Valve1 end
//--- Valve2 start
Valve2::Valve2(){
	line=Pin_source(Valve2_PIN);
}
int Valve2::_digitalWrite(){
	return GPIO_ReadOutputDataBit(Valve2_PIN);
}

void Valve2::_digitalWrite(int value){
	if (value == 0) {
		GPIO_ResetBits(Valve2_PIN);
	} else {
		GPIO_SetBits(Valve2_PIN);
	}
}

int Valve2::_digitalRead(){
	return GPIO_ReadInputDataBit(Valve2_PIN);
}

int Valve2::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, Valve2_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Valve2::_setupDigitalOut(){
	Init_port(GPIO_Mode_OUT, Valve2_PIN, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Valve2::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, Valve2_PIN, GPIO_PuPd_UP, GPIO_OType_PP);
	return 0;
}

int Valve2::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, Valve2_PIN, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 0;
}
//--- Valve2 end

//---MD start
int Md0r::_digitalWrite(){
	return GPIO_ReadOutputDataBit(MD0R);
}

void Md0r::_digitalWrite(int value){
	if (value == 0)GPIO_ResetBits(MD0R);
	else GPIO_SetBits(MD0R);
}

int Md0r::_digitalRead(){
	return GPIO_ReadInputDataBit(MD0R);
}

int Md0r::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, MD0R, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Md0r::_setupDigitalOut(){
	mode = MODE_DIGITAL;
	Init_port(GPIO_Mode_OUT, MD0R, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Md0r::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, MD0R, GPIO_PuPd_UP, GPIO_OType_PP);
	return 0;
}

int Md0r::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, MD0R, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 0;
}

int Md0r::_setupPwmOut(float frequency, float duty){
	int i;
	mode = MODE_PWM;
	i = Init_PWM(MD0RSET, frequency);
	Set_duty(MD0RTIMCH, duty);
	return i;
}

void Md0r::_pwmWrite(float duty){
	Set_duty(MD0RTIMCH, duty);
}

float Md0r::_pwmWrite(){
	return Read_duty(MD0RTIMCH);
}
//--- Md0r end

//--- Md0l start
int Md0l::_digitalWrite(){
	return GPIO_ReadOutputDataBit(MD0L);
}

void Md0l::_digitalWrite(int value){
	if (value == 0)GPIO_ResetBits(MD0L);
	else GPIO_SetBits(MD0L);
}

int Md0l::_digitalRead(){
	return GPIO_ReadInputDataBit(MD0L);
}

int Md0l::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, MD0L, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Md0l::_setupDigitalOut(){
	mode = MODE_DIGITAL;
	Init_port(GPIO_Mode_OUT, MD0L, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Md0l::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, MD0L, GPIO_PuPd_UP, GPIO_OType_PP);
	return 0;
}

int Md0l::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, MD0L, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 0;
}

int Md0l::_setupPwmOut(float frequency, float duty){
	int i;
	mode = MODE_PWM;
	i = Init_PWM(MD0LSET, frequency);
	Set_duty(MD0LTIMCH, duty);
	return i;
}

void Md0l::_pwmWrite(float duty){
	Set_duty(MD0LTIMCH, duty);
}

float Md0l::_pwmWrite(){
	return Read_duty(MD0LTIMCH);
}
//--- Md0l end

//--- Md1r start
int Md1r::_digitalWrite(){
	return GPIO_ReadOutputDataBit(MD1R);
}

void Md1r::_digitalWrite(int value){
	if (value == 0)GPIO_ResetBits(MD1R);
	else GPIO_SetBits(MD1R);
}

int Md1r::_digitalRead(){
	return GPIO_ReadInputDataBit(MD1R);
}

int Md1r::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, MD1R, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Md1r::_setupDigitalOut(){
	mode = MODE_DIGITAL;
	Init_port(GPIO_Mode_OUT, MD1R, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Md1r::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, MD1R, GPIO_PuPd_UP, GPIO_OType_PP);
	return 0;
}

int Md1r::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, MD1R, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 0;
}

int Md1r::_setupPwmOut(float frequency, float duty){
	int i;
	mode = MODE_PWM;
	i = Init_PWM(MD1RSET, frequency);
	Set_duty(MD1RTIMCH, duty);
	return i;
}

void Md1r::_pwmWrite(float duty){
	Set_duty(MD1RTIMCH, duty);
}

float Md1r::_pwmWrite(){
	return Read_duty(MD1RTIMCH);
}
//--- Md1r end

//--- Md1lstart
int Md1l::_digitalWrite(){
	return GPIO_ReadOutputDataBit(MD1L);
}

void Md1l::_digitalWrite(int value){
	if (value == 0)GPIO_ResetBits(MD1L);
	else GPIO_SetBits(MD1L);
}

int Md1l::_digitalRead(){
	return GPIO_ReadInputDataBit(MD1L);
}

int Md1l::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, MD1L, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Md1l::_setupDigitalOut(){
	mode = MODE_DIGITAL;
	Init_port(GPIO_Mode_OUT, MD1L, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Md1l::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, MD1L, GPIO_PuPd_UP, GPIO_OType_PP);
	return 0;
}

int Md1l::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, MD1L, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 0;
}

int Md1l::_setupPwmOut(float frequency, float duty){
	int i;
	mode = MODE_PWM;
	i = Init_PWM(MD1LSET, frequency);
	Set_duty(MD1LTIMCH, duty);
	return i;
}

void Md1l::_pwmWrite(float duty){
	Set_duty(MD1LTIMCH, duty);
}

float Md1l::_pwmWrite(){
	return Read_duty(MD1LTIMCH);
}
//--- Md1l end

//--- Md2r start
int Md2r::_digitalWrite(){
	return GPIO_ReadOutputDataBit(MD2R);
}

void Md2r::_digitalWrite(int value){
	if (value == 0)GPIO_ResetBits(MD2R);
	else GPIO_SetBits(MD2R);
}

int Md2r::_digitalRead(){
	return GPIO_ReadInputDataBit(MD2R);
}

int Md2r::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, MD2R, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Md2r::_setupDigitalOut(){
	mode = MODE_DIGITAL;
	Init_port(GPIO_Mode_OUT, MD2R, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Md2r::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, MD2R, GPIO_PuPd_UP, GPIO_OType_PP);
	return 0;
}

int Md2r::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, MD2R, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 0;
}

int Md2r::_setupPwmOut(float frequency, float duty){
	int i;
	mode = MODE_PWM;
	i = Init_PWM(MD2RSET, frequency);
	Set_duty(MD2RTIMCH, duty);
	return i;
}

void Md2r::_pwmWrite(float duty){
	Set_duty(MD2RTIMCH, duty);
}

float Md2r::_pwmWrite(){
	return Read_duty(MD2RTIMCH);
}
//--- Md2r end

//--- Md2l start
int Md2l::_digitalWrite(){
	return GPIO_ReadOutputDataBit(MD2L);
}

void Md2l::_digitalWrite(int value){
	if (value == 0)GPIO_ResetBits(MD2L);
	else GPIO_SetBits(MD2L);
}

int Md2l::_digitalRead(){
	return GPIO_ReadInputDataBit(MD2L);
}

int Md2l::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, MD2L, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Md2l::_setupDigitalOut(){
	mode = MODE_DIGITAL;
	Init_port(GPIO_Mode_OUT, MD2L, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Md2l::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, MD2L, GPIO_PuPd_UP, GPIO_OType_PP);
	return 0;
}

int Md2l::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, MD2L, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 0;
}

int Md2l::_setupPwmOut(float frequency, float duty){
	int i;
	mode = MODE_PWM;
	i = Init_PWM(MD2LSET, frequency);
	Set_duty(MD2LTIMCH, duty);
	return i;
}

void Md2l::_pwmWrite(float duty){
	Set_duty(MD2LTIMCH, duty);
}

float Md2l::_pwmWrite(){
	return Read_duty(MD2LTIMCH);
}
//--- Md2l end
//--- Md3r start
int Md3r::_digitalWrite(){
	return GPIO_ReadOutputDataBit(MD3R);
}

void Md3r::_digitalWrite(int value){
	if (value == 0)GPIO_ResetBits(MD3R);
	else GPIO_SetBits(MD3R);
}

int Md3r::_digitalRead(){
	return GPIO_ReadInputDataBit(MD3R);
}

int Md3r::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, MD3R, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Md3r::_setupDigitalOut(){
	mode = MODE_DIGITAL;
	Init_port(GPIO_Mode_OUT, MD3R, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Md3r::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, MD3R, GPIO_PuPd_UP, GPIO_OType_PP);
	return 0;
}

int Md3r::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, MD3R, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 0;
}

int Md3r::_setupPwmOut(float frequency, float duty){
	int i;
	mode = MODE_PWM;
	i = Init_PWM(MD3RSET, frequency);
	Set_duty(MD3RTIMCH, duty);
	return i;
}

void Md3r::_pwmWrite(float duty){
	Set_duty(MD3RTIMCH, duty);
}

float Md3r::_pwmWrite(){
	return Read_duty(MD3RTIMCH);
}
//--- Md3r end
//--- Md3lstart
int Md3l::_digitalWrite(){
	return GPIO_ReadOutputDataBit(MD3L);
}

void Md3l::_digitalWrite(int value){
	if (value == 0)GPIO_ResetBits(MD3L);
	else GPIO_SetBits(MD3L);
}

int Md3l::_digitalRead(){
	return GPIO_ReadInputDataBit(MD3L);
}

int Md3l::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, MD3L, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Md3l::_setupDigitalOut(){
	mode = MODE_DIGITAL;
	Init_port(GPIO_Mode_OUT, MD3L, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 0;
}

int Md3l::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, MD3L, GPIO_PuPd_UP, GPIO_OType_PP);
	return 0;
}

int Md3l::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, MD3L, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 0;
}

int Md3l::_setupPwmOut(float frequency, float duty){
	int i;
	mode = MODE_PWM;
	i = Init_PWM(MD3LSET, frequency);
	Set_duty(MD3LTIMCH, duty);
	return i;
}

void Md3l::_pwmWrite(float duty){
	Set_duty(MD3LTIMCH, duty);
}

float Md3l::_pwmWrite(){
	return Read_duty(MD3LTIMCH);
}
//--- Md3l end

//--- Md4r start
int Md4r::_digitalWrite(){
	return GPIO_ReadOutputDataBit(MD4R);
}

void Md4r::_digitalWrite(int value){
	if (value == 4)GPIO_ResetBits(MD4R);
	else GPIO_SetBits(MD4R);
}

int Md4r::_digitalRead(){
	return GPIO_ReadInputDataBit(MD4R);
}

int Md4r::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, MD4R, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 4;
}

int Md4r::_setupDigitalOut(){
	mode = MODE_DIGITAL;
	Init_port(GPIO_Mode_OUT, MD4R, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 4;
}

int Md4r::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, MD4R, GPIO_PuPd_UP, GPIO_OType_PP);
	return 4;
}

int Md4r::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, MD4R, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 4;
}

int Md4r::_setupPwmOut(float frequency, float duty){
	int i;
	mode = MODE_PWM;
	i = Init_PWM(MD4RSET, frequency);
	Set_duty(MD4RTIMCH, duty);
	return i;
}

void Md4r::_pwmWrite(float duty){
	Set_duty(MD4RTIMCH, duty);
}

float Md4r::_pwmWrite(){
	return Read_duty(MD4RTIMCH);
}
//--- Md4r end
//--- Md4l start
int Md4l::_digitalWrite(){
	return GPIO_ReadOutputDataBit(MD4L);
}

void Md4l::_digitalWrite(int value){
	if (value == 4)GPIO_ResetBits(MD4L);
	else GPIO_SetBits(MD4L);
}

int Md4l::_digitalRead(){
	return GPIO_ReadInputDataBit(MD4L);
}

int Md4l::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, MD4L, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 4;
}

int Md4l::_setupDigitalOut(){
	mode = MODE_DIGITAL;
	Init_port(GPIO_Mode_OUT, MD4L, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 4;
}

int Md4l::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, MD4L, GPIO_PuPd_UP, GPIO_OType_PP);
	return 4;
}

int Md4l::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, MD4L, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 4;
}

int Md4l::_setupPwmOut(float frequency, float duty){
	int i;
	mode = MODE_PWM;
	i = Init_PWM(MD4LSET, frequency);
	Set_duty(MD4LTIMCH, duty);
	return i;
}

void Md4l::_pwmWrite(float duty){
	Set_duty(MD4LTIMCH, duty);
}

float Md4l::_pwmWrite(){
	return Read_duty(MD4LTIMCH);
}
//--- Md4l end
//--- Md5r start
int Md5r::_digitalWrite(){
	return GPIO_ReadOutputDataBit(MD5R);
}

void Md5r::_digitalWrite(int value){
	if (value == 4)GPIO_ResetBits(MD5R);
	else GPIO_SetBits(MD5R);
}

int Md5r::_digitalRead(){
	return GPIO_ReadInputDataBit(MD5R);
}

int Md5r::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, MD5R, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 4;
}

int Md5r::_setupDigitalOut(){
	mode = MODE_DIGITAL;
	Init_port(GPIO_Mode_OUT, MD5R, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 4;
}

int Md5r::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, MD5R, GPIO_PuPd_UP, GPIO_OType_PP);
	return 4;
}

int Md5r::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, MD5R, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 4;
}

int Md5r::_setupPwmOut(float frequency, float duty){
	int i;
	mode = MODE_PWM;
	i = Init_PWM(MD5RSET, frequency);
	Set_duty(MD5RTIMCH, duty);
	return i;
}

void Md5r::_pwmWrite(float duty){
	Set_duty(MD5RTIMCH, duty);
}

float Md5r::_pwmWrite(){
	return Read_duty(MD5RTIMCH);
}
//--- Md5r end
//--- Md5l start
int Md5l::_digitalWrite(){
	return GPIO_ReadOutputDataBit(MD5L);
}

void Md5l::_digitalWrite(int value){
	if (value == 4)GPIO_ResetBits(MD5L);
	else GPIO_SetBits(MD5L);
}

int Md5l::_digitalRead(){
	return GPIO_ReadInputDataBit(MD5L);
}

int Md5l::_setupDigitalIn(){
	Init_port(GPIO_Mode_IN, MD5L, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 4;
}

int Md5l::_setupDigitalOut(){
	mode = MODE_DIGITAL;
	Init_port(GPIO_Mode_OUT, MD5L, GPIO_PuPd_NOPULL, GPIO_OType_PP);
	return 4;
}

int Md5l::_setupDigitalInPullUp(){
	Init_port(GPIO_Mode_IN, MD5L, GPIO_PuPd_UP, GPIO_OType_PP);
	return 4;
}

int Md5l::_setupDigitalInPullDown(){
	Init_port(GPIO_Mode_IN, MD5L, GPIO_PuPd_DOWN, GPIO_OType_PP);
	return 4;
}

int Md5l::_setupPwmOut(float frequency, float duty){
	int i;
	mode = MODE_PWM;
	i = Init_PWM(MD5LSET, frequency);
	Set_duty(MD5LTIMCH, duty);
	return i;
}

void Md5l::_pwmWrite(float duty){
	Set_duty(MD5LTIMCH, duty);
}

float Md5l::_pwmWrite(){
	return Read_duty(MD5LTIMCH);
}

//--- Enc0 start
namespace encoder {
int Enc0::setup()
{
	Init_encoder(ENC0TIM, ENC0);
	return 0;
}

void Enc0::cycle() {
	count_ = is_reverse ? -Select_encoder_count(ENC0TIM) : Select_encoder_count(ENC0TIM);
	radian_ = (float)count_ / cpr_ * 2.f * f_pi;
}
//--- Enc0 end
//--- Enc1 start
int Enc1::setup()
{
	Init_encoder(ENC1TIM, ENC1);
	return 0;
}

void Enc1::cycle() {
	count_ = is_reverse ? -Select_encoder_count(ENC1TIM) : Select_encoder_count(ENC1TIM);
	radian_ = (float)count_ / cpr_ * 2.f * f_pi;
}
//--- Enc1 end
//--- Enc2 start
int Enc2::setup()
{
	Init_encoder(ENC2TIM, ENC2);
	return 0;
}

void Enc2::cycle() {
	count_ = is_reverse ? -Select_encoder_count(ENC2TIM) : Select_encoder_count(ENC2TIM);
	radian_ = (float)count_ / cpr_ * 2.f * f_pi;
}
//--- Enc2 end
//--- Enc3 start
int Enc3::setup()
{
	Init_encoder(ENC3TIM, ENC3);
	return 0;
}

void Enc3::cycle() {
	count_ = is_reverse ? -Select_encoder_count(ENC3TIM) : Select_encoder_count(ENC3TIM);
	radian_ = (float)count_ / cpr_ * 2.f * f_pi;
}
}

//--- Enc3 end
//--- Serial0 start
int serialParitySelect(int parity){
	if(parity==Serial::SERIAL_PARITY_EVEN){
		return USART_Parity_Even;
	}else if(parity==Serial::SERIAL_PARITY_ODD){
		return USART_Parity_Odd;
	}else
		return USART_Parity_No;
}

int serialLengthSelect(int wordLength){
	if(wordLength==9){
		return USART_WordLength_9b;
	}else
		return USART_WordLength_8b;
}

SerialHandler *Serial0::Handler;
RingBuffer<char,256> Serial0::txBuf;
int Serial0::isTransmitting=0;

int Serial0::readNewLine=Serial0::SERIAL0_NEWLINE_NULL;
int Serial0::writeNewLine=Serial0::SERIAL0_NEWLINE_NULL;
char Serial0::oldWriteChar=0;
char Serial0::oldReadChar=0;
#define SERIAL0_TXINT

int Serial0::_setup(int baudrate, SerialHandler &HandlerArg, int parity, int wordLength)
{
#ifdef SERIAL0_TXINT
	Init_USARTi(SERIAL0CH,baudrate,SERIAL0TxRx, serialParitySelect(parity), serialLengthSelect(wordLength));
#else
	Init_USART(SERIAL0CH,baudrate,SERIAL0TxRx, serialParitySelect(parity), serialLengthSelect(wordLength));
#endif
	Handler=&HandlerArg;
	Handler->serialHandlerSetup(this);
	return 0;
}

void Serial0::writeChar(char value)
{
	transmit(value);
}

void serial0_rxInterrupt(){
	char value=USART_ReceiveData(SERIAL0CH);
	//char value1=0;
	/*		if(Serial0::writeNewLine==Serial0::SERIAL0_NEWLINE_LF){
			if(value=='\r'){
				if(Serial0::oldWriteChar!='\n')value='\n';
			}
			Serial0::oldWriteChar=value;
		}
		if(Serial0::writeNewLine==Serial0::SERIAL0_NEWLINE_CR){
			if(value=='\n'){
				if(Serial0::oldWriteChar!='\r')value='\r';
			}
			Serial0::oldWriteChar=value;
		}
		if(Serial0::writeNewLine==Serial0::SERIAL0_NEWLINE_CRLF){
			if(value=='\n'||value=='\r'){
				if(Serial0::oldWriteChar!='\n'||Serial0::oldWriteChar!='\r'){
					Serial0::oldWriteChar=value;
					value='\r';
					value1='\n';
				}
			}else{
				Serial0::oldWriteChar=value;
			}
		}*/
	Serial0::Handler->serialReadChar(value);
	//	if(value1)Serial0::Handler->serialReadChar(value1);
	//USART_ClearFlag(SERIAL0CH,USART_FLAG_RXNE);
	USART_ClearITPendingBit(SERIAL0CH,USART_IT_RXNE);

}

void serial0_txInterrupt(){
#ifdef SERIAL0_TXINT
	if(Serial0::txBuf.isEmpty()==0) USART_SendData(SERIAL0CH,Serial0::txBuf.read());
	if(Serial0::txBuf.isEmpty()==1) Serial0::isTransmitting=0;
	//USART_ClearFlag(SERIAL0CH,USART_FLAG_TC);
	USART_ClearITPendingBit(SERIAL0CH,USART_IT_TC);
#endif
}
void Serial0::transmit(char value){
#ifdef SERIAL0_TXINT
	char value1=0;
	if(writeNewLine==SERIAL0_NEWLINE_LF){
		if(value=='\r'){
			if(oldWriteChar!='\n')value='\n';
		}
		oldWriteChar=value;
	}
	if(writeNewLine==SERIAL0_NEWLINE_CR){
		if(value=='\n'){
			if(oldWriteChar!='\r')value='\r';
		}
		oldWriteChar=value;
	}
	if(writeNewLine==SERIAL0_NEWLINE_CRLF){
		if(value=='\n'||value=='\r'){
			if(oldWriteChar!='\n'||oldWriteChar!='\r'){
				oldWriteChar=value;
				value='\r';
				value1='\n';
			}
		}else{
			oldWriteChar=value;
		}
	}
	if(isTransmitting==0){
		USART_SendData(SERIAL0CH, value);
		//if(value1)USART_SendData(SERIAL0CH, value1);
		if(value1)while(txBuf.write(value1));
		isTransmitting=1;
	}else{
		while(txBuf.write(value));
		if(value1)while(txBuf.write(value1));
	}
#if 0
	if(isTransmitting==0){
		USART_SendData(SERIAL0CH, value);
		if(value=='\n')txBuf.write('\r');
		if(value=='\r')txBuf.write('\n');
		isTransmitting=1;
	}else{
		while(txBuf.write(value));
		if(value=='\n')while(txBuf.write('\r'));
		if(value=='\r')while(txBuf.write('\n'));
	}
#endif

#else
	while(USART_GetFlagStatus(SERIAL0CH, USART_FLAG_TXE) == RESET);
	USART_SendData(SERIAL0CH, value);
	if(value=='\n'){
		while(USART_GetFlagStatus(SERIAL0CH, USART_FLAG_TXE) == RESET);
		USART_SendData(SERIAL0CH, '\r');
	}
	if(value=='\r'){
		while(USART_GetFlagStatus(SERIAL0CH, USART_FLAG_TXE) == RESET);
		USART_SendData(SERIAL0CH, '\n');
	}
#endif
}

void Serial0::setReadNewLineLF(){readNewLine=SERIAL0_NEWLINE_LF;}
void Serial0::setReadNewLineCR(){readNewLine=SERIAL0_NEWLINE_CR;}
void Serial0::setReadNewLineCRLF(){readNewLine=SERIAL0_NEWLINE_CRLF;}
void Serial0::setWriteNewLineLF(){writeNewLine=SERIAL0_NEWLINE_LF;}
void Serial0::setWriteNewLineCR(){writeNewLine=SERIAL0_NEWLINE_CR;}
void Serial0::setWriteNewLineCRLF(){writeNewLine=SERIAL0_NEWLINE_CRLF;}


extern "C" void USART3_IRQHandler(void) {
	//serial0_rxInterrupt();
	//if (USART_GetFlagStatus(SERIAL0CH, USART_FLAG_TC) == SET)serial0_txInterrupt();
	//if (USART_GetFlagStatus(SERIAL0CH, USART_IT_RXNE) == SET)serial0_rxInterrupt();
	if (USART_GetITStatus(SERIAL0CH, USART_IT_TC) == SET)serial0_txInterrupt();
	if (USART_GetITStatus(SERIAL0CH, USART_IT_RXNE) == SET)serial0_rxInterrupt();
}

void std_char_out(char value) {
	//	while(USART_GetFlagStatus(SERIAL0CH, USART_FLAG_TXE) == RESET);
	//	USART_SendData(SERIAL0CH, value);
	Serial0::transmit(value);
}

void std_char_out_with_flush(char value){
	while(USART_GetFlagStatus(SERIAL0CH, USART_FLAG_TXE) == RESET);
	USART_SendData(SERIAL0CH, value);
	while(USART_GetFlagStatus(SERIAL0CH, USART_FLAG_TXE) == RESET);
}

int std_char_out_setup() {
	Serial0::Handler = new SerialHandler();
#ifdef SERIAL0_TXINT
	Init_USARTi(SERIAL0CH, 9600, SERIAL0TxRx, 0, 8);
#else
	Init_USART(SERIAL0CH, 9600, SERIAL0TxRx, 0, 8);
#endif
	return 0;
}

SerialHandler *Serial1::Handler;
RingBuffer<char, 256> Serial1::txBuf;
int Serial1::isTransmitting = 0;
#define SERIAL1_TXINT
int Serial1::_setup(int baudrate, SerialHandler &HandlerArg, int parity, int wordLength)
{
#ifdef SERIAL1_TXINT
	Init_USARTi(SERIAL1CH, baudrate, SERIAL1TxRx, serialParitySelect(parity), serialLengthSelect(wordLength));
#else
	Init_USART(SERIAL1CH, baudrate, SERIAL1TxRx,serialParitySelect(parity), serialLengthSelect(wordLength));
#endif
	Handler = &HandlerArg;
	Handler->serialHandlerSetup(this);
	return 0;
}

void Serial1::writeChar(char value)
{
	transmit(value);
}

void serial1_rxInterrupt() {
	Serial1::Handler->serialReadChar(USART_ReceiveData(SERIAL1CH));
	//USART_ClearFlag(SERIAL1CH, USART_FLAG_RXNE);
	USART_ClearITPendingBit(SERIAL1CH,USART_IT_RXNE);
}

void serial1_txInterrupt() {
#ifdef SERIAL1_TXINT
	if (Serial1::txBuf.isEmpty() == 0) USART_SendData(SERIAL1CH, Serial1::txBuf.read());
	if (Serial1::txBuf.isEmpty() == 1) Serial1::isTransmitting = 0;
	//USART_ClearFlag(SERIAL1CH, USART_FLAG_TC);
	USART_ClearITPendingBit(SERIAL1CH,USART_IT_TC);
#endif
}

void Serial1::transmit(char value) {
#ifdef SERIAL1_TXINT
	if (isTransmitting == 0) {
		USART_SendData(SERIAL1CH, value);
		isTransmitting = 1;
	}
	else {
		while (txBuf.write(value));
	}
#else
	while (USART_GetFlagStatus(SERIAL1CH, USART_FLAG_TXE) == RESET);
	USART_SendData(SERIAL1CH, value);
#endif
}


extern "C" void UART4_IRQHandler(void) {
	//serial1_rxInterrupt();
	//if (USART_GetFlagStatus(SERIAL1CH, USART_FLAG_TC) == SET)serial1_txInterrupt();
	//if (USART_GetFlagStatus(SERIAL1CH, USART_IT_RXNE) == SET)serial1_rxInterrupt();
	if (USART_GetITStatus(SERIAL1CH, USART_IT_TC) == SET)serial1_txInterrupt();
	if (USART_GetITStatus(SERIAL1CH, USART_IT_RXNE) == SET)serial1_rxInterrupt();
}

//--- serial1 end
SerialHandler *Serial2::Handler;
RingBuffer<char, 256> Serial2::txBuf;
int Serial2::isTransmitting = 0;
#define SERIAL2_TXINT

int Serial2::_setup(int baudrate, SerialHandler &HandlerArg, int parity, int wordLength)
{
#ifdef SERIAL2_TXINT
	Init_USARTi(SERIAL2CH, baudrate, SERIAL2TxRx, serialParitySelect(parity), serialLengthSelect(wordLength));
#else
	Init_USART(SERIAL2CH, baudrate, SERIAL2TxRx,serialParitySelect(parity), serialLengthSelect(wordLength));
#endif
	Handler = &HandlerArg;
	Handler->serialHandlerSetup(this);
	return 0;
}

void Serial2::writeChar(char value)
{
	transmit(value);
}

void serial2_rxInterrupt() {
	Serial2::Handler->serialReadChar(USART_ReceiveData(SERIAL2CH));
	//USART_ClearFlag(SERIAL2CH, USART_FLAG_RXNE);
	USART_ClearITPendingBit(SERIAL2CH,USART_IT_RXNE);
}

void serial2_txInterrupt() {
#ifdef SERIAL2_TXINT
	if (Serial2::txBuf.isEmpty() == 0) USART_SendData(SERIAL2CH, Serial2::txBuf.read());
	if (Serial2::txBuf.isEmpty() == 1) Serial2::isTransmitting = 0;
	//USART_ClearFlag(SERIAL2CH, USART_FLAG_TC);
	USART_ClearITPendingBit(SERIAL2CH,USART_IT_TC);
#endif
}

void Serial2::transmit(char value) {
#ifdef SERIAL2_TXINT
	if (isTransmitting == 0) {
		USART_SendData(SERIAL2CH, value);
		isTransmitting = 1;
	}
	else {
		while (txBuf.write(value));
	}
#else
	while (USART_GetFlagStatus(SERIAL2CH, USART_FLAG_TXE) == RESET);
	USART_SendData(SERIAL2CH, value);
#endif
}


extern "C" void UART5_IRQHandler(void) {
	//serial2_rxInterrupt();
	//if (USART_GetFlagStatus(SERIAL2CH, USART_FLAG_TC) == SET)serial2_txInterrupt();
	//if (USART_GetFlagStatus(SERIAL2CH, USART_IT_RXNE) == SET)serial2_rxInterrupt();
	if (USART_GetITStatus(SERIAL2CH, USART_IT_TC) == SET)serial2_txInterrupt();
	if (USART_GetITStatus(SERIAL2CH, USART_IT_RXNE) == SET)serial2_rxInterrupt();
}
//--- serial2 end

int WatchDog::setup() {
	return Init_watchdog();
}

void WatchDog::cycle() {
	watchdog_reset();
}
