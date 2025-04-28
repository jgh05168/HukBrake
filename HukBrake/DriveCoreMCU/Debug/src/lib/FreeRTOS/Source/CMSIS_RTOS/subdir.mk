################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/lib/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.c 

OBJS += \
./src/lib/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.o 

C_DEPS += \
./src/lib/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.d 


# Each subdirectory must supply rules for building sources it contributes
src/lib/FreeRTOS/Source/CMSIS_RTOS/%.o src/lib/FreeRTOS/Source/CMSIS_RTOS/%.su src/lib/FreeRTOS/Source/CMSIS_RTOS/%.cyclo: ../src/lib/FreeRTOS/Source/CMSIS_RTOS/%.c src/lib/FreeRTOS/Source/CMSIS_RTOS/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F103xB -c -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/bsp" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/application" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/common" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/common/core" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/common/hw/include" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/hw" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/lib" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/lib/cube_f103xx/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/lib/cube_f103xx/Drivers/CMSIS/Include" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/lib/cube_f103xx/Drivers/STM32F1xx_HAL_Driver/Inc" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/lib/FreeRTOS/Source/CMSIS_RTOS" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/lib/FreeRTOS/Source/include" -I"C:/Embedded_Study/HukBrake/HukBrake/DriveCoreMCU/src/lib/FreeRTOS/Source/portable/GCC/ARM_CM3" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-src-2f-lib-2f-FreeRTOS-2f-Source-2f-CMSIS_RTOS

clean-src-2f-lib-2f-FreeRTOS-2f-Source-2f-CMSIS_RTOS:
	-$(RM) ./src/lib/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.cyclo ./src/lib/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.d ./src/lib/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.o ./src/lib/FreeRTOS/Source/CMSIS_RTOS/cmsis_os.su

.PHONY: clean-src-2f-lib-2f-FreeRTOS-2f-Source-2f-CMSIS_RTOS

