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

  Changelog:
   2013-06-10: Initial version by Mark Ruys
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
    AUTO_DETECT,
    DHT11,
    DHT22,
    AM2302,  // Packaged DHT22
    RHT03    // Equivalent to DHT22
  }
  DHT_MODEL_t;

  typedef enum {
    ERROR_NONE = 0,
    ERROR_TIMEOUT,
    ERROR_CHECKSUM
  }
  DHT_ERROR_t;

  DHT(int pin, DHT_MODEL_t model=AUTO_DETECT);

  void setup();

  float getTemperature();
  float getHumidity();

  DHT_ERROR_t getStatus();

  DHT_MODEL_t getModel();

  int getMinimalDelay(); // minimal delay between readings

protected:
  void readSensor();

  float temperature;
  float humidity;

private:
  int pin;
  DHT_MODEL_t model;
  DHT_ERROR_t error;
  unsigned long lastReadTime;
};

#endif /*dht_h*/




