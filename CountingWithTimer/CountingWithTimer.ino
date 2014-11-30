/*
 * CSCE236 Embedded Systems Spring
 * Carrick Detweiler
 */

#include <avr/io.h>
#include <Arduino.h>

// Pin 13 has an LED connected on most Arduino boards:
#define BOARD_LED 13
#define RED 4
#define GREEN 5
#define BLUE 6

#define BUTTON 8

int counter = 0;
char message[80];

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
  digitalWrite(BUTTON, LOW);     
  
  //Init counter1
  TCCR1A = 0; //Normal mode 0xffff top, rolls over
  //TCCR1A = (1 << WGM10) | (1 << WGM11); //PWM Phase correct, 10 bit
  //TCCR1B = (1 << CS12) | (1 << CS11); //Clock T1 falling edge
  //TCCR1B = (1 << CS12) | (1 << CS10); //Clock/1024
  TCCR1B = (1 << CS10); // Clock/1
  TCCR1C = 0;
  
  //Set ICR to zero, high byte first
  ICR1H = 0;
  ICR1L = 0;
  //Set counter to zero, high byte first
  TCNT1H = 0;
  TCNT1L = 0;  
  
  TIMSK1 = 0;
  TIFR1 = 0;
  
  counter = TCNT1;
  
  Serial.println("Finished setup");
}

void loop() {
  if (!digitalRead(BUTTON)) {
    // Reset TCNT1 and ICR1
    TCNT1 = 0;
    ICR1 = 0;
    
    // Delay
    delay(100);
    
    // Increment counter
    counter++;
    
    if(ICR1 != 0) {
      // There was a bounce
      // Calculated time is whatever is in the register
      Serial.print("MicroSeconds: ");
      Serial.print(ICR1/16.0);
      Serial.print("\r\n");
    } else {
      // Print number of button presses
      Serial.print("No Bounce... Number of Button presses: ");
      Serial.print(counter);
      Serial.print("\r\n");
    }
  } 
}


