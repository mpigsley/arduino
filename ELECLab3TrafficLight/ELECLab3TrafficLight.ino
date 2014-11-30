/*
 *  ELEC307 Electrical Engineering Lab 1
 *  Mitchel Pigsley
 *  ELECLab3TrafficLight.ino
 *  2/8/13
 *
 */

#define DEBUG_MESSAGE_MAX_LENGTH 80
#define PORT_DATA (unsigned char *) 0x2B
#define PORT_DDR (unsigned char *) 0x2A
#define PORT_PIN (unsigned char *) 0x29
#define SWITCH_MASK 0x10

volatile unsigned char *PortD_Pin;

unsigned char *PortD_Data;
unsigned char *PortD_DDR;
unsigned char CurrentD;
unsigned char PreviousD;

char message[DEBUG_MESSAGE_MAX_LENGTH];

// Set to Main Green at default
uint8_t state = 0;

void setup() {
  // Enable internal pullup resistor by setting bit to 1
  PortD_Data = PORT_DATA;
  *PortD_Data |= SWITCH_MASK;

  // Make the pin an input by setting bit to 0
  PortD_DDR = PORT_DDR;
  *PortD_DDR &= ~SWITCH_MASK;

  // Get the initial value at the pin
  PortD_Pin = PORT_PIN;
  CurrentD = *PortD_Pin & SWITCH_MASK;

  // Set previous as that value as well
  PreviousD = CurrentD;

  //snprintf(message, DEBUG_MESSAGE_MAX_LENGTH, "State = 0x%2.2x\n\r", CurrentD);

  Serial.begin(9600);
  Serial.write("Setup Complete\n\r");
}

void loop() {
  // If state is not a yellow light at either main or secondary,
  // then poll the pin.
  if (state == 0 || state == 3) {
    while (PreviousD == CurrentD) {
      // Constantly read in the value on PORT D, PIN 4.
      CurrentD = *PortD_Pin & SWITCH_MASK;
    }
  }

  // If main green and sensor turns on
  if (state == 0 && CurrentD == 0) {
    state = 1;
  // If secondary green and sensor turns off
  } else if (state == 3 && PreviousD == 0) {
    state = 2;
  // If secondary yellow go back to main green
  } else if (state == 2) {
    state = 0;
  // If main yellow go to secondary green
  } else if (state == 1) {
    state = 3;
  } else {
    // Do Nothing...
    // Will catch situation where sensor is still triggered
    // and the secondary is green.
  }

  // Previous is now the current
  PreviousD = CurrentD;
  
  // Write to serial
  snprintf(message, DEBUG_MESSAGE_MAX_LENGTH, "State = %x\n\r", state);
  Serial.write(message);
  
  // Wait for 2 seconds
  delay(2000);

}
