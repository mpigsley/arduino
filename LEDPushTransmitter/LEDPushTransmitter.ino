#include <VirtualWire.h>

void setup()
{
  Serial.begin(9600);	  // Debugging only
  Serial.println("setup");
  pinMode(13,OUTPUT);

  // Initialise the IO and ISR
  vw_setup(2400);	 // Bits per sec
}

void loop()
{
  const char *msg = "hello";

  digitalWrite(13, true); // Flash a light to show transmitting
  vw_send((uint8_t *)msg, strlen(msg));
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(13, false);
  delay(1000);
}


