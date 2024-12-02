// TODO : Alternate Mode


/** \file GPIO.c
   * \brief implementation des fonctions d'initialisation des GPIO et les fonctions d'usage.
   * \author Florent CHARRUAUD
   * \date 23 janvier 2014
   */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1_gpio.h"
#include "stm32f1xx_hal.h"
#include "config.h"
/*
 * Ce module logiciel a pour but de simplifier l'utilisation des fonctions HAL_GPIO_...
 *
 * Attention à penser à appeler au moins une fois, lors de l'initialisation, la fonction :
 * 		BSP_GPIO_Enable();
 *
 * Initialiser une broche en entrée (exemple) :
 * 		BSP_GPIO_PinCfg(GPIOA,GPIO_PIN_0,GPIO_MODE_INPUT,GPIO_NO_PULL,GPIO_SPEED_HIGH);
 * 			variante tirage pour appliquer des tirages haut ou bas : GPIO_PULLUP, GPIO_PULLDOWN
 * 			variante mode   pour utiliser une "alternate function" en entrée : GPIO_MODE_AF_INPUT
 *
 * Initialiser une broche en sortie (exemple) :
 * 		BSP_GPIO_PinCfg(GPIOA,GPIO_PIN_0,GPIO_MODE_OUTPUT_PP,GPIO_PULLUP,GPIO_SPEED_HIGH);
 * 			variante pour utiliser une "alternate function" en sortie : GPIO_MODE_AF_OD ou GPIO_MODE_AF_PP
 *
 *
 * Pour manipuler les broches ainsi configurées :
 * 		b = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);	-> renvoie 0 ou 1... selon l'état de la broche en entrée
 * 		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_0,GPIO_PIN_SET);		-> écrit 0 ou 1 sur la broche en sortie
 * 		HAL_GPIO_TogglePin(GPIOA,GPIO_PIN_0);					-> inverse l'état de la broche en sortie (0 <-> 1)
 *
 */



/* Private functions ---------------------------------------------------------*/
/**
 * @brief	Configuration des entrees/sorties des broches du microcontroleur.
 * @func	void BSP_GPIO_Enable(void)
 * @post	Activation des horloges des peripheriques GPIO
 */
void BSP_GPIO_Enable(void)
{
	//Activation des horloges des peripheriques GPIOx
	__HAL_RCC_GPIOA_CLK_ENABLE(); // Now defined in macros : stm32f1_hal_rcc.h (417)
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();

	__HAL_RCC_AFIO_CLK_ENABLE();

#if !MY_BLUEPILL_IS_COUNTERFEIT
	//Sur certaines bluepill contrefaites, il y a une mauvaise gestion du remap du JTAG... (notamment remarquée si l'oscillateur HSE est actif).
	//Donc si vous avez vraiment besoin de PA15, réactivez ceci... mais éviter d'utiliser la macro USE_MIDI ou autre usage de l'USB qui nécessite le HSE.
	__HAL_AFIO_REMAP_SWJ_NOJTAG();	//Pour pouvoir utiliser PA15 (et retirer l'affectation de cette broche au JTAG, non utilisé)
#endif
}


/**
 * @brief Fonction generale permettant de configurer une broche
 * @func void BSP_GPIO_PinCfg(GPIO_TypeDef * GPIOx, uint32_t GPIO_Pin, uint32_t GPIO_Mode, uint32_t GPIO_Pull, uint32_t GPIO_Speed, uint32_t GPIO_Alternate)
 * @param GPIOx : peut-etre GPIOA-G
 * @param GPIO_Pin : GPIO_PIN_X avec X correspondant a la (ou les) broche souhaitee
 * @param GPIO_Mode : GPIO_MODE_INPUT, GPIO_MODE_OUTPUT_PP, GPIO_MODE_OUTPUT_OD, GPIO_MODE_AF_PP, GPIO_MODE_AF_OD ou GPIO_MODE_ANALOG
 * @param GPIO_Pull : GPIO_NOPULL, GPIO_PULLUP, GPIO_PULLDOWN
 * @param GPIO_Speed : GPIO_SPEED_LOW (2MHz), GPIO_SPEED_MEDIUM (25MHz), GPIO_SPEED_HIGH (100MHz)
  */
void BSP_GPIO_PinCfg(GPIO_TypeDef * GPIOx, uint32_t GPIO_Pin, uint32_t GPIO_Mode, uint32_t GPIO_Pull, uint32_t GPIO_Speed)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//Structure contenant les arguments de la fonction GPIO_Init
	GPIO_InitStructure.Pin = GPIO_Pin;
	GPIO_InitStructure.Mode = GPIO_Mode;
	GPIO_InitStructure.Pull = GPIO_Pull;
	GPIO_InitStructure.Speed = GPIO_Speed;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStructure);
}



