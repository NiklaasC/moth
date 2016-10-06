//  Servo controller

#include "servoController.h"

void ServoController::initialise(const int config[5], int data[9]) {
  servoPin          = config[0];
  mechanicalMinimum = config[1];
  mechanicalMaximum = config[2];
  inverted          = config[3];
  maximumRange      = config[4];
  
  position          = data[0];
  midpoint          = data[1];
  range             = data[2];
  source            = data[3];
  target            = data[4];
  progress          = data[5];
  duration          = data[6];
  direction         = data[7];
  isMoving          = data[8];
}

void ServoController::update(unsigned long dt) {
  
}
