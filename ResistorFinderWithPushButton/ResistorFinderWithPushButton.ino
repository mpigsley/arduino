/*
  Find the value of any resistor using a simple voltage divider. 
  The values average with increasing amount of time. The readings
  are taken every second and outputted to the Serial Display. 
  If the button is pressed then the average is reset back to zero.
  
  Voltage out is on Analog input 0
  Switch pin is on pin 8

  Take readings on input voltage and non-variable resistor
  and insert into the global variables below.
*/

// Readings
double inputVoltage = 5.01;
double resistor = 9870;

int voPin = 0;
int switchPin = 8;
boolean lastButton = LOW;
boolean currentButton = LOW;
int counter = 1;
int resistTotal = 0;

void setup() {
  Serial.begin(9600);
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

boolean isPressed() {
  boolean check = false;
  currentButton = debounce(lastButton);
  if (currentButton == HIGH && lastButton == LOW) {
    check = true;
  }
  lastButton = currentButton;
  return check;
}

void loop() {
  // If pressed reset totals
  if(isPressed()) {
    counter = 1;
    resistTotal = 0;
  }
  
  // Math to find Resistor then Print
  double x = analogRead(voPin);
  x = inputVoltage-((x/1028)*inputVoltage);  
  Serial.print("Voltage Out: ");
  Serial.println(x);
  x = x/inputVoltage;
  resistTotal += (resistor*x)/(1-x);
  Serial.print("Resistor Value: ");
  Serial.println(resistTotal/counter);
  Serial.print("Counter: ");
  Serial.println(counter);
  Serial.println();
  
  // Increment Couter
  counter++;
  
  // Delay 1sec
  delay(1000);
}
