################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../lib/hal/src/stm32f1xx_hal.c \
../lib/hal/src/stm32f1xx_hal_adc.c \
../lib/hal/src/stm32f1xx_hal_adc_ex.c \
../lib/hal/src/stm32f1xx_hal_can.c \
../lib/hal/src/stm32f1xx_hal_cec.c \
../lib/hal/src/stm32f1xx_hal_cortex.c \
../lib/hal/src/stm32f1xx_hal_crc.c \
../lib/hal/src/stm32f1xx_hal_dac.c \
../lib/hal/src/stm32f1xx_hal_dac_ex.c \
../lib/hal/src/stm32f1xx_hal_dma.c \
../lib/hal/src/stm32f1xx_hal_eth.c \
../lib/hal/src/stm32f1xx_hal_exti.c \
../lib/hal/src/stm32f1xx_hal_flash.c \
../lib/hal/src/stm32f1xx_hal_flash_ex.c \
../lib/hal/src/stm32f1xx_hal_gpio.c \
../lib/hal/src/stm32f1xx_hal_gpio_ex.c \
../lib/hal/src/stm32f1xx_hal_hcd.c \
../lib/hal/src/stm32f1xx_hal_i2c.c \
../lib/hal/src/stm32f1xx_hal_i2s.c \
../lib/hal/src/stm32f1xx_hal_irda.c \
../lib/hal/src/stm32f1xx_hal_iwdg.c \
../lib/hal/src/stm32f1xx_hal_mmc.c \
../lib/hal/src/stm32f1xx_hal_msp_template.c \
../lib/hal/src/stm32f1xx_hal_nand.c \
../lib/hal/src/stm32f1xx_hal_nor.c \
../lib/hal/src/stm32f1xx_hal_pccard.c \
../lib/hal/src/stm32f1xx_hal_pcd.c \
../lib/hal/src/stm32f1xx_hal_pcd_ex.c \
../lib/hal/src/stm32f1xx_hal_pwr.c \
../lib/hal/src/stm32f1xx_hal_rcc.c \
../lib/hal/src/stm32f1xx_hal_rcc_ex.c \
../lib/hal/src/stm32f1xx_hal_rtc.c \
../lib/hal/src/stm32f1xx_hal_rtc_ex.c \
../lib/hal/src/stm32f1xx_hal_sd.c \
../lib/hal/src/stm32f1xx_hal_smartcard.c \
../lib/hal/src/stm32f1xx_hal_spi.c \
../lib/hal/src/stm32f1xx_hal_spi_ex.c \
../lib/hal/src/stm32f1xx_hal_sram.c \
../lib/hal/src/stm32f1xx_hal_tim.c \
../lib/hal/src/stm32f1xx_hal_tim_ex.c \
../lib/hal/src/stm32f1xx_hal_uart.c \
../lib/hal/src/stm32f1xx_hal_usart.c \
../lib/hal/src/stm32f1xx_hal_wwdg.c \
../lib/hal/src/stm32f1xx_ll_adc.c \
../lib/hal/src/stm32f1xx_ll_crc.c \
../lib/hal/src/stm32f1xx_ll_dac.c \
../lib/hal/src/stm32f1xx_ll_dma.c \
../lib/hal/src/stm32f1xx_ll_exti.c \
../lib/hal/src/stm32f1xx_ll_fsmc.c \
../lib/hal/src/stm32f1xx_ll_gpio.c \
../lib/hal/src/stm32f1xx_ll_i2c.c \
../lib/hal/src/stm32f1xx_ll_pwr.c \
../lib/hal/src/stm32f1xx_ll_rcc.c \
../lib/hal/src/stm32f1xx_ll_rtc.c \
../lib/hal/src/stm32f1xx_ll_sdmmc.c \
../lib/hal/src/stm32f1xx_ll_spi.c \
../lib/hal/src/stm32f1xx_ll_tim.c \
../lib/hal/src/stm32f1xx_ll_usart.c \
../lib/hal/src/stm32f1xx_ll_usb.c \
../lib/hal/src/stm32f1xx_ll_utils.c 

