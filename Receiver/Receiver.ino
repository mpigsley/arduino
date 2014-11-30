/*
  Code for controller that receives code of 
  '1' and turns on and off a light.
*/

#include <VirtualWire.h>

const int led_pin = 13;
const int transmit_pin = 1;
const int receive_pin = 0;

void setup()
{
    delay(1000);
    Serial.begin(9600);	// Debugging only
    Serial.println("setup");

    // Initialise the IO and ISR
    vw_set_tx_pin(transmit_pin);
    vw_set_rx_pin(receive_pin);
    vw_setup(3000);

    vw_rx_start();       // Start the receiver PLL running
    
    // Pin Setup
    pinMode(led_pin,OUTPUT);
}

boolean is_switched = false;

void loop() {
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
    
    digitalWrite(led_pin, HIGH);
    delay(2000);
    digitalWrite(led_pin, LOW);
    delay(2000);
//    // Check if message was recieved
//    if (vw_get_message(buf, &buflen)) {
//      // Check if correct message
//      if (buf[0] == 0) {
//        // Force led_pin either HIGH or LOW
//        if (is_switched == false) {
//          digitalWrite(led_pin, HIGH); // Flash a light to show received good message
//          is_switched = true;
//        } else {
//          digitalWrite(led_pin, LOW);
//          is_switched = false;
//        }
//      }         
//    }
}
