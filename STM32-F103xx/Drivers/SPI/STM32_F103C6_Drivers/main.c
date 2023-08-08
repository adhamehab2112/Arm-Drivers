/*
 * main.c
 *
 *  Created on: Jul 4, 2023
 *      Author: adham
 */

//#define MCU_ACT_AS_SLAVE
#define MCU_ACT_AS_MASTER



#include"includes/stm32_f103_x6.h"
#include "MCAL/GPIO/GPIO.h"
#include"HAL/LCD/LCD.h"
#include"HAL/KEYPAD/KEYPAD.h"
#include "MCAL/EXTI/EXTI.h"
#include"MCAL/RCC/RCC.h"
#include"MCAL/USART/USART.h"
#include"MCAL/SPI/SPI.h"

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

void USART_IRQ_Callback(void)
{
	MCAL_USART_Recive_Data(USART1, &recived_data, Polling_Disable);
	MCAL_USART_Send_Data(USART1, &recived_data, Polling_Disable);
	//send data to SPI
	MCAL_GPIO_Pin_Write(GPIOA, GPIO_PIN4, GPIO_PIN_LOW);
	MCAL_SPI_Tx_Rx_Data(SPI1, &recived_data, Polling_Enable);
	MCAL_GPIO_Pin_Write(GPIOA, GPIO_PIN4, GPIO_PIN_HIGH);
}
int main()
{
	CLK_init();

//***********************USAT initialization*****************************
	USART_Config_t MyUART ;
	MyUART.USART_BAUDRATE = USART_BAUDRATE_115200 ;
	MyUART.USART_HW_FLOW_CONTROL = USART_NO_FLOW_CONTROL ;
	MyUART.USART_IRQ_ENABLE = USART_IRQ_RXNE_INT ;
	MyUART.USART_MODE = USART_RX_TX_MODE ;
	MyUART.USART_PARITY = USART_PARITY_DIS ;
	MyUART.USART_PAYLOAD_LENGTH = USART_PAYLOAD_8B ;
	MyUART.USART_STOP_BITS = USART_ONE_STOP_BIT ;
	MyUART.call_back_ptr = USART_IRQ_Callback ;
	MCAL_USART_Init(USART1, &MyUART);
	MCAL_USART_Set_GPIO_Pins(USART1);

//***********************SPI initialization*****************************
	SPI_Config_t MySPI ;
	//common config for both master & slave
	MySPI.Clk_Phase = SPI_CLK_PHASE_SECOND_EDGE_CAPTURE ;
	MySPI.Clk_Polarity = SPI_CLK_POL_ONE_IDLE ;
	MySPI.Data_Size = SPI_DATA_8BIT_MODE ;
	MySPI.Frame_Format = SPI_MSB_FIRIST ;
	MySPI.Prescaller = SPI_PRESCALLER_8 ;
	MySPI.Comm_Mode = SPI_DIRECTION_2LINES ;
#ifdef MCU_ACT_AS_MASTER
	MySPI.Device_Mode = SPI_MASTER ;
	MySPI.IRQEn = SPI_IRQ_DIS ;
	MySPI.NSS = SPI_NSS_SW_SSI_SET ;
	MySPI.Call_Back_Ptr = NULL ;
#endif
	MCAL_SPI_Init(SPI1, &MySPI);
	MCAL_SPI_Set_GPIO(SPI1);


//configure SS on PORTA PIN4 BY GPIO
	GPIO_Pin_Config_t MyGPIO ;
	MyGPIO.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
	MyGPIO.GPIO_PIN_NUMBER = GPIO_PIN4 ;
	MyGPIO.GPIO_PIN_MODE = GPIO_OUTPUT_PP;
	MCAL_GPIO_Init(GPIOA, &MyGPIO);
	MCAL_GPIO_Pin_Write(GPIOA, GPIO_PIN4, GPIO_PIN_HIGH);

	while(1)
	{
		//***Loop Forever*****//
	}
	return 0 ;
}
void wait(uint32_t time)
{
	uint32_t i , j;

	for( i=0 ; i<time ; i++)
		for(j=0 ; j<255 ; j++);
}
