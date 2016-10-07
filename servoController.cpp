//  Servo controller

#include "servoController.h"
#include "tween.h"

void ServoController::initialise(const int config[5], int data[8]) {
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
  
  isMoving          = false;      //  Is the servo busy with a current instruction
  isTwitching       = false;      //  Is the servo twitching
  twitchTime        = 0;          //  Last twitch
  twitchInterval    = 1000;       //  Next time the leg should twitch - this is updated through behaviours and can be a moving target(!)
  
  attach(servoPin);
}

void ServoController::update(unsigned long dt) {
  //  Constrain to mechanical minimum and maximum
  position = constrain(position, mechanicalMinimum, mechanicalMaximum);
  writeMicroseconds(position);
}

//  A continual sweep from source to target to source ... to target
void ServoController::breath(unsigned long dt) {
  //  Ignores isMoving
  isMoving = 1;
  
  progress += dt * direction;
  
  //  Do this everytime the leg moves . . . nah . . . maybe everytime the behaviour is changed
  //  This is where inversion matters!
  
  //updateBounds();
  
  //  Loop through progress cycles
  if (progress >= duration ) {
    progress = duration;
    direction = -1;
  }
  if (progress <= 0 ) {
    progress = 0;
    direction = 1;
  }
  
  //  Work out current position
  position = source + ((target - source) * sinusoidalInOut(progress/duration));
}
//  A single sweep from source to target to source
void ServoController::twitch(unsigned long dt) {
  //  The servo is in twitch mode
  //  updates a timer until the servo reaches a threshold
  //  servo moves through a cycle
  //  servo stops ... another interval is being continuously set in behaviours
  
  //  Check if the twitchInterval is reached ... only if the leg isn't currently twitching
  if ((millis() - twitchTime >= twitchInterval) && !isTwitching) {
    isTwitching = true;
    isMoving = true;
  }
  
  if (isTwitching) {
    progress += dt * direction;
    
    //  Update sweep bounds in behaviours
    
    if (progress >= duration) {
      progress = duration;
      direction = -1;
    }
    if (progress <= 0) {
      progress = 0;
      direction = 1;
      isMoving = false;
      isTwitching = false;
      //  Update the last time the servo twitches
      twitchTime = millis();
    }
    
    position = source + ((target - source) * sinusoidalInOut(progress/duration));
  }
}

//  Sets the
void ServoController::updateBounds(int mid, int rng) {
  //  Update bounds - being inverted just inverts the src and target fields!
  midpoint  = mid;
  range     = rng;
  if (!inverted) {
    //  Normal
    source  = mid - rng/2;
    target  = mid + rng/2;
  } else {
    //  Inverted
    source  = mid + rng/2;
    target  = mid - rng/2;
  }
}

void ServoController::setTwitchInterval(int interval) {
  twitchInterval = interval;
}

void ServoController::setDuration(int dur) {
  duration = dur;
}
