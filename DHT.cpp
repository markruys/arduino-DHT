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

//
// Datasheets:
// - http://www.micro4you.com/files/sensor/DHT11.pdf
// - http://www.adafruit.com/datasheets/DHT22.pdf
// - http://dlnmh9ip6v2uc.cloudfront.net/datasheets/Sensors/Weather/RHT03.pdf
// - http://meteobox.tk/files/AM2302.pdf

#include "DHT.h"

DHT::DHT(int pin, DHT_MODEL_t model) {
  DHT::pin = pin;
  DHT::model = model;

  DHT::lastReadTime = millis();

  // Determine sensor model
  if ( model == AUTO_DETECT) {
    if ( read().error == ERROR_TIMEOUT ) {
      model = DHT11;
    }
    else {
      model = DHT22;
    }
  }
}

DHT::DHT_MODEL_t DHT::getModel() {
  return model;
}

int DHT::getMinimalDelay() {
  return model == DHT11 ? 1010 : 2020;
}

DHT::DHT_t DHT::read()
{
  DHT_t results;
  results.temperature = NAN;
  results.humidity = NAN;

  // Make sure we don't poll the sensor too often
  // - Max sample rate DHT11 is 1 Hz   (duty cicle 1000 ms)
  // - Max sample rate DHT22 is 0.5 Hz (duty cicle 2000 ms)
  unsigned long startTime = millis();
  if ( (unsigned long)(startTime - lastReadTime) < (model == DHT11 ? 1000L : 2000L) ) {
   results.error = ERROR_TOO_QUICK;
   return results;
  }
  lastReadTime = startTime;

  // Request sample

  digitalWrite(pin, LOW); // Send start signal
  pinMode(pin, OUTPUT);
  if ( model == DHT11 ) {
    delay(18);
  }
  else {
    // This will fail for a DHT11 - that's how we can detect such a device
    delayMicroseconds(800);
  }

  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH); // Switch bus to receive data

  word humidity;
  word temperature;
  word data;

  // We're going to read 83 edges:
  // - First a FALLING, RISING, and FALLING edge for the start bit
  // - Then 40 bits: RISING and then a FALLING edge per bit
  // To keep our code simple, we accept any HIGH or LOW reading if it's max 85 usecs long

  for ( int8_t i = -3 ; i < 2 * 40; i++ ) {
    byte age;
    startTime = micros();

    do {
      age = (unsigned long)(micros() - startTime);
      if ( age > 85 ) {
        results.error = DHT::ERROR_TIMEOUT;
        return results;
      }
    }
    while ( digitalRead(pin) == (i & 1) ? HIGH : LOW );

    if ( i >= 0 && (i & 1) ) {
      // Now we are being fed our 40 bits
      data <<= 1;

      // A zero max 30 usecs, a one at least 68 usecs.
      if ( age > 30 ) {
        data |= 1; // we got a one
      }
    }

    switch ( i ) {
      case 31:
        humidity = data;
        break;
      case 63:
        temperature = data;
        data = 0;
        break;
    }
  }

  // Verify checksum

  if ( (byte)(((byte)humidity) + (humidity >> 8) + ((byte)temperature) + (temperature >> 8)) != data ) {
    results.error = ERROR_CHECKSUM;
    return results;
  }

  // Store readings

  if ( model == DHT11 ) {
    results.humidity = humidity >> 8;
    results.temperature = temperature >> 8;
  }
  else {
    results.humidity = humidity * 0.1;

    if ( temperature & 0x8000 ) {
      temperature = -(int16_t)(temperature & 0x7FFF);
    }
    results.temperature = ((int16_t)temperature) * 0.1;
  }

  results.error = ERROR_NONE;

  return results;
}


