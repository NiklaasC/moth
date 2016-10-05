//  IR Proximity Sensor
#include "proximity.h"

//  Constructor
//  Sets the pin used to communicate with the IR Proximity Sensor
Proximity::Proximity(int setPin) {
  pin = setPin;

  //  Initialise all the data to 0
  for (int index = 0; index < dataSize; index++) {
    data[index] = 0;
    differential[index] = 0;
  }
}

//  Updates the value from the IR sensor. Creates a smoothed value and a smoothed differential value.
void Proximity::update() {
  //  Remove the old index value from the running total
  total = total - data[dataIndex];
  //  Read the new value
  data[dataIndex] = analogRead(pin);
  //  Add the new value to the running total
  total = total + data[dataIndex];

  //  Calculate new differential value
  differentialTotal = differentialTotal - differential[dataIndex];
  if (dataIndex == 0) {
    differential[dataIndex] = data[dataIndex] - data[dataSize-1];
  } else {
    differential[dataIndex] = data[dataIndex] - data[dataIndex-1];
  }
  differentialTotal = differentialTotal + differential[dataIndex];

  //  Increment the index
  dataIndex = dataIndex + 1;
  if (dataIndex >= dataSize) {
    dataIndex = 0;
  }
  //  Calculate moving average
  average = total / dataSize;
  differentialAverage = differentialTotal / dataSize;
}

//  Get the smoothed value from the IR sensor
int Proximity::getAverage() {
  return average;
}

//  Get the smoothed differential value from the IR sensor
int Proximity::getDifferential() {
  return differentialAverage;
}
