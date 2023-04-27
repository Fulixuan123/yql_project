################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../User/doublecontorl/dj_con.c 

OBJS += \
./User/doublecontorl/dj_con.o 

C_DEPS += \
./User/doublecontorl/dj_con.d 


# Each subdirectory must supply rules for building sources it contributes
User/doublecontorl/%.o User/doublecontorl/%.su User/doublecontorl/%.cyclo: ../User/doublecontorl/%.c User/doublecontorl/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F072xB -c -I../Core/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc -I../Drivers/STM32F0xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F0xx/Include -I../Drivers/CMSIS/Include -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I"E:/mlcode/TESTPRO/User/include" -Og -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-User-2f-doublecontorl

clean-User-2f-doublecontorl:
	-$(RM) ./User/doublecontorl/dj_con.cyclo ./User/doublecontorl/dj_con.d ./User/doublecontorl/dj_con.o ./User/doublecontorl/dj_con.su

.PHONY: clean-User-2f-doublecontorl

