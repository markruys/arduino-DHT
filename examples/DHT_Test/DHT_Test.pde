#include "DHT.h"

DHT dht = DHT(2); // data pin 2

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)");

  dht.setup();
}

void loop()
{
  delay(dht.getMinimumSamplingPeriod());

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(dht.getHumidity(), 1);
  Serial.print("\t\t");
  Serial.println(dht.getTemperature(), 1);
}

