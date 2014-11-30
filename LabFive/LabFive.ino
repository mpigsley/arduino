/*
 *
 *
 *
 *
 *
 */
 
#define BUTTON_PIN 3
#define RED_PIN 5
#define GREEN_PIN 6
#define BLUE_PIN 7


SIGNAL(INT1_vect) {
  
}

void setup() {
  // SETUP pins
  pinMode(BUTTON_PIN,INPUT_PULLUP);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  EICRA = 0x08;  // Falling Edge
  EIMSK = 0x02;  // Turn on External Interrupt
  
  digitalWrite(RED_PIN, HIGH);
  delay(200);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, HIGH);
  delay(200);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, HIGH);
  delay(200);
  digitalWrite(BLUE_PIN, LOW);

}
 
void loop() {
   
}
