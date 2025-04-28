################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/common/core/qbuffer.c 

OBJS += \
./src/common/core/qbuffer.o 

C_DEPS += \
./src/common/core/qbuffer.d 


# Each subdirectory must supply rules for building sources it contributes
src/common/core/%.o src/common/core/%.su src/common/core/%.cyclo: ../src/common/core/%.c src/common/core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xB -c -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/bsp" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/application" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/common" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/common/core" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/common/hw/include" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/hw" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/lib" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/lib/cube_f103xx/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/lib/cube_f103xx/Drivers/CMSIS/Include" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/lib/cube_f103xx/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/lib/FreeRTOS/Source/CMSIS_RTOS" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/lib/FreeRTOS/Source/include" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/lib/FreeRTOS/Source/portable/GCC/ARM_CM3" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src-2f-common-2f-core

clean-src-2f-common-2f-core:
	-$(RM) ./src/common/core/qbuffer.cyclo ./src/common/core/qbuffer.d ./src/common/core/qbuffer.o ./src/common/core/qbuffer.su

.PHONY: clean-src-2f-common-2f-core

