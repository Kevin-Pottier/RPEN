/* 
 * File:   matrix.h
 * Author: ponsguil
 *
 * Created on 19 juin 2017, 11:29
 */

#ifndef MATRIX_H
#define	MATRIX_H

#include <stdint.h>


typedef uint8_t color_t;
#define COLOR_BLACK     0b00000000
#define COLOR_RED       0b00000100
#define COLOR_GREEN     0b00000010
#define COLOR_BLUE      0b00000001
#define COLOR_YELLOW    0b00000110
#define COLOR_CYAN      0b00000011
#define COLOR_PURPLE    0b00000101
#define COLOR_WHITE     0b00000111

void MATRIX_init(void);
void Matrix_test_capteur(void);
void MATRIX_display(color_t color[32][32]);


#endif	/* MATRIX_H */

