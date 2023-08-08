/*
 * KEYPAD.c
 *
 *  Created on: Jul 7, 2023
 *      Author: adham
 */
#include"KEYPAD.h"
#include"../../includes/stm32_f103_x6.h"
#include"../../MCAL/GPIO/GPIO.h"
void HAL_KEYPAD_Init()
{
	GPIO_Pin_Config_t keypad_config ;
	keypad_config.GPIO_PIN_NUMBER = ROW0 ;
	keypad_config.GPIO_PIN_MODE = GPIO_INPUT_PUP ;
	MCAL_GPIO_Init(KEYPAD_PORT, &keypad_config);

	keypad_config.GPIO_PIN_NUMBER = ROW1 ;
	keypad_config.GPIO_PIN_MODE = GPIO_INPUT_PUP ;
	MCAL_GPIO_Init(KEYPAD_PORT, &keypad_config);

	keypad_config.GPIO_PIN_NUMBER = ROW2 ;
	keypad_config.GPIO_PIN_MODE = GPIO_INPUT_PUP ;
	MCAL_GPIO_Init(KEYPAD_PORT, &keypad_config);

	keypad_config.GPIO_PIN_NUMBER = ROW3 ;
	keypad_config.GPIO_PIN_MODE = GPIO_INPUT_PUP ;
	MCAL_GPIO_Init(KEYPAD_PORT, &keypad_config);

	keypad_config.GPIO_PIN_NUMBER = COL0 ;
	keypad_config.GPIO_PIN_MODE = GPIO_OUTPUT_PP ;
	keypad_config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
	MCAL_GPIO_Init(KEYPAD_PORT, &keypad_config);

	keypad_config.GPIO_PIN_NUMBER = COL1 ;
	keypad_config.GPIO_PIN_MODE = GPIO_OUTPUT_PP ;
	keypad_config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
	MCAL_GPIO_Init(KEYPAD_PORT, &keypad_config);

	keypad_config.GPIO_PIN_NUMBER = COL2 ;
	keypad_config.GPIO_PIN_MODE = GPIO_OUTPUT_PP ;
	keypad_config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
	MCAL_GPIO_Init(KEYPAD_PORT, &keypad_config);

	keypad_config.GPIO_PIN_NUMBER = COL3 ;
	keypad_config.GPIO_PIN_MODE = GPIO_OUTPUT_PP ;
	keypad_config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
	MCAL_GPIO_Init(KEYPAD_PORT, &keypad_config);
}

uint8_t HAL_KEYPAD_Get_PressedKey()
{
	MCAL_GPIO_Pin_Write(KEYPAD_PORT, COL0, GPIO_PIN_HIGH);
	MCAL_GPIO_Pin_Write(KEYPAD_PORT, COL1, GPIO_PIN_HIGH);
	MCAL_GPIO_Pin_Write(KEYPAD_PORT, COL2, GPIO_PIN_HIGH);
	MCAL_GPIO_Pin_Write(KEYPAD_PORT, COL3, GPIO_PIN_HIGH);
	uint8_t pressedKey = NO_PRESSED_KEY	;
	uint8_t row_itr , col_itr ;
	uint8_t keypadButtomns[ROW_NUM][COL_NUM] = KEYPAD_ARR_VALUES ;
	for(col_itr = COL_OFFSET ; col_itr < (COL_OFFSET+COL_NUM) ; col_itr++)
	{
		MCAL_GPIO_Pin_Write(KEYPAD_PORT, col_itr, GPIO_PIN_LOW);
		for(row_itr=ROW_OFFSET ; row_itr < (ROW_OFFSET+ROW_NUM) ; row_itr++ )
		{
			if(MCAL_GPIO_Pin_Read(KEYPAD_PORT, row_itr) == GPIO_PIN_LOW)
			{
				while(MCAL_GPIO_Pin_Read(KEYPAD_PORT, row_itr) == GPIO_PIN_LOW); // wait until key release
				return keypadButtomns[row_itr-ROW_OFFSET][col_itr-COL_OFFSET];
			}

		}
		MCAL_GPIO_Pin_Write(KEYPAD_PORT, col_itr, GPIO_PIN_HIGH);
	}
	return pressedKey ;
}

