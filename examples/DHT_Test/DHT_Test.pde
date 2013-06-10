
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

  DHT::DHT_t results = dht.read();

  switch ( results.error ) {
    case DHT::ERROR_NONE:
      Serial.print("OK");
      break;

    case DHT::ERROR_TOO_QUICK:
      Serial.print("TOO QUICK");
      break;

    case DHT::ERROR_TIMEOUT:
      Serial.print("TIMEOUT");
      break;

    case DHT::ERROR_CHECKSUM:
      Serial.print("CHECKSUM");
      break;

    default:
      Serial.print("ERROR");
      Serial.print(results.error);
      break;
  }
  Serial.print("\t");
  Serial.print(results.humidity, 1);
  Serial.print("\t\t");
  Serial.println(results.temperature, 1);
}

