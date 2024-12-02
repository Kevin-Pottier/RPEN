/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "config.h"
#include "midi.h"
#include "usbd_midi_if.h"
#include "usb_device.h"
#if USE_MIDI

extern PCD_HandleTypeDef hpcd_USB_FS;


void MIDI_init(void)
{
	__HAL_RCC_AFIO_CLK_ENABLE();
	__HAL_RCC_PWR_CLK_ENABLE();
	__HAL_AFIO_REMAP_SWJ_NONJTRST();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	MX_USB_DEVICE_Init();
}

void MIDI_demo(void)
{
	MIDI_init();
	static uint8_t note = 0;
	while(1)
	{
		HAL_Delay(100);
		MIDI_send_message(MIDI_NOTE_ON, 1, note, 80);
		HAL_Delay(100);
		MIDI_send_message(MIDI_NOTE_OFF, 1, note, 0);
		note = (note+1)%128;
	}
}


static uint8_t _buffer[64];
static uint8_t _bufferIndex = 0;

void MIDI_send_message(uint8_t message, uint8_t channel, uint8_t data1, uint8_t data2)
{
	if (_bufferIndex >= 63 - 4)
	{
		// Discard unsent messages
		_bufferIndex = 0;
	}

	_buffer[_bufferIndex] = message >> 4;
	_bufferIndex++;
	_buffer[_bufferIndex] = message | (channel & 0x0F);
	_bufferIndex++;
	_buffer[_bufferIndex] = data1 & 0x7F;
	_bufferIndex++;
	_buffer[_bufferIndex] = data2 & 0x7F;
	_bufferIndex++;

	if (MIDI_Transmit_FS(_buffer, _bufferIndex) == USBD_OK)
	{
		_bufferIndex = 0;
	}
}



void HAL_PCD_MspInit(PCD_HandleTypeDef* hpcd)
{
  if(hpcd->Instance==USB)
  {
	  __HAL_RCC_USB_CLK_ENABLE();
	  __HAL_RCC_GPIOA_CLK_ENABLE();

	  HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0, 0);
	  HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
  }
}

/**
* @brief PCD MSP De-Initialization
* This function freeze the hardware resources used in this example
* @param hpcd: PCD handle pointer
* @retval None
*/
void HAL_PCD_MspDeInit(PCD_HandleTypeDef* hpcd)
{
  if(hpcd->Instance==USB)
  {
    __HAL_RCC_USB_CLK_DISABLE();

    HAL_NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
  }
}


void USB_LP_CAN1_RX0_IRQHandler(void)
{
  HAL_PCD_IRQHandler(&hpcd_USB_FS);
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{

}

#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
