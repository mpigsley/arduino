// 
// FILE:  dht_test.pde
// PURPOSE: DHT library test sketch for Arduino
//

#include <DHTTemperature.h>

dht DHT;

uint8_t temperaturePin = 2;

void setup() {
  Serial.begin(115200);
  Serial.println("DHT TEST PROGRAM ");
  Serial.print("LIBRARY VERSION: ");
  Serial.println(DHT_LIB_VERSION);
  Serial.println();
  Serial.println("Type\tStatus\tHumidity (%)\tTemperature (F)\tTemperature (C)");
}

void loop()
{
  // READ DATA
  Serial.print("DHT22, \t");
  int chk = DHT.read22(temperaturePin);
  switch (chk)
  {
    case DHTLIB_OK:  
                Serial.print("OK,\t"); 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                Serial.print("Checksum error,\t"); 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                Serial.print("Time out error,\t"); 
                break;
    default: 
                Serial.print("Unknown error,\t"); 
                break;
  }
  // DISPLAY DATA
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t\t");
  Serial.print((DHT.temperature*9)/5 + 32, 1);
  Serial.print(",\t\t\t");
  Serial.println(DHT.temperature, 1);

  delay(1000);
}