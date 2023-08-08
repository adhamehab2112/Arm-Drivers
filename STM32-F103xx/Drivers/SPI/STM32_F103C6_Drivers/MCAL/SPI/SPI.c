/*
 * SPI.c
 *
 *  Created on: Jul 31, 2023
 *      Author: adham
 */

#include"SPI.h"
//==================================================
//=====================Global Variables=============
SPI_Config_t *Global_SPI_Config[2]={NULL,NULL};
//==================================================


//===============================================
//=========================Macros================
#define SPI1_INDEX		0
#define SPI2_INDEX		1
#define SPI_SR_TXE		1<<1
#define SPI_SR_RXNE		1<<0
//================================================

//===============================================
//=================ISRQ Handler===================
void SPI1_IRQHandler(void)
{
	struct SPI_IRQ_SRC IRQSrc ;
	IRQSrc.TXE = ((SPI1->SPI_SR & 1<<1)>>1);
	IRQSrc.RXNE = ((SPI1->SPI_SR) & 1<<0);
	IRQSrc.ERR = ((SPI1->SPI_SR & 1<<4)>>4);
	Global_SPI_Config[SPI1_INDEX]->Call_Back_Ptr(IRQSrc);

}

void SPI2_IRQHandler(void)
{
	struct SPI_IRQ_SRC IRQSrc ;
	IRQSrc.TXE = ((SPI2->SPI_SR & 1<<1)>>1);
	IRQSrc.RXNE = ((SPI2->SPI_SR) & 1<<0);
	IRQSrc.ERR = ((SPI2->SPI_SR & 1<<4)>>4);
	Global_SPI_Config[SPI2_INDEX]->Call_Back_Ptr(IRQSrc);

}
//===============================================
//=========Functions Implementation===============

/**================================================================
* @Fn    		: MCAL_SPI_Init
* @brief 		: Initialize the SPI peripheral
* @param [in] 	: SPI instance and SPI Configuration Structure
* @param [out] 	: void
* @retval 		: none
* Note			: this function has no prerequisites used to initialize the SPI peripheral
*===================================================================*/
void MCAL_SPI_Init(SPI_Registers_t *SPIx , SPI_Config_t *SPIconfig)
{
	uint16_t tempReg_CR1 = 0x0 ;
	uint16_t tempReg_CR2 = 0x0 ;
	if(SPIx == SPI1)
	{
		Global_SPI_Config[SPI1_INDEX] = SPIconfig ;
		RCC_SPI1_CLK_EN();
	}
	else if(SPIx == SPI2)
	{
		Global_SPI_Config[SPI2_INDEX] = SPIconfig ;
		RCC_SPI2_CLK_EN();
	}

	//1-Enable SPI CR1[SPE:6]
	tempReg_CR1 |= 1<<6 ;
	//2-Initialize the SPI MODE : Slave / Master
	tempReg_CR1 |=SPIconfig->Device_Mode ;
	//3-Communication Mode
	tempReg_CR1 |= SPIconfig->Comm_Mode ;
	//4-Frame Format
	tempReg_CR1 |= SPIconfig->Frame_Format ;
	//5-Data Size
	tempReg_CR1 |= SPIconfig->Data_Size ;
	//6-CLK Phase
	tempReg_CR1 |= SPIconfig->Clk_Phase ;
	//7-CLK Polarity
	tempReg_CR1 |= SPIconfig->Clk_Polarity ;
	//8-NSS
	if(SPIconfig->NSS == SPI_NSS_HW_SSO_EN)
		tempReg_CR2 |= SPIconfig->NSS ;
	else if(SPIconfig->NSS == SPI_NSS_HW_SSO_DIS)
		tempReg_CR2 &= SPIconfig->NSS ;
	else
		tempReg_CR1 |= SPIconfig->NSS ;
	//9-PreScaller
	tempReg_CR1 |= SPIconfig->Prescaller ;
	//10-Interrupt Enable/Disable
	if(SPIconfig->IRQEn != SPI_IRQ_DIS)
	{
		tempReg_CR2 |= SPIconfig->IRQEn ;
		if(SPIx==SPI1)
			NVIC_IRQ35_SPI1_EN();
		else if(SPIx==SPI2)
			NVIC_IRQ36_SPI2_EN();

	}
		//Initialize the registers
		SPIx->SPI_CR1 = tempReg_CR1 ;
		SPIx->SPI_CR2 = tempReg_CR2 ;

}

/**================================================================
* @Fn    		: MCAL_SPI_DeInit
* @brief 		: DeInitialize the SPI peripheral
* @param [in] 	: SPI instance
* @param [out] 	: void
* @reutval 		: none
* Note			: this function has no prerequisites used to Deinitialize the SPI peripheral
*===================================================================*/
void MCAL_SPI_DeInit(SPI_Registers_t *SPIx)
{
	if(SPIx==SPI1)
	{
		NVIC_IRQ35_SPI1_DIS();
		RCC_SPI1_CLK_DIS() ;
	}
	else if(SPIx==SPI2)
	{
		NVIC_IRQ36_SPI2_DIS();
		RCC_SPI2_CLK_DIS() ;
	}
}

