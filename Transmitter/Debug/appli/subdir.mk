################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../appli/button_1.c \
../appli/foo.c \
../appli/led.c \
../appli/main.c 

S_UPPER_SRCS += \
../appli/assembleur.S 

OBJS += \
./appli/assembleur.o \
./appli/button_1.o \
./appli/foo.o \
./appli/led.o \
./appli/main.o 

S_UPPER_DEPS += \
./appli/assembleur.d 

C_DEPS += \
./appli/button_1.d \
./appli/foo.d \
./appli/led.d \
./appli/main.d 


# Each subdirectory must supply rules for building sources it contributes
appli/%.o: ../appli/%.S appli/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m3 -g3 -c -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@" "$<"
appli/%.o appli/%.su appli/%.cyclo: ../appli/%.c appli/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F1 -DNUCLEO_F103RB -DSTM32F103RBTx -DSTM32 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../lib/bsp -I../lib//middleware/MIDI/ -I../lib/bsp/VL53L0X -I../lib/bsp/MPU6050 -I../lib/bsp/iks01a1 -I../lib/bsp/iks01a1/Common -I../lib/bsp/iks01a1/hts221 -I../lib/bsp/iks01a1/lis3mdl -I../lib/bsp/iks01a1/lps22hb -I../lib/bsp/iks01a1/lps25hb -I../lib/bsp/iks01a1/lsm6ds0 -I../lib/bsp/iks01a1/lsm6ds3 -I../lib/bsp/tft_ili9341 -I../lib/bsp/tft_pcd8544 -I../lib/bsp/MCP23S17 -I../lib/hal/inc -I../lib/bsp/Common -I../lib/bsp/lcd2x16 -I../lib/bsp/MLX90614 -I../lib/bsp/MatrixKeyboard -I../lib/bsp/MatrixLed -I../lib/CMSIS/core -I../lib/CMSIS/device -I../lib/middleware/FatFs -I../lib/middleware/FatFs/src -I../lib/middleware/FatFs/src/drivers -I../appli -O0 -ffunction-sections -fdata-sections -Wall -Wextra -Wconversion -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

clean: clean-appli

clean-appli:
	-$(RM) ./appli/assembleur.d ./appli/assembleur.o ./appli/button_1.cyclo ./appli/button_1.d ./appli/button_1.o ./appli/button_1.su ./appli/foo.cyclo ./appli/foo.d ./appli/foo.o ./appli/foo.su ./appli/led.cyclo ./appli/led.d ./appli/led.o ./appli/led.su ./appli/main.cyclo ./appli/main.d ./appli/main.o ./appli/main.su

.PHONY: clean-appli

