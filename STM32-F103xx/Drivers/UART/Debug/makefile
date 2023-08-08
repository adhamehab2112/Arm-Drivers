################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include Startup/subdir.mk
-include Src/subdir.mk
-include STM32_F103C6_Drivers/MCAL/USART/subdir.mk
-include STM32_F103C6_Drivers/MCAL/RCC/subdir.mk
-include STM32_F103C6_Drivers/MCAL/GPIO/subdir.mk
-include STM32_F103C6_Drivers/MCAL/EXTI/subdir.mk
-include STM32_F103C6_Drivers/HAL/LCD/subdir.mk
-include STM32_F103C6_Drivers/HAL/KEYPAD/subdir.mk
-include STM32_F103C6_Drivers/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
stm32_drivers.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
stm32_drivers.list \

OBJCOPY_BIN += \
stm32_drivers.bin \


# All Target
all: stm32_drivers.elf secondary-outputs

# Tool invocations
stm32_drivers.elf: $(OBJS) $(USER_OBJS) D:\KIROLOS\ DIPLOMA\uinte7\ interface\drivers\STM32F103C6TX_FLASH.ld
	arm-none-eabi-gcc -gdwarf-2 -o "stm32_drivers.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m3 -T"D:\KIROLOS DIPLOMA\uinte7 interface\drivers\STM32F103C6TX_FLASH.ld" --specs=nosys.specs -Wl,-Map="stm32_drivers.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

stm32_drivers.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "stm32_drivers.list"
	@echo 'Finished building: $@'
	@echo ' '

stm32_drivers.bin: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O binary $(EXECUTABLES) "stm32_drivers.bin"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_BIN)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
