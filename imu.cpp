//  IMU Sensor Class
#include "imu.h"

#define SMOOTHED  0
#define OLD       1
#define DIFF      2
#define RAW       3

void IMU::initialise() {
  //  Use the settings struct to set the communication mode
  settings.device.commInterface = IMU_MODE_I2C;
  settings.device.mAddress      = LSM9DS1_M;
  settings.device.agAddress     = LSM9DS1_AG;
  
  if ( !begin() ) {
    Serial.println("Failed to communicate with IMU");
    //  Loop and await reset
    while (1) {
      ;
    }
  }
}

void IMU::update() {
  //  Update the accelerometers, gyroscopes and magnetometers.
  readAccel();  //  Updates ax, ay, az
  //  readGyro();   //  Updates gx, gy, gz
  //  readMag();    //  Updates mx, my, mz
  
  //  Calculate smoothed accelerometer data and differential
  //  Input accelerometer data
  accelerometerData[0][3] = calcAccel(ax);
  accelerometerData[1][3] = calcAccel(ay);
  accelerometerData[2][3] = calcAccel(az);
  
  //  For each axis use exponential smoothing
  //  0 = smoothed data, 1 = last value, 2 = differential, 3 = RAW!
  for(int count = 0; count < 2; count++ ) {
    accelerometerData[count][OLD] = accelerometerData[count][SMOOTHED];
    accelerometerData[count][SMOOTHED] = (weight * accelerometerData[count][RAW]) + (1 - weight)*accelerometerData[count][SMOOTHED];
    accelerometerData[count][DIFF] = accelerometerData[count][SMOOTHED] - accelerometerData[count][OLD];
  }
}

//  The IMU is mounted sideways so what would be the Z axis is actually Y so Orientation is read from accelData[1]
float IMU::getOrientation() {
  return accelerometerData[1][0];
}
float IMU::getOrientationDifferential() {
  return accelerometerData[1][2];
}

//  Returns the maximum differential of all axis
float IMU::getJostle() {
  float maximum = abs(accelerometerData[0][2]);
  maximum = max( maximum, abs(accelerometerData[1][2]) );
  maximum = max( maximum, abs(accelerometerData[2][2]) );
  return maximum;
}

void IMU::debugIMU() {
  Serial.print(accelerometerData[1][0]);
  Serial.print(" ");
  Serial.println(getJostle());
}
