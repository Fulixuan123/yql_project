################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/pid/dj_pid.c 

OBJS += \
./User/pid/dj_pid.o 

C_DEPS += \
./User/pid/dj_pid.d 


# Each subdirectory must supply rules for building sources it contributes
User/pid/%.o User/pid/%.su User/pid/%.cyclo: ../User/pid/%.c User/pid/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"E:/mlcode/TESTPRO/User/include" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-pid

clean-User-2f-pid:
	-$(RM) ./User/pid/dj_pid.cyclo ./User/pid/dj_pid.d ./User/pid/dj_pid.o ./User/pid/dj_pid.su

.PHONY: clean-User-2f-pid

