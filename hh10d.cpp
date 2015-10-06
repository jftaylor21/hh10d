#include "hh10d.hpp"

#include <Arduino.h>

#include <Wire.h>
#ifndef TwoWire_h
#error Need to #include <Wire.h>
#endif

#include <FreqCount.h> // github.com/PaulStoffregen/FreqCount
#ifndef FreqCount_h
#error Need to #include <FreqCount.h> // github.com/PaulStoffregen/FreqCount
#endif

namespace
{
  bool hh10Initialized = false;
  double freqScaling = 1;
}

void HH10D::initialize(int readPeriod)
{
  // the FreqCount library is hardcoded to use pin 5 for input
  const int FREQCOUNT_PIN(5);
  pinMode(FREQCOUNT_PIN, INPUT);
  FreqCount.begin(readPeriod);
  freqScaling = 1000 / readPeriod;
  
  Wire.begin();
  
  hh10Initialized = true;
}

namespace
{
  // Reads and returns calibration data from HH10D.
  // Returns 0 if error occurs while reading or uninitialized.
  int readCalibration(uint8_t memoryAddress)
  {
    int ret(0);
    if (hh10Initialized)
    {    
      // hh10 device address is always 81
      const uint8_t DEVICE_ADDRESS(81);
      
      // set address to read calibration from
      Wire.beginTransmission(DEVICE_ADDRESS);
      Wire.write(memoryAddress);
      Wire.endTransmission();
      
      // begin reading calibration
      const uint8_t NUM_BYTES(2);
      Wire.requestFrom(DEVICE_ADDRESS, NUM_BYTES);
      
      // read bytes and combine into the calibration data
      if (Wire.available())
      {
        ret = Wire.read();
        if (Wire.available())
        {
          ret = ret * 256 + Wire.read();
        }
      }
    }
    return ret;
  }
}

int HH10D::getSensitivity()
{
  int ret = 0;
  if (hh10Initialized)
  {
    const uint8_t SENSITIVITY_ADDRESS(10);
    ret = readCalibration(SENSITIVITY_ADDRESS);
  }
  return ret;
}

int HH10D::getOffset()
{
  int ret = 0;
  if (hh10Initialized)
  {
    const uint8_t OFFSET_ADDRESS(12);
    ret = readCalibration(OFFSET_ADDRESS);
  }
  return ret;
}


namespace
{
  // Returns last frequency measurement or 0 if uninitialized.
  unsigned long getFrequency()
  {
    static unsigned long lastFrequency(0);
    if (hh10Initialized)
    {    
      if (FreqCount.available())
      {
        lastFrequency = FreqCount.read();
      }
    }
    return lastFrequency;
  }
}

double HH10D::getHumidity(int sensitivity, int offset)
{
  double ret = 0;
  if (hh10Initialized)
  {
    unsigned long freq = getFrequency();
    if (freq != 0)
    {
      // formula from www.hoperf.com/upload/sensor/hh10d.pdf
      ret = (offset - freq*freqScaling) * sensitivity / 4096.;
    }
  }
  return ret;
}