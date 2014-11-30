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
#define BUMP 3
#define PROXSENSOR 19
#define PROXIMITY_FREQ 0x89  // Proximity IR test signal freq, 0-3
#define PROXIMITY_MOD 0x8A  // proximity modulator timing
#define IR_CURRENT 0x83  // sets IR current in steps of 10mA 0-200mA
#define AMBIENT_PARAMETER 0x84  // Configures ambient light measures
#define COMMAND_0 0x80  // starts measurments, relays data ready info
#define PROXIMITY_RESULT_MSB 0x87  // High byte of proximity measure
#define PROXIMITY_RESULT_LSB 0x88  // low byte of proximity measure

Servo leftS;
Servo rightS;

int min;
int max;
int distance = 2450;
boolean bumped;
boolean isOnTrack;

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
  pinMode(BUMP,INPUT_PULLUP);

  // Variables
  bumped = false;
  isOnTrack = false;
  min = 50;
  max = 130;

  // Attach Servos
  leftS.attach(SERVOLEFT);
  rightS.attach(SERVORIGHT);

  // Stop Servos
  leftS.write(90);
  rightS.write(90);

  // Setup Interrupt
  EICRA = (1 << ISC11);
  EIMSK = (1 << INT1);

  // VCNL400 Setup
  writeByte(AMBIENT_PARAMETER, 0x0F);  // Single conversion mode, 128 averages
  writeByte(IR_CURRENT, 20);  // Set IR current to 200mA
  writeByte(PROXIMITY_FREQ, 2);  // 781.25 kHz
  writeByte(PROXIMITY_MOD, 0x81);  // 129, recommended by Vishay

  // Blink Light & Print Complete
  Serial.println("Setup Complete!");
  digitalWrite(BLUE, HIGH);
  delay(100);
  digitalWrite(BLUE, LOW);
}

void loop() {
  if (!bumped) {
    digitalWrite(GREEN, HIGH);
    wallFollow();
    digitalWrite(GREEN, LOW);
  } 
  else {
    if (!isOnTrack) {
      straight(false);
      delay(250);
      digitalWrite(BLUE, HIGH);
      dRight();
      delay(750);
      straight(true);
      while(!getLeftSensor() && !getRightSensor());
      digitalWrite(BLUE, LOW);
      isOnTrack = true;
    } else {
      digitalWrite(RED, HIGH);
      lineFollow(); 
      digitalWrite(RED, LOW);
    }

  }
}

SIGNAL(INT1_vect) {
  cli();
  // Bump
  bumped = true;
  sei();
}

void wallFollow() {
  // Sensor too large
  if (readProximity() > distance + 10) {
    dRight();
  }
  // Sensor too small
  else if (readProximity() < distance - 10) {
    dLeft();
  }
  // Sensor just right
  else  {
    straight(true);
  }
}

void lineFollow() {
  boolean left = getLeftSensor();
  boolean right = getRightSensor();

  if (!left && right) {
    straight(true);
  } 
  else if (!left && !right) {
    dRight();
  } 
  else {
    dLeft();
  }
}

boolean getLeftSensor() {
  if (analogRead(SENSORLEFT) < 700) {
    return false;
  } 
  else {
    return true; 
  }
}

boolean getRightSensor() {
  if (analogRead(SENSORRIGHT) < 700) {
    return false;
  } 
  else {
    return true; 
  }
}

void straight(boolean forward) {
  if (forward) {
    leftS.write(max);
    rightS.write(min);
  } 
  else {
    leftS.write(min);
    rightS.write(max);
  }
}

void dLeft() {
  leftS.write(90);
  rightS.write(min);
}

void dRight() {
  leftS.write(max);
  rightS.write(90);
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





