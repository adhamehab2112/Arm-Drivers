/*
 * main.c
 *
 *  Created on: Jul 4, 2023
 *      Author: adham
 */
#include"includes/stm32_f103_x6.h"
#include "MCAL/GPIO/GPIO.h"
#include"HAL/LCD/LCD.h"
#include"HAL/KEYPAD/KEYPAD.h"
#include "MCAL/EXTI/EXTI.h"
#include"MCAL/RCC/RCC.h"
#include"MCAL/USART/USART.h"

void CLK_init()
{
	// Enable Clock for GPIOA
	RCC_GPIOA_CLK_EN();
	// Enable Clock for GPIOB
	RCC_GPIOB_CLK_EN();
	//RCC ENABLE FOR EXTI
	RCC_AFIO_CLK_EN();
}
uint8_t recived_data ;
int main()
{
	CLK_init();
	USART_Config_t MyUART ;
	MyUART.USART_BAUDRATE = USART_BAUDRATE_115200 ;
	MyUART.USART_HW_FLOW_CONTROL = USART_NO_FLOW_CONTROL ;
	MyUART.USART_IRQ_ENABLE = USART_IRQ_DIS ;
	MyUART.USART_MODE = USART_RX_TX_MODE ;
	MyUART.USART_PARITY = USART_PARITY_DIS ;
	MyUART.USART_PAYLOAD_LENGTH = USART_PAYLOAD_8B ;
	MyUART.USART_STOP_BITS = USART_ONE_STOP_BIT ;
	MyUART.call_back_ptr = NULL ;
	MCAL_USART_Init(USART1, &MyUART);
	MCAL_USART_Set_GPIO_Pins(USART1);


	while(1)
	{

		
		MCAL_USART_Recive_Data(USART1, &recived_data, Polling_Enable);
		MCAL_USART_Send_Data(USART1, &recived_data, Polling_Enable);

	}
	return 0 ;
}
void wait(uint32_t time)
{
	uint32_t i , j;

	for( i=0 ; i<time ; i++)
		for(j=0 ; j<255 ; j++);
}
