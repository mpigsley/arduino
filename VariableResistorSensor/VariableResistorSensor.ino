/*
  Reads the votage on the resistPin from a voltage divider circuit.
  A variable resistor is used to change the values of the divider 
  and therefore change the values sensed.
  Sensor input on pin 0
*/

int resistPin = 0;

void setup() {
  Serial.begin(9600);
}

void loop() {
  double x = analogRead(resistPin);
  x = 5.01-((x/1028)*5.01);
  Serial.print("Voltage Out: ");
  Serial.println(x);
  x = x/5.01;
  double r = (9800*x)/(1-x);
  Serial.print("Resistor Value: ");
  Serial.println(r);
  delay(1000);
}
