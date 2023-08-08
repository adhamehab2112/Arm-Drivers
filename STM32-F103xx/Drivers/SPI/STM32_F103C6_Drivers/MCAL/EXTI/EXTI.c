/*
 * EXTI.c
 *
 *  Created on: Jul 8, 2023
 *      Author: adham
 */
//--------------------------------
//Includes
#include"EXTI.h"
#include"../GPIO/GPIO.h"
//--------------------------------

//-------------------------------
//Macros
#define AFIO_EXTICR_VALUES(GPIO_PORT) ((GPIO_PORT == GPIOA)?0:\
									   (GPIO_PORT == GPIOB)?1:\
									   (GPIO_PORT == GPIOC)?2:\
									   (GPIO_PORT == GPIOD)?3:0 )
//-------------------------------

//-------------------------------
//Global variables

void (* Global_ptr_callBack[15])(void);

//-------------------------------

//--------------------------------
//Private Functions
void Private_Enable_NVIC(uint16_t IRQ)
{
	switch(IRQ)
			{
			case 0 :
				NVIC_IRQ6_EXTI0_EN();
				break;
			case 1 :
				NVIC_IRQ7_EXTI1_EN();
				break;
			case 2 :
				NVIC_IRQ8_EXTI2_EN();
				break;
			case 3 :
				NVIC_IRQ9_EXTI3_EN();
				break;
			case 4 :
				NVIC_IRQ10_EXTI4_EN();
				break;
			case 5 :
			case 6 :
			case 7 :
			case 8 :
			case 9 :
				NVIC_IRQ23_EXTI9_5_EN();
				break;
			case 10 :
			case 11 :
			case 12 :
			case 13 :
			case 14 :
			case 15 :
				NVIC_IRQ40_EXTI15_10_EN();
				break;
			}
}
void Private_Disable_NVIC(uint16_t IRQ)
{
	switch(IRQ)
			{
			case 0 :
				NVIC_IRQ6_EXTI0_DIS();
				break;
			case 1 :
				NVIC_IRQ7_EXTI1_DIS();
				break;
			case 2 :
				NVIC_IRQ8_EXTI2_DIS();
				break;
			case 3 :
				NVIC_IRQ9_EXTI3_DIS();
				break;
			case 4 :
				NVIC_IRQ10_EXTI4_DIS();
				break;
			case 5 :
			case 6 :
			case 7 :
			case 8 :
			case 9 :
				NVIC_IRQ23_EXTI9_5_DIS();
				break;
			case 10 :
			case 11 :
			case 12 :
			case 13 :
			case 14 :
			case 15 :
				NVIC_IRQ40_EXTI15_10_DIS();
				break;
			}
}

void Private_EXTI_Update(EXTI_Config_t *Cnfg)
{
	// config the GPIO ports
	GPIO_Pin_Config_t GPIO_EXTI_cnfg ;
	GPIO_EXTI_cnfg.GPIO_PIN_NUMBER = Cnfg->EXT_PIN_CONFIG.GPIO_PIN ;
	GPIO_EXTI_cnfg.GPIO_PIN_MODE = AFIO_INPUT ;
	MCAL_GPIO_Init(Cnfg->EXT_PIN_CONFIG.GPIO_EXTI_PORT, &GPIO_EXTI_cnfg);

	uint8_t AFIO_EXTICR_Index ;
	AFIO_EXTICR_Index = (Cnfg->EXT_PIN_CONFIG.EXTI_TNP_LINE) /4 ;
	uint8_t AFIO_EXTICR_Position ;
	AFIO_EXTICR_Position = ((Cnfg->EXT_PIN_CONFIG.EXTI_TNP_LINE)%4)*4;

	//1-4bits of selected EXTI must be cleared firist
	AFIO->ADIO_EXTICR[AFIO_EXTICR_Index] &= ~(0xF<<AFIO_EXTICR_Position);
	//2-the value to be assigned to EXTICR according to trm
	AFIO->ADIO_EXTICR[AFIO_EXTICR_Index] |= ((AFIO_EXTICR_VALUES(Cnfg->EXT_PIN_CONFIG.GPIO_EXTI_PORT)& 0x0f)<<AFIO_EXTICR_Position);
	//3-Update the Trigger State
	//reset pins first
	EXTI->EXTI_RTSR *= ~(1<<Cnfg->EXT_PIN_CONFIG.EXTI_TNP_LINE);
	EXTI->EXTI_FTSR *= ~(1<<Cnfg->EXT_PIN_CONFIG.EXTI_TNP_LINE);
	if(Cnfg->TRIGGER_CASE == EXTI_RISING_EDGE)
		EXTI->EXTI_RTSR |= (1<<Cnfg->EXT_PIN_CONFIG.EXTI_TNP_LINE);
	else if(Cnfg->TRIGGER_CASE == EXTI_FALLING_EDGE)
		EXTI->EXTI_FTSR |= (1<<Cnfg->EXT_PIN_CONFIG.EXTI_TNP_LINE);
	else if(Cnfg->TRIGGER_CASE == EXTI_RISINGANDFALLING_EDGE)
	{
		EXTI->EXTI_RTSR |= (1<<Cnfg->EXT_PIN_CONFIG.EXTI_TNP_LINE);
		EXTI->EXTI_FTSR |= (1<<Cnfg->EXT_PIN_CONFIG.EXTI_TNP_LINE);
	}
	//4-Update the IRQ call back
	Global_ptr_callBack[Cnfg->EXT_PIN_CONFIG.EXTI_TNP_LINE] = Cnfg->PTR_CALLBACK_FUN ;
	//5-Enable/Disable NVIC and IRQ
	if(Cnfg->IRQ_EN == IRQ_ENABLE)
	{
		EXTI->EXTI_IMR |= (1<<Cnfg->EXT_PIN_CONFIG.EXTI_TNP_LINE);
		Private_Enable_NVIC(Cnfg->EXT_PIN_CONFIG.EXTI_TNP_LINE);
	}
	else if(Cnfg->IRQ_EN == IRQ_DISABLE)
	{
		EXTI->EXTI_IMR &= ~(1<<Cnfg->EXT_PIN_CONFIG.EXTI_TNP_LINE);
		Private_Disable_NVIC(Cnfg->EXT_PIN_CONFIG.EXTI_TNP_LINE);
	}

}

