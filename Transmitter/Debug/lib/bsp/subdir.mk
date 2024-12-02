################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/bsp/stm32f1_adc.c \
../lib/bsp/stm32f1_extit.c \
../lib/bsp/stm32f1_flash.c \
../lib/bsp/stm32f1_gpio.c \
../lib/bsp/stm32f1_i2c.c \
../lib/bsp/stm32f1_motorDC.c \
../lib/bsp/stm32f1_pwm.c \
../lib/bsp/stm32f1_rtc.c \
../lib/bsp/stm32f1_sd.c \
../lib/bsp/stm32f1_spi.c \
../lib/bsp/stm32f1_stepper_motor.c \
../lib/bsp/stm32f1_sys.c \
../lib/bsp/stm32f1_timer.c \
../lib/bsp/stm32f1_uart.c \
../lib/bsp/stm32f1_weak_it.c \
../lib/bsp/stm32f1xx_nucleo.c \
../lib/bsp/system_stm32f1xx.c \
../lib/bsp/systick.c 

OBJS += \
./lib/bsp/stm32f1_adc.o \
./lib/bsp/stm32f1_extit.o \
./lib/bsp/stm32f1_flash.o \
./lib/bsp/stm32f1_gpio.o \
./lib/bsp/stm32f1_i2c.o \
./lib/bsp/stm32f1_motorDC.o \
./lib/bsp/stm32f1_pwm.o \
./lib/bsp/stm32f1_rtc.o \
./lib/bsp/stm32f1_sd.o \
./lib/bsp/stm32f1_spi.o \
./lib/bsp/stm32f1_stepper_motor.o \
./lib/bsp/stm32f1_sys.o \
./lib/bsp/stm32f1_timer.o \
./lib/bsp/stm32f1_uart.o \
./lib/bsp/stm32f1_weak_it.o \
./lib/bsp/stm32f1xx_nucleo.o \
./lib/bsp/system_stm32f1xx.o \
./lib/bsp/systick.o 

C_DEPS += \
./lib/bsp/stm32f1_adc.d \
./lib/bsp/stm32f1_extit.d \
./lib/bsp/stm32f1_flash.d \
./lib/bsp/stm32f1_gpio.d \
./lib/bsp/stm32f1_i2c.d \
./lib/bsp/stm32f1_motorDC.d \
./lib/bsp/stm32f1_pwm.d \
./lib/bsp/stm32f1_rtc.d \
./lib/bsp/stm32f1_sd.d \
./lib/bsp/stm32f1_spi.d \
./lib/bsp/stm32f1_stepper_motor.d \
./lib/bsp/stm32f1_sys.d \
./lib/bsp/stm32f1_timer.d \
./lib/bsp/stm32f1_uart.d \
./lib/bsp/stm32f1_weak_it.d \
./lib/bsp/stm32f1xx_nucleo.d \
./lib/bsp/system_stm32f1xx.d \
./lib/bsp/systick.d 


# Each subdirectory must supply rules for building sources it contributes
lib/bsp/%.o lib/bsp/%.su lib/bsp/%.cyclo: ../lib/bsp/%.c lib/bsp/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F1 -DNUCLEO_F103RB -DSTM32F103RBTx -DSTM32 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../lib/bsp -I../lib//middleware/MIDI/ -I../lib/bsp/VL53L0X -I../lib/bsp/MPU6050 -I../lib/bsp/iks01a1 -I../lib/bsp/iks01a1/Common -I../lib/bsp/iks01a1/hts221 -I../lib/bsp/iks01a1/lis3mdl -I../lib/bsp/iks01a1/lps22hb -I../lib/bsp/iks01a1/lps25hb -I../lib/bsp/iks01a1/lsm6ds0 -I../lib/bsp/iks01a1/lsm6ds3 -I../lib/bsp/tft_ili9341 -I../lib/bsp/tft_pcd8544 -I../lib/bsp/MCP23S17 -I../lib/hal/inc -I../lib/bsp/Common -I../lib/bsp/lcd2x16 -I../lib/bsp/MLX90614 -I../lib/bsp/MatrixKeyboard -I../lib/bsp/MatrixLed -I../lib/CMSIS/core -I../lib/CMSIS/device -I../lib/middleware/FatFs -I../lib/middleware/FatFs/src -I../lib/middleware/FatFs/src/drivers -I../appli -O0 -ffunction-sections -fdata-sections -Wall -Wextra -Wconversion -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

