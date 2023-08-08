/*
 * GPIO.h
 *
 *  Created on: Jul 4, 2023
 *      Author: adham
 */

#ifndef MCAL_GPIO_GPIO_H_
#define MCAL_GPIO_GPIO_H_

//-----------------------------
//Includes
#include"../../includes/std_macros.h"
#include"../../includes/std_types.h"
#include"../../includes/stm32_f103_x6.h"
//-----------------------------


//-----------------------------
//User type definitions (structures)
typedef struct
{
	uint16_t	GPIO_PIN_NUMBER ; //@ref:GPIO_PIN_NUMBERS
	uint8_t		GPIO_PIN_MODE 	; //@ref:GPIO_PIN_MODE
	uint8_t		GPIO_OUT_SPEED  ; //@ref:GPIO_SPEED

}GPIO_Pin_Config_t;
//-----------------------------



//-----------------------------
//Macros Configuration References
//@GPIO PIN NUMBER
#define GPIO_PIN0	0
#define GPIO_PIN1	1
#define GPIO_PIN2	2
#define GPIO_PIN3	3
#define GPIO_PIN4	4
#define GPIO_PIN5	5
#define GPIO_PIN6	6
#define GPIO_PIN7	7
#define GPIO_PIN8	8
#define GPIO_PIN9	9
#define GPIO_PIN10	10
#define GPIO_PIN11	11
#define GPIO_PIN12	12
#define GPIO_PIN13	13
#define GPIO_PIN14	14
#define GPIO_PIN15	15

//@ref:GPIO_PIN_MODE
//0: Analog mode
//1: Floating input (reset state)
//2: Input with pull-up
//3:Input with pull-down
//4: General purpose output push-pull
//5: General purpose output Open-drain
//6: Alternate function output Push-pull
//7: Alternate function output Open-drain
//8:	Alternate function input floating
#define GPIO_INPUT_ANALOG	0
#define GPIO_INPUT_FLOAT	1
#define GPIO_INPUT_PUP		2
#define GPIO_INPUT_PDWN		3
#define GPIO_OUTPUT_PP		4
#define GPIO_OUTPUT_OD		5
#define AFIO_OUTPUT_PP		6
#define AFIO_OUTPUT_OD		7
#define AFIO_INPUT			8

//@ref:GPIO_SPEED
//0: Input mode (reset state)
//1: Output mode, max speed 10 MHz.
//2: Output mode, max speed 2 MHz.
//3: Output mode, max speed 50 MHz.
#define GPIO_OUT_SPEED_10MHZ			1
#define GPIO_OUT_SPEED_2MHZ				2
#define GPIO_OUT_SPEED_50MHZ			3


//@ref:PIN_VALUE
#define GPIO_PIN_LOW 	0
#define GPIO_PIN_HIGH	1

//@ref:LOCK_STATE
#define LOCK_FAIL		0
#define LOCK_DONE		1
//-----------------------------
/*======================================================
 * 													   *
 * 			APIs Supported by "MCAL GPIO DRIVER"       *
 * 				                                       *
 *======================================================
 */

//initialization APIs
void MCAL_GPIO_Init(GPIOx_Registers_t *GPIOx ,GPIO_Pin_Config_t *Pin_Config );
void MCAL_GPIO_DeInit(GPIOx_Registers_t *GPIOx);

//write APIs
void MCAL_GPIO_Pin_Write(GPIOx_Registers_t *GPIOx , uint16_t Pin_number , uint8_t value);
void MCAL_GPIO_Port_Write(GPIOx_Registers_t *GPIOx ,  uint16_t value);
void MCAL_GPIO_Pin_Toggle(GPIOx_Registers_t *GPIOx , uint16_t Pin_number);

//read APIs
uint8_t MCAL_GPIO_Pin_Read(GPIOx_Registers_t *GPIOx , uint16_t Pin_number);
uint16_t MCAL_GPIO_Port_Read(GPIOx_Registers_t *GPIOx);

uint8_t MCAL_GPIO_Pin_Lock(GPIOx_Registers_t *GPIOx , uint16_t Pin_number);

#endif /* MCAL_GPIO_GPIO_H_ */
