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
boolean isMainLeft = false;
double division = .5;
int max;
int min;
int current = 0;
int previous = 0;
int count = 0;

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
  int difference = 30;
  min = 90 - difference;
  max = 90 + difference;

  // Blink Light & Print Complete
  Serial.println("Setup Complete!");
  digitalWrite(BLUE, HIGH);
  delay(100);
  digitalWrite(BLUE, LOW);
}

void loop() {
  leftVal = 98;
  rightVal = 83;
  
  // Set Drive State
  drive();
  
  Serial.print(analogRead(SENSORLEFT));
  Serial.print("\t");
  Serial.print(analogRead(SENSORRIGHT));  
  Serial.print("\n");
}

void decisionState() {
  // Left on, Right off line (Forward)
  if (getLeftSensor() && !getRightSensor()) {
    // Back on Track
    reTracking = false;

    // Light
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, HIGH);
    
    if (isMainLeft) {
      // Straight
      straight();
    } else {
      // Left
      left();
    }
  } 

  // Both on Line (Right)
  else if (getLeftSensor() && getRightSensor()) {
    // Back on Track
    reTracking = false;

    // Light
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, HIGH);

    if (isMainLeft) {
      // Right
      right();
    } else {
      // Left
      left();
    }
  }

  // Neither on line (Left)
  else if (!getLeftSensor() && !getRightSensor() && !reTracking) {
    // Light
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);

    // Update Values
    previous = current;
    current = millis();

    if (isMainLeft) {
      // Left
      left();
    } else {
      // Right
      right();
    }
  }

  // If right is on & left is off  (Right)
  else if(!getLeftSensor() && getRightSensor()) {
    // Get Back on Track
    reTracking = true;

    // Light
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, HIGH);
    digitalWrite(BLUE, LOW);

    if (isMainLeft) {
      // Right
      right();
    } else {
      // Straight
      straight();
    }
  }
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
  if (leftVal != max) {
    leftVal += division;  
  }
  if (rightVal != min) {
    rightVal -= division;
  } 
}

void left() {
  // Change left wheel
  if (leftVal > min + 20) {
    leftVal -= division;  
  } 
  else if (leftVal < min + 20) {
    leftVal += division; 
  } 
  else {
    // It is at 90
  }

  // Change right wheel
  if (rightVal != min) {
    rightVal -= division;  
  }
}

void right() {
  // Change left wheel
  if (rightVal > max - 20) {
    rightVal -= division;  
  } 
  else if (rightVal < max - 20) {
    rightVal += division; 
  } 
  else {
    // It is at 90 
  }

  // Change right wheel
  if (leftVal != max) {
    leftVal += division;  
  }
}

void drive() {
  leftS.write((int)leftVal);
  rightS.write((int)rightVal);
}