clean: clean-lib-2f-bsp

clean-lib-2f-bsp:
	-$(RM) ./lib/bsp/stm32f1_adc.cyclo ./lib/bsp/stm32f1_adc.d ./lib/bsp/stm32f1_adc.o ./lib/bsp/stm32f1_adc.su ./lib/bsp/stm32f1_extit.cyclo ./lib/bsp/stm32f1_extit.d ./lib/bsp/stm32f1_extit.o ./lib/bsp/stm32f1_extit.su ./lib/bsp/stm32f1_flash.cyclo ./lib/bsp/stm32f1_flash.d ./lib/bsp/stm32f1_flash.o ./lib/bsp/stm32f1_flash.su ./lib/bsp/stm32f1_gpio.cyclo ./lib/bsp/stm32f1_gpio.d ./lib/bsp/stm32f1_gpio.o ./lib/bsp/stm32f1_gpio.su ./lib/bsp/stm32f1_i2c.cyclo ./lib/bsp/stm32f1_i2c.d ./lib/bsp/stm32f1_i2c.o ./lib/bsp/stm32f1_i2c.su ./lib/bsp/stm32f1_motorDC.cyclo ./lib/bsp/stm32f1_motorDC.d ./lib/bsp/stm32f1_motorDC.o ./lib/bsp/stm32f1_motorDC.su ./lib/bsp/stm32f1_pwm.cyclo ./lib/bsp/stm32f1_pwm.d ./lib/bsp/stm32f1_pwm.o ./lib/bsp/stm32f1_pwm.su ./lib/bsp/stm32f1_rtc.cyclo ./lib/bsp/stm32f1_rtc.d ./lib/bsp/stm32f1_rtc.o ./lib/bsp/stm32f1_rtc.su ./lib/bsp/stm32f1_sd.cyclo ./lib/bsp/stm32f1_sd.d ./lib/bsp/stm32f1_sd.o ./lib/bsp/stm32f1_sd.su ./lib/bsp/stm32f1_spi.cyclo ./lib/bsp/stm32f1_spi.d ./lib/bsp/stm32f1_spi.o ./lib/bsp/stm32f1_spi.su ./lib/bsp/stm32f1_stepper_motor.cyclo ./lib/bsp/stm32f1_stepper_motor.d ./lib/bsp/stm32f1_stepper_motor.o ./lib/bsp/stm32f1_stepper_motor.su ./lib/bsp/stm32f1_sys.cyclo ./lib/bsp/stm32f1_sys.d ./lib/bsp/stm32f1_sys.o ./lib/bsp/stm32f1_sys.su ./lib/bsp/stm32f1_timer.cyclo ./lib/bsp/stm32f1_timer.d ./lib/bsp/stm32f1_timer.o ./lib/bsp/stm32f1_timer.su ./lib/bsp/stm32f1_uart.cyclo ./lib/bsp/stm32f1_uart.d ./lib/bsp/stm32f1_uart.o ./lib/bsp/stm32f1_uart.su ./lib/bsp/stm32f1_weak_it.cyclo ./lib/bsp/stm32f1_weak_it.d ./lib/bsp/stm32f1_weak_it.o ./lib/bsp/stm32f1_weak_it.su ./lib/bsp/stm32f1xx_nucleo.cyclo ./lib/bsp/stm32f1xx_nucleo.d ./lib/bsp/stm32f1xx_nucleo.o ./lib/bsp/stm32f1xx_nucleo.su ./lib/bsp/system_stm32f1xx.cyclo ./lib/bsp/system_stm32f1xx.d ./lib/bsp/system_stm32f1xx.o ./lib/bsp/system_stm32f1xx.su ./lib/bsp/systick.cyclo ./lib/bsp/systick.d ./lib/bsp/systick.o ./lib/bsp/systick.su

.PHONY: clean-lib-2f-bsp

