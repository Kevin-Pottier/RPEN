/**
 * TIMER.h
 *
 *  Created on: 29 oct. 2013
 *      Author: Nirgal
 */

#ifndef TIMER_H_
#define TIMER_H_
#include "stm32f1xx_hal.h"
#include "macro_types.h"

#include "config.h"
#if USE_BSP_TIMER


typedef enum
{
	TIMER1_ID,
	TIMER2_ID,
	TIMER3_ID,
	TIMER4_ID,
	TIMER_ID_NB
}timer_id_e;

/**
 * @brief	Initialisation et lancement du timer sélectionné.
 * 			Cette fonction lance de timer et le configure Il peut declancher une IT quand il y a debordement du timer.
 * @func 	void TIMER_run_us(void)
 * @param id du timer : cf timer_id_e
 * @param us temps en us code sur un 32bits non signe
 * @param enable_irq : TRUE : active les IT, FALSE ne les active pas. En cas d'activation des IT, l'utilisateur doit ecrire une fonction TIMERx_user_handler_it. Par defaut, ces fonctions ecrites dans ce fichier mais avec l'attribut weak (elles peuvent donc etre reecrites)
 * @post	Le timer et son horloge sont activés, ses interruptions autorisées, et son décompte lancé.
 */
void TIMER_run_us(timer_id_e timer_id, uint32_t us, bool_e enable_irq);

/**
 * @brief	accesseur sur le handler.
 * @func 	void TIMER_get_phandler(void)
 */
TIM_HandleTypeDef * TIMER_get_phandler(timer_id_e timer_id);
uint16_t TIMER_read(timer_id_e timer_id);
void TIMER_write(timer_id_e timer_id, uint16_t counter);

void TIMER_set_period(timer_id_e timer_id, uint16_t period);

void TIMER_set_prescaler(timer_id_e timer_id, uint16_t prescaler);

void TIMER_enable_PWM(timer_id_e timer_id, uint16_t TIM_CHANNEL_x, uint16_t duty, bool_e remap, bool_e negative_channel);

void TIMER_set_duty(timer_id_e timer_id, uint16_t TIM_CHANNEL_x, uint16_t duty);


/**
 * @brief	stop le timer selectionne.
 * 			
 * @func 	void TIMER_stop(void)
 * @pre 	Le timer a ete initialise
 * @post	Le timer est desactivés
 */
void TIMER_stop(timer_id_e timer_id);

/**
 * @brief	acquite les IT sur le timer sélectionné.
 * 			
 * @func 	void clear_it_status(void)
 * @pre 	Le timer a ete initialise
 * @post	Le timer est acquité
 */
void clear_it_status(timer_id_e timer_id);


#endif
#endif /* TIMER_H_ */
