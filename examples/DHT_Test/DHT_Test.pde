
#include "DHT.h"

DHT dht = DHT(2); // data pin 2

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)");
}

void loop()
{
  delay(dht.getMinimalDelay());

  switch ( dht.getStatus() ) {
    case DHT::ERROR_NONE:
      Serial.print("OK");
      break;

    case DHT::ERROR_TIMEOUT:
      Serial.print("TIMEOUT");
      break;

    case DHT::ERROR_CHECKSUM:
      Serial.print("CHECKSUM");
      break;

    default:
      Serial.print("ERROR");
      break;
  }
  Serial.print("\t");
  Serial.print(dht.getHumidity(), 1);
  Serial.print("\t\t");
  Serial.println(dht.getTemperature(), 1);
}

