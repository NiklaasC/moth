//  IMU Sensor Class
#include "ad_imu.h"

#define SMOOTHED  0
#define OLD       1
#define DIFF      2
#define RAW       3

void IMU::initialise() {
  /* Initialise the sensor */
  if(!begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("No BNO055 detected ... Check your wiring or I2C ADDR!");
    while(1);
  }
  
  delay(1000);
    
  setExtCrystalUse(true);
}

void IMU::update() {
  
  sensors_event_t event;
  getEvent(&event);
  imu::Vector<3> acc = getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER);
  
  //  Calculate smoothed accelerometer data and differential
  //  Input accelerometer data
  accelerometerData[0][3] = event.orientation.y;
  
  accelerometerData[1][3] = acc.x();
  accelerometerData[2][3] = acc.y();
  accelerometerData[3][3] = acc.z();
  
  //  For each axis use exponential smoothing
  //  0 = smoothed data, 1 = last value, 2 = differential, 3 = RAW!
  for(int count = 0; count < 3; count++ ) {
    accelerometerData[count][OLD] = accelerometerData[count][SMOOTHED];
    accelerometerData[count][SMOOTHED] = (weight * accelerometerData[count][RAW]) + (1 - weight)*accelerometerData[count][SMOOTHED];
    accelerometerData[count][DIFF] = accelerometerData[count][SMOOTHED] - accelerometerData[count][OLD];
  }
}

//  The IMU is mounted sideways so what would be the Z axis is actually Y so Orientation is read from accelData[1]
float IMU::getOrientation() {
  //  Can't use map as it is integer only - divide by 90 (range: +90 -90) to get number between -1 and 1
  //  Added a negative to reverse the direction of the Orientation
  //  We have the IMU upside-down
  float temp = -accelerometerData[0][0] / 90;
  
  return temp;
}
float IMU::getOrientationDifferential() {
  //  change to accelerometer diff? No need ... do I even use this?
  return accelerometerData[1][2];
}

//  Returns the maximum differential of all axis
float IMU::getJostle() {
  float maximum = abs(accelerometerData[1][2]);
  maximum = max( maximum, abs(accelerometerData[2][2]) );
  maximum = max( maximum, abs(accelerometerData[3][2]) );
  return maximum;
}

void IMU::debugIMU() {
  Serial.print(accelerometerData[0][0]);
  Serial.print(" ");
  Serial.print(getJostle());
  Serial.print(" ");
  Serial.println(getOrientation());
}
