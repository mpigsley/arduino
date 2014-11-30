/*
 *  Delay_Distortion_Testing.ino
 *  Mitchel Pigsley
 *  Fernando Urias-Cordero
 *  Casey Berger
 *  Last Update:
 *    4/15/13
 *
 */

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint16_t ticks;
uint16_t ticksX;

#define ADBUFF_SIZE 1900
#define ADBUFF_MASK 0x076D
uint8_t adBuf[ADBUFF_SIZE];
uint16_t volatile adBufInPtr;
uint16_t adBufOutPtr;

uint8_t digiSig;
uint16_t delayVal;
uint8_t distLevel;
uint8_t mean = 121;
uint16_t delayCount;
uint16_t sampleDelay;
uint8_t potTimer;
uint16_t potVal;
boolean isPotRead;

SIGNAL(ADC_vect) {
  // There is a Value to read
  if (!isPotRead) {
    adBuf[adBufInPtr++] = ADCH; // Put Value Into Buffer & Increase Pointer
    adBufInPtr &= ADBUFF_MASK;
  } 
  else {
    potVal = (ADC >> 6) & 0x03FF;
    ADMUX = 0xE0;
    isPotRead = false;
  }

  if (potTimer == 0) {
    // Start ADC Converson on ADC 1
    ADMUX = 0xE1; // Change Mux & how it looks in ADC
    ADCSRA |= 0x40; // Start another ADC Conversion
    isPotRead = true;
  }
}

SIGNAL (TIMER1_OVF_vect) {
  // ~ 8kHz
  TCNT1 = 65285;
  ticks++;
  potTimer++;

  // Start ADC Conversion
  ADCSRA |= 0x40;
}

void processADValue() {
  // Changes Depending on SPDT Switch
  // DISTORTION
  if ((PINB >> 3) & 1 == 1) {
    // Get Value Out
    digiSig = adBuf[adBufOutPtr++];
    adBufOutPtr &= ADBUFF_MASK;
    processDist();  
  } 
  // DELAY
  else {
    //delayVal = (adBufOutPtr - (int)(((1800)*potVal)/0x03FF)) & ADBUFF_MASK;
    delayVal = 1000;
//    if ((adBufInPtr - adBufOutPtr) >= delayVal || 
//      ((adBufInPtr - adBufOutPtr) < 0 && 
//      (adBufInPtr - adBufOutPtr + ADBUFF_SIZE) >= delayVal)) {
//      // Samples are dayed by the delayVal so output at adBufOutPtr
//      digiSig = adBuf[adBufOutPtr++];
//      adBufOutPtr &= ADBUFF_MASK;
//    } 
//    else {
//      // Do Nothing 
//    }

//    if (adBufInPtr - delayVal >= adBuf[0]) {
//      digiSig = adBuf[adBufInPtr - delayVal];
//      adBufOutPtr++;
//      adBufOutPtr &= ADBUFF_MASK;
//    } else {
//      digiSig = adBuf[adBufInPtr - delayVal + ADBUFF_SIZE];
//      adBufOutPtr++;
//      adBufOutPtr &= ADBUFF_MASK;
//    }
     
     digiSig = adBuf[(adBufInPtr - delayVal) & ADBUFF_MASK];
     adBufOutPtr++;
     adBufOutPtr &= ADBUFF_MASK;
  }
     

  // Output to Pins for Exteral DAC
  PORTB = 0;
  PORTB |= ((digiSig & 0xC0) >> 6);
  PORTD = 0; 
  PORTD |= ((digiSig & 0x3F) << 2);
}

void processDist() {
  // Find Distortion Level
  distLevel = 10 + (19*potVal) / 0x03FF;

  // Hard clipping at Peak
  if (digiSig > mean + distLevel) {
    digiSig = mean + distLevel;  
  }

  // Hard clipping at Valley
  if (digiSig < mean - distLevel) {
    digiSig = mean - distLevel;
  }

}

int main() {
  // Pins
  // Set DAC for Output & DistDelSel as Input
  DDRB = 0xF3;
  DDRD = 0xFC;

  // UART
  UBRR0H = 0;      // Set BAUD Rate
  UBRR0L = 104;
  UCSR0C = 0x06;   // Set 8-bit Word Size
  UCSR0B = 0x98;   // Set USART Interrupt & Enable Bits

  // ADC
  ADMUX = 0xE0;    // Set Reference Voltage & MUX
  ADCSRA = 0xCF;   // ADC & Interrupt Enable
  ADCSRB = 0x00;

  // Timer 1
  TCCR1A = 0x00;   
  TCCR1B = 0x02;   // Prescalar: clk/8
  TIMSK1 = 0x01;   // Set Overflow Interrupt

  // Turn on Interrupts
  sei();

  // Set Global Vars
  ticks = 0;
  ticksX = 0;
  isPotRead = false;

  // Loop
  while(1) {

    if (ticks != ticksX) {
      // Every .125 milli-s
      ticksX++;

      // Process ADC Value
      if (adBufInPtr != adBufOutPtr) {
        processADValue();
      }      
    }

  }

}