//--------------------------------
void MCAL_EXTI_Init(EXTI_Config_t *Config)
{
	Private_EXTI_Update(Config);
}
void MCAL_EXTI_DeInit(void)
{ // -> remember that RCC does not has an access in the EXTI registers

	// 1- reset the EXTI registers
	EXTI->EXTI_EMR = 	0x00000000;
	EXTI->EXTI_IMR = 	0x00000000;
	EXTI->EXTI_FTSR = 	0x00000000;
	EXTI->EXTI_SWIER = 	0x00000000;
	EXTI->EXTI_RTSR = 	0x00000000;
	EXTI->EXTI_PR = 	0xFFFFFFFF;


	//2- disable EXTI IRQ from NVIC @ref : NVIC IRQ EN/DIS MACROS
	NVIC_IRQ6_EXTI0_DIS()		;
	NVIC_IRQ7_EXTI1_DIS()		;
	NVIC_IRQ8_EXTI2_DIS()		;
	NVIC_IRQ9_EXTI3_DIS()		;
	NVIC_IRQ10_EXTI4_DIS()		;
	NVIC_IRQ23_EXTI9_5_DIS()	;
	NVIC_IRQ40_EXTI15_10_DIS()	;
}
void MCAL_EXTI_Update(EXTI_Config_t *Config)
{
	Private_EXTI_Update(Config);
}


/*======================================================
 * 													   *
 * 			    ISR FUNCTRIONS						   *
 * 				                                       *
 *======================================================
 */
void EXTI0_IRQHandler(void)
{
	// CLear by write 1 into pending register
	EXTI->EXTI_PR |= 1<<0 ;
	// CALLBACK IRQ_CALL
	Global_ptr_callBack[0]();

}
void EXTI1_IRQHandler(void)
{
	// CLear by write 1 into pending register
	EXTI->EXTI_PR |= 1<<1 ;
	// CALLBACK IRQ_CALL
	Global_ptr_callBack[1]();
}
void EXTI2_IRQHandler(void)
{
	// CLear by write 1 into pending register
	EXTI->EXTI_PR |= 1<<2 ;
	// CALLBACK IRQ_CALL
	Global_ptr_callBack[2]();
}
void EXTI3_IRQHandler(void)
{
	// CLear by write 1 into pending register
	EXTI->EXTI_PR |= 1<<3 ;
	// CALLBACK IRQ_CALL
	Global_ptr_callBack[3]();
}
void EXTI4_IRQHandler(void)
{
	// CLear by write 1 into pending register
	EXTI->EXTI_PR |= 1<<4 ;
	// CALLBACK IRQ_CALL
	Global_ptr_callBack[4]();
}
void EXTI9_5_IRQHandler(void)
{
	if(EXTI->EXTI_PR & 1<<5){EXTI->EXTI_PR |= 1<<5 ; Global_ptr_callBack[5]();}
	if(EXTI->EXTI_PR & 1<<6){EXTI->EXTI_PR |= 1<<6 ; Global_ptr_callBack[6]();}
	if(EXTI->EXTI_PR & 1<<7){EXTI->EXTI_PR |= 1<<7 ; Global_ptr_callBack[7]();}
	if(EXTI->EXTI_PR & 1<<8){EXTI->EXTI_PR |= 1<<8 ; Global_ptr_callBack[8]();}
	if(EXTI->EXTI_PR & 1<<9){EXTI->EXTI_PR |= 1<<9 ; Global_ptr_callBack[9]();}
}
void EXTI15_10_IRQHandler(void)
{
	if(EXTI->EXTI_PR & 1<<10){EXTI->EXTI_PR |= 1<<10 ; Global_ptr_callBack[10]();}
	if(EXTI->EXTI_PR & 1<<11){EXTI->EXTI_PR |= 1<<11 ; Global_ptr_callBack[11]();}
	if(EXTI->EXTI_PR & 1<<12){EXTI->EXTI_PR |= 1<<12 ; Global_ptr_callBack[12]();}
	if(EXTI->EXTI_PR & 1<<13){EXTI->EXTI_PR |= 1<<13 ; Global_ptr_callBack[13]();}
	if(EXTI->EXTI_PR & 1<<14){EXTI->EXTI_PR |= 1<<14 ; Global_ptr_callBack[14]();}
	if(EXTI->EXTI_PR & 1<<15){EXTI->EXTI_PR |= 1<<15 ; Global_ptr_callBack[15]();}
}
