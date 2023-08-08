/*
 * KEYPAD.h
 *
 *  Created on: Jul 7, 2023
 *      Author: adham
 */

#ifndef HAL_KEYPAD_KEYPAD_H_
#define HAL_KEYPAD_KEYPAD_H_
//-----------------------------
//Includes
#include"../../includes/std_macros.h"
#include"../../includes/std_types.h"
#include"../../includes/stm32_f103_x6.h"
#include"../../MCAL/GPIO/GPIO.h"
//-----------------------------

//-----------------------------
//KEYPAD configurations
#define KEYPAD_PORT				GPIOB
#define KEYPAD_ARR_VALUES		{{7,8,9,0},{4,5,6,0},{3,2,1,0},{0,0,0,0}}
#define NO_PRESSED_KEY			255

#define ROW_OFFSET				7
#define ROW_NUM					4

#define COL_OFFSET				3
#define COL_NUM					4


enum KEYPAD_PINS {
	COL0=3 , COL1 ,
	COL2  , COL3 ,
	ROW0 , ROW1 ,
	ROW2 , ROW3
};
//-----------------------------


/*======================================================
 * 													   	*
 * 			APIs Supported by "HAL KEYPAD DRIVER"       *
 * 				                                        *
 *======================================================
 */
void HAL_KEYPAD_Init();
uint8_t HAL_KEYPAD_Get_PressedKey();
#endif /* HAL_KEYPAD_KEYPAD_H_ */
