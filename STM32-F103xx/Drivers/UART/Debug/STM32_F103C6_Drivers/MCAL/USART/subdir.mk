################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../STM32_F103C6_Drivers/MCAL/USART/USART.c 

OBJS += \
./STM32_F103C6_Drivers/MCAL/USART/USART.o 

C_DEPS += \
./STM32_F103C6_Drivers/MCAL/USART/USART.d 


# Each subdirectory must supply rules for building sources it contributes
STM32_F103C6_Drivers/MCAL/USART/USART.o: ../STM32_F103C6_Drivers/MCAL/USART/USART.c
	arm-none-eabi-gcc -gdwarf-2 "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32 -DSTM32F1 -DSTM32F103C6Tx -DDEBUG -c -I../Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"STM32_F103C6_Drivers/MCAL/USART/USART.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

