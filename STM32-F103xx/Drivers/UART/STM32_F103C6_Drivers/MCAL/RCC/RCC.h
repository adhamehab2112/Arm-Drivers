/*
 * RCC.h
 *
 *  Created on: Jul 16, 2023
 *      Author: adham
 */

#ifndef MCAL_RCC_RCC_H_
#define MCAL_RCC_RCC_H_
#include "../../includes/stm32_f103_x6.h"
#include "../../includes/std_types.h"
#include "../../includes/std_macros.h"
//=================================================
//		          MCAL RCC APIs
//=================================================
uint32_t MCAL_RCC_Get_PCLK1_Freq();
uint32_t MCAL_RCC_Get_PCLK2_Freq();
uint32_t MCAL_RCC_Get_HCLK_Freq();
uint32_t MCAL_RCC_Get_SYSCLK_Freq();

//*~*~*~**~*~*~**~*~*~**~*~*~*
//MACROS
#define HSI_CLK		8000000UL;
//*~*~*~**~*~*~**~*~*~**~*~*~*

#endif /* MCAL_RCC_RCC_H_ */