OBJS += \
./lib/hal/src/stm32f1xx_hal.o \
./lib/hal/src/stm32f1xx_hal_adc.o \
./lib/hal/src/stm32f1xx_hal_adc_ex.o \
./lib/hal/src/stm32f1xx_hal_can.o \
./lib/hal/src/stm32f1xx_hal_cec.o \
./lib/hal/src/stm32f1xx_hal_cortex.o \
./lib/hal/src/stm32f1xx_hal_crc.o \
./lib/hal/src/stm32f1xx_hal_dac.o \
./lib/hal/src/stm32f1xx_hal_dac_ex.o \
./lib/hal/src/stm32f1xx_hal_dma.o \
./lib/hal/src/stm32f1xx_hal_eth.o \
./lib/hal/src/stm32f1xx_hal_exti.o \
./lib/hal/src/stm32f1xx_hal_flash.o \
./lib/hal/src/stm32f1xx_hal_flash_ex.o \
./lib/hal/src/stm32f1xx_hal_gpio.o \
./lib/hal/src/stm32f1xx_hal_gpio_ex.o \
./lib/hal/src/stm32f1xx_hal_hcd.o \
./lib/hal/src/stm32f1xx_hal_i2c.o \
./lib/hal/src/stm32f1xx_hal_i2s.o \
./lib/hal/src/stm32f1xx_hal_irda.o \
./lib/hal/src/stm32f1xx_hal_iwdg.o \
./lib/hal/src/stm32f1xx_hal_mmc.o \
./lib/hal/src/stm32f1xx_hal_msp_template.o \
./lib/hal/src/stm32f1xx_hal_nand.o \
./lib/hal/src/stm32f1xx_hal_nor.o \
./lib/hal/src/stm32f1xx_hal_pccard.o \
./lib/hal/src/stm32f1xx_hal_pcd.o \
./lib/hal/src/stm32f1xx_hal_pcd_ex.o \
./lib/hal/src/stm32f1xx_hal_pwr.o \
./lib/hal/src/stm32f1xx_hal_rcc.o \
./lib/hal/src/stm32f1xx_hal_rcc_ex.o \
./lib/hal/src/stm32f1xx_hal_rtc.o \
./lib/hal/src/stm32f1xx_hal_rtc_ex.o \
./lib/hal/src/stm32f1xx_hal_sd.o \
./lib/hal/src/stm32f1xx_hal_smartcard.o \
./lib/hal/src/stm32f1xx_hal_spi.o \
./lib/hal/src/stm32f1xx_hal_spi_ex.o \
./lib/hal/src/stm32f1xx_hal_sram.o \
./lib/hal/src/stm32f1xx_hal_tim.o \
./lib/hal/src/stm32f1xx_hal_tim_ex.o \
./lib/hal/src/stm32f1xx_hal_uart.o \
./lib/hal/src/stm32f1xx_hal_usart.o \
./lib/hal/src/stm32f1xx_hal_wwdg.o \
./lib/hal/src/stm32f1xx_ll_adc.o \
./lib/hal/src/stm32f1xx_ll_crc.o \
./lib/hal/src/stm32f1xx_ll_dac.o \
./lib/hal/src/stm32f1xx_ll_dma.o \
./lib/hal/src/stm32f1xx_ll_exti.o \
./lib/hal/src/stm32f1xx_ll_fsmc.o \
./lib/hal/src/stm32f1xx_ll_gpio.o \
./lib/hal/src/stm32f1xx_ll_i2c.o \
./lib/hal/src/stm32f1xx_ll_pwr.o \
./lib/hal/src/stm32f1xx_ll_rcc.o \
./lib/hal/src/stm32f1xx_ll_rtc.o \
./lib/hal/src/stm32f1xx_ll_sdmmc.o \
./lib/hal/src/stm32f1xx_ll_spi.o \
./lib/hal/src/stm32f1xx_ll_tim.o \
./lib/hal/src/stm32f1xx_ll_usart.o \
./lib/hal/src/stm32f1xx_ll_usb.o \
./lib/hal/src/stm32f1xx_ll_utils.o 

