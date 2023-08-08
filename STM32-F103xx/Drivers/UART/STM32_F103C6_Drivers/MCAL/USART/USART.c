/*
 * USART.c
 *
 *  Created on: Jul 16, 2023
 *      Author: adham
 */

//------------------------------
//Includes
#include "USART.h"
//-------------------------------

//*~*~*~**~*~*~**~*~*~**~*~*~*
//Global Variables

// Note this driver Support only one UART at a time if you want to support all uarts u can create an array of configurations
USART_Config_t *Gl_Config = NULL ;

//*~*~*~**~*~*~**~*~*~**~*~*~*
void MCAL_USART_Init(USART_Registers_t *USARTx , USART_Config_t *config )
{
	Gl_Config = config ;
//-------------------1-Enable clock for USART------------------------------------------
	if(USARTx == USART1)
		RCC_USART1_CLK_EN();
	else if(USARTx == USART2)
		RCC_USART2_CLK_EN();
	else if(USARTx == USART3)
		RCC_USART3_CLK_EN();
//-------------------2-Enable USART MODULE------------------------------------------
	USARTx->USART_CR1 |= 1<<13 ;
//-------------------3-Enable USART Tx/Rx-------------------------------------------
	USARTx->USART_CR1 |= Gl_Config->USART_MODE ;
//-------------------4-Payload Length-----------------------------------------------
	USARTx->USART_CR1 |= Gl_Config->USART_PAYLOAD_LENGTH ;
//-------------------5-Parity Config------------------------------------------------
	USARTx->USART_CR1 |= Gl_Config->USART_PARITY ;
//-------------------6-Stop Bits Config------------------------------------------------
	USARTx->USART_CR2 |= Gl_Config->USART_STOP_BITS ;
//-------------------7-Stop Bits Config------------------------------------------------
	USARTx->USART_CR3 = Gl_Config->USART_HW_FLOW_CONTROL;
//-------------------8-BaudRate Configuration------------------------------------------
	uint32_t BRR , PCLK ;
	if(USARTx == USART1)
	{
		PCLK = MCAL_RCC_Get_PCLK2_Freq();
	}
	else
	{
		PCLK = MCAL_RCC_Get_PCLK1_Freq();
	}
	BRR = USART_BRR_REG(PCLK,Gl_Config->USART_BAUDRATE);
	USARTx->USART_BRR = BRR ;
//-------------------9-USART INT enable/disable------------------------------------------
	if(Gl_Config->USART_IRQ_ENABLE != USART_IRQ_DIS)
	{
		USARTx->USART_CR1 |= Gl_Config->USART_IRQ_ENABLE ;
//-------------------10-NVIC INT enable/disable------------------------------------------
			if(USARTx == USART1)
			{
				NVIC_IRQ37_USART1_EN();
			}
			else if(USARTx == USART2)
			{
				NVIC_IRQ38_USART2_EN();
			}
			else if(USARTx == USART3)
			{
				NVIC_IRQ39_USART3_EN();
			}
	}


}
void MCAL_USART_DeInit(USART_Registers_t *USARTx)
{
	if(USARTx == USART1)
	{
		RCC_USART1_CLK_DIS();
		NVIC_IRQ37_USART1_DIS();
	}
	else if(USARTx == USART2)
	{
		RCC_USART2_CLK_DIS();
		NVIC_IRQ38_USART2_DIS();
	}
	else if(USARTx == USART3)
	{
		RCC_USART3_CLK_DIS();
		NVIC_IRQ39_USART3_DIS();
	}
}
void MCAL_USART_Send_Data(USART_Registers_t *USARTx , uint16_t *dataBuffer ,enum PollingMechanism PoolingEn)
{
	// wait until TxE is set {according to TRM procedure } -> at polling mechanism
	if( PoolingEn == Polling_Enable)
	{
		while(!(USARTx->USART_SR&(1<<7))) ;
	}
	// check the USART data length ->take care that according to TRM always the last bit is for parity
	if(Gl_Config->USART_PAYLOAD_LENGTH == USART_PAYLOAD_9B)
	{
		USARTx->USART_DR = (*dataBuffer & (uint16_t)0x1FF) ; // 9bits
	}
	else
	{
		USARTx->USART_DR = (*dataBuffer & (uint16_t)0xFF) ; // 8bits
	}

}
void MCAL_USART_Wait_TC(USART_Registers_t *USARTx)
{
	while(!GET_BIT(USARTx->USART_SR,6)); // WAIT UNTIL ALL DATA TO BE SEND
}
void MCAL_USART_Recive_Data(USART_Registers_t *USARTx , uint16_t *dataBuffer ,enum PollingMechanism PoolingEn)
{
	// wait until RXNE is set {according to TRM procedure } -> at polling mechanism
		if( PoolingEn == Polling_Enable)
		{
			while(!(USARTx->USART_SR&(1<<5))) ;
		}
	// check the USART data length ->take care that according to TRM always the last bit is for parity
		if(Gl_Config->USART_PAYLOAD_LENGTH == USART_PAYLOAD_9B)
		{
			if(Gl_Config->USART_PARITY == USART_PARITY_DIS)
				*((uint16_t *)dataBuffer) = USARTx->USART_DR ;
				else
					*((uint16_t *)dataBuffer) = (USARTx->USART_DR & (uint8_t)0xFF );
		}
		else
		{
			if(Gl_Config->USART_PARITY == USART_PARITY_DIS)
				*((uint16_t *)dataBuffer) = USARTx->USART_DR ;
			else
				*((uint16_t *)dataBuffer) = (USARTx->USART_DR & (uint8_t)0x7F );
		}

}

