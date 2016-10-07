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
int servoData[totalServos][8] = {
//  pos   mid   rng   src   targt k   dur   dir
  { 1000, 1125,  250, 1000, 1250, 0,  1000, 1 },
  { 1250, 1125,  250, 1250, 1000, 0,  1000, 1 },
  { 1111, 1222,  222, 1111, 1333, 0,  3000, 1 },
  { 1167, 1222,  110, 1167, 1277, 0,  3000, 1 },
  { 1167, 1222,  110, 1167, 1277, 0,  5000, 1 }
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
const int movementInterval    = 1000/30;
const int debugInterval       = 1000/1;

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
  updateSensors();
  updateBehaviour();
  updateMovement();
  debug();
}

void updateMovement() {
  if ( millis() - movementTime >= movementInterval ) {
    unsigned long lastTime = movementTime;
    movementTime = millis();
    //  Get actual DT for actual accuracy
    unsigned long dt = movementTime - lastTime;
    
    rLeg.update(dt);
    lLeg.update(dt);
    upAbdomen.update(dt);
    sideAbdomen.update(dt);
    wings.update(dt);
  }
}