C_DEPS += \
./lib/hal/src/stm32f1xx_hal.d \
./lib/hal/src/stm32f1xx_hal_adc.d \
./lib/hal/src/stm32f1xx_hal_adc_ex.d \
./lib/hal/src/stm32f1xx_hal_can.d \
./lib/hal/src/stm32f1xx_hal_cec.d \
./lib/hal/src/stm32f1xx_hal_cortex.d \
./lib/hal/src/stm32f1xx_hal_crc.d \
./lib/hal/src/stm32f1xx_hal_dac.d \
./lib/hal/src/stm32f1xx_hal_dac_ex.d \
./lib/hal/src/stm32f1xx_hal_dma.d \
./lib/hal/src/stm32f1xx_hal_eth.d \
./lib/hal/src/stm32f1xx_hal_exti.d \
./lib/hal/src/stm32f1xx_hal_flash.d \
./lib/hal/src/stm32f1xx_hal_flash_ex.d \
./lib/hal/src/stm32f1xx_hal_gpio.d \
./lib/hal/src/stm32f1xx_hal_gpio_ex.d \
./lib/hal/src/stm32f1xx_hal_hcd.d \
./lib/hal/src/stm32f1xx_hal_i2c.d \
./lib/hal/src/stm32f1xx_hal_i2s.d \
./lib/hal/src/stm32f1xx_hal_irda.d \
./lib/hal/src/stm32f1xx_hal_iwdg.d \
./lib/hal/src/stm32f1xx_hal_mmc.d \
./lib/hal/src/stm32f1xx_hal_msp_template.d \
./lib/hal/src/stm32f1xx_hal_nand.d \
./lib/hal/src/stm32f1xx_hal_nor.d \
./lib/hal/src/stm32f1xx_hal_pccard.d \
./lib/hal/src/stm32f1xx_hal_pcd.d \
./lib/hal/src/stm32f1xx_hal_pcd_ex.d \
./lib/hal/src/stm32f1xx_hal_pwr.d \
./lib/hal/src/stm32f1xx_hal_rcc.d \
./lib/hal/src/stm32f1xx_hal_rcc_ex.d \
./lib/hal/src/stm32f1xx_hal_rtc.d \
./lib/hal/src/stm32f1xx_hal_rtc_ex.d \
./lib/hal/src/stm32f1xx_hal_sd.d \
./lib/hal/src/stm32f1xx_hal_smartcard.d \
./lib/hal/src/stm32f1xx_hal_spi.d \
./lib/hal/src/stm32f1xx_hal_spi_ex.d \
./lib/hal/src/stm32f1xx_hal_sram.d \
./lib/hal/src/stm32f1xx_hal_tim.d \
./lib/hal/src/stm32f1xx_hal_tim_ex.d \
./lib/hal/src/stm32f1xx_hal_uart.d \
./lib/hal/src/stm32f1xx_hal_usart.d \
./lib/hal/src/stm32f1xx_hal_wwdg.d \
./lib/hal/src/stm32f1xx_ll_adc.d \
./lib/hal/src/stm32f1xx_ll_crc.d \
./lib/hal/src/stm32f1xx_ll_dac.d \
./lib/hal/src/stm32f1xx_ll_dma.d \
./lib/hal/src/stm32f1xx_ll_exti.d \
./lib/hal/src/stm32f1xx_ll_fsmc.d \
./lib/hal/src/stm32f1xx_ll_gpio.d \
./lib/hal/src/stm32f1xx_ll_i2c.d \
./lib/hal/src/stm32f1xx_ll_pwr.d \
./lib/hal/src/stm32f1xx_ll_rcc.d \
./lib/hal/src/stm32f1xx_ll_rtc.d \
./lib/hal/src/stm32f1xx_ll_sdmmc.d \
./lib/hal/src/stm32f1xx_ll_spi.d \
./lib/hal/src/stm32f1xx_ll_tim.d \
./lib/hal/src/stm32f1xx_ll_usart.d \
./lib/hal/src/stm32f1xx_ll_usb.d \
./lib/hal/src/stm32f1xx_ll_utils.d 


