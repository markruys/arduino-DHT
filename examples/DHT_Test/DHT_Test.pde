#include "DHT.h"

DHT dht;

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)");

  dht.setup(2); // data pin 2
}

void loop()
{
  delay(dht.getMinimumSamplingPeriod());

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(dht.getHumidity(), 1);
  Serial.print("\t\t");
  Serial.print(dht.getTemperature(), 1);
  Serial.print("\t\t");
  Serial.println(dht.toFahrenheit(dht.getTemperature()), 1);
}

