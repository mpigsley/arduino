/*
 *  Mitchel Pigsley
 *  PreLab #4
 *  Introduction to Microcontroller Interrupts
 *  prelabFour.c
 *
 */

#include <avr/io.h>
#include <avr/interrupt.h>

unsigned short newTimeStampCopy;
unsigned short oldTimeStampCopy;

ISR(TIMER1_CAPT_vect) {
  // Update the timeStampCopy variable when there is a rising edge
  unsigned short *inputCaptureRegister1;

  inputCaptureRegister1 = (unsigned short *) 0x86;
  newTimeStampCopy = *inputCaptureRegister1;
}



int main() {
  // Setup
  // Normal Port Operations
  TCCR1A &= ~((1 << COM1B1) | (1 << COM1B0));

  // Configure Waveform Generation Mode for Normal Operation
  TCCR1B &= ~((1 << WGM13) | (1 << WGM12));
  TCCR1A &= ~((1 << WGM11) | (1 << WGM10));

  // Configure the Timer Clock Source
  TCCR1B &= ~((1 << CS12) | (1 << CS10));
  TCCR1B |= (1 << CS11);

  // Configure the Edge Type (Rising Edge)
  TCCR1B |= (1 << ICES1);

  // Configure PORTB0 as an input via DDRB
  DDRB &= ~(1 << DDB0);

  // Enable Input Capture Interrupt via ICIE1
  TIMSK1 |= (1 << ICIE1);
  
  // Start the serial
  Serial.begin(9600);
  Serial.write("Setup Complete\n\r");

  while(1) {
    // Loop
    if (newTimeStampCopy != oldTimeStampCopy) {
      // Then there has been an interrupt
      if (newTimeStampCopy < oldTimeStampCopy) {
        // It has hit 255 and reset... need to compensate
        Serial.write("Period: ");
        Serial.write(((newTimeStampCopy + 0xFF) - oldTimeStampCopy)/2000000);
        Serial.write("s\n\r\n\r");
      } else {
        // No need to compensate
        Serial.write("Period: ");
        Serial.write((newTimeStampCopy - oldTimeStampCopy)/2000000);
        Serial.write("s\n\r\n\r");
      }
      
      // Set it so that we can continue to check 
      // if there has been an interrupt
      oldTimeStampCopy = newTimeStampCopy;
    }
    Serial.println("Yup");
  }
}
