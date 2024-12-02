/*
 * dialog.h
 *
 *  Created on: 29 mai 2019
 *      Author: Nirgal
 */

#ifndef MIDDLEWARE_DIALOG_H_
#define MIDDLEWARE_DIALOG_H_


typedef void(*callback_dialog_send_byte_t)(uint8_t c);	//Type pointeur sur fonction


void DIALOG_DEMO(void);

void DIALOG_init(callback_dialog_send_byte_t callback);


HAL_StatusTypeDef DIALOG_process_byte(uint8_t c, uint8_t ** pdatas);


/**
 * @brief	Cette fonction permet l'envoi d'un message sur la liaison série.
 * @pre		Le tableau datas doit contenir au moins 'size' octet. Sinon, le pointeur 'datas' peut être NULL.
 */
void DIALOG_send_packet(uint16_t size, uint8_t * datas);


#endif /* MIDDLEWARE_DIALOG_H_ */
