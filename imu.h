//  IMU Sensor Class

#ifndef IMU_H
#define IMU_H

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>

//  IMU Setup
//  I2C SETUP: SDO_XM and SDO_G are both pulled high
#define LSM9DS1_M   0x1E    //  Would be 0x1C if SDO_M is LOW
#define LSM9DS1_AG  0x6B    //  Would be 0x6A if SDO_AG is LOW

//  Add or subtract a declination to get a more accurate heading
//  http://www.ngdc.noaa.gov/geomag-web/#declination
#define DECLINATION -1.71   //  Declination (degrees) in Roath, Cardiff

class IMU : public LSM9DS1 {
  private:
    float weight = 0.2;
    float dataAY              = 0;
    float dataAYOld           = 0;
    float dataAYDifferential  = 0;
    
  public:
    void initialise();
    void update();
    float getOrientation();
    float getOrientationDifferential();
    void debugIMU();
};
  
#endif
