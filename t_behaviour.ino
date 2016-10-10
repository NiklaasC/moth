//  Behaviour

enum behaviourMode {
  behaviour_normal,
  behaviour_attract,
  behaviour_flutter,
  behaviour_startled,
  behaviour_jostled,
  behaviour_held,
  behaviour_play
};

int currentBehaviour = behaviour_normal;

//  Specific state for thrashing about ... busking
//  boolean attract = false; <= This is now it's own state!
unsigned long attractTime = millis();
long attractInterval = 60000; // 60 seconds between busking

//  max breath = 24 breaths in 20 seconds ~1 breath (in and out) per second i cycle in 1000 - 1 cycle per second
//  min breath = 6 in 30 seconds ~1 breath in 5 seconds 1 cycle in 5000 millis or 0.2 cycles per second
//  breath = 0.2 -> 1 ... with 0.2 being
//  500 = panicked! 1000 = fast 2500 = chill 3000 = very chill
int breathSpeed = 2000;
//  Most servos are MAX RANGE ~ 60
int breathAmplitude = 20;

//  For random twitches - used by the servo controller twitch state to set a random twitch interval
//  Range is total so twitciness varies from twitchiness - range/2 to + range/2
int twitchiness = 3000;
int twitchRange = 4000;

int happyness = 500;
int anger     = 500;

boolean modeInit = false;

void updateBehaviour() {
  if (millis() - behaviourTime >= behaviourInterval) {
    behaviourTime = millis();
    //  Update behaviour
    //  Adjust emotions
    //  Realign reality
    //  Map alternate belief-system
    //  Reticulating Splines
    switch (currentBehaviour) {
      case behaviour_normal:
        normalMode();
        break;
      case behaviour_attract:
        attractMode();
        break;
      case behaviour_flutter:
        flutterMode();
        break;
      case behaviour_startled:
        startledMode();
        break;
      case behaviour_jostled:
        jostledMode();
        break;
      case behaviour_held:
        heldMode();
        break;
      case behaviour_play:
        playMode();
        break;
      default:
        //  If there's an error ... just assume that it's being man-handled to be safe
        jostledMode();
    }
  }
}

void normalMode() {
  //  upabdomen = always breathing
  //  more excited = breathing faster
  
  //  Happy - legs more outwards
  //  calmer movements
  //  Angry - legs are closer
  //  Faster movements
  
  //  Abdomen code
  //  Abdomen up-and-down is an indication of breathing so this should nearly always be going.
  //  Behaviour changes the amplitude (range) and position
  //  If the moth is in attractMode
  if (!modeInit) {
    Serial.println("NORMAL MODE");
    //  Init code!
    upAbdomen.setBounds(15,breathAmplitude);
    upAbdomen.setDuration(breathSpeed);
    upAbdomen.setMode(upAbdomen.Breath);
    
    rLeg.setBounds(30,40);                //  Get from happy/angry code
    rLeg.setDuration(1600);               //  Get from happy/angry code
    rLeg.setTwitchInterval(twitchiness, twitchRange);
    rLeg.setMode(rLeg.Twitch);
    
    lLeg.setBounds(30,40);
    lLeg.setDuration(1600);
    lLeg.setTwitchInterval(twitchiness, twitchRange);
    lLeg.setMode(lLeg.Twitch);
    
    sideAbdomen.setBounds(28,0);
    sideAbdomen.setDuration(1000);
    sideAbdomen.setMode(sideAbdomen.Move);
    
    modeInit = true;
    //  Reset attractMode Timer
    attractTime = millis();
  }
  //  After a mode has been set ... just check to see if it should be in a different mode ... then set the different mode:
  //  Possible states from here are - attract - play - joslted - startled
  //  Priority low => high
  //  Check contitions for each, but let the code follow through ... so that the last condition met (i.e highest Priority)
  //  takes priority
  
  //  Check to see if it hasn't been disturbed - then set to attractMode
  if (millis() - attractTime >= attractInterval) {
    changeMode(behaviour_attract);
  }
  //  If the moth is being played with - the IR sensor is over a certain limit and it is happy!
  //  ???
  
  //  If the moth is startled - STARTLED! STARTLED!!!
  if (startled) {
    changeMode(behaviour_startled);
  }
  //  If the moth is moved ...
  if (jostled) {
    changeMode(behaviour_jostled);
  }
  //  If no conditions are met ... then we just continue in the same state.
}

void attractMode() {
  Serial.println("ATTRACT MODE");
  //  bored -> attractMode - looks like it struggles to get onto it's back
  //  ... fast alternating movements with left and right ... and pushing itself up with it's tail?
  //  stop breathing update
  //  sideways movement with tail.
  //  Legs move alternate to each other at same speed
  //  pause abdomen update
  //  swish abdomen in other direction to leg currently being extended
}

void flutterMode() {
  if (!modeInit) {
    Serial.println("FLUTTER MODE");
    //  wings extend and twitch when it is on it's front in a known orientationStatus
    //  otherwise
    
    //  Open wings
    wings.setBounds(40,0);
    wings.setDuration(4000);                    //  Nice and slow!
    wings.setMode(wings.Move);
    
    modeInit = true;
  }
  
  if (wings.getFinishMoveStatus()) {
    //  Done with opening wings ... flutter!
    //  60 is open!
    wings.setBounds(50,20);
    wings.setDuration(breathSpeed);             //  GET THIS FROM BREATHING CODE!
    wings.setMode(wings.Breath);
    
    //  Reset MOVE flags
    wings.resetMoveStatus();
  }
  //  CHECK OTHER CONDITIONS
}

void startledMode() {
  //  Under Attack
  //  curl up
  //  wait
  //  slowly come back to life ... change to normal mode;
  if (!modeInit) {
    Serial.println("STARTLED MODE");
    
    modeInit = true;
  }
  //  CHECKS
}

void jostledMode() {
  //  Picked up!
  //  curl up wait for being repossitioned ... or held carefully
  //  knownorientation + rightside up => wing flutter
  //  knownorientation + upsidedown => normal mode
  //  unknown orientation but stable => held mode
  if (!modeInit) {
    Serial.println("JOSTLED MODE");
    
    modeInit = true;
  }
  //  CHECKS
}

void heldMode() {
  if (!modeInit) {
    Serial.println("HELD MODE");
    //  Held!
    modeInit = true;
  }
  //  CHECK OTHER STATES
}

void playMode() {
  if (!modeInit) {
    Serial.println("PLAY MODE");
    //  both legs moving simulateneously, like trying to grab at the person interacting with it.
    
    initMode = true;
  }
  //  CHECK OTHER STATES
}

void changeMode(int newMode) {
  modeInit = false;
  currentBehaviour = newMode;
}