//Set GPIO pins -> Recomended setting for uart in TRM [PIN ALTERNATIVE TABLE]
void MCAL_USART_Set_GPIO_Pins(USART_Registers_t *USARTx)
{
	GPIO_Pin_Config_t USART_Pin_Config ;
	if(USARTx == USART1)
	{
		//PA9 -> TX
		//PA10 -> RX
		//PA11 -> CTS
		//PA12 -> RTS

		//1-Set Tx pin :
		USART_Pin_Config.GPIO_PIN_NUMBER = GPIO_PIN9 ;
		USART_Pin_Config.GPIO_PIN_MODE = AFIO_OUTPUT_PP ;
		USART_Pin_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ;
		MCAL_GPIO_Init(GPIOA, &USART_Pin_Config);
		//2-Set Rx pin :
		USART_Pin_Config.GPIO_PIN_NUMBER = GPIO_PIN10 ;
		USART_Pin_Config.GPIO_PIN_MODE = AFIO_INPUT ;
		USART_Pin_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ;
		MCAL_GPIO_Init(GPIOA, &USART_Pin_Config);
		//3-Set CTS pin :
		if(Gl_Config->USART_HW_FLOW_CONTROL == USART_CTS_EN || Gl_Config->USART_HW_FLOW_CONTROL == USART_RTS_CTS_EN)
		{
		USART_Pin_Config.GPIO_PIN_NUMBER = GPIO_PIN11 ;
		USART_Pin_Config.GPIO_PIN_MODE = AFIO_INPUT ;
		USART_Pin_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ;
		MCAL_GPIO_Init(GPIOA, &USART_Pin_Config);
		}
		if(Gl_Config->USART_HW_FLOW_CONTROL == USART_RTS_EN || Gl_Config->USART_HW_FLOW_CONTROL == USART_RTS_CTS_EN)
		{
			USART_Pin_Config.GPIO_PIN_NUMBER = GPIO_PIN12 ;
			USART_Pin_Config.GPIO_PIN_MODE = AFIO_INPUT ;
			USART_Pin_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ;
			MCAL_GPIO_Init(GPIOA, &USART_Pin_Config);
		}

	}
	else if(USARTx == USART1)
		{
			//PA2 -> TX
			//PA3 -> RX
			//PA0 -> CTS
			//PA1 -> RTS

			//1-Set Tx pin :
			USART_Pin_Config.GPIO_PIN_NUMBER = GPIO_PIN2 ;
			USART_Pin_Config.GPIO_PIN_MODE = AFIO_OUTPUT_PP ;
			USART_Pin_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ;
			MCAL_GPIO_Init(GPIOA, &USART_Pin_Config);
			//2-Set Rx pin :
			USART_Pin_Config.GPIO_PIN_NUMBER = GPIO_PIN3 ;
			USART_Pin_Config.GPIO_PIN_MODE = AFIO_INPUT ;
			USART_Pin_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ;
			MCAL_GPIO_Init(GPIOA, &USART_Pin_Config);
			//3-Set CTS pin :
			if(Gl_Config->USART_HW_FLOW_CONTROL == USART_CTS_EN || Gl_Config->USART_HW_FLOW_CONTROL == USART_RTS_CTS_EN)
			{
			USART_Pin_Config.GPIO_PIN_NUMBER = GPIO_PIN0 ;
			USART_Pin_Config.GPIO_PIN_MODE = AFIO_INPUT ;
			USART_Pin_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ;
			MCAL_GPIO_Init(GPIOA, &USART_Pin_Config);
			}
			if(Gl_Config->USART_HW_FLOW_CONTROL == USART_RTS_EN || Gl_Config->USART_HW_FLOW_CONTROL == USART_RTS_CTS_EN)
			{
				USART_Pin_Config.GPIO_PIN_NUMBER = GPIO_PIN1 ;
				USART_Pin_Config.GPIO_PIN_MODE = AFIO_INPUT ;
				USART_Pin_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ;
				MCAL_GPIO_Init(GPIOA, &USART_Pin_Config);
			}
		}
	else if(USARTx == USART1)
	{
				//PB10 -> TX
				//PB11 -> RX
				//PB12 -> CTS
				//PB13 -> RTS

				//1-Set Tx pin :
				USART_Pin_Config.GPIO_PIN_NUMBER = GPIO_PIN10 ;
				USART_Pin_Config.GPIO_PIN_MODE = AFIO_OUTPUT_PP ;
				USART_Pin_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ;
				MCAL_GPIO_Init(GPIOA, &USART_Pin_Config);
				//2-Set Rx pin :
				USART_Pin_Config.GPIO_PIN_NUMBER = GPIO_PIN11 ;
				USART_Pin_Config.GPIO_PIN_MODE = AFIO_INPUT ;
				USART_Pin_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ;
				MCAL_GPIO_Init(GPIOA, &USART_Pin_Config);
				//3-Set CTS pin :
				if(Gl_Config->USART_HW_FLOW_CONTROL == USART_CTS_EN || Gl_Config->USART_HW_FLOW_CONTROL == USART_RTS_CTS_EN)
				{
				USART_Pin_Config.GPIO_PIN_NUMBER = GPIO_PIN12 ;
				USART_Pin_Config.GPIO_PIN_MODE = AFIO_INPUT ;
				USART_Pin_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ;
				MCAL_GPIO_Init(GPIOA, &USART_Pin_Config);
				}
				if(Gl_Config->USART_HW_FLOW_CONTROL == USART_RTS_EN || Gl_Config->USART_HW_FLOW_CONTROL == USART_RTS_CTS_EN)
				{
					USART_Pin_Config.GPIO_PIN_NUMBER = GPIO_PIN13 ;
					USART_Pin_Config.GPIO_PIN_MODE = AFIO_INPUT ;
					USART_Pin_Config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ;
					MCAL_GPIO_Init(GPIOA, &USART_Pin_Config);
				}

	}

}


//=================================================================
//					      USART ISR
//=================================================================
USART1_IRQHandler()
{
	Gl_Config->call_back_ptr;
}
USART2_IRQHandler()
{
	Gl_Config->call_back_ptr;
}
USART3_IRQHandler()
{
	Gl_Config->call_back_ptr;
}


