/*
  Takes the user input and turns on the LED.
  1 is LED on
  0 is LED off
  Other inputs result in error message  
*/

int ledPin = 13;

void setup() {
 Serial.begin(9600); 
 pinMode(ledPin,OUTPUT);
}

void loop() {
  while(Serial.available() == 0);
  int val = Serial.read() - '0';
  if (val == 1) {
    Serial.println("LED is on!");
    digitalWrite(ledPin,HIGH);
  } else if (val == 0) {
    Serial.println("LED is off..");
    digitalWrite(ledPin,LOW);
  } else {
    Serial.println("Incorrect input");
  }
}
