################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/hal/src/Legacy/stm32f1xx_hal_can.c 

OBJS += \
./lib/hal/src/Legacy/stm32f1xx_hal_can.o 

C_DEPS += \
./lib/hal/src/Legacy/stm32f1xx_hal_can.d 


# Each subdirectory must supply rules for building sources it contributes
lib/hal/src/Legacy/%.o lib/hal/src/Legacy/%.su lib/hal/src/Legacy/%.cyclo: ../lib/hal/src/Legacy/%.c lib/hal/src/Legacy/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F1 -DNUCLEO_F103RB -DSTM32F103RBTx -DSTM32 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../lib/bsp -I../lib//middleware/MIDI/ -I../lib/bsp/VL53L0X -I../lib/bsp/MPU6050 -I../lib/bsp/iks01a1 -I../lib/bsp/iks01a1/Common -I../lib/bsp/iks01a1/hts221 -I../lib/bsp/iks01a1/lis3mdl -I../lib/bsp/iks01a1/lps22hb -I../lib/bsp/iks01a1/lps25hb -I../lib/bsp/iks01a1/lsm6ds0 -I../lib/bsp/iks01a1/lsm6ds3 -I../lib/bsp/tft_ili9341 -I../lib/bsp/tft_pcd8544 -I../lib/bsp/MCP23S17 -I../lib/hal/inc -I../lib/bsp/Common -I../lib/bsp/lcd2x16 -I../lib/bsp/MLX90614 -I../lib/bsp/MatrixKeyboard -I../lib/bsp/MatrixLed -I../lib/CMSIS/core -I../lib/CMSIS/device -I../lib/middleware/FatFs -I../lib/middleware/FatFs/src -I../lib/middleware/FatFs/src/drivers -I../appli -O0 -ffunction-sections -fdata-sections -Wall -Wextra -Wconversion -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

clean: clean-lib-2f-hal-2f-src-2f-Legacy

clean-lib-2f-hal-2f-src-2f-Legacy:
	-$(RM) ./lib/hal/src/Legacy/stm32f1xx_hal_can.cyclo ./lib/hal/src/Legacy/stm32f1xx_hal_can.d ./lib/hal/src/Legacy/stm32f1xx_hal_can.o ./lib/hal/src/Legacy/stm32f1xx_hal_can.su

.PHONY: clean-lib-2f-hal-2f-src-2f-Legacy

