#include <FreqCount.h>
#include <Wire.h>
#include <hh10d.hpp>

void setup() {
  Serial.begin(9600);
  HH10D::initialize();
}

void loop() {
  // Don't need to read these constants more than once.
  // Can now remove extra wires for reading them.
  const int HH10D_OFFSET(7732);
  const int HH10D_SENSITIVITY(402);

  Serial.print(HH10D::getHumidity(HH10D_SENSITIVITY, HH10D_OFFSET));
  Serial.println("%");
}
