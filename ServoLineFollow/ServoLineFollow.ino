/*
 *  CSCE236 Embedded Systems
 *  Mitchel Pigsley
 *  ServoLineFollow.ino
 *  2/12/13
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
 
 
 Servo left;
 Servo right;
 char inChar[50];
 uint8_t count = 0;
 uint8_t turnTimer = 1;
 uint8_t straightTimer = 1;
 int leftVal = 90;
 int rightVal = 90;
 
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
   left.attach(SERVOLEFT);
   right.attach(SERVORIGHT);
   
   // Blink Light & Print Complete
   Serial.println("Setup Complete!");
   digitalWrite(BLUE, HIGH);
   delay(100);
   digitalWrite(BLUE, LOW);
 }
 
 boolean debounce(boolean last) {
  delay(50);
  boolean current = digitalRead(BUTTON);
    if (last != current) {
      return current;
    }
   return last;
 }

 void loop() { 
   // If line is stradled or both sensors are inside
   if ((!getLeftSensor && !getRightSensor()) || (getLeftSensor && getRightSensor())) {
     forward();
     turnTimer = 1;
   }
   
  // If left or right sensor hits line
  if (getLeftSensor()) {
     turnLeft(turnTimer);
     turnTimer++;
  } else {
     turnRight(turnTimer); 
     turnTimer++;
  }
 }
 
 /*
  * getLeftSensor()
  * Return: True for on line
  *         False for off line
  */
 boolean getLeftSensor() {
   if (analogRead(SENSORLEFT) < 150) {
     return false;
   } else {
     return true; 
   }
 }
 
  /*
  * getRightSensor()
  * Return: True for on line
  *         False for off line
  */
 boolean getRightSensor() {
   if (analogRead(SENSORRIGHT) < 150) {
     return false;
   } else {
     return true; 
   }
 }
 
 /*
  * forward()
  * The cart will be set to forward
  */
  void forward() {
   int forwardVal; 
   if ((90 - leftVal) > (90 - rightVal)) {
     if (leftVal < 0) {
       leftVal = -leftVal; 
     }
     if (rightVal < 0) {
       rightVal = -rightVal;
     }
     forwardVal = 90 - leftVal;
   } else {
     if (leftVal < 0) {
       leftVal = -leftVal; 
     }
     if (rightVal < 0) {
       rightVal = -rightVal; 
     }
     forwardVal = 90 - rightVal;
   }
   
   if (forwardVal < 0) {
     forwardVal = 10;  
   } else if (forwardVal > 30) {
     forwardVal = 30; 
   }
   
   // Forward
   left.write(90 + forwardVal);
   right.write(90 - forwardVal);
 }
 
 /*
  * turnLeft(uint8_t time)
  * The cart will be set to turn left
  */
 void turnLeft(uint8_t time) {
   if ((leftVal - (time * TIMEFACTOR)) < 60) {
     leftVal = 60; 
   }
   
   if ((rightVal - (time * TIMEFACTOR)) < 60) {
     rightVal = 60;
   }
   
   left.write(leftVal);
   right.write(rightVal);
 }
 
 /*
  * turnRight(uint8_t time)
  * The cart will be set to turn right
  */
 void turnRight(uint8_t time) {
   if ((leftVal + (time * TIMEFACTOR)) > 120) {
     leftVal = 120; 
   }
   
   if ((rightVal + (time * TIMEFACTOR)) > 120) {
     rightVal = 120;
   }
   
   left.write(leftVal);
   right.write(rightVal);
 }
