/*
 * std_macros.h
 *
 *  Created on: Jul 4, 2023
 *      Author: adham
 */

#ifndef INCLUDES_STD_MACROS_H_
#define INCLUDES_STD_MACROS_H_

//setting a specific bit in a given register
//@BIT_MATH macros
#define SET_BIT(REG,BIT)			(REG |= 1<<BIT)
#define CLR_BIT(REG,BIT)			(REG &= ~(1<<BIT))
#define TOGGLE_BIT(REG,BIT)			(REG ^= (1<<BIT))
#define GET_BIT(REG,BIT)			(((REG)&(1<<BIT))>>BIT)
#define SET_REG(REG)				(REG = 0xFFFFFFFF)
#define CLR_REG(REG)				(REG = 0x00000000)



#endif /* INCLUDES_STD_MACROS_H_ */
