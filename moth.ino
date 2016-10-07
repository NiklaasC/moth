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
const int servoConfig[totalServos][6] = {
//  pin Min   Max   Inv MaxRng  Offset
  { 8,  0,    180,  0,  180,    +0 },  //  Right Leg  //  Moth's Right Leg
  { 9,  0,    180,  1,  180,    +0 },  //  Left Leg
  { 10, 0,    180,  0,  180,    +0 },  //  Up Abdomen
  { 11, 0,    180,  0,  180,    +0 },  //  Side Abdomen
  { 12, 0,    60,  0,  60,      +0 }     //  Wings             Calibrated
};

//  Servo status
int servoData[totalServos][8] = {
//  pos   mid   rng   src   targt k   dur   dir
  { 0,    0,    0,    0,    0,    0,  1000, 1 },
  { 0,    0,    0,    0,    0,    0,  1000, 1 },
  { 0,    0,    0,    0,    0,    0,  1000, 1 },
  { 0,    0,    0,    0,    0,    0,  1000, 1 },
  { 0,    0,    0,    0,    0,    0,  1000, 1 }
};

//  Servo Initilisation
ServoController rLeg;
ServoController lLeg;
ServoController upAbdomen;
ServoController sideAbdomen;
ServoController wings;

//  Timers
unsigned long sensorTime      = millis();
unsigned long behaviourTime   = millis();
unsigned long movementTime    = millis();
unsigned long debugTime       = millis();

const int sensorInterval      = 1000/15;
const int behaviourInterval   = 1000/15;
const int movementInterval    = 1000/15;
const int debugInterval       = 1000/15;

char debugMode = 'D';

//  Initialising servos, communication and configuration
void setup() {
  //  Start serial communication
  Serial.begin(115200);
  Serial.println("RESET");
  
  iMU.initialise();
  
  //  Initialise and attach servos
         rLeg.initialise(servoConfig[0], servoData[0]);
         lLeg.initialise(servoConfig[1], servoData[1]);
    upAbdomen.initialise(servoConfig[2], servoData[2]);
  sideAbdomen.initialise(servoConfig[3], servoData[3]);
        wings.initialise(servoConfig[4], servoData[4]);
}

void loop() {
  //  Take sensor data and apply filters to set flags
  updateSensors();
  //  Take sensor data and adjust the moths internal mental state - set movement parameters
  updateBehaviour();
  //  Apply the changes made by behaviour to move the moth
  updateMovement();
  //  Debug output
  debug();
}

void updateMovement() {
  if (millis() - movementTime >= movementInterval) {
    unsigned long lastTime = movementTime;
    movementTime = millis();
    //  Get actual DT for actual accuracy
    unsigned long dt = movementTime - lastTime;
    
    //  These update the position regardless of current operating mode
    rLeg.update(dt);
    lLeg.update(dt);
    upAbdomen.update(dt);
    sideAbdomen.update(dt);
    wings.update(dt);
  }
}
