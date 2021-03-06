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
  
  currentMode       = Breath;
  isMoving          = false;      //  Is the servo busy with a current instruction
  isTwitching       = false;      //  Is the servo twitching
  twitchTime        = millis();          //  Last twitch
  twitchInterval    = 3000;       //  Next time the leg should twitch - this is updated through behaviours and can be a moving target(!)
  startedPMove       = false;
  finishedPMove      = false;
  
  //  MOVE  ////////////////////////////////
  startedMove = false;
  finishedMove = false;
  
  //////////////////////////////////////////
  
  attach(servoPin);
}

void ServoController::update(unsigned long dt) {
  switch (currentMode) {
    case Breath:
      breath(dt);
      break;
    case Twitch:
      twitch(dt);
      break;
    case Move:
      move(dt);
      break;
    case Stop:
      stop();
      break;
  }
  //  Constrain to mechanical minimum and maximum
  position = constrain(position, mechanicalMinimum, mechanicalMaximum);
  write(position);
  //Serial.println(position);
}

//  A continual sweep from source to target to source ... to target
void ServoController::breath(unsigned long dt) {
  //  Ignores isMoving
  isMoving = 1;
  
  progress += dt * direction;
  
  //  Loop through progress cycles
  if (progress >= duration ) {
    progress = duration;
    direction = -1;
  }
  if (progress <= 0 ) {
    progress = 0;
    direction = 1;
  }
  float t1 = float(progress);
  float t2 = float(duration);
  float k = t1/t2;
  //  Work out current position
  position = source + ((target - source) * sinusoidalInOut(k));
}
//  A single sweep from source to target to source
void ServoController::twitch(unsigned long dt) {
  //  The servo is in twitch mode
  //  updates a timer until the servo reaches a threshold
  //  servo moves through a cycle
  //  servo stops ... another interval is being continuously set in behaviours
  
  //  Check if the twitchInterval is reached ... only if the leg isn't currently twitching
  if ((millis() - twitchTime >= twitchInterval) && !isTwitching) {
    //twitchTime = millis();
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
      twitchInterval = random(twitchiness - twitchRange/2, twitchiness + twitchRange/2);
    }
    
    float t1 = float(progress);
    float t2 = float(duration);
    float k = t1/t2;
    //  Work out current position
    position = source + ((target - source) * sinusoidalInOut(k));
  }
}

void ServoController::move(unsigned long dt) {
  //  Move from source to target ... then stay at the target
  
  if (!startedMove) {
    startedMove = true;
    finishedMove = false;
    //  At the start of the move ...
    //  Set the progress to 0 ... and the source to the current position!
    progress = 0;
    source = position;
    //  I use this with just a midpoint and a range of 0!
    
    if (target >= position) {
      direction = +1;
    } else {
      direction = -1;
    }
  }
  if (!finishedMove) {
    progress += dt * direction;
  }
  //  Loop through progress cycles
  if (progress >= duration) {
    //  Set to 0 because we are changing the definition of source and target
    //  Any progress value will now get you to the destination
    //  Set to 0 so that if there is a transition to another state, progress is essentially set to source.
    progress = 0;
    direction = 1;
    isMoving = false;
    finishedMove = true;
    //  This should stop us getting into trouble transitioning from ... transitions to breath or sweep
    source = target;
    midpoint = target;
    range = 0;
    
    //  Somewhere else needs to reset startedMove ???
    //  TEST  TEST  TEST  TEST  TEST
  }
  
  //  Work out current position
  float t1 = float(progress);
  float t2 = float(duration);
  float k = t1/t2;
  //  Work out current position
  position = source + ((target - source) * sinusoidalInOut(k));
}

void ServoController::stop() {
  //  Does nothing!
  //  Holds current position
}

/*
//  If this is triggered while direction is -1 ... then the moth goes ballistic!
void ServoController::panic(unsigned long dt) {
  //  Move from source to target ... then stay at the target
  
  if (!startedPMove) {
    startedPMove = true;
    finishedPMove = false;
    //  At the start of the move ...
    //  Set the progress to 0 ... and the source to the current position!
    progress = 0;
    source = position;
  }
  if (finishedPMove == false) {
    progress += dt * direction;
  }
  //  Loop through progress cycles
  if (progress >= duration) {
    //  Set to 0 because we are changing the definition of source and target
    //  Any progress value will now get you to the destination
    //  Set to 0 so that if there is a transition to another state, progress is essentially set to source.
    progress = 0;
    direction = 1;
    isMoving = false;
    finishedPMove = true;
    //  This should stop us getting into trouble transitioning from ... transitions to breath or sweep
    source = target;
    midpoint = target;
    range = 0;
    
    //  Somewhere else needs to reset startedMove ???
    //  TEST  TEST  TEST  TEST  TEST
  }
  
  //  Work out current position
  float t1 = float(progress);
  float t2 = float(duration);
  float k = t1/t2;
  //  Work out current position
  position = source + ((target - source) * sinusoidalInOut(k));
}
*/
//  Move statuses
bool ServoController::getStartMoveStatus() {
  return startedMove;
}
bool ServoController::getFinishMoveStatus() {
  return finishedMove;
}

void ServoController::resetMoveStatus() {
  startedMove = false;
  finishedMove = false;
}

void ServoController::setMode(int m) {
  //  Resets flags for Move mode to prevent accidental triggers
  startedMove = false;
  finishedMove = false;
  //  Sets the current mode between Breathing, Twitching and Move
  currentMode = m;
}

//  Sets the
void ServoController::setBounds(int mid, int rng) {
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

void ServoController::setTwitchInterval(int a, int b) {
  twitchiness = a;
  twitchRange = b;
}

void ServoController::setDuration(int dur) {
  duration = dur;
}

void ServoController::debug() {
  Serial.print(position); Serial.print(" ");
  Serial.print(midpoint); Serial.print(" ");
  Serial.print(range); Serial.print(" ");
  Serial.print(source); Serial.print(" ");
  Serial.print(target); Serial.print(" ");
  Serial.print(progress); Serial.print(" ");
  Serial.print(duration); Serial.print(" ");
  Serial.println(direction);
}
