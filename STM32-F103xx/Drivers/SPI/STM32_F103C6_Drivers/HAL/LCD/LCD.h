/*
 * LCD.h
 *
 *  Created on: Jul 6, 2023
 *      Author: adham
 */

#ifndef HAL_LCD_LCD_H_
#define HAL_LCD_LCD_H_
//-----------------------------
//Includes
#include"../../includes/std_macros.h"
#include"../../includes/std_types.h"
#include"../../includes/stm32_f103_x6.h"
#include"../../MCAL/GPIO/GPIO.h"
//-----------------------------
#define F_CPU 1000000UL
#define LCD_DATA_BITS_MODE 8

#if((LCD_DATA_BITS_MODE != 4) && (LCD_DATA_BITS_MODE != 8))

#error "Number of Data bits should be equal to 4 or 8"

#endif

//-----------------------------
//LCD Pins
#define LCD_DATA_PORT		GPIOA
#define LCD_COMM_PORT		GPIOA

#define LCD_COMM_RS			GPIO_PIN8
#define LCD_COMM_RW			GPIO_PIN9
#define LCD_COMM_EN			GPIO_PIN10

#define LCD_DATA_D0			GPIO_PIN0
#define LCD_DATA_D1			GPIO_PIN1
#define LCD_DATA_D2			GPIO_PIN2
#define LCD_DATA_D3			GPIO_PIN3
#define LCD_DATA_D4			GPIO_PIN4
#define LCD_DATA_D5			GPIO_PIN5
#define LCD_DATA_D6			GPIO_PIN6
#define LCD_DATA_D7			GPIO_PIN7

//-----------------------------




//-----------------------------
//LCD Commands
#define LCD_CLR_SCREEN					(0x01)
#define LCD_RETURN_HOME					(0x02)
#define	LCD_ENTERY_MODE					(0x06)
#define LCD_DISPLAY_CURSOR_ON			(0x0E)
#define LCDC_DISPLAY_CURSOR_ON_BLINK 	(0x0F)
#define LCD_BEGIN_AT_FIRIST_ROW			(0x80)
#define LCD_8BIT_MODE_SELECT			(0x38)
#define LCD_4BIT_MODE_SELECT_1			(0x33)
#define LCD_4BIT_MODE_SELECT_2			(0x32)

//-----------------------------




//-----------------------------
//LCD Rows and Columns
enum Rows
{
	LCD_ROW1=0,
	LCD_ROW2,
};

enum Columns
{
	LCD_Column1=0, LCD_Column2, LCD_Column3, LCD_Column4, LCD_Column5,
	LCD_Column6, LCD_Column7, LCD_Column8, LCD_Column9, LCD_Column10,
	LCD_Column11, LCD_Column12, LCD_Column13, LCD_Column14, LCD_Column15,
	LCD_Column16, LCD_Column17, LCD_Column18, LCD_Column19, LCD_Column20,
	LCD_Column21, LCD_Column22, LCD_Column23, LCD_Column24, LCD_Column25,
	LCD_Column26,	LCD_Column27, LCD_Column28, LCD_Column29, LCD_Column30,
	LCD_Column31, LCD_Column32
	};

//-----------------------------



//-----------------------------
/*======================================================
 * 													   *
 * 			APIs Supported by "HAL LCD DRIVER"       *
 * 				                                       *
 *======================================================
 */
void HAL_LCD_Display_Char(uint8_t data);
void HAL_LCD_Send_Command(uint8_t comm);
void HAL_LCD_Clear_Screen(void);
void HAL_LCD_Init(void);
void HAL_LCD_GoToXY(uint8_t row , uint8_t col);
void HAL_LCD_Display_String(uint8_t *ptr_str);
void HAL_LCD_Display_Number(uint8_t number);
void HAL_LCD_Display_MultiDigNum(uint32_t number);

//-----------------------------




#endif /* HAL_LCD_LCD_H_ */
