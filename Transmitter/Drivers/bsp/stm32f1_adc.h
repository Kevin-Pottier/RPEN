/*
 * stm32f1_adc.h
 *
 *  Created on: 4 mai 2016
 *      Author: Nirgal
 */

#ifndef BSP_STM32F1_ADC_H_
#define BSP_STM32F1_ADC_H_
#include "config.h"
#if USE_ADC

	#if !USE_BSP_TIMER
		#error "USE_ADC suppose que l'on active également USE_BSP_TIMER !"
	#endif
#include "macro_types.h"
#include <stdint.h>

	void DEMO_adc_statemachine (void);

	/**
	 * @brief Enumération des convertisseurs analogique numérique sélectionnable
	 */
	typedef enum{
		ADC_0 = 0,
		ADC_1,
		ADC_2,
		ADC_3,
		ADC_4,
		ADC_5,
		ADC_6,
		ADC_7,
		ADC_8,
		ADC_9,
		ADC_10,
		ADC_11,
		ADC_12,
		ADC_13,
		ADC_14,
		ADC_15,
		ADC_16,
		ADC_17,
		ADC_18,
		ADC_CHANNEL_NB
	}adc_id_e;

	/**
	 * @brief	Cette fonction initialise le périphérique ADC1, ainsi que le DMA (direct memory access).
	 * @func	void ADC_init(void)
	 * @post	A l'issue de l'exécution de cette fonction, des conversions analogiques sont menées en permanence par l'ADC... et mettent à jour un tableau adc_converted_value que l'on peut consulter.
	 */
	void ADC_init(void);


	void ADC_Deinit(void);

	/**
	* @brief  	Cette fonction permet de récupérer les valeurs mesurées par l'ADC.
	* @func  	int16_t ADC_getValue(adc_id_e channel)
	* @param	channel : un canal de ADC_0 à ADC_15
	* @retval 	un entier signé sur 16 bits, correspondant à la valeur demandée, pouvant aller de 0 à 4095 (l'ADC mesure sur 12 bits).
	* @retval	Cette fonction retourne -1 si l'on demande un canal ADC qui n'a pas été initialisé (le define USE_ADCx correspondant est commenté)
	*/
	int16_t ADC_getValue(adc_id_e channel);

	/**
		* @brief  	Cette fonction permet de savoir si un nouvel échantillon est disponible
		* @func  	bool_e ADC_is_new_sample_available(void)
		* @retval 	TRUE si un nouvel échantillon est disponible
		* @post		Attention, on considère dans cette fonction qu'un appel abaisse le flag... !
		*/
	bool_e ADC_is_new_sample_available(void);


	//Informe le module ADC qu'on souhaite qu'une fonction soit appelée à chaque fin de conversion des canaux.
	//Cette fonction est donnée en paramètre.
	void ADC_set_callback_function(callback_fun_t callback);


#endif
#endif /* BSP_STM32F1_ADC_H_ */
