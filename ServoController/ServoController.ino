/*
 *  CSCE236 Embedded Systems
 *  Mitchel Pigsley
 *  ServoController.ino
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
 #define TIMEFACTOR 10
 
 Servo left;
 Servo right;
 char inChar[50];
 uint8_t count = 0;
 
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
   // If button is pressed Make a box
   if(!debounce(digitalRead(BUTTON))) {
     turnBox();
   }
 }
 
 void turnBox() {
   turnRight(90);
   straight(90);
   turnRight(90);
   straight(90);
   turnRight(90);
   straight(90);
   turnRight(90);
   straight(90);
 }
 
 void turnLeft(int degree) {
   if (degree > 0) {
     right.write(0);
     delay(degree * TIMEFACTOR);
     right.write(90);
   } else if (degree < 0) {
     right.write(1800);
     delay(-degree * TIMEFACTOR);
     right.write(90);
   } else {
     // Do nothing because degree is zero
   }
   
 }
 
 void turnRight(int degree) {
   if (degree > 0) {
     left.write(180);
     delay(degree * TIMEFACTOR);
     left.write(90);
   } else if (degree < 0) {
     left.write(0);
     delay(-degree * TIMEFACTOR);
     left.write(90);
   } else {
     // Do nothing because degree is zero
   }
 }
 
 void straight(int length) {
   if (length > 0) {
     // Forward
     left.write(180);
     right.write(0);
     delay(length * TIMEFACTOR);
     left.write(90);
     right.write(90);
   } else if (length < 0) {
     // Backward
     left.write(0);
     right.write(180);
     delay(-length * TIMEFACTOR);
     right.write(90);
     left.write(90);
   } else {
     // Do nothing becuase degree is zero
   }
 }
 
 
 void serialEvent() {
   count = 0;
   while(Serial.available()) {
     inChar[count] = (char)Serial.read();
     count++;
   }

   if(inChar[0] == 'l') {
     char temp[10];
     for (int i = 1; i < count; i++) {
       temp[i-1] = inChar[i];
     }
     turnLeft(atoi(temp));
   } else if (inChar[0] == 'r') {
     char temp[5];
     for (int i = 1; i < count; i++) {
       temp[i-1] = inChar[i];
     }
     turnRight(atoi(temp));
   } else if (inChar[0] == 's') {
     char temp[5];
     for (int i = 1; i < count; i++) {
       temp[i-1] = inChar[i];
     }
     straight(atoi(temp));
   } else if(inChar[0] == 'b') {
     turnBox();
   }  else{
     // Do nothing.. incorrect input
   }
   
   Serial.write(inChar);
   Serial.write("\n\r");
 }
