/*
 * RCC.c
 *
 *  Created on: Jul 16, 2023
 *      Author: adham
 */


#include"RCC.h"
uint8_t Priv_APBprescaler[8] =  {0,0,0,0,1,2,3,4};
uint8_t Priv_AHBprescaler[16] = {0,0,0,0,0,0,0,0,1,2,3,4,6,7,8,9};

uint32_t MCAL_RCC_Get_PCLK1_Freq()
{
	return (MCAL_RCC_Get_HCLK_Freq()>>Priv_APBprescaler[(RCC->RCC_CFGR>>8)&0b111]);
}
uint32_t MCAL_RCC_Get_PCLK2_Freq()
{
	return (MCAL_RCC_Get_HCLK_Freq()>>Priv_APBprescaler[(RCC->RCC_CFGR>>11)&0b111]);
}
uint32_t MCAL_RCC_Get_HCLK_Freq()
{
	return (MCAL_RCC_Get_SYSCLK_Freq()>>Priv_AHBprescaler[(RCC->RCC_CFGR>>4)&0xF]);
}
uint32_t MCAL_RCC_Get_SYSCLK_Freq()
{
	switch((RCC->RCC_CFGR>>2)&0b11)
		{
		case 0 :
			return HSI_CLK ;
			break ;
		case 1 :
			return HSI_CLK ;
			break ;
		case 2 :
			return 16000000 ;
			break ;

		}
}
