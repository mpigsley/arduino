/*
 *
 *
 *
 */
 
void setup() {
  // Setup Serial
  Serial.begin(9600);
}

void loop() {
  Serial.println(analogRead(0));
  delay(1000);
}
