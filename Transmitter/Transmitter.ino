/*
  Code for controller that transmits signal to
  specified receiver depending on button pressed.
*/

#include <VirtualWire.h>

#define TXPIN 1
#define LED 13
char inChar[10];
int count;

void setup() {
  // Initialise the IO and ISR
  vw_set_tx_pin(TXPIN);
  vw_setup(9600);
  
  // Pin Setup
  pinMode(LED, OUTPUT);
  
  // Setup Serial
  Serial.begin(3000);
}

void loop() {
  // Chill Bro
}

void SerialEvent() {
  count = 0;
  while(Serial.available()) {
    inChar[count] = (char)Serial.read();
    count++;
  }
  vw_send((uint8_t *)inChar, count+1);
}