/**================================================================
* @Fn    		: MCAL_SPI_ReciveData
* @brief 		: Used to receive data on the SPI bus
* @param [in] 	: SPI instance   , ptr to the data buffer , enum instance for the polling state
* @param [out] 	: void
* @reutval 		: none
* Note			: this function request that the SPI peripheral is already initialized
*===================================================================*/
void MCAL_SPI_ReciveData(SPI_Registers_t *SPIx , uint16_t *ptrDataBuffer , enum SPI_PollingMechanism PollState)
{
	if(PollState == SPI_Polling_Enable)
		while(!((SPIx->SPI_SR)&SPI_SR_RXNE));
	*ptrDataBuffer = SPIx->SPI_DR ;
}
//================================================

/**================================================================
* @Fn    		: MCAL_SPI_SendData
* @brief 		: Used to Send data on the SPI bus
* @param [in] 	: SPI instance   , ptr to the data buffer , enum instance for the polling state
* @param [out] 	: void
* @reutval 		: none
* Note			: this function request that the SPI peripheral is already initialized
*===================================================================*/
void MCAL_SPI_SendData(SPI_Registers_t *SPIx , uint16_t *ptrDataBuffer , enum SPI_PollingMechanism PollState)
{
	if(PollState == SPI_Polling_Enable)
		while(!((SPIx->SPI_SR)&SPI_SR_TXE));
    SPIx->SPI_DR = *ptrDataBuffer  ;
}

/**================================================================
* @Fn    		: MCAL_SPI_Tx_Rx_Data
* @brief 		: Used to Send and Receive Data SPI bus
* @param [in] 	: SPI instance   , ptr to the data buffer , enum instance for the polling state
* @param [out] 	: void
* @reutval 		: none
* Note			: this function request that the SPI peripheral is already initialized
*===================================================================*/
void MCAL_SPI_Tx_Rx_Data(SPI_Registers_t *SPIx , uint16_t *ptrDataBuffer , enum SPI_PollingMechanism PollState)
{
	if(PollState == SPI_Polling_Enable)
		while(!((SPIx->SPI_SR)&SPI_SR_TXE));
    SPIx->SPI_DR = *ptrDataBuffer  ;
	if(PollState == SPI_Polling_Enable)
		while(!((SPIx->SPI_SR)&SPI_SR_RXNE));
	*ptrDataBuffer = SPIx->SPI_DR ;
}

