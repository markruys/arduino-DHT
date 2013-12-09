/******************************************************************
  DHT Temperature & Humidity Sensor library for Arduino.

  Features:
  - Support for DHT11 and DHT22/AM2302/RHT03
  - Auto detect sensor model
  - Very low memory footprint
  - Very small code

  http://www.github.com/markruys/arduino-DHT

  Written by Mark Ruys, mark@paracas.nl.

  BSD license, check license.txt for more information.
  All text above must be included in any redistribution.

  Datasheets:
  - http://www.micro4you.com/files/sensor/DHT11.pdf
  - http://www.adafruit.com/datasheets/DHT22.pdf
  - http://dlnmh9ip6v2uc.cloudfront.net/datasheets/Sensors/Weather/RHT03.pdf
  - http://meteobox.tk/files/AM2302.pdf

  Changelog:
   2013-06-10: Initial version
   2013-06-12: Refactored code
   2013-07-01: Add a resetTimer method
 ******************************************************************/

#ifndef dht_h
#define dht_h

#if ARDUINO < 100
  #include <WProgram.h>
#else
  #include <Arduino.h>
#endif

class DHT
{
public:

  typedef enum {
    ERROR_NONE = 0,
    ERROR_TIMEOUT,
    ERROR_CHECKSUM
  }
  DHT_ERROR_t;

  void setup(uint8_t pin);
  void resetTimer();

  int16_t getHumidity() {
    readSensor();
    return humidity;
  }
  
  int16_t getTemperature() {
    readSensor();
    int16_t temp = (int16_t)temperature;
    return (temperature & 0x8000)? -temp: temp;
  }

  DHT_ERROR_t getStatus() { return error; };

  int getMinimumSamplingPeriod() { return 2000; }

private:
  void readSensor();

  uint16_t humidity, temperature;
  uint8_t pin;
  DHT_ERROR_t error;
  unsigned long lastReadTime;
};

#endif /*dht_h*/
