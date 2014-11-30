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

int distance = 2340;

volatile boolean isAvoiding;

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


  // Attach Servos
  leftS.attach(SERVOLEFT);
  rightS.attach(SERVORIGHT);

  // Stop Servos
  leftS.write(90);
  rightS.write(90);

  // Setup Interrupt
  EICRA = (1 << ISC11);
  EIMSK = (1 << INT1);
  isAvoiding = false;

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
  if (!isAvoiding) {
    digitalWrite(BLUE, HIGH);
    //straight();
    lineFollow();
  } 
  else {
    // Backup
    leftS.write(69);
    rightS.write(110);
    delay(500);
    digitalWrite(RED, LOW);
    digitalWrite(GREEN, HIGH);
    
    // Right
    right();
    delay(1000);
    
    // Straight
    straight();
    delay(1000);
    
    // Left
    left();
    delay(1000);

    // Straight
    straight();
    delay(2000);

    // Left
    left();
    delay(500);
    
    straight();
    // Wait until cart hits a sensor
    while(!getLeftSensor() && !getRightSensor());

    isAvoiding = false;
    digitalWrite(GREEN, LOW);
  }
//  Serial.println(analogRead(SENSORLEFT));
//  Serial.println(analogRead(SENSORRIGHT));
//  delay(1000);
}

SIGNAL(INT1_vect) {
  digitalWrite(BLUE, LOW);
  digitalWrite(RED, HIGH);
  // Bumper
  isAvoiding = true;
}

void sensorFollow() {
  // Sensor too large
  if (readProximity() > distance + 10) {
    right();
  }
  // Sensor too small
  else if (readProximity() < distance - 10) {
    right();
  }
  // Sensor just right
  else  {
    straight();
  }
}

void lineFollow() {
  boolean left = getLeftSensor();
  boolean right = getRightSensor();
  
  if (!left && right) {
    straight();
  } 
  else if (!left && !right) {
    right();
  } 
  else {
    left();
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

void straight() {
  leftS.write(110);
  rightS.write(70);
}

void left() {
  leftS.write(90);
  rightS.write(70);
}

void right() {
  leftS.write(110);
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
