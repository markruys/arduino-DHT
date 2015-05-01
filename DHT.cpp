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

#include "DHT.h"

void DHT::setup(uint8_t pin, DHT_MODEL_t model)
{
  DHT::pin = pin;
  DHT::model = model;

  if ( model == AUTO_DETECT) {
    DHT::model = DHT22;
    readSensor();
    if ( error == ERROR_TIMEOUT ) {
      DHT::model = DHT11;
      // Warning: in case we auto detect a DHT11, you should wait at least 1000 msec
      // before your first read request. Otherwise you will get a time out error.
    }
  }
}

#ifndef OPTIMIZE_SRAM_SIZE

const char* DHT::getStatusString()
{
  switch ( error ) {
    case DHT::ERROR_TIMEOUT:
      return "TIMEOUT";

    case DHT::ERROR_CHECKSUM:
      return "CHECKSUM";

    default:
      return "OK";
  }
}

#else

// At the expense of 26 bytes of extra PROGMEM, we save 11 bytes of
// SRAM by using the following method:

prog_char P_OK[]       PROGMEM = "OK";
prog_char P_TIMEOUT[]  PROGMEM = "TIMEOUT";
prog_char P_CHECKSUM[] PROGMEM = "CHECKSUM";

const char *DHT::getStatusString() {
  prog_char *c;
  switch ( error ) {
    case DHT::ERROR_CHECKSUM:
      c = P_CHECKSUM; break;

    case DHT::ERROR_TIMEOUT:
      c = P_TIMEOUT; break;

    default:
      c = P_OK; break;
  }

  static char buffer[9];
  strcpy_P(buffer, c);

  return buffer;
}

#endif

void DHT::readSensor()
{
  temperature = NAN;
  humidity = NAN;

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

  pinMode(pin, INPUT_PULLUP); // Switch bus to receive data

  // We're going to read 41 pulses, which each consist of a transition to HIGH
  // and then back to LOW again. The first pulse is the start bit and goes for
  // ~80us. Then after that, a 26-28us pulse is a 0, and a 70us pulse is a 1.
  //
  // It's helpful to disable interrupts for the whole duration of capture, since
  // at 8 MHz a timer interrupt takes ~10us, long enough to cause a short "0"
  // pulse to be missed. However, this will cause millis() and micros() to give
  // an inaccurate result.

  noInterrupts();

  word rawHumidity;
  word rawTemperature;
  word data;

  unsigned calibration = 0;

  for ( int8_t i = -1 ; i < 40; i++ ) {
    byte width = (byte)pulseIn(pin, HIGH, 150);

	if (width == 0) {
	  error = ERROR_TIMEOUT;
      interrupts();
	  return;
	}

    if ( i >= 0 ) {
      // Now we are being fed our 40 bits
      data <<= 1;

      if ( width > 45 ) {
        data |= 1; // we got a one
      }

      switch ( i ) {
        case 15:
          rawHumidity = data;
          break;
        case 31:
          rawTemperature = data;
          data = 0;
          break;
      }
    }
  }
  interrupts();

  // Verify checksum

  if ( (byte)(((byte)rawHumidity) + (rawHumidity >> 8) + ((byte)rawTemperature) + (rawTemperature >> 8)) != data ) {
    error = ERROR_CHECKSUM;
    return;
  }

  // Store readings

  if ( model == DHT11 ) {
    humidity = rawHumidity >> 8;
    temperature = rawTemperature >> 8;
  }
  else {
    humidity = rawHumidity * 0.1;

    if ( rawTemperature & 0x8000 ) {
      rawTemperature = -(int16_t)(rawTemperature & 0x7FFF);
    }
    temperature = ((int16_t)rawTemperature) * 0.1;
  }

  error = ERROR_NONE;
}
