#include "config_spi.h"
#include "mcutime.h"
#include "name_converter.h"
#include <string.h>
#include <stdio.h>
#include <math.h>


void Init_spi(SPI_TypeDef *use_spi,GPIO_TypeDef *GPIOxClk, uint16_t clkPin,GPIO_TypeDef *GPIOxMosi, uint16_t mosiPin,GPIO_TypeDef *GPIOxMiso, uint16_t misoPin,int mode,int direction){
//	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef	SPI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	if(use_spi == SPI1){
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	}else if(use_spi == SPI2){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	}else if(use_spi == SPI3){
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,ENABLE);
	}
	/*RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	GPIO_StructInit(&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = sclPin | sdaPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;*/

	Init_port(GPIO_Mode_AF,GPIOxClk,clkPin,GPIO_PuPd_NOPULL,GPIO_OType_PP);
	Init_port(GPIO_Mode_AF,GPIOxMosi,mosiPin,GPIO_PuPd_NOPULL,GPIO_OType_PP);
	Init_port(GPIO_Mode_AF,GPIOxMiso,misoPin,GPIO_PuPd_NOPULL,GPIO_OType_PP);

	if(use_spi == SPI1){
	//	GPIO_Init(GPIOxScl, &GPIO_InitStructure);
		//GPIO_Init(GPIOxSda, &GPIO_InitStructure);
		GPIO_PinAFConfig(GPIOxClk , Pin_select_source(clkPin) , GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOxMosi , Pin_select_source(mosiPin) , GPIO_AF_SPI1);
		GPIO_PinAFConfig(GPIOxMiso , Pin_select_source(misoPin) , GPIO_AF_SPI1);
	}else if(use_spi == SPI2){
		//GPIO_Init(GPIOxScl, &GPIO_InitStructure);
		//GPIO_Init(GPIOxSda, &GPIO_InitStructure);
		GPIO_PinAFConfig(GPIOxClk , Pin_select_source(clkPin) , GPIO_AF_SPI2);
		GPIO_PinAFConfig(GPIOxMosi , Pin_select_source(mosiPin) , GPIO_AF_SPI2);
		GPIO_PinAFConfig(GPIOxMiso , Pin_select_source(misoPin) , GPIO_AF_SPI2);
	}else if(use_spi == SPI3){
//		GPIO_Init(GPIOxScl, &GPIO_InitStructure);
		//GPIO_PinAFConfig(GPIOxScl, Pin_select_source(sclPin), GPIO_AF_I2C3);
//		GPIO_Init(GPIOxSda, &GPIO_InitStructure);
		//GPIO_PinAFConfig(GPIOxSda, Pin_select_source(sdaPin), GPIO_AF_I2C3);
		GPIO_PinAFConfig(GPIOxClk , Pin_select_source(clkPin) , GPIO_AF_SPI3);
		GPIO_PinAFConfig(GPIOxMosi , Pin_select_source(mosiPin) , GPIO_AF_SPI3);
		GPIO_PinAFConfig(GPIOxMiso , Pin_select_source(misoPin) , GPIO_AF_SPI3);

	}

	/*SPI通信の動作設定*/
	SPI_StructInit(&SPI_InitStructure);

	if(direction==0)SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	else if(direction==1)SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_Tx;

	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;

	if(mode==0 || mode==1)SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	else if(mode==2 || mode==3)SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;

	if(mode==0 || mode==2)SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	else if(mode==1 || mode==3)SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;

	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;
	SPI_Cmd(use_spi,ENABLE);
	SPI_Init(use_spi,&SPI_InitStructure);


	/* Configure the SPI event priority *///割り込み優先
	if(use_spi==SPI1){
		NVIC_InitStructure.NVIC_IRQChannel = SPI1_IRQn;
	}else if(use_spi==SPI2){
		NVIC_InitStructure.NVIC_IRQChannel = SPI2_IRQn;
	}else if(use_spi==SPI3){
		NVIC_InitStructure.NVIC_IRQChannel = SPI3_IRQn;
	}
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//グループ優先度２
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//サブ優先度
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}