# Each subdirectory must supply rules for building sources it contributes
lib/hal/src/%.o lib/hal/src/%.su lib/hal/src/%.cyclo: ../lib/hal/src/%.c lib/hal/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DSTM32F1 -DNUCLEO_F103RB -DSTM32F103RBTx -DSTM32 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../lib/bsp -I../lib//middleware/MIDI/ -I../lib/bsp/VL53L0X -I../lib/bsp/MPU6050 -I../lib/bsp/iks01a1 -I../lib/bsp/iks01a1/Common -I../lib/bsp/iks01a1/hts221 -I../lib/bsp/iks01a1/lis3mdl -I../lib/bsp/iks01a1/lps22hb -I../lib/bsp/iks01a1/lps25hb -I../lib/bsp/iks01a1/lsm6ds0 -I../lib/bsp/iks01a1/lsm6ds3 -I../lib/bsp/tft_ili9341 -I../lib/bsp/tft_pcd8544 -I../lib/bsp/MCP23S17 -I../lib/hal/inc -I../lib/bsp/Common -I../lib/bsp/lcd2x16 -I../lib/bsp/MLX90614 -I../lib/bsp/MatrixKeyboard -I../lib/bsp/MatrixLed -I../lib/CMSIS/core -I../lib/CMSIS/device -I../lib/middleware/FatFs -I../lib/middleware/FatFs/src -I../lib/middleware/FatFs/src/drivers -I../appli -O0 -ffunction-sections -fdata-sections -Wall -Wextra -Wconversion -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@"  -mfloat-abi=soft -mthumb -o "$@"

clean: clean-lib-2f-hal-2f-src

