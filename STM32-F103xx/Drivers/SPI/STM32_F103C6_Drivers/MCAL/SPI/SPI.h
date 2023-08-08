/*
 * SPI.h
 *
 *  Created on: Jul 31, 2023
 *      Author: adham
 */

#ifndef MCAL_SPI_SPI_H_
#define MCAL_SPI_SPI_H_
//--------------------------------------------------
//**********Includes********************************
#include "../../includes/stm32_f103_x6.h"
#include "../../includes/std_macros.h"
#include "../../includes/std_types.h"
#include"../GPIO/GPIO.h"
//--------------------------------------------------

//----------------------------------------------------
struct SPI_IRQ_SRC
{
	uint8_t TXE  :  1 ;
	uint8_t RXNE :  1 ;
	uint8_t ERR  :  1 ;
	uint8_t RES  :  5 ;
};
//***********Configuration Structure****************
typedef struct
{
	uint16_t Device_Mode     ; //@ref : SPI_DEVICE_MODE
	uint16_t Comm_Mode       ; //@ref : SPI_COMM_MODE
	uint16_t Frame_Format    ; //@ref : SPI_FRAME_FORMAT
	uint16_t Data_Size 	     ; //@ref : SPI_DATA_SIZE
	uint16_t Clk_Polarity    ; //@ref : SPI_CLK_POLARITY
	uint16_t Clk_Phase       ; //@ref : SPI_CLK_PHASE
	uint16_t NSS		     ; //@ref : SPI_NSS
	uint16_t Prescaller      ; //@ref : SPI_PRESCALLER
	uint16_t IRQEn           ; //@ref : SPI_IRQ_STATE
	void (* Call_Back_Ptr)(struct SPI_IRQ_SRC IRQ_Src) ; // callBack pointer to function
}SPI_Config_t;

enum SPI_PollingMechanism
{
	SPI_Polling_Disable,
	SPI_Polling_Enable
};


//----------------------------------------------------

//----------------------------------------------------
//****************Reference Macros********************

//@ref : SPI_DEVICE_MODE -> CR1 REG [MSTR:BIT2]
#define SPI_SLAVE		0x00000000ul
#define SPI_MASTER		(1ul<<2)

//@ref : SPI_COMM_MODE -> CR1 REG [BIDIMODE:15 | BIDIOE:14 | RXONLY:10]
#define SPI_DIRECTION_2LINES			0x00000000ul
#define SPI_DIRECTION_2LINES_RX_ONLY	(1ul<<10)
#define SPI_DIRECTION_1LINE_RX_ONLY 	(1ul<<15)
#define SPI_DIRECTION_1LINE_TX_ONLY		((1ul<<15)|(1ul<<14))

//@ref : SPI_FRAME_FORMAT -> CR1 REG [LSS:7]
#define SPI_MSB_FIRIST		0x00000000ul
#define SPI_LSB_FIRIST		(1ul<<7)

//@ref : SPI_DATA_SIZE -> CR1 REG [DFF:11]
#define SPI_DATA_8BIT_MODE		0x00000000ul
#define SPI_DATA_16BIT_MODE		(1ul<<11)

//@ref : SPI_CLK_POLARITY -> CR1 REG [CPOL:1]
#define SPI_CLK_POL_ZERO_IDLE		0x00000000ul
#define SPI_CLK_POL_ONE_IDLE		(1ul<<1)

//@ref : SPI_CLK_PHASE -> CR1 REG [CPHA:0]
#define SPI_CLK_PHASE_FIRIST_EDGE_CAPTURE	 0x00000000ul
#define SPI_CLK_PHASE_SECOND_EDGE_CAPTURE  (1ul<<0)

//@ref : SPI_NSS -> Software/Hardware -> CR1[SSM:9 & SSI:8] -> CR2[SSOE:2]
//Hardware
#define SPI_NSS_HW_SLAVE	 0x00000000ul
#define SPI_NSS_HW_SSO_EN	 (1ul<<2)    // CR2
#define SPI_NSS_HW_SSO_DIS	~(1ul<<2)	// anding

//Software
#define SPI_NSS_SW_SSI_RESET	(1ul<<9)
#define SPI_NSS_SW_SSI_SET		((1ul<<9)|(1ul<<8))

//@ref : SPI_PRESCALLER -> CR1[BRR:5:3]
#define SPI_PRESCALLER_2	0x00000000ul
#define SPI_PRESCALLER_4	(0b001<<3)
#define SPI_PRESCALLER_8	(0b010<<3)
#define SPI_PRESCALLER_16	(0b011<<3)
#define SPI_PRESCALLER_32	(0b100<<3)
#define SPI_PRESCALLER_64	(0b101<<3)
#define SPI_PRESCALLER_128	(0b110<<3)
#define SPI_PRESCALLER_256	(0b111<<3)

//@ref : SPI_IRQ_STATE -> CR2[TXEIE:7 & RXNEIE:6 & ERRIE:5]
#define SPI_IRQ_DIS		0x00000000ul
#define SPI_IRQ_TXE_EN	(1ul<<7)
#define SPI_IRQ_RXNE_EN	(1ul<<6)
#define SPI_IRQ_ERR_EN	(1ul<<5)
//----------------------------------------------------

//=====================================================
//		          MCAL SPI APIs
void MCAL_SPI_Init(SPI_Registers_t *SPIx , SPI_Config_t *SPIconfig);
void MCAL_SPI_DeInit(SPI_Registers_t *SPIx);
void MCAL_SPI_ReciveData(SPI_Registers_t *SPIx , uint16_t *ptrDataBuffer , enum SPI_PollingMechanism PollState);
void MCAL_SPI_SendData(SPI_Registers_t *SPIx , uint16_t *ptrDataBuffer , enum SPI_PollingMechanism PollState);
void MCAL_SPI_Tx_Rx_Data(SPI_Registers_t *SPIx , uint16_t *ptrDataBuffer , enum SPI_PollingMechanism PollState);
void MCAL_SPI_Set_GPIO(SPI_Registers_t *SPIx);
//=====================================================


#endif /* MCAL_SPI_SPI_H_ */
