//  The Moth Project
//  Copyright (C) 2016, The Ensemble

//  INCLUDES
#include "proximity.h"
#include <Servo.h>
#include "servoController.h"
//  IMU
//  Arduino IDE complains about including libraries from within classes unless they are included here too
//  Investigate cause and find a solution!
#include <Wire.h>
#include <SPI.h>
#include <SparkFunLSM9DS1.h>
#include "imu.h"

//  Sensors
Proximity iR(A3);
IMU       iMU;

//  Servos
const int totalServos = 5;
//  Configuration data
const int servoConfig[totalServos][5] = {
//  pin Min   Max   Inv MaxRng
  { 8,  1000, 1250, 0,  250 },  //  Leg
  { 9,  1000, 1250, 1,  250 },  //  Leg
  { 10, 1000, 1444, 0,  444 },  //  Abdomen
  { 11, 1000, 1444, 0,  444 },  //  Abdomen
  { 12, 1000, 1444, 0,  444 }   //  Wings
};

//  Servo status
int servoData[totalServos][9] = {
//  pos   mid   rng   src   targt k   dur   dir isMov
  { 1000, 1125,  250, 1000, 1250, 0,  1000, 1,  0 },
  { 1250, 1125,  250, 1250, 1000, 0,  1000, 1,  0 },
  { 1111, 1222,  222, 1111, 1333, 0,  3000, 1,  1 },
  { 1167, 1222,  110, 1167, 1277, 0,  3000, 1,  1 },
  { 1167, 1222,  110, 1167, 1277, 0,  5000, 1,  0 }
};

//  Servo Initilisation
ServoController rLeg;

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
  Serial.println("RESET");
  //  delay(500);
  iMU.initialise();
  
  //  Attach servos
  rLeg.initialise(servoConfig[0], servoData[0]);
}

void loop() {
  updateSensors();
  updateBehaviour();
  //  Move ...
  rLeg.update(30);
  //  ... abdomen
  //  ... wings
  debug();
}
