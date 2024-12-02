/*
 * midi.h
 *
 *  Created on: 6 avr. 2020
 *      Author: Nirgal
 */

#ifndef MIDDLEWARE_MIDI_MIDI_H_
#define MIDDLEWARE_MIDI_MIDI_H_


#define MIDI_NOTE_ON        0x90
#define MIDI_NOTE_OFF       0x80
#define MIDI_CONTROL_CHANGE 0xB0
#define MIDI_PROGRAM_CHANGE 0xC0

#define MIDI_CC_VOLUME 7
#define MIDI_INSTRUMENT_LEAD1 80

void MIDI_init(void);
void MIDI_send_message(uint8_t message, uint8_t channel, uint8_t data1, uint8_t data2);
void MIDI_demo(void);

#endif /* MIDDLEWARE_MIDI_MIDI_H_ */
