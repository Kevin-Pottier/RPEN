/*
 * stm32f1_weak_it.c
 *
 *  Created on: 29 mars 2018
 *      Author: Nirgal
 */
#include "stm32f1xx_hal.h"
#include "stm32f1_sys.h"

const char * msg = "IT not handled in user code : %s";

__weak void NMI_Handler(void)
{
	dump_printf(msg, "NMI");
	while(1);
}

__weak void HardFault_Handler(void)
{
	dump_printf(msg, "HardFault");
	while(1);
}

__weak void MemManage_Handler(void)
{
	dump_printf(msg, "MemManage");
	while(1);
}

__weak void BusFault_Handler(void)
{
	dump_printf(msg, "BusFault");
	while(1);
}

__weak void UsageFault_Handler(void)
{
	dump_printf(msg, "UsageFault");
	while(1);
}

__weak void SVC_Handler(void)
{
	dump_printf(msg, "SVC");
	while(1);
}

__weak void DebugMon_Handler(void)
{
	dump_printf(msg, "DebugMon");
	while(1);
}

__weak void PendSV_Handler(void)
{
	dump_printf(msg, "PendSV");
	while(1);
}

__weak void SysTick_Handler(void)
{
	dump_printf(msg, "SysTick");
	while(1);
}

__weak void WWDG_IRQHandler(void)
{
	dump_printf(msg, "WWDG");
	while(1);
}

__weak void PVD_IRQHandler(void)
{
	dump_printf(msg, "PVD");
	while(1);
}

__weak void TAMPER_IRQHandler(void)
{
	dump_printf(msg, "TAMPER");
	while(1);
}

__weak void RTC_IRQHandler(void)
{
	dump_printf(msg, "RTC");
	while(1);
}

__weak void FLASH_IRQHandler(void)
{
	dump_printf(msg, "FLASH");
	while(1);
}

__weak void RCC_IRQHandler(void)
{
	dump_printf(msg, "RCC");
	while(1);
}

__weak void EXTI0_IRQHandler(void)
{
	dump_printf(msg, "EXTI0");
	while(1);
}

__weak void EXTI1_IRQHandler(void)
{
	dump_printf(msg, "EXTI1");
	while(1);
}

__weak void EXTI2_IRQHandler(void)
{
	dump_printf(msg, "EXTI2");
	while(1);
}

__weak void EXTI3_IRQHandler(void)
{
	dump_printf(msg, "EXTI3");
	while(1);
}

__weak void EXTI4_IRQHandler(void)
{
	dump_printf(msg, "EXTI4");
	while(1);
}

__weak void DMA1_Channel1_IRQHandler(void)
{
	dump_printf(msg, "DMA1_Channel1");
	while(1);
}

__weak void DMA1_Channel2_IRQHandler(void)
{
	dump_printf(msg, "DMA1_Channel2");
	while(1);
}

__weak void DMA1_Channel3_IRQHandler(void)
{
	dump_printf(msg, "DMA1_Channel3");
	while(1);
}

__weak void DMA1_Channel4_IRQHandler(void)
{
	dump_printf(msg, "DMA1_Channel4");
	while(1);
}

__weak void DMA1_Channel5_IRQHandler(void)
{
	dump_printf(msg, "DMA1_Channel5");
	while(1);
}

__weak void DMA1_Channel6_IRQHandler(void)
{
	dump_printf(msg, "DMA1_Channel6");
	while(1);
}

__weak void DMA1_Channel7_IRQHandler(void)
{
	dump_printf(msg, "DMA1_Channel7");
	while(1);
}

__weak void ADC1_2_IRQHandler(void)
{
	dump_printf(msg, "ADC1_2");
	while(1);
}

__weak void USB_HP_CAN1_TX_IRQHandler(void)
{
	dump_printf(msg, "USB_HP_CAN1_TX");
	while(1);
}

__weak void USB_LP_CAN1_RX0_IRQHandler(void)
{
	dump_printf(msg, "USB_LP_CAN1_RX0");
	while(1);
}

__weak void CAN1_RX1_IRQHandler(void)
{
	dump_printf(msg, "CAN1_RX1");
	while(1);
}

__weak void CAN1_SCE_IRQHandler(void)
{
	dump_printf(msg, "CAN1_SCE");
	while(1);
}

__weak void EXTI9_5_IRQHandler(void)
{
	dump_printf(msg, "EXTI9_5");
	while(1);
}

__weak void TIM1_BRK_IRQHandler(void)
{
	dump_printf(msg, "TIM1_BRK");
	while(1);
}

__weak void TIM1_UP_IRQHandler(void)
{
	dump_printf(msg, "TIM1_UP");
	while(1);
}

__weak void TIM1_TRG_COM_IRQHandler(void)
{
	dump_printf(msg, "TIM1_TRG_COM");
	while(1);
}

__weak void TIM1_CC_IRQHandler(void)
{
	dump_printf(msg, "TIM1_CC");
	while(1);
}

__weak void TIM2_IRQHandler(void)
{
	dump_printf(msg, "TIM2");
	while(1);
}

__weak void TIM3_IRQHandler(void)
{
	dump_printf(msg, "TIM3");
	while(1);
}

__weak void TIM4_IRQHandler(void)
{
	dump_printf(msg, "TIM4");
	while(1);
}

__weak void I2C1_EV_IRQHandler(void)
{
	dump_printf(msg, "I2C1_EV");
	while(1);
}

__weak void I2C1_ER_IRQHandler(void)
{
	dump_printf(msg, "I2C1_ER");
	while(1);
}

__weak void I2C2_EV_IRQHandler(void)
{
	dump_printf(msg, "I2C2_EV");
	while(1);
}

__weak void I2C2_ER_IRQHandler(void)
{
	dump_printf(msg, "I2C2_ER");
	while(1);
}

__weak void SPI1_IRQHandler(void)
{
	dump_printf(msg, "SPI1");
	while(1);
}

__weak void SPI2_IRQHandler(void)
{
	dump_printf(msg, "SPI2");
	while(1);
}

__weak void USART1_IRQHandler(void)
{
	dump_printf(msg, "USART1");
	while(1);
}

__weak void USART2_IRQHandler(void)
{
	dump_printf(msg, "USART2");
	while(1);
}

__weak void USART3_IRQHandler(void)
{
	dump_printf(msg, "USART3");
	while(1);
}

__weak void EXTI15_10_IRQHandler(void)
{
	dump_printf(msg, "EXTI15_10");
	while(1);
}

__weak void RTC_Alarm_IRQHandler(void)
{
	dump_printf(msg, "RTC_Alarm");
	while(1);
}

__weak void USBWakeUp_IRQHandler(void)
{
	dump_printf(msg, "USBWakeUp");
}

