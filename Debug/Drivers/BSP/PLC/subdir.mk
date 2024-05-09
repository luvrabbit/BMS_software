################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/PLC/plc.c 

OBJS += \
./Drivers/BSP/PLC/plc.o 

C_DEPS += \
./Drivers/BSP/PLC/plc.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/PLC/%.o Drivers/BSP/PLC/%.su Drivers/BSP/PLC/%.cyclo: ../Drivers/BSP/PLC/%.c Drivers/BSP/PLC/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-PLC

clean-Drivers-2f-BSP-2f-PLC:
	-$(RM) ./Drivers/BSP/PLC/plc.cyclo ./Drivers/BSP/PLC/plc.d ./Drivers/BSP/PLC/plc.o ./Drivers/BSP/PLC/plc.su

.PHONY: clean-Drivers-2f-BSP-2f-PLC

