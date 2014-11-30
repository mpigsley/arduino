/*
  On button press the LED increasaes in brightness until it reaches 
  the maximum brightness and gets reset to 0.
  LED pin on 11 to ground
  Button input on pin 8
*/

int switchPin = 8;
int ledPin = 11;
boolean lastButton = LOW;
boolean currentButton = LOW;
int ledLevel = 0;

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
    ledLevel += 51;
  }
  
  lastButton = currentButton;
  
  if (ledLevel > 255) {
    ledLevel = 0;
  }
  
  analogWrite(ledPin,ledLevel);
}