/**================================================================
* @Fn    		: MCAL_SPI_Set_GPIO
* @brief 		: Used set the GPIO pins for SPI
* @param [in] 	: SPI instance
* @param [out] 	: void
* @reutval 		: none
* Note			: this function request that the SPI peripheral is already initialized
*===================================================================*/
void MCAL_SPI_Set_GPIO(SPI_Registers_t *SPIx)
{
	GPIO_Pin_Config_t GPIO_Config ;
	if(SPIx == SPI1)
	{
		//PA4 : NSS
		//PA5 : SCLK
		//PA6 : MISO
		//PA7 : MOSI
		if(Global_SPI_Config[SPI1_INDEX]->Device_Mode == SPI_MASTER)
		{
			//1-Initialize NSS Pin
			switch(Global_SPI_Config[SPI1_INDEX]->NSS)
			{
			case SPI_NSS_HW_SSO_DIS :
				GPIO_Config.GPIO_PIN_NUMBER = GPIO_PIN4 ;
				GPIO_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
				GPIO_Config.GPIO_PIN_MODE = GPIO_INPUT_FLOAT ;
				MCAL_GPIO_Init(GPIOA, &GPIO_Config);
				break;
			case SPI_NSS_HW_SSO_EN :
				GPIO_Config.GPIO_PIN_NUMBER = GPIO_PIN4 ;
				GPIO_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
				GPIO_Config.GPIO_PIN_MODE = AFIO_OUTPUT_PP ;
				MCAL_GPIO_Init(GPIOA, &GPIO_Config);
				break;

			}
			//2-Initialize SCLK Pin
			GPIO_Config.GPIO_PIN_NUMBER = GPIO_PIN5 ;
			GPIO_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
			GPIO_Config.GPIO_PIN_MODE = AFIO_OUTPUT_PP ;
			MCAL_GPIO_Init(GPIOA, &GPIO_Config);

			//3-Initialize  MISO Pin
			GPIO_Config.GPIO_PIN_NUMBER = GPIO_PIN6 ;
			GPIO_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
			GPIO_Config.GPIO_PIN_MODE = GPIO_INPUT_FLOAT ;
			MCAL_GPIO_Init(GPIOA, &GPIO_Config);

			//4-Initialize MOSI Pin
			GPIO_Config.GPIO_PIN_NUMBER = GPIO_PIN7 ;
			GPIO_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
			GPIO_Config.GPIO_PIN_MODE = AFIO_OUTPUT_PP ;
			MCAL_GPIO_Init(GPIOA, &GPIO_Config);

		}
		else // slave
		{
			if(Global_SPI_Config[SPI1_INDEX]->NSS == SPI_NSS_HW_SLAVE)
			{
				GPIO_Config.GPIO_PIN_NUMBER = GPIO_PIN4 ;
				GPIO_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
				GPIO_Config.GPIO_PIN_MODE = GPIO_INPUT_FLOAT ;
				MCAL_GPIO_Init(GPIOA, &GPIO_Config);
			}
			//2-Initialize SCLK Pin
			GPIO_Config.GPIO_PIN_NUMBER = GPIO_PIN5 ;
			GPIO_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
			GPIO_Config.GPIO_PIN_MODE = GPIO_INPUT_FLOAT ;
			MCAL_GPIO_Init(GPIOA, &GPIO_Config);

			//3-Initialize  MISO Pin
			GPIO_Config.GPIO_PIN_NUMBER = GPIO_PIN6 ;
			GPIO_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
			GPIO_Config.GPIO_PIN_MODE = AFIO_OUTPUT_PP ;
			MCAL_GPIO_Init(GPIOA, &GPIO_Config);

			//4-Initialize MOSI Pin
			GPIO_Config.GPIO_PIN_NUMBER = GPIO_PIN7 ;
			GPIO_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
			GPIO_Config.GPIO_PIN_MODE = GPIO_INPUT_FLOAT ;
			MCAL_GPIO_Init(GPIOA, &GPIO_Config);
		}
	}
	else if(SPIx == SPI2)
	{
		//PB12 : NSS
		//PB13: SCLK
		//PB14: MISO
		//PB15: MOSI
		if(Global_SPI_Config[SPI2_INDEX]->Device_Mode == SPI_MASTER)
		{
			//1-Initialize NSS Pin
			switch(Global_SPI_Config[SPI2_INDEX]->NSS)
			{
			case SPI_NSS_HW_SSO_DIS :
				GPIO_Config.GPIO_PIN_NUMBER = GPIO_PIN12 ;
				GPIO_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
				GPIO_Config.GPIO_PIN_MODE = GPIO_INPUT_FLOAT ;
				MCAL_GPIO_Init(GPIOB, &GPIO_Config);
				break;
			case SPI_NSS_HW_SSO_EN :
				GPIO_Config.GPIO_PIN_NUMBER = GPIO_PIN12 ;
				GPIO_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
				GPIO_Config.GPIO_PIN_MODE = AFIO_OUTPUT_PP ;
				MCAL_GPIO_Init(GPIOB, &GPIO_Config);
				break;

			}
			//2-Initialize SCLK Pin
			GPIO_Config.GPIO_PIN_NUMBER = GPIO_PIN13 ;
			GPIO_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
			GPIO_Config.GPIO_PIN_MODE = AFIO_OUTPUT_PP ;
			MCAL_GPIO_Init(GPIOB, &GPIO_Config);

			//3-Initialize  MISO Pin
			GPIO_Config.GPIO_PIN_NUMBER = GPIO_PIN14 ;
			GPIO_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
			GPIO_Config.GPIO_PIN_MODE = GPIO_INPUT_FLOAT ;
			MCAL_GPIO_Init(GPIOB, &GPIO_Config);

			//4-Initialize MOSI Pin
			GPIO_Config.GPIO_PIN_NUMBER = GPIO_PIN15 ;
			GPIO_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
			GPIO_Config.GPIO_PIN_MODE = AFIO_OUTPUT_PP ;
			MCAL_GPIO_Init(GPIOB, &GPIO_Config);

		}
		else // slave
		{
			if(Global_SPI_Config[SPI2_INDEX]->NSS == SPI_NSS_HW_SLAVE)
			{
				GPIO_Config.GPIO_PIN_NUMBER = GPIO_PIN12 ;
				GPIO_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
				GPIO_Config.GPIO_PIN_MODE = GPIO_INPUT_FLOAT ;
				MCAL_GPIO_Init(GPIOB, &GPIO_Config);
			}
			//2-Initialize SCLK Pin
			GPIO_Config.GPIO_PIN_NUMBER = GPIO_PIN13 ;
			GPIO_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
			GPIO_Config.GPIO_PIN_MODE = GPIO_INPUT_FLOAT ;
			MCAL_GPIO_Init(GPIOB, &GPIO_Config);

			//3-Initialize  MISO Pin
			GPIO_Config.GPIO_PIN_NUMBER = GPIO_PIN14 ;
			GPIO_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
			GPIO_Config.GPIO_PIN_MODE = AFIO_OUTPUT_PP ;
			MCAL_GPIO_Init(GPIOB, &GPIO_Config);

			//4-Initialize MOSI Pin
			GPIO_Config.GPIO_PIN_NUMBER = GPIO_PIN15 ;
			GPIO_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
			GPIO_Config.GPIO_PIN_MODE = GPIO_INPUT_FLOAT ;
			MCAL_GPIO_Init(GPIOB, &GPIO_Config);
		}
	}
}
//================================================
