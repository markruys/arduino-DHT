#include "DHT.h"

DHT dht;

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println("Status\tHumidity (%)\tTemperature (C)");

  dht.setup(2); // data pin 2
}

void loop()
{
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity()*0.1;
  float temperature = dht.getTemperature()*0.1;

  Serial.print(!dht.getStatus());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.println(temperature, 1);
}
