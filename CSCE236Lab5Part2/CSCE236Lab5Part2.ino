
#include <avr/io.h>

//Board LED is pin 13
#define LED_PIN 6

//Pin the button is on
#define BUTTON 3

/**
 * Setups up the timer interrupt to happen every 100 milliseconds.
 **/
void setupTimerInterrupt(){
   //Reset to defaults
  TCCR1A = TCCR1B = 0;
  // CTC with OCR1A as top
  TCCR1B |= (1 << WGM12);
  //Set clock to clk/256
  TCCR1B |= (1 << CS12);
  //Enable interrupt on match with OCR1A
  TIMSK1 = (1 << OCIE1A);
  
  //LAB CODE -- FILL IN
  //Configure OCR1A (the counter top) to generate an interrupt
  //every 100ms.
  //OCR1A = ;
}

void toggleLED(){
  static uint8_t on = 0;
  if(on){
    digitalWrite(LED_PIN,HIGH);
  }else{
    digitalWrite(LED_PIN,LOW);
  }
  on = !on;
}

/**
 * This function is called whenever the timer interrupt match 1A
 * is generated.
 **/
SIGNAL(TIMER1_COMPA_vect){
  //LAB CODE
  //UNCOMMENT THIS LINE TO TOGGLE ON INTERRUPT
  //toggleLED();
}


void setup() {
  Serial.begin(9600);
  Serial.print("Starting up...");

  //Setup the LED
  pinMode(LED_PIN,OUTPUT);
  digitalWrite(LED_PIN,LOW);
  
  //Setup the button
  pinMode(BUTTON,INPUT);
  //Pullup
  digitalWrite(BUTTON,HIGH);
  
  Serial.println("startup complete.");
  setupTimerInterrupt();  
}

uint16_t current;
uint16_t last = 0;;

void loop() {
  //LAB CODE -- FILL IN
  //Write code that will toggle the LED every 100ms.
  //Use the toggleLED() and millis() functions.
//  current = millis();
//  if (current - last > 199) {
//    toggleLED(); 
//    last = millis();
//  }
  
  
  //If the button is pressed, print the ADC values
  if(digitalRead(BUTTON) == LOW){
    Serial.println("----------------------");
    Serial.print("Analog port A0 value: "); Serial.println(analogRead(A0));
    Serial.print("Analog port A1 value: "); Serial.println(analogRead(A1));
    Serial.print("Analog port A2 value: "); Serial.println(analogRead(A2));
    Serial.print("Analog port A3 value: "); Serial.println(analogRead(A3));
    Serial.print("Analog port A4 value: "); Serial.println(analogRead(A4));
    Serial.print("Analog port A5 value: "); Serial.println(analogRead(A5));

  }

}
