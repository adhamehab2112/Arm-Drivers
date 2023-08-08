/*
 * GPIO.c
 *
 *  Created on: Jul 4, 2023
 *      Author: adham
 */


//***********************
//-Includes
#include "GPIO.h"
//***********************
/*======================================================
 * 													   *
 * 			Private Functions 					       *
 * 				                                       *
 *======================================================
 */
uint8_t Private_Get_CRLH_Position(uint16_t pinNumber)
{
	switch(pinNumber)
	{
	case GPIO_PIN0 :
		return 0	;
		break		;
	case GPIO_PIN1  :
		return 4	;
		break		;
	case GPIO_PIN2 :
		return 8	;
		break		;
	case GPIO_PIN3 :
		return 12	;
		break		;
	case GPIO_PIN4 :
		return 16	;
		break		;

	case GPIO_PIN5 :
		return 20	;
		break		;
	case GPIO_PIN6 :
		return 24	;
		break		;
	case GPIO_PIN7 :
		return 28	;
		break		;
	case GPIO_PIN8 :
		return 0	;
		break		;
	case GPIO_PIN9  :
		return 4	;
		break		;
	case GPIO_PIN10 :
		return 8	;
		break		;
	case GPIO_PIN11 :
		return 12	;
		break		;
	case GPIO_PIN12 :
		return 16	;
		break		;

	case GPIO_PIN13 :
		return 20	;
		break		;
	case GPIO_PIN14 :
		return 24	;
		break		;
	case GPIO_PIN15 :
		return 28	;
		break		;
	default 		:
		return -1	;
		break		;


	}
}
/**================================================================
* @Fn    		: MCAL_GPIO_Init
* @brief 		: Function used to initialize a GPIO pin as ip/op/analog/alternative
* @param [in] 	: Port base address - configurations
* @param [out] 	: none
* @retval 		: none
* Note			: remember that according to our LQFP48 package we only have ports : A-B-C-D
*===================================================================*/

void MCAL_GPIO_Init(GPIOx_Registers_t *GPIOx ,GPIO_Pin_Config_t *Pin_Config )
{
	volatile uint32_t *configReg = NULL ;
	uint8_t pinMode = 0 ;
	configReg = (Pin_Config->GPIO_PIN_NUMBER >GPIO_PIN7)? &GPIOx->GPIOx_CRH:&GPIOx->GPIOx_CRL;
	// clear MODEx and CNFx
	*configReg &= ~(0xF<<Private_Get_CRLH_Position(Pin_Config->GPIO_PIN_NUMBER));
	//check if any output mode selected
	if(Pin_Config->GPIO_PIN_MODE == GPIO_OUTPUT_OD ||Pin_Config->GPIO_PIN_MODE == GPIO_OUTPUT_PP || Pin_Config->GPIO_PIN_MODE == AFIO_OUTPUT_OD || Pin_Config->GPIO_PIN_MODE == AFIO_OUTPUT_PP )
	{
		//define MODEx and CNFx
		pinMode = ((((Pin_Config->GPIO_PIN_MODE - 4)<<2) | (Pin_Config->GPIO_OUT_SPEED)) & 0x0F);

	}
	//check if any input mode selected
	else
	{
		if(Pin_Config->GPIO_PIN_MODE == GPIO_INPUT_PUP || Pin_Config->GPIO_PIN_MODE == GPIO_INPUT_PDWN)
		{
			pinMode =((GPIO_INPUT_PUP<<2)|0x0)&0x0F ;
			if(Pin_Config->GPIO_PIN_MODE == GPIO_INPUT_PUP)
				(GPIOx->GPIOx_ODR) |= (1<<Pin_Config->GPIO_PIN_NUMBER);
			else
				(GPIOx->GPIOx_ODR) &= ~(1<<Pin_Config->GPIO_PIN_NUMBER);
		}
		else if(Pin_Config->GPIO_PIN_MODE == AFIO_INPUT)
		{
			pinMode =((GPIO_INPUT_FLOAT<<2)|0x0)&0x0F ;
		}
		else
		{
			pinMode =(((Pin_Config->GPIO_PIN_MODE)<<2)  | 0x0)&0x0F ;

		}
	}
	(*configReg) |= (pinMode<<Private_Get_CRLH_Position(Pin_Config->GPIO_PIN_NUMBER));
}

/**================================================================
* @Fn    		: MCAL_GPIO_DeInit
* @brief 		: Function used to deinitialize a GPIO port
* @param [in] 	: Port base address
* @param [out] 	: none
* @retval 		: none
* Note			: remember that according to our LQFP48 package we only have ports : A-B-C-D
*===================================================================*/
void MCAL_GPIO_DeInit(GPIOx_Registers_t *GPIOx)
{
	// can be done by 2 methods
	// 1@ put the reset values in CRL&CRH registers
	GPIOx->GPIOx_CRL = 0x44444444 	; // reset value
	GPIOx->GPIOx_CRH = 0x44444444 	;
	GPIOx->GPIOx_BRR = 0x00000000 	;
	GPIOx->GPIOx_BSRR = 0x00000000 	;
	GPIOx->GPIOx_ODR = 0x00000000 	;
	GPIOx->GPIOx_LCKR = 0x00000000 	;
	// GPIOx->GPIO_IDR (read only register )
	// 2@reset the peripheral using RCC
}

