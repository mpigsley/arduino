/*
 *  CSCE236 Embedded Systems
 *  Mitchel Pigsley
 *  MorseCode.ino
 *  2/7/13
 *
 */

#include "morse.h"

#define BOARD_LED 13
#define BUTTON 2

boolean prev = 1;
long pressedOn = 0, pressedOff = 0, pauseOn = 0, pauseOff = 0;

void setup() {
  // Start Serial
  Serial.begin(9600);
  
  // Make LEDs an output
  DDRD |= 0x70;
  
  // Make Button an input
  DDRD &= ~0x04;
  
  // Enable pullup resistor for the button
  PORTD |= 0x04;
  
  // Morse for "Hi W"
  morseBlinkString(LED_RED,"Hi",2);
  morseBlinkString(LED_BLUE,"W",1);
}

boolean debounce(boolean last) {
  delay(50);
  boolean current = digitalRead(BUTTON);
    if (last != current) {
      return current;
    }
   return last;
}

void loop() {
  // Get the button input
  boolean current = debounce(digitalRead(BUTTON));
  if (prev != current) {
    // If button now turned off
    if (current == 1) {
      pressedOff = millis();
      pauseOn = pressedOff;
      
      // Check time.. and print
      if ((pressedOff-pressedOn) > 1000) {
        Serial.println('D');
      } else {
        Serial.println('d');
      }
      
    // If button now turned on
    } else {
      pressedOn = millis(); 
    }
    
    // Current state is now Previous state
    prev = current;
  }
  
  if (prev != 0) {
    pauseOff = millis();
    // Check time... and print (if necessary)
    if ((pauseOff-pauseOn) > 1000) {
      Serial.println('s');
      pauseOn = millis();
    } 
  }
}
