/*
 * USART.h
 *
 *  Created on: Jul 16, 2023
 *      Author: adham
 */

#ifndef MCAL_USART_USART_H_
#define MCAL_USART_USART_H_

//------------------------------
//Includes
#include "../../includes/stm32_f103_x6.h"
#include "../../includes/std_macros.h"
#include "../../includes/std_types.h"
#include"../GPIO/GPIO.h"
#include"../EXTI/EXTI.h"
#include"../RCC/RCC.h"
//-------------------------------

//-------------------------------
//Configuration Structure
typedef struct
{
	uint8_t USART_MODE      	 	; //@ref: USART_MODES_DEFINE -> reciver | transmitter | reciver_transmitter
	uint32_t USART_BAUDRATE 	 	; //@ref: USART_BR_SELECTION
	uint8_t USART_PAYLOAD_LENGTH 	; //@ref : USART_PAYLOAD_LENGHT_DEFINE
	uint8_t USART_PARITY		 	; //@ref : USART_PARITY_SELECTION
	uint8_t USART_STOP_BITS		 	; //@ref : USART_STOP_BITS_DEFINE
	uint8_t USART_HW_FLOW_CONTROL	; //@ref : USART_HWFLOWCONTROL_DEFINE
	uint8_t	USART_IRQ_ENABLE		; //@ref : USART_IRQ_DEFINE
	void(*call_back_ptr)(void)		; // call back pointer
}USART_Config_t;
enum PollingMechanism
{
	Polling_Disable ,
	Polling_Enable
};
//-------------------------------

//*~*~*~**~*~*~**~*~*~**~*~*~*
//Define MACROS

//@USART_MODE_DEFINE
#define USART_TX_MODE		(uint32_t)(1<<3) // according to TRM
#define USART_RX_MODE		(uint32_t)(1<<2)
#define USART_RX_TX_MODE	(uint32_t)((1<<2)|(1<<3))

//@USART_BR_SELECTION
#define USART_BAUDRATE_2400		2400
#define USART_BAUDRATE_9600		9600
#define USART_BAUDRATE_19200	19200
#define USART_BAUDRATE_57600	57600
#define USART_BAUDRATE_115200	115200
#define USART_BAUDRATE_230400	230400
#define USART_BAUDRATE_460800	460800
#define USART_BAUDRATE_921600	921600
#define USART_BAUDRATE_2250000	2250000

//@USART_PAYLOAD_LENGHT_DEFINE
#define USART_PAYLOAD_8B	(uint32_t)(0)
#define USART_PAYLOAD_9B	(uint32_t)(1<<12)

//@USART_PAYLOAD_LENGHT_DEFINE
#define USART_PARITY_DIS	(uint32_t)(0)
#define USART_PARTY_EVEN	(uint32_t)(1<<10)
#define USART_PARTY_ODD		(uint32_t)(1<<10 | 1<<9)

//@USART_STOP_BITS_DEFINE
#define USART_ONE_STOP_BIT		(uint32_t)(0)
#define USART_HALF_STOP_BIT		(uint32_t)(1<<12)
#define USART_TWO_STOP_BIT		(uint32_t)(2<<12)
#define	USART_ONE_HALF_STOP_BIT	(uint32_t)(3<<12)

//@USART_HWFLOWCONTROL_DEFINE
#define USART_NO_FLOW_CONTROL	(uint32_t)(0)
#define USART_CTS_EN			(uint32_t)(1<<9)
#define USART_RTS_EN			(uint32_t)(1<<8)
#define USART_RTS_CTS_EN		(uint32_t)(1<<8 | 1<<9)

//@USART_IRQ_DEFINE
#define USART_IRQ_DIS			(uint32_t)(0)
#define USART_IRQ_PARITY_INT 	(uint32_t)(1<<8)
#define USART_IRQ_TXE_INT		(uint32_t)(1<<7)
#define USART_IRQ_TXC_INT		(uint32_t)(1<<6)
#define USART_IRQ_RXNE_INT		(uint32_t)(1<<5)

//*~*~*~**~*~*~**~*~*~**~*~*~*

//*~*~*~**~*~*~**~*~*~**~*~*~*

//Baud Rate Magic Macros
//Baudrate calculation
//USARTDIV = fclk /(16*Baudrate)
//USARTDIV_Mul100  = (uint32)(100*fclk)/(16*Baudrate)
//USARTDIV_Mul100  = (uint32)(25*fclk)/(4*Baudrate)
//DIV_Mantissa_MUL100 = Integar part (USARTDIV) *100
//DIV_Mantissa = Integar part (USARTDIV)
//DIV_Fraction = ((USARTDIV_Mul100 - DIV_Mantissa_MUL100 ) * 16 )/100)
#define USARDIV(_PCLK_,_BAUD_)					(uint32_t)(_PCLK_/(16*_BAUD_))
#define USARDIV_MUL100(_PCLK_,_BAUD_)			(uint32_t)((25*_PCLK_)/(4*_BAUD_))
#define MANTISSA_MUL100(_PCLK_,_BAUD_)			(uint32_t)(USARDIV(_PCLK_,_BAUD_)*100)
#define MANTISSA(_PCLK_,_BAUD_)					(uint32_t)USARDIV(_PCLK_,_BAUD_)
#define DIV_FACTOR(_PCLK_,_BAUD_)				(uint32_t)(((USARDIV_MUL100(_PCLK_,_BAUD_)- MANTISSA_MUL100(_PCLK_,_BAUD_))*16) / 100)
#define USART_BRR_REG(_PCLK_,_BAUD_)			(MANTISSA(_PCLK_,_BAUD_)<<4)|(DIV_FACTOR(_PCLK_,_BAUD_)&0xF)
//*~*~*~**~*~*~**~*~*~**~*~*~*

//=================================================
//		          MCAL USART APIs
//=================================================

void MCAL_USART_Init(USART_Registers_t *USARTx , USART_Config_t *config );
void MCAL_USART_DeInit(USART_Registers_t *USARTx);
void MCAL_USART_Send_Data(USART_Registers_t *USARTx , uint16_t *dataBuffer ,enum PollingMechanism PoolingEn);
void MCAL_USART_Recive_Data(USART_Registers_t *USARTx , uint16_t *dataBuffer ,enum PollingMechanism PoolingEn);
void MCAL_USART_Wait_TC(USART_Registers_t *USARTx);
void MCAL_USART_Set_GPIO_Pins(USART_Registers_t *USARTx);
#endif /* MCAL_USART_USART_H_ */
