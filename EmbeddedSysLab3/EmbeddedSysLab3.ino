/*
 * CSCE236 Embedded Systems Spring
 * Carrick Detweiler
 */

#include <avr/io.h>

// Pin 13 has an LED connected on most Arduino boards:
#define BOARD_LED 13
#define RED 8
#define GREEN 9
#define BLUE 10

#define BUTTON 5

/**
 * Init all of the LEDs and test them
 **/ 
void LEDInit(){
  Serial.begin(9600);
  pinMode(BOARD_LED, OUTPUT);     
  pinMode(RED, OUTPUT);     
  pinMode(GREEN, OUTPUT);     
  pinMode(BLUE, OUTPUT);     

   //Turn all off
  digitalWrite(RED,LOW);
  digitalWrite(GREEN,LOW);
  digitalWrite(BLUE,LOW);

  //Test LEDs
  Serial.print("Testing LEDs...");
  digitalWrite(RED,HIGH);
  delay(200);
  digitalWrite(RED,LOW);
  digitalWrite(GREEN,HIGH);
  delay(200);
  digitalWrite(GREEN,LOW);
  digitalWrite(BLUE,HIGH);
  delay(200);
  digitalWrite(BLUE,LOW);
  Serial.println("done.");
  }

void setup() {                
  Serial.begin(9600);
  Serial.println("Starting up.");
  
  LEDInit();
  pinMode(BUTTON, INPUT);  
  //Enable pullup  
  digitalWrite(BUTTON, HIGH);     
   
  // Set bits for fast mode 
  TCCR1B = 0;
  TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS10);
  TCCR1A = 0;
  TCCR1A = (1 << WGM11) | (1 << COM1A1) | (1 << COM1B1);
  TCCR1C = 0;
  
  TCNT1H = 0;
  TCNT1L = 0;
  
  // Set frequency and duty cycle
  ICR1 = 16000;
  OCR1B = 1000;

  Serial.println("Finished setup");
}


void loop() {
  //Button Pressed
  if((digitalRead(BUTTON) == LOW)){
    //Turn LED off briefly to dim
    OCR1B = 1000;   
  } else {
    OCR1B = 8000; 
  }
  delay(100);
}


