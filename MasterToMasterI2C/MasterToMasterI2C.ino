/*
 *  MasterToMasterI2C.ino
 *  Mitchel Pigsley
 *  4/4/12
 *
 */


#include <Wire.h>
#define BUTTON 3
#define LED 5

void setup() {
  Wire.begin(4); // join i2c bus
  Wire.onReceive(receiveEvent);
  Serial.begin(9600);
  pinMode(BUTTON,INPUT_PULLUP);
  pinMode(LED, OUTPUT);
}

boolean debounce(boolean last) {
  boolean current = digitalRead(BUTTON);
  if (last != current) {
    delay(5);
    current = digitalRead(BUTTON);
  }
  return current;
}

boolean previous;
boolean current;

void loop() {
  current = debounce(digitalRead(BUTTON));
  // If button is Pressed
  if(current == LOW && current != previous) {
    Serial.println("my button pressed");
    Wire.beginTransmission(4);
    Wire.write("I"); 
    Wire.endTransmission();
  } 
  else if (current == HIGH && current != previous) {
    Wire.beginTransmission(4);
    Wire.write("O"); 
    Wire.endTransmission();
  }
  previous = current;
}

void receiveEvent(int howMany) {
  char c = Wire.read();
  if (c == 'I') {
    digitalWrite(LED, HIGH);
    Serial.println("other button pressed");
  } else if (c == 'O') {
    digitalWrite(LED, LOW);
  }
}



