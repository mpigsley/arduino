/*
 *  LabFour.ino
 *  ADC Conversion 
 *
 */

void setup() {
  // Setup up Voltage Reference
  ADMUX |= (1 << REFS0);
  
  // Enable ADC
  ADCSRA |= (1 << ADEN);
  
  // Set clock between 50kHz & 200KHz
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
  
  // Start Serial
  Serial.begin(9600);
  
}

void loop() {
  // Cycle through the Mux Values
  int i= 0;
  uint8_t temp = 0;
  uint16_t var[4];
  
  for (i; i < 4; i++) {
    temp = i;
    ADMUX = (ADMUX & 0xF0) | (temp & 0xF);
    
    // Start Conversion
    ADCSRA |= (1 << ADSC);
    
    // Wait until finished
    while ((ADCSRA >> ADSC) & 1);
    
    // Read Values from register
    var[i] = ADC;
  }
  
  // Print out results
  i = 0;
  for (i; i < 4; i++) {
    Serial.print(i);
    Serial.print(": ");
    Serial.println(var[i]);
    Serial.print("Voltage: ");
    float multiplier = 5.0/1024;
    Serial.println(var[i]*multiplier);
  }
  
  // Delay
  delay(1000);
  
}
