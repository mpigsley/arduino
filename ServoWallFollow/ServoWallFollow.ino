/*
 *  CSCE236 Embedded Systems
 *  Mitchel Pigsley & Chase Heble
 *  ServoWallFolow.ino
 *  4/14/13
 *
 */
 
#include <Servo.h>
#include <Wire.h>
 
#define BUTTON 2
#define RED 5
#define GREEN 6
#define BLUE 7
#define SERVORIGHT 9
#define SERVOLEFT 10
#define SENSORRIGHT 0
#define SENSORLEFT 1
#define PROXSENSOR 19
#define PROXIMITY_FREQ 0x89  // Proximity IR test signal freq, 0-3
#define PROXIMITY_MOD 0x8A  // proximity modulator timing
#define IR_CURRENT 0x83  // sets IR current in steps of 10mA 0-200mA
#define AMBIENT_PARAMETER 0x84  // Configures ambient light measures
#define COMMAND_0 0x80  // starts measurments, relays data ready info
#define PROXIMITY_RESULT_MSB 0x87  // High byte of proximity measure
#define PROXIMITY_RESULT_LSB 0x88  // low byte of proximity measure
#define FIVE_CENT 2815
#define TEN_CENT 2425

Servo leftS;
Servo rightS;
uint32_t distance = TEN_CENT;

void setup() {
   // Start Serial & Wire
  Serial.begin(9600);
  Wire.begin();

  // Make LEDs an Output
  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(BLUE,OUTPUT);

  // Make Button an input
  pinMode(BUTTON,INPUT_PULLUP);

  // Attach Servos
  leftS.attach(SERVOLEFT);
  rightS.attach(SERVORIGHT);
  
  leftS.write(90);
  rightS.write(90);
  
  // VCNL400 Setup
  writeByte(AMBIENT_PARAMETER, 0x0F);  // Single conversion mode, 128 averages
  writeByte(IR_CURRENT, 20);  // Set IR current to 200mA
  writeByte(PROXIMITY_FREQ, 2);  // 781.25 kHz
  writeByte(PROXIMITY_MOD, 0x81);  // 129, recommended by Vishay
  
  int start = millis();
  while(millis() - start < 5000) {
    if (debounce(digitalRead(BUTTON)) == LOW) {
      distance = FIVE_CENT;
    }
  }
  
  // Blink Light & Print Complete
  Serial.println("Setup Complete!");
  digitalWrite(BLUE, HIGH);
  delay(100);
  digitalWrite(BLUE, LOW);
}

void loop() {
  // Sensor too large
  Serial.println(readProximity());
  delay(500);
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

unsigned int readProximity() {
  unsigned int data;
  byte temp;
  
  temp = readByte(COMMAND_0);
  writeByte(COMMAND_0, temp | 0x08);  // command the sensor to perform a proximity measure
  
  while(!(readByte(COMMAND_0)&0x20)) 
    ;  // Wait for the proximity data ready bit to be set
  data = readByte(PROXIMITY_RESULT_MSB) << 8;
  data |= readByte(PROXIMITY_RESULT_LSB);
  
  return data;
}

void writeByte(byte address, byte data) {
  Wire.beginTransmission(PROXSENSOR);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}

byte readByte(byte address) {
  byte data;
  
  Wire.beginTransmission(PROXSENSOR);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(PROXSENSOR, 1);
  while(!Wire.available())
    ;
  data = Wire.read();

  return data;
}
