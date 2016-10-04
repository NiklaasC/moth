//  The Moth Project
//  Copyright (C) 2016, The Ensemble

//  INCLUDES
#include "proximity.h"
#include <Servo.h>
//  IMU
//  Arduino IDE complains about including libraries from within classes unless they are included here too
//  Investigate cause and find a solution!
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
#include "imu.h"

//  Sensors
Proximity iR( A3 );
IMU iMU;

//  Servos

//  Timers
unsigned long sensorTime      = millis();
unsigned long behaviourTime   = millis();
unsigned long debugTime       = millis();

const int sensorInterval      = 1000/15;
const int behaviourInterval   = 1000/15;
const int debugInterval       = 1000/1;

char debugMode = 'D';

//  Initialising servos, communication and configuration
void setup() {
  //  Start serial communication
  Serial.begin(115200);
  Serial.println("R E S E T");
  //  delay(500);
  iMU.initialise();
  
  //  Attach servos
}

void loop() {
  updateSensors();
  updateBehaviour();
  //  Move ...
  //  ... legs
  //  ... abdomen
  //  ... wings
  debug();
}
