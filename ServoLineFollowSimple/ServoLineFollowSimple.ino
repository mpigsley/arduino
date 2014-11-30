/*
 *  CSCE236 Embedded Systems
 *  Mitchel Pigsley
 *  ServoLineFollowNew.ino
 *  3/14/13
 *
 */

#include <Servo.h>

#define BUTTON 2
#define RED 4
#define GREEN 5
#define BLUE 6
#define SERVORIGHT 9
#define SERVOLEFT 10
#define TIMEFACTOR 1
#define SENSORRIGHT 0
#define SENSORLEFT 1

Servo leftS;
Servo rightS;

double leftVal;
double rightVal; 
boolean reTracking = false;
double division = .5;
int direction;
int max;
int min;

void setup() {
  // Start Serial
  Serial.begin(9600);

  // Make LEDs an Output
  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(BLUE,OUTPUT);

  // Make Button an input
  pinMode(BUTTON,INPUT_PULLUP);

  // Attach Servos
  leftS.attach(SERVOLEFT);
  rightS.attach(SERVORIGHT);

  // Set Servo Vals
  leftVal = 90;
  rightVal = 90;

  // Set min & max vals
  int difference = 25;
  min = 90 - difference;
  max = 90 + difference;

  // Blink Light & Print Complete
  Serial.println("Setup Complete!");
  digitalWrite(BLUE, HIGH);
  delay(100);
  digitalWrite(BLUE, LOW);
}

void loop() {
  // Left on, Right off line (Forward)
  if (getLeftSensor() && !getRightSensor()) {
    // Back on Track
    reTracking = false;

    // Light
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, HIGH);

    // Straight
    straight();
  } 

  // Both on Line (Right)
  else if (getLeftSensor() && getRightSensor() && !reTracking) {
    // Light
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, HIGH);

    // Straight
    right();
  }

  // Neither on line (Left)
  else if (!getLeftSensor() && !getRightSensor() && !reTracking) {
    // Light
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);

    // Left
    left();
  }

  // If right is on & left is off  (Right)
  else if(!getLeftSensor() && getRightSensor()) {
    // Get Back on Track
    reTracking = true;

    // Light
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, LOW);

    // Right
    right();
  }


  // Set Drive State
  drive();
}

boolean debounce(boolean last) {
  delay(50);
  boolean current = digitalRead(BUTTON);
  if (last != current) {
    return current;
  }
  return last;
}

boolean getLeftSensor() {
  if (analogRead(SENSORLEFT) < 150) {
    return false;
  } 
  else {
    return true; 
  }
}

boolean getRightSensor() {
  if (analogRead(SENSORRIGHT) < 150) {
    return false;
  } 
  else {
    return true; 
  }
}

void straight() {
  if (leftVal < max) {
    leftVal += division;  
  }
  if (rightVal > min) {
    rightVal -= division;
  } 
}

void left() {
  // Change left wheel
  if (leftVal > 82) {
    leftVal -= division;  
  } 
  else if (leftVal < 82) {
    leftVal += division; 
  } 
  else {
    // It is at 90
  }

  // Change right wheel
  if (rightVal > min) {
    rightVal -= division;  
  }
}

void right() {
  // Change left wheel
  if (rightVal > 108) {
    rightVal -= division;  
  } 
  else if (rightVal < 108) {
    rightVal += division; 
  } 
  else {
    // It is at 90 
  }

  // Change right wheel
  if (leftVal < max) {
    leftVal += division;  
  }
}

void drive() {
  leftS.write((int)leftVal);
  rightS.write((int)rightVal);
}








