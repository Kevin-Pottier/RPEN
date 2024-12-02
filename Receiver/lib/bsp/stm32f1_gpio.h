// TODO : INCLUDES


/***
   * \file GPIO.h
   * \brief fichier .h pour GPIO
   * \author Samuel Poiraud
   * \date mars 2016
   */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#ifndef GPIO_H_
#define GPIO_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"


/* Private function prototypes -----------------------------------------------*/


void BSP_GPIO_Enable(void);
void BSP_GPIO_PinCfg(GPIO_TypeDef * GPIOx, uint32_t GPIO_Pin, uint32_t GPIO_Mode, uint32_t GPIO_Pull, uint32_t GPIO_Speed);



#endif /* GPIO_H_ */
