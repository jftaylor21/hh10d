#ifndef HH10D_HPP
#define HH10D_HPP

/**
 * Reads humidity from HH10D Humidity Sensor
 * See www.hoperf.com/upload/sensor/hh10d.pdf
 */
namespace HH10D
{
  /**
   * Initializes HH10D hardware.
   *
   * \param readPeriod Length of time in msec between new sensor measurements.
   *                   Longer periods correspond to more accurate measurements.
   */
  void initialize(int readPeriod=1000);
  
  /**
   * Reads sensitivity constant from HH10D.
   * Note: This is a hardware specific constant that only needs read once.
   *
   * \return Sensitivity constant.
   */
  int getSensitivity();
  
  /**
   * Reads offset constant from HH10D.
   * Note: This is a hardware specific constant that only needs read once.
   *
   * \return Offset constant.
   */
  int getOffset();
  
  /**
   * Reads humidity from HH10D. Returns previous humidity if no new sensor data
   * is available.
   *
   * \param sensitivity HH10D sensitivity constant.
   * \param offset HH10 offset constant.
   *
   * \return Relative humidity in percent.
   */
  double getHumidity(int sensitivity, int offset);
}

#endif