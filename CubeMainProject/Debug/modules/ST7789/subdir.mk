################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../modules/ST7789/ST7789.c 

OBJS += \
./modules/ST7789/ST7789.o 

C_DEPS += \
./modules/ST7789/ST7789.d 


# Each subdirectory must supply rules for building sources it contributes
modules/ST7789/%.o modules/ST7789/%.su modules/ST7789/%.cyclo: ../modules/ST7789/%.c modules/ST7789/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F411xE -c -I../Core/Inc -I"D:/projects/SmartWatch/SW/CubeMainProject/modules/CLI" -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I"D:/projects/SmartWatch/SW/CubeMainProject/modules/ST7789" -I"D:/projects/SmartWatch/SW/CubeMainProject/thirdparty" -I"D:/projects/SmartWatch/SW/CubeMainProject/modules/CST816" -I"D:/projects/SmartWatch/SW/CubeMainProject/modules/nvm" -I"D:/projects/SmartWatch/SW/CubeMainProject/modules/power" -I"D:/projects/SmartWatch/SW/CubeMainProject/modules/RTC" -I"D:/projects/SmartWatch/SW/CubeMainProject/modules/CLI" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-modules-2f-ST7789

clean-modules-2f-ST7789:
	-$(RM) ./modules/ST7789/ST7789.cyclo ./modules/ST7789/ST7789.d ./modules/ST7789/ST7789.o ./modules/ST7789/ST7789.su

.PHONY: clean-modules-2f-ST7789

