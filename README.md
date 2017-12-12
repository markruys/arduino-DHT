DHT
===

An Arduino library for reading the DHT family of temperature and humidity sensors.    
Forked from [arduino-DHT][https://github.com/markruys/arduino-DHT]
Original written by Mark Ruys, <mark@paracas.nl>.    
Updated to work with ESP32 by Bernd Giesecke, <bernd@giesecke.tk>.   
For ESP32 (a multi core/ multi processing SOC) task switching must be disabled while reading data from the sensor.    
Function computeHeatIndex was integrated from Adafruit DHT library.    
 
Features
--------
  - Support for DHT11 and DHT22, AM2302, RHT03
  - Auto detect sensor model
  - Low memory footprint
  - Very small code

Usage
-----

```
#include "DHTesp.h"

DHTesp dht;

void setup()
{
  Serial.begin(9600);

  dht.setup(2); // data pin 2
}

void loop()
{
  delay(dht.getMinimumSamplingPeriod());

  Serial.print(dht.getHumidity());
  Serial.print("\t");
  Serial.print(dht.getTemperature());
}
```
Also check out the [example] how to read out your sensor. For all the options, see [dhtesp.h][header].

Installation
------------

Place the [DHTesp][download] library folder in your `<arduinosketchfolder>/libraries/` folder. You may need to create the `libraries` subfolder if its your first library. Restart the Arduino IDE. 

[download]: https://github.com/beegee-tokyo/arduino-DHTesp/archive/master.zip "Download DHT library"
[example]: https://github.com/beegee-tokyo/arduino-DHTesp/blob/master/examples/DHT_Test/DHT_Test.pde "Show DHT example"
[header]: https://beegee-tokyo/arduino-DHTesp/blob/master/DHT.h "Show header file"