/**================================================================
* @Fn    		: MCAL_GPIO_Pin_Write
* @brief 		: Function used to write a specific value @ref:PIN_VALUE on a pin
* @param [in] 	: Port base address - Pin number - Value
* @param [out] 	: none
* @retval 		: none
* Note			: remember that according to our LQFP48 package we only have ports : A-B-C-D
*===================================================================*/
void MCAL_GPIO_Pin_Write(GPIOx_Registers_t *GPIOx , uint16_t Pin_number , uint8_t value)
{
	if(value == GPIO_PIN_HIGH)
	{
		GPIOx->GPIOx_ODR |= (1<<Pin_number);
	}
	else
	{
		GPIOx->GPIOx_ODR &= ~(1<<Pin_number);
	}
}
/**================================================================
* @Fn    		: MCAL_GPIO_Port_Write
* @brief 		: Function used to write a specific value @ref:PIN_VALUE on a port
* @param [in] 	: Port base address  - Value
* @param [out] 	: none
* @retval 		: none
* Note			: remember that according to our LQFP48 package we only have ports : A-B-C-D
*===================================================================*/
void MCAL_GPIO_Port_Write(GPIOx_Registers_t *GPIOx ,  uint16_t value)
{
	GPIOx->GPIOx_ODR = (uint32_t)value ;
}
/**================================================================
* @Fn    		: MCAL_GPIO_Pin_Read
* @brief 		: Function used to read a pin value
* @param [in] 	: Port base address  - pin number
* @param [out] 	: pin_value
* @retval 		: @ref:PIN_VALUE
* Note			: remember that according to our LQFP48 package we only have ports : A-B-C-D
*===================================================================*/
uint8_t MCAL_GPIO_Pin_Read(GPIOx_Registers_t *GPIOx , uint16_t Pin_number)
{

	if(GET_BIT(GPIOx->GPIOx_IDR,Pin_number)==1)
		return GPIO_PIN_HIGH ;
	else
		return GPIO_PIN_LOW ;

}
/**================================================================
* @Fn    		: MCAL_GPIO_Pin_Toggle
* @brief 		: Function used to write a specific value @ref:PIN_VALUE on a port
* @param [in] 	: Port base address  - pin number
* @param [out] 	: none
* @retval 		: none
* Note			: remember that according to our LQFP48 package we only have ports : A-B-C-D
*===================================================================*/
void MCAL_GPIO_Pin_Toggle(GPIOx_Registers_t *GPIOx , uint16_t Pin_number)
{
	GPIOx->GPIOx_ODR ^= (1<<Pin_number);
}
/**================================================================
* @Fn    		: MCAL_GPIO_Port_Read
* @brief 		: Function used to read a port value
* @param [in] 	: Port base address  - pin number
* @param [out] 	: pin_value
* @retval 		: @ref:PIN_VALUE
* Note			: remember that according to our LQFP48 package we only have ports : A-B-C-D
*===================================================================*/
uint16_t MCAL_GPIO_Port_Read(GPIOx_Registers_t *GPIOx)
{
	uint16_t portValue;
	portValue = (uint16_t)GPIOx->GPIOx_IDR;
	return portValue ;
}
/**================================================================
* @Fn    		: MCAL_GPIO_Pin_Lcok
* @brief 		: Function used to Lock the pin during runtime
* @param [in] 	: Port base address  - pin number
* @param [out] 	: pin_value
* @retval 		: @ref:LOCK_STATE
* Note			: remember that according to our LQFP48 package we only have ports : A-B-C-D
*===================================================================*/
uint8_t MCAL_GPIO_Pin_Lock(GPIOx_Registers_t *GPIOx , uint16_t Pin_number)
{
	//	Bit 16 LCKK[16]: Lock key
	//	This bit can be read anytime. It can only be modified using the Lock Key Writing Sequence.
	//	0: Port configuration lock key not active
	//	1: Port configuration lock key active. GPIOx_LCKR register is locked until the next reset.
	//	LOCK key writing sequence:
	//	Write 1
	//	Write 0
	//	Write 1
	//	Read 0
	//	Read 1 (this read is optional but confirms that the lock is active)
	//	Note: During the LOCK Key Writing sequence, the value of LCK[15:0] must not change.
	//	Any error in the lock sequence will abort the lock.
	//	Bits 15:0 LCKy: Port x Lock bit y (y= 0 .. 15)
	//	These bits are read write but can only be written when the LCKK bit is 0.
	//	0: Port configuration not locked
	//	1: Port configuration locked.
	volatile uint32_t temp = 1<<16 ; // set lock[16]
	temp |= (1<<Pin_number);
	//	Write 1
	//	Write 0
	//	Write 1
	//	Read 0
	GPIOx->GPIOx_LCKR = temp ;
	GPIOx->GPIOx_LCKR = (1<<Pin_number);
	GPIOx->GPIOx_LCKR = temp ;
	temp = GPIOx->GPIOx_LCKR ;
	if((uint32_t) (GPIOx->GPIOx_LCKR&(1<<16)))
	{
		return LOCK_DONE ;
	}
	else
	{
		return LOCK_FAIL ;
	}

}
