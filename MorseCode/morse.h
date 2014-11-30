/****************************************************************************
*
*   Copyright (c) 2011 Carrick Detweiler
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*
****************************************************************************/

#include <avr/io.h>
#include <Arduino.h>

#ifndef MORSE_H_
#define MORSE_H_

/**
 * Macros to use when calling the below functions to indicate which
 * LED(s) to use.  Each is defined as a different bit, so you can OR
 * these together to output on multiple LEDs at once.  For
 * instance:
 *     morseBlinkString(LED_RED | LED_GREEN, "Hello World", 11);
 **/
#define LED_RED (1<<0)
#define LED_GREEN (1<<1)
#define LED_BLUE (1<<2)

#ifdef __cplusplus
extern "C" {
#endif
  
/**
 * Sends the right blink sequence for character c on the specified
 * led.  Note that the valid range for c is 'a'-'z' or 'A'-'Z'.
 **/
void morseBlinkChar(uint8_t led, char c);

/**
 * Sends the specified string (with len characters) as morse code
 * blinks over the specified LED.  Note that the valid range for c is
 * 'a'-'z' or 'A'-'Z'.
 **/
void morseBlinkString(uint8_t led, char *c, uint8_t len);

#ifdef __cplusplus
}
#endif

#endif /** MORSE_H_ **/

