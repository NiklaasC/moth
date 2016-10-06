//  Servo controller

#ifndef SERVO_C_H
#define SERVO_C_H

#include <Servo.h>

class ServoController : public Servo {
  private:
    //  Mechanical constraints - Do not change once initialised!
    int servoPin;
    int mechanicalMinimum;
    int mechanicalMaximum;
    bool inverted;
    
    //  Operational variables that will change often
    int position;
    //  Do I want a modifiable midpoint ... or just a range that operates between the minimum and rangemax
    //  Is it important for the legs to move in a small range near their full extention?
    //  Do Moths do yoga? Is it reaching out for someone or something? Is it useful for the tail? Wings?
    //  Probably!
    int midpoint;
    int range;
    //  Source and target set using midpoint and range ... but can be set independently of them ...
    int source;
    int target;
    
    //  Tweening variables
    float progress;
    float duration;
    int direction;
    
    //  Flag to see if the servo is busy
    bool isMoving;
    
    //  Servo Data
    //  [servo][pin,mechanicalMinimum,mechanicalMaximum,Inverted]
    /*
    const int servoConfig[totalServos][4] = {
    //  pin   min   max   inv
      { 9,    50,   95,   0 },
      { 10,   85,   130,  1 },
      //  {,,,},
      { 12,   50,   130,  0 }
      //  {,,,}
    }
    //  [servo][position,midPoint,range,source,target,progress,time,direction]
    float servoData[totalServos][10] = {
    // change these to microsecond values 1000 = 0, 1500 = 90, 2000 = 180
    //  pos mid     rng   src   trg     k   time  dir maxRng, isMoving
      { 70, 72.5,   20,   62.5, 82.5,   0,  3000, 1,  20,     0 },
      { 105, 107.5,  20,   97.5, 117.5,  0,  3000, 1,  20,     0 },
      // {,,,,,,,},
      { 90, 90,     40,   70,   110,    0,  5000, 1,  40,     1 }
      // {,,,,,,,}
    };*/
  public:
    void initialise();
    void update();
};

#endif
