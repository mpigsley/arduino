#include <VirtualWire.h>

void setup()
{
  Serial.begin(9600);	// Debugging only
  Serial.println("setup");
  pinMode(13,OUTPUT);

  // Initialise the IO and ISR
  vw_setup(2400);	 // Bits per sec

  vw_rx_start();       // Start the receiver PLL running
}

void loop()
{
  uint8_t buf[VW_MAX_MESSAGE_LEN];
  uint8_t buflen = VW_MAX_MESSAGE_LEN;

  if (vw_get_message(buf, &buflen)) // Non-blocking
  {
    int i;

    digitalWrite(13, true); // Flash a light to show received good message
    // Message with a good checksum received, dump it.
    Serial.print("Got: ");

    for (i = 0; i < buflen; i++)
    {
      Serial.print(buf[i], HEX);
      Serial.print(" ");
    }
    Serial.println("");
    digitalWrite(13, false);
  }
}


