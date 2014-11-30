/*
  On button press the LED changes from high to low or vice versa
  LED pin on 13 to ground
  Button input on pin 8
*/

int switchPin = 8;
int ledPin = 13;
boolean lastButton = LOW;
boolean currentButton = LOW;
boolean ledOn = false;

void setup() {
  pinMode(ledPin,OUTPUT);
  pinMode(switchPin,INPUT);
}

boolean debounce(boolean last) {
    boolean current = digitalRead(switchPin);
    if (last != current) {
      delay(5);
      current = digitalRead(switchPin);
    }
    return current;
}

void loop() {
  currentButton = debounce(lastButton);
  if (currentButton == HIGH && lastButton == LOW) {
    ledOn = !ledOn;
  }
  lastButton = currentButton;
  
  digitalWrite(ledPin,ledOn);
}
