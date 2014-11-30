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
  delay(400);
  digitalWrite(RED,LOW);
  digitalWrite(GREEN,HIGH);
  delay(400);
  digitalWrite(GREEN,LOW);
  digitalWrite(BLUE,HIGH);
  delay(400);
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
    
  Serial.println("Finished setup");
}


void loop() {
  static uint8_t buttonPressed = 0;
  static uint32_t pressCount = 0;
  
  if((digitalRead(BUTTON) == LOW) && (buttonPressed == 0)){
   Serial.print("Pressed: "); 
   pressCount++;
   Serial.println(pressCount);
   buttonPressed = 1;

  }
  
  if((digitalRead(BUTTON) == HIGH) && (buttonPressed == 1)){
   Serial.println("Released"); 
   buttonPressed = 0;
  }

}
