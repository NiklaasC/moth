//  IR Proximity Sensor

#ifndef PROXIMITY_H
#define PROXIMITY_H

#include <Arduino.h>

class Proximity {
  private:
    int pin;
    static const int dataSize = 6;  //  Number of samples for smoothing
    int data[dataSize];
    int differential[dataSize];
    int dataIndex = 0;
    int total;
    int differentialTotal;
    int average;
    int differentialAverage;

  public:
    Proximity(int setPin);
    void update();
    int getAverage();
    int getDifferentialAverage();

};

#endif
