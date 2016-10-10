//  IMU Sensor Class
#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

class IMU : public Adafruit_BNO055 {
  private:
    float weight              = 0.2;  //  For exponential smoothing from 0 to 1
    
    //  First Dimension           Second Dimension
    //  0 = x, 1 = Y, 2 = z       0 = smoothed data, 1 = last value, 2 = differential, 3 = RAW!
    float accelerometerData[4][4] = {{0.0}};
    
  public:
    void initialise();
    void update();
    float getOrientation();
    float getOrientationDifferential();
    float getJostle();
    void debugIMU();
};
  
#endif
