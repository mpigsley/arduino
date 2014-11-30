/*
 *  CSCE236 Embedded Systems
 *  Mitchel Pigsley
 *  MorseCode.ino
 *  2/7/13
 *
 */

#include "morse.h"

#define BOARD_LED 13
#define BUTTON 7

long first, last;
char *lastFour;
char current = 'a';
int count = 0, check = 0;

void setup() {
  // Make LEDs an output
  DDRB |= 0x07;
  
  // Make Button an input
  DDRD &= ~0x80;
  
  // Enable pullup resistor for the button
  PORTD |= 0x80;

  // Start Serial
  Serial.begin(9600);
  
  // Morse for "Hi W"
  morseBlinkString(LED_RED,"Hi",2);
  morseBlinkString(LED_BLUE,"W",1);
}

void loop() {
  count = 0;
  check = 0;
  
  while (!check) {
    if (!digitalRead(BUTTON)) {
      // Define button begin
      first = millis();
      
      // delay to debounce
      delay(5);
      
      // Wait until button is let up
      while (!digitalRead(BUTTON));
      
      // Define button end
      last = millis();
      
      // If less than a second
      if ((last-first) <= 1000) {
        Serial.println("d");
        current = 'd';
      // If more than a second
      } else {
        Serial.println("D");
        current = 'D';
      }
    } else {
      // Define pause beginning
      first = millis();
      
      // delay for bounce out of button press
      delay(5);
      
      // wait until putton press
      while (digitalRead(BUTTON));
      
      // Define pause end
      last = millis();
      
      // If second is more that 1 second
      if ((last-first) > 1000) {
        Serial.println("s");
        current = 's';
        check = 1;
      }
    }
      
    // Store dot or dash if not a space
    if (current != 's') {
      lastFour[count] = current;
    }
    
    // Increment count
    count++;
  }
  
  if (count == 3) {
    if (lastFour[0] = 'd', lastFour[1] = 'D', lastFour[2] = 'd') {
      PORTB |= 1 << 0;
      delay(1000);
      PORTB &= ~(1 << 0);
    } else if (lastFour[0] = 'D', lastFour[1] = 'D', lastFour[2] = 'd') {
      PORTB |= 1 << 1;
      delay(1000);
      PORTB &= ~(1 << 1);
    }
  } else if (count == 4) {
    if (lastFour[0] = 'D', lastFour[1] = 'd', lastFour[2] = 'd', lastFour[4] = 'd') {
      PORTB |= 1 << 2;
      delay(1000);
      PORTB &= ~(1 << 2);
    }
  } else {
    // Do nothing
  }
    
}
