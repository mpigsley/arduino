void setup() {
  // Set Serial and Print Complete
  Serial.begin(9600);
  Serial.println("Finished Setup!\n\r");
}
volatile uint3_t var = 1;
volatile uint8_t i = 0xFFFF;

long first, last;

void loop() {
  first = micros();
  for (i = 0; i < 255; i++) {
    var = var *3.1346; 
  } 
  last = micros();
  Serial.print("MicroSeconds: ");
  Serial.print((float)(last - first)/(255));
  Serial.print("\r\n");
}
