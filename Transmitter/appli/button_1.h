/*
 * button.h
 *
 *  Created on: 26 juin 2019
 *      Author: Nirgal
 */

#ifndef BUTTON_1_H_
#define BUTTON_1_H_

typedef enum
{
	BUTTON_EVENT_NONE,
	BUTTON_EVENT_SHORT_PRESS,
	BUTTON_EVENT_LONG_PRESS
}button_event_e;

void BUTTON_1_init(void);
void BUTTON_2_init(void);
void BUTTON_3_init(void);



button_event_e BUTTON_1_state_machine(void);
button_event_e BUTTON_2_state_machine(void);
button_event_e BUTTON_3_state_machine(void);

#endif /* BUTTON_2_H_ */