clean-lib-2f-hal-2f-src:
	-$(RM) ./lib/hal/src/stm32f1xx_hal.cyclo ./lib/hal/src/stm32f1xx_hal.d ./lib/hal/src/stm32f1xx_hal.o ./lib/hal/src/stm32f1xx_hal.su ./lib/hal/src/stm32f1xx_hal_adc.cyclo ./lib/hal/src/stm32f1xx_hal_adc.d ./lib/hal/src/stm32f1xx_hal_adc.o ./lib/hal/src/stm32f1xx_hal_adc.su ./lib/hal/src/stm32f1xx_hal_adc_ex.cyclo ./lib/hal/src/stm32f1xx_hal_adc_ex.d ./lib/hal/src/stm32f1xx_hal_adc_ex.o ./lib/hal/src/stm32f1xx_hal_adc_ex.su ./lib/hal/src/stm32f1xx_hal_can.cyclo ./lib/hal/src/stm32f1xx_hal_can.d ./lib/hal/src/stm32f1xx_hal_can.o ./lib/hal/src/stm32f1xx_hal_can.su ./lib/hal/src/stm32f1xx_hal_cec.cyclo ./lib/hal/src/stm32f1xx_hal_cec.d ./lib/hal/src/stm32f1xx_hal_cec.o ./lib/hal/src/stm32f1xx_hal_cec.su ./lib/hal/src/stm32f1xx_hal_cortex.cyclo ./lib/hal/src/stm32f1xx_hal_cortex.d ./lib/hal/src/stm32f1xx_hal_cortex.o ./lib/hal/src/stm32f1xx_hal_cortex.su ./lib/hal/src/stm32f1xx_hal_crc.cyclo ./lib/hal/src/stm32f1xx_hal_crc.d ./lib/hal/src/stm32f1xx_hal_crc.o ./lib/hal/src/stm32f1xx_hal_crc.su ./lib/hal/src/stm32f1xx_hal_dac.cyclo ./lib/hal/src/stm32f1xx_hal_dac.d ./lib/hal/src/stm32f1xx_hal_dac.o ./lib/hal/src/stm32f1xx_hal_dac.su ./lib/hal/src/stm32f1xx_hal_dac_ex.cyclo ./lib/hal/src/stm32f1xx_hal_dac_ex.d ./lib/hal/src/stm32f1xx_hal_dac_ex.o ./lib/hal/src/stm32f1xx_hal_dac_ex.su ./lib/hal/src/stm32f1xx_hal_dma.cyclo ./lib/hal/src/stm32f1xx_hal_dma.d ./lib/hal/src/stm32f1xx_hal_dma.o ./lib/hal/src/stm32f1xx_hal_dma.su ./lib/hal/src/stm32f1xx_hal_eth.cyclo ./lib/hal/src/stm32f1xx_hal_eth.d ./lib/hal/src/stm32f1xx_hal_eth.o ./lib/hal/src/stm32f1xx_hal_eth.su ./lib/hal/src/stm32f1xx_hal_exti.cyclo ./lib/hal/src/stm32f1xx_hal_exti.d ./lib/hal/src/stm32f1xx_hal_exti.o ./lib/hal/src/stm32f1xx_hal_exti.su ./lib/hal/src/stm32f1xx_hal_flash.cyclo ./lib/hal/src/stm32f1xx_hal_flash.d ./lib/hal/src/stm32f1xx_hal_flash.o ./lib/hal/src/stm32f1xx_hal_flash.su ./lib/hal/src/stm32f1xx_hal_flash_ex.cyclo ./lib/hal/src/stm32f1xx_hal_flash_ex.d ./lib/hal/src/stm32f1xx_hal_flash_ex.o ./lib/hal/src/stm32f1xx_hal_flash_ex.su ./lib/hal/src/stm32f1xx_hal_gpio.cyclo ./lib/hal/src/stm32f1xx_hal_gpio.d ./lib/hal/src/stm32f1xx_hal_gpio.o ./lib/hal/src/stm32f1xx_hal_gpio.su ./lib/hal/src/stm32f1xx_hal_gpio_ex.cyclo ./lib/hal/src/stm32f1xx_hal_gpio_ex.d ./lib/hal/src/stm32f1xx_hal_gpio_ex.o ./lib/hal/src/stm32f1xx_hal_gpio_ex.su ./lib/hal/src/stm32f1xx_hal_hcd.cyclo ./lib/hal/src/stm32f1xx_hal_hcd.d ./lib/hal/src/stm32f1xx_hal_hcd.o ./lib/hal/src/stm32f1xx_hal_hcd.su ./lib/hal/src/stm32f1xx_hal_i2c.cyclo ./lib/hal/src/stm32f1xx_hal_i2c.d ./lib/hal/src/stm32f1xx_hal_i2c.o ./lib/hal/src/stm32f1xx_hal_i2c.su ./lib/hal/src/stm32f1xx_hal_i2s.cyclo ./lib/hal/src/stm32f1xx_hal_i2s.d ./lib/hal/src/stm32f1xx_hal_i2s.o ./lib/hal/src/stm32f1xx_hal_i2s.su ./lib/hal/src/stm32f1xx_hal_irda.cyclo ./lib/hal/src/stm32f1xx_hal_irda.d ./lib/hal/src/stm32f1xx_hal_irda.o ./lib/hal/src/stm32f1xx_hal_irda.su ./lib/hal/src/stm32f1xx_hal_iwdg.cyclo ./lib/hal/src/stm32f1xx_hal_iwdg.d ./lib/hal/src/stm32f1xx_hal_iwdg.o ./lib/hal/src/stm32f1xx_hal_iwdg.su ./lib/hal/src/stm32f1xx_hal_mmc.cyclo ./lib/hal/src/stm32f1xx_hal_mmc.d ./lib/hal/src/stm32f1xx_hal_mmc.o ./lib/hal/src/stm32f1xx_hal_mmc.su ./lib/hal/src/stm32f1xx_hal_msp_template.cyclo ./lib/hal/src/stm32f1xx_hal_msp_template.d ./lib/hal/src/stm32f1xx_hal_msp_template.o ./lib/hal/src/stm32f1xx_hal_msp_template.su ./lib/hal/src/stm32f1xx_hal_nand.cyclo ./lib/hal/src/stm32f1xx_hal_nand.d ./lib/hal/src/stm32f1xx_hal_nand.o ./lib/hal/src/stm32f1xx_hal_nand.su ./lib/hal/src/stm32f1xx_hal_nor.cyclo ./lib/hal/src/stm32f1xx_hal_nor.d ./lib/hal/src/stm32f1xx_hal_nor.o ./lib/hal/src/stm32f1xx_hal_nor.su ./lib/hal/src/stm32f1xx_hal_pccard.cyclo ./lib/hal/src/stm32f1xx_hal_pccard.d ./lib/hal/src/stm32f1xx_hal_pccard.o ./lib/hal/src/stm32f1xx_hal_pccard.su ./lib/hal/src/stm32f1xx_hal_pcd.cyclo ./lib/hal/src/stm32f1xx_hal_pcd.d ./lib/hal/src/stm32f1xx_hal_pcd.o ./lib/hal/src/stm32f1xx_hal_pcd.su ./lib/hal/src/stm32f1xx_hal_pcd_ex.cyclo ./lib/hal/src/stm32f1xx_hal_pcd_ex.d ./lib/hal/src/stm32f1xx_hal_pcd_ex.o ./lib/hal/src/stm32f1xx_hal_pcd_ex.su ./lib/hal/src/stm32f1xx_hal_pwr.cyclo ./lib/hal/src/stm32f1xx_hal_pwr.d ./lib/hal/src/stm32f1xx_hal_pwr.o ./lib/hal/src/stm32f1xx_hal_pwr.su ./lib/hal/src/stm32f1xx_hal_rcc.cyclo ./lib/hal/src/stm32f1xx_hal_rcc.d ./lib/hal/src/stm32f1xx_hal_rcc.o ./lib/hal/src/stm32f1xx_hal_rcc.su ./lib/hal/src/stm32f1xx_hal_rcc_ex.cyclo ./lib/hal/src/stm32f1xx_hal_rcc_ex.d ./lib/hal/src/stm32f1xx_hal_rcc_ex.o ./lib/hal/src/stm32f1xx_hal_rcc_ex.su ./lib/hal/src/stm32f1xx_hal_rtc.cyclo ./lib/hal/src/stm32f1xx_hal_rtc.d ./lib/hal/src/stm32f1xx_hal_rtc.o ./lib/hal/src/stm32f1xx_hal_rtc.su ./lib/hal/src/stm32f1xx_hal_rtc_ex.cyclo ./lib/hal/src/stm32f1xx_hal_rtc_ex.d ./lib/hal/src/stm32f1xx_hal_rtc_ex.o ./lib/hal/src/stm32f1xx_hal_rtc_ex.su ./lib/hal/src/stm32f1xx_hal_sd.cyclo ./lib/hal/src/stm32f1xx_hal_sd.d ./lib/hal/src/stm32f1xx_hal_sd.o ./lib/hal/src/stm32f1xx_hal_sd.su ./lib/hal/src/stm32f1xx_hal_smartcard.cyclo ./lib/hal/src/stm32f1xx_hal_smartcard.d ./lib/hal/src/stm32f1xx_hal_smartcard.o ./lib/hal/src/stm32f1xx_hal_smartcard.su ./lib/hal/src/stm32f1xx_hal_spi.cyclo ./lib/hal/src/stm32f1xx_hal_spi.d ./lib/hal/src/stm32f1xx_hal_spi.o ./lib/hal/src/stm32f1xx_hal_spi.su ./lib/hal/src/stm32f1xx_hal_spi_ex.cyclo ./lib/hal/src/stm32f1xx_hal_spi_ex.d ./lib/hal/src/stm32f1xx_hal_spi_ex.o ./lib/hal/src/stm32f1xx_hal_spi_ex.su ./lib/hal/src/stm32f1xx_hal_sram.cyclo ./lib/hal/src/stm32f1xx_hal_sram.d ./lib/hal/src/stm32f1xx_hal_sram.o ./lib/hal/src/stm32f1xx_hal_sram.su ./lib/hal/src/stm32f1xx_hal_tim.cyclo ./lib/hal/src/stm32f1xx_hal_tim.d ./lib/hal/src/stm32f1xx_hal_tim.o ./lib/hal/src/stm32f1xx_hal_tim.su ./lib/hal/src/stm32f1xx_hal_tim_ex.cyclo ./lib/hal/src/stm32f1xx_hal_tim_ex.d ./lib/hal/src/stm32f1xx_hal_tim_ex.o ./lib/hal/src/stm32f1xx_hal_tim_ex.su ./lib/hal/src/stm32f1xx_hal_uart.cyclo ./lib/hal/src/stm32f1xx_hal_uart.d ./lib/hal/src/stm32f1xx_hal_uart.o
	-$(RM) ./lib/hal/src/stm32f1xx_hal_uart.su ./lib/hal/src/stm32f1xx_hal_usart.cyclo ./lib/hal/src/stm32f1xx_hal_usart.d ./lib/hal/src/stm32f1xx_hal_usart.o ./lib/hal/src/stm32f1xx_hal_usart.su ./lib/hal/src/stm32f1xx_hal_wwdg.cyclo ./lib/hal/src/stm32f1xx_hal_wwdg.d ./lib/hal/src/stm32f1xx_hal_wwdg.o ./lib/hal/src/stm32f1xx_hal_wwdg.su ./lib/hal/src/stm32f1xx_ll_adc.cyclo ./lib/hal/src/stm32f1xx_ll_adc.d ./lib/hal/src/stm32f1xx_ll_adc.o ./lib/hal/src/stm32f1xx_ll_adc.su ./lib/hal/src/stm32f1xx_ll_crc.cyclo ./lib/hal/src/stm32f1xx_ll_crc.d ./lib/hal/src/stm32f1xx_ll_crc.o ./lib/hal/src/stm32f1xx_ll_crc.su ./lib/hal/src/stm32f1xx_ll_dac.cyclo ./lib/hal/src/stm32f1xx_ll_dac.d ./lib/hal/src/stm32f1xx_ll_dac.o ./lib/hal/src/stm32f1xx_ll_dac.su ./lib/hal/src/stm32f1xx_ll_dma.cyclo ./lib/hal/src/stm32f1xx_ll_dma.d ./lib/hal/src/stm32f1xx_ll_dma.o ./lib/hal/src/stm32f1xx_ll_dma.su ./lib/hal/src/stm32f1xx_ll_exti.cyclo ./lib/hal/src/stm32f1xx_ll_exti.d ./lib/hal/src/stm32f1xx_ll_exti.o ./lib/hal/src/stm32f1xx_ll_exti.su ./lib/hal/src/stm32f1xx_ll_fsmc.cyclo ./lib/hal/src/stm32f1xx_ll_fsmc.d ./lib/hal/src/stm32f1xx_ll_fsmc.o ./lib/hal/src/stm32f1xx_ll_fsmc.su ./lib/hal/src/stm32f1xx_ll_gpio.cyclo ./lib/hal/src/stm32f1xx_ll_gpio.d ./lib/hal/src/stm32f1xx_ll_gpio.o ./lib/hal/src/stm32f1xx_ll_gpio.su ./lib/hal/src/stm32f1xx_ll_i2c.cyclo ./lib/hal/src/stm32f1xx_ll_i2c.d ./lib/hal/src/stm32f1xx_ll_i2c.o ./lib/hal/src/stm32f1xx_ll_i2c.su ./lib/hal/src/stm32f1xx_ll_pwr.cyclo ./lib/hal/src/stm32f1xx_ll_pwr.d ./lib/hal/src/stm32f1xx_ll_pwr.o ./lib/hal/src/stm32f1xx_ll_pwr.su ./lib/hal/src/stm32f1xx_ll_rcc.cyclo ./lib/hal/src/stm32f1xx_ll_rcc.d ./lib/hal/src/stm32f1xx_ll_rcc.o ./lib/hal/src/stm32f1xx_ll_rcc.su ./lib/hal/src/stm32f1xx_ll_rtc.cyclo ./lib/hal/src/stm32f1xx_ll_rtc.d ./lib/hal/src/stm32f1xx_ll_rtc.o ./lib/hal/src/stm32f1xx_ll_rtc.su ./lib/hal/src/stm32f1xx_ll_sdmmc.cyclo ./lib/hal/src/stm32f1xx_ll_sdmmc.d ./lib/hal/src/stm32f1xx_ll_sdmmc.o ./lib/hal/src/stm32f1xx_ll_sdmmc.su ./lib/hal/src/stm32f1xx_ll_spi.cyclo ./lib/hal/src/stm32f1xx_ll_spi.d ./lib/hal/src/stm32f1xx_ll_spi.o ./lib/hal/src/stm32f1xx_ll_spi.su ./lib/hal/src/stm32f1xx_ll_tim.cyclo ./lib/hal/src/stm32f1xx_ll_tim.d ./lib/hal/src/stm32f1xx_ll_tim.o ./lib/hal/src/stm32f1xx_ll_tim.su ./lib/hal/src/stm32f1xx_ll_usart.cyclo ./lib/hal/src/stm32f1xx_ll_usart.d ./lib/hal/src/stm32f1xx_ll_usart.o ./lib/hal/src/stm32f1xx_ll_usart.su ./lib/hal/src/stm32f1xx_ll_usb.cyclo ./lib/hal/src/stm32f1xx_ll_usb.d ./lib/hal/src/stm32f1xx_ll_usb.o ./lib/hal/src/stm32f1xx_ll_usb.su ./lib/hal/src/stm32f1xx_ll_utils.cyclo ./lib/hal/src/stm32f1xx_ll_utils.d ./lib/hal/src/stm32f1xx_ll_utils.o ./lib/hal/src/stm32f1xx_ll_utils.su

.PHONY: clean-lib-2f-hal-2f-src

