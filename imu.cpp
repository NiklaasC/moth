//  IMU Sensor Class
#include "imu.h"

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
  readGyro();   //  Updates gx, gy, gz
  readAccel();  //  Updates ax, ay, az
  readMag();    //  Updates mx, my, mz
  
  dataAYOld = dataAY;
  dataAY = (weight * calcAccel(ay)) + (1 - weight) * dataAY;
  dataAYDifferential = dataAY - dataAYOld;
  
  /*
  Serial.print(calcGyro(gx));
  Serial.print(" ");
  Serial.print(calcGyro(gy));
  Serial.print(" ");
  Serial.print(calcGyro(gz));
  Serial.print(" ");
  Serial.print(calcAccel(ax));
  Serial.print(" ");
  Serial.print(calcAccel(ay));
  Serial.print(" ");
  Serial.print(calcAccel(az));
  Serial.print(" ");
  Serial.print(calcMag(mx));
  Serial.print(" ");
  Serial.print(calcMag(my));
  Serial.print(" ");
  */
  
  /*
  Serial.print(dataAY);
  Serial.print(" ");
  Serial.print(dataAYDifferential);
  Serial.print(" ");
  Serial.println(calcAccel(ay));
  */
  
  //  Serial.println(dataAX);
  //  attitude();
}

float IMU::getOrientation() {
  return dataAY;
}

float IMU::getOrientationDifferential() {
  return dataAYDifferential;
}

void IMU::debugIMU() {
  Serial.print(dataAY);
  Serial.print(" ");
  Serial.println(dataAYDifferential);
}
