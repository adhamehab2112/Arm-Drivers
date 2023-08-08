/*
 * LCD.c
 *
 *  Created on: Jul 6, 2023
 *      Author: adham
 */
#include"LCD.h"
#include"../../MCAL/GPIO/GPIO.h"

void wait_ms(uint32_t time)
{
	uint32_t i , j;

	for( i=0 ; i<time ; i++)
		for(j=0 ; j<255 ; j++);
}
void Private_LCD_Pins_init()
{
	// RS init
	GPIO_Pin_Config_t lcd_config ;
	lcd_config.GPIO_PIN_MODE = GPIO_OUTPUT_PP	;
	lcd_config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
	lcd_config.GPIO_PIN_NUMBER = LCD_COMM_RS ;
	MCAL_GPIO_Init(LCD_COMM_PORT, &lcd_config);

	//RW init
	lcd_config.GPIO_PIN_MODE = GPIO_OUTPUT_PP	;
	lcd_config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
	lcd_config.GPIO_PIN_NUMBER = LCD_COMM_RW ;
	MCAL_GPIO_Init(LCD_COMM_PORT, &lcd_config);
	//EN init
	lcd_config.GPIO_PIN_MODE = GPIO_OUTPUT_PP	;
	lcd_config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
	lcd_config.GPIO_PIN_NUMBER = LCD_COMM_EN ;
	MCAL_GPIO_Init(LCD_COMM_PORT, &lcd_config);

	#if(LCD_DATA_BITS_MODE == 4)
	//DATA init
	//D4
	lcd_config.GPIO_PIN_MODE = GPIO_OUTPUT_PP	;
	lcd_config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
	lcd_config.GPIO_PIN_NUMBER = LCD_DATA_D4 ;
	MCAL_GPIO_Init(LCD_DATA_PORT, &lcd_config);
	//D5
	lcd_config.GPIO_PIN_MODE = GPIO_OUTPUT_PP	;
	lcd_config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
	lcd_config.GPIO_PIN_NUMBER = LCD_DATA_D5 ;
	MCAL_GPIO_Init(LCD_DATA_PORT, &lcd_config);
	//D6
	lcd_config.GPIO_PIN_MODE = GPIO_OUTPUT_PP	;
	lcd_config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
	lcd_config.GPIO_PIN_NUMBER = LCD_DATA_D6 ;
	MCAL_GPIO_Init(LCD_DATA_PORT, &lcd_config);
	//D7
	lcd_config.GPIO_PIN_MODE = GPIO_OUTPUT_PP	;
	lcd_config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
	lcd_config.GPIO_PIN_NUMBER = LCD_DATA_D7 ;
	MCAL_GPIO_Init(LCD_DATA_PORT, &lcd_config);
#elif(LCD_DATA_BITS_MODE == 8)
	//DATA init
		//D0
		lcd_config.GPIO_PIN_MODE = GPIO_OUTPUT_PP	;
		lcd_config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
		lcd_config.GPIO_PIN_NUMBER = LCD_DATA_D0 ;
		MCAL_GPIO_Init(LCD_DATA_PORT, &lcd_config);
		//D1
		lcd_config.GPIO_PIN_MODE = GPIO_OUTPUT_PP	;
		lcd_config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
		lcd_config.GPIO_PIN_NUMBER = LCD_DATA_D1 ;
		MCAL_GPIO_Init(LCD_DATA_PORT, &lcd_config);
		//D2
		lcd_config.GPIO_PIN_MODE = GPIO_OUTPUT_PP	;
		lcd_config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
		lcd_config.GPIO_PIN_NUMBER = LCD_DATA_D2 ;
		MCAL_GPIO_Init(LCD_DATA_PORT, &lcd_config);
		//D3
		lcd_config.GPIO_PIN_MODE = GPIO_OUTPUT_PP	;
		lcd_config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
		lcd_config.GPIO_PIN_NUMBER = LCD_DATA_D3 ;
		MCAL_GPIO_Init(LCD_DATA_PORT, &lcd_config);
		//D4
		lcd_config.GPIO_PIN_MODE = GPIO_OUTPUT_PP	;
		lcd_config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
		lcd_config.GPIO_PIN_NUMBER = LCD_DATA_D4 ;
		MCAL_GPIO_Init(LCD_DATA_PORT, &lcd_config);
		//D5
		lcd_config.GPIO_PIN_MODE = GPIO_OUTPUT_PP	;
		lcd_config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
		lcd_config.GPIO_PIN_NUMBER = LCD_DATA_D5 ;
		MCAL_GPIO_Init(LCD_DATA_PORT, &lcd_config);
		//D6
		lcd_config.GPIO_PIN_MODE = GPIO_OUTPUT_PP	;
		lcd_config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
		lcd_config.GPIO_PIN_NUMBER = LCD_DATA_D6 ;
		MCAL_GPIO_Init(LCD_DATA_PORT, &lcd_config);
		//D7
		lcd_config.GPIO_PIN_MODE = GPIO_OUTPUT_PP	;
		lcd_config.GPIO_OUT_SPEED = GPIO_OUT_SPEED_10MHZ ;
		lcd_config.GPIO_PIN_NUMBER = LCD_DATA_D7 ;
		MCAL_GPIO_Init(LCD_DATA_PORT, &lcd_config);

#endif
	MCAL_GPIO_Pin_Write(LCD_COMM_PORT, LCD_COMM_EN, GPIO_PIN_LOW);
	MCAL_GPIO_Pin_Write(LCD_COMM_PORT, LCD_COMM_RS, GPIO_PIN_LOW);
	MCAL_GPIO_Pin_Write(LCD_COMM_PORT, LCD_COMM_RW, GPIO_PIN_LOW);
}
void Private_LCD_Kick()
{
	MCAL_GPIO_Pin_Write(LCD_COMM_PORT, LCD_COMM_EN, GPIO_PIN_HIGH);
	wait_ms(50);
	MCAL_GPIO_Pin_Write(LCD_COMM_PORT, LCD_COMM_EN, GPIO_PIN_LOW);
}
void HAL_LCD_Init(void)
{
	wait_ms(20);
	Private_LCD_Pins_init();
	wait_ms(15);
	HAL_LCD_Clear_Screen();
	#if(LCD_DATA_BITS_MODE == 8)
	HAL_LCD_Send_Command(LCD_8BIT_MODE_SELECT);
	#elif(LCD_DATA_BITS_MODE == 4)
	HAL_LCD_Send_Command(LCD_4BIT_MODE_SELECT_1);
	HAL_LCD_Send_Command(LCD_4BIT_MODE_SELECT_2);
	HAL_LCD_Send_Command(0x28);
	#endif
	HAL_LCD_Send_Command(LCD_ENTERY_MODE);
	HAL_LCD_Send_Command(LCD_BEGIN_AT_FIRIST_ROW);
	HAL_LCD_Send_Command(LCDC_DISPLAY_CURSOR_ON_BLINK);


}
void HAL_LCD_Send_Command(uint8_t comm)
{
	MCAL_GPIO_Pin_Write(LCD_COMM_PORT, LCD_COMM_RW, GPIO_PIN_LOW);
	MCAL_GPIO_Pin_Write(LCD_COMM_PORT, LCD_COMM_RS, GPIO_PIN_LOW);
	#if(LCD_DATA_BITS_MODE == 8)
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D0,GET_BIT(comm,0));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D1,GET_BIT(comm,1));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D2,GET_BIT(comm,2));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D3,GET_BIT(comm,3));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D4,GET_BIT(comm,4));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D5,GET_BIT(comm,5));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D6,GET_BIT(comm,6));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D7,GET_BIT(comm,7));

	wait_ms(1);
	Private_LCD_Kick();
	#elif(LCD_DATA_BITS_MODE == 4)
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D4,GET_BIT(comm,4));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D5,GET_BIT(comm,5));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D6,GET_BIT(comm,6));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D7,GET_BIT(comm,7));
	wait_ms(1)
	Private_LCD_Kick();
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D0,GET_BIT(comm,0));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D1,GET_BIT(comm,1));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D2,GET_BIT(comm,2));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D3,GET_BIT(comm,3));
	wait_ms(1)
	Private_LCD_Kick();
	#endif
}
void HAL_LCD_Display_Char(uint8_t data)
{	MCAL_GPIO_Pin_Write(LCD_COMM_PORT, LCD_COMM_RW, GPIO_PIN_LOW);
	MCAL_GPIO_Pin_Write(LCD_COMM_PORT, LCD_COMM_RS, GPIO_PIN_HIGH);
	#if(LCD_DATA_BITS_MODE == 8)
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D0,GET_BIT(data,0));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D1,GET_BIT(data,1));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D2,GET_BIT(data,2));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D3,GET_BIT(data,3));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D4,GET_BIT(data,4));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D5,GET_BIT(data,5));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D6,GET_BIT(data,6));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D7,GET_BIT(data,7));

	wait_ms(1);
	Private_LCD_Kick();
	#elif(LCD_DATA_BITS_MODE == 4)
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D4,GET_BIT(data,4));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D5,GET_BIT(data,5));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D6,GET_BIT(data,6));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D7,GET_BIT(data,7));
	Private_LCD_Kick();
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D0,GET_BIT(data,0));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D1,GET_BIT(data,1));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D2,GET_BIT(data,2));
	MCAL_GPIO_Pin_Write(LCD_DATA_PORT,LCD_DATA_D3,GET_BIT(data,3));
	Private_LCD_Kick();


	#endif

}
void HAL_LCD_Clear_Screen(void)
{
	HAL_LCD_Send_Command(LCD_CLR_SCREEN);
}
void HAL_LCD_Display_String(uint8_t *ptr_str)
{
	uint8_t itr = 0 ;
	while(ptr_str[itr] != '\0')
	{
		HAL_LCD_Display_Char(ptr_str[itr]);
		itr ++;
	}
}
void HAL_LCD_GoToXY(uint8_t row , uint8_t col)
{
	if(row == LCD_ROW1)
	{
		if(col>=LCD_Column1 && col<16)
		{
			HAL_LCD_Send_Command(0x80+col);
		}
	}
	else if(row == LCD_ROW2)
	{
		if(col>=LCD_Column1 && col<16)
		{
			HAL_LCD_Send_Command(0xC0+col);
		}
	}
}
void HAL_LCD_Display_Number(uint8_t number)
{
	HAL_LCD_Display_Char((number+48));
}
