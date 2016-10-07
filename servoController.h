//  Servo controller

#ifndef SERVO_C_H
#define SERVO_C_H

#include <Arduino.h>
#include <Servo.h>

class ServoController : public Servo {
  private:
    //  Mechanical constraints - Do not change once initialised!
    int servoPin;
    int mechanicalMinimum;
    int mechanicalMaximum;
    int inverted;           //  0 = normal or 1 = inverted
    int maximumRange;       //  Might be handy?
    
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
    int progress;       //  From 0 to 1000
    int duration;       //  In ms per transition - i.e. From recoiled to extended. NOT the whole cycle!
    int direction;      //  1 or -1
    
    //  Flags indicating servo behaviour
    bool isMoving;
    bool isTwitching;
    int twitchTime;
    int twitchInterval;
    
  public:
    void initialise(const int config[5], int data[9]);
    void update(unsigned long deltaTime);
    void breath(unsigned long deltaTime);
    void twitch(unsigned long deltaTime);
    void updateBounds(int midpoint, int range);
    void setTwitchInterval(int interval);
    void setDuration(int dur);
};

#endif
