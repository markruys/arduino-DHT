#include "arduino-DHT.h"

DHT dht;

void setup()
{
  Serial.begin(9600);
  Serial.println();

  dht.setup(2); // data pin 2
  if ( dht.getModel() == DHT::DHT11 ) {
    Serial.println("Auto-detected DHT11");
  } else if ( dht.getModel() == DHT::DHT22 ) {
    Serial.println("Auto-detected DHT22");  
  } 

  Serial.println("Status\tHumidity (%)\tTemperature (C)\t(F)");
}

void loop()
{
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  Serial.print(dht.getStatusString());
  Serial.print("\t");
  Serial.print(humidity, 1);
  Serial.print("\t\t");
  Serial.print(temperature, 1);
  Serial.print("\t\t");
  Serial.println(dht.toFahrenheit(temperature), 1);
}

