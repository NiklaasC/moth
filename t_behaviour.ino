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
long attractInterval = 30000; // 60 seconds between busking

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
int twitchRange = 4000;   //  Timer range
int twitchAmplitude = 50; //  Physical movement range



boolean modeInit = false;

void updateBehaviour() {
  if (millis() - behaviourTime >= behaviourInterval) {
    
    unsigned long lastTime = behaviourTime;
    behaviourTime = millis();
    //  Get actual DT for actual accuracy
    unsigned long dt = behaviourTime - lastTime;
    
    //  Update behaviour
    //  Adjust emotions
    //  Realign reality
    //  Map alternate belief-system
    //  Reticulating Splines
    switch (currentBehaviour) {
      case behaviour_normal:
        normalMode(dt);
        break;
      case behaviour_attract:
        attractMode(dt);
        break;
      case behaviour_flutter:
        flutterMode(dt);
        break;
      case behaviour_startled:
        startledMode(dt);
        break;
      case behaviour_jostled:
        jostledMode(dt);
        break;
      case behaviour_held:
        heldMode(dt);
        break;
      case behaviour_play:
        playMode(dt);
        break;
      default:
        Serial.println("DEFAULT! ERROR!");
        //  If there's an error ... just assume that it's being man-handled to be safe
        jostledMode(dt);
    }
    
    Serial.println(scared);
  }
}

////////////////////////////////////////////////////////////////////////////////////////  NORMAL
void normalMode(unsigned long dt) {
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
    
    rLeg.setBounds(25,twitchAmplitude);                //  Get from happy/angry code
    rLeg.setDuration(breathSpeed);               //  Get from happy/angry code
    rLeg.setTwitchInterval(twitchiness, twitchRange);
    rLeg.setMode(rLeg.Twitch);
    
    lLeg.setBounds(30,twitchAmplitude);
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

///////////////////////////////////////////////////////////////////////////////////////// ATTRACT
void attractMode(unsigned long dt) {
  if (!modeInit) {
    Serial.println("ATTRACT MODE");
    //  bored -> attractMode - looks like it struggles to get onto it's back
    //  ... fast alternating movements with left and right ... and pushing itself up with it's tail?
    //  stop breathing update
    //  sideways movement with tail.
    //  Legs move alternate to each other at same speed
    //  pause abdomen update
    //  swish abdomen in other direction to leg currently being extended
    
    //  Get current time!
    attractTime = millis();
    
    upAbdomen.setMode(upAbdomen.Stop);
    
    rLeg.setBounds(25,twitchAmplitude);                 //  Get from happy/angry code
    rLeg.setDuration(500);                              //  Get from happy/angry code
    rLeg.setTwitchInterval(1000, 0);
    rLeg.setMode(rLeg.Twitch);
    
    lLeg.setBounds(30,twitchAmplitude);
    lLeg.setDuration(500);
    lLeg.setTwitchInterval(1000, 0);
    lLeg.setMode(lLeg.Twitch);
    
    sideAbdomen.setBounds(28,50);
    sideAbdomen.setDuration(1000);
    sideAbdomen.setMode(sideAbdomen.Breath);
    
    modeInit = true;
  }
  
  
  
  if (millis() - attractTime >= 5000) {
    
    sideAbdomen.setBounds(28,0);
    sideAbdomen.setDuration(100);
    sideAbdomen.setMode(sideAbdomen.Breath);
    
    changeMode(behaviour_normal);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////// FLUTTER
void flutterMode(unsigned long dt) {
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
    wings.setBounds(50,breathAmplitude*2);
    wings.setDuration(breathSpeed);             //  GET THIS FROM BREATHING CODE!
    wings.setMode(wings.Breath);
    
    //  Reset MOVE flags
    wings.resetMoveStatus();
  }
  //  CHECK OTHER CONDITIONS
  //  Once it's fluttering - the moth is on its front
  //  It can't do anything ... so it'll just do it's thing!
  //  Just recoil if it's picked up!
  if (jostled) {
    //  CLOSE WINGS
    wings.setBounds(0,0);
    wings.setDuration(100);                    //  FAST
    wings.setMode(wings.Breath);
    
    changeMode(behaviour_jostled);
  }
}

////////////////////////////////////////////////////////////////////////////////////////////  STARTLED
void startledMode(unsigned long deltaTime) {
  //  Under Attack
  //  curl up
  //  wait
  //  slowly come back to life ... change to normal mode;
  if (!modeInit) {
    Serial.println("STARTLED MODE");
    //  USE STARTLED TIME
    
    upAbdomen.setBounds(55,0);
    upAbdomen.setDuration(breathSpeed);
    upAbdomen.setMode(upAbdomen.Breath);
    
    rLeg.setBounds(6,0);
    rLeg.setMode(rLeg.Twitch);
    
    lLeg.setBounds(45,0);
    lLeg.setMode(lLeg.Twitch);
    
    sideAbdomen.setBounds(28,0);
    sideAbdomen.setDuration(100);
    sideAbdomen.setMode(sideAbdomen.Breath);
    
    modeInit = true;
  }
  
  if (millis() - startledTime >= 3000) {
    scared = scared - (deltaTime / ((startledInterval - 3000)/1000));
    if (scared < 0) {
      scared = 0;
    }
  }
  
  //  Use transition helpers to smooth between states
  upAbdomen.setBounds(tH(scared, 55, 15),tH(scared,0,breathAmplitude));
  rLeg.setBounds(tH(scared, 6, 25),tH(scared,0,twitchAmplitude));
  lLeg.setBounds(tH(scared,45,30),tH(scared,0,twitchAmplitude));
  
  //  CHECKS
  if (!startled && scared <= 0) {
    scared = 0;
    changeMode(behaviour_normal);
  }
  
  if (jostled) {
    scared = 0;
    changeMode(behaviour_jostled);
  }
}

/////////////////////////////////////////////////////////////////////////////////////////// JOSTLED
void jostledMode(unsigned long dt) {
  //  Picked up!
  
  if (!modeInit) {
    Serial.println("JOSTLED MODE");
    //  curl up wait for being repositioned ... or held carefully
    
    upAbdomen.setBounds(35,0);
    upAbdomen.setDuration(breathSpeed);
    upAbdomen.setMode(upAbdomen.Breath);
    
    rLeg.setBounds(6,0);
    rLeg.setMode(rLeg.Twitch);
    
    lLeg.setBounds(45,0);
    lLeg.setMode(lLeg.Twitch);
    
    sideAbdomen.setBounds(28,0);
    sideAbdomen.setDuration(100);
    sideAbdomen.setMode(sideAbdomen.Breath);
    
    modeInit = true;
  }
  
  if (millis() - jostleTime >= 3000) {
    jostledTimeout = jostledTimeout - (dt / ((jostleInterval - 3000)/1000));
    if (jostledTimeout < 0) {
      jostledTimeout = 0;
    }
  }
  //  CHECKS
  //  unknown orientation but stable => held mode
  //  knownorientation + rightside up => wing flutter
  //  knownorientation + upsidedown => normal mode
  upAbdomen.setBounds(tH(jostledTimeout, 55, 15),tH(jostledTimeout,0,breathAmplitude/2));
  rLeg.setBounds(tH(jostledTimeout, 6, 25),tH(jostledTimeout,0,twitchAmplitude/2));
  lLeg.setBounds(tH(jostledTimeout,45,30),tH(jostledTimeout,0,twitchAmplitude/2));
  
  if (jostledTimeout == 0) {
    if (!orientationChange) {
      if(upsidedown) {
        //  Start transitioning to normal state
        scared = 0;
        changeMode(behaviour_normal);
      } else {
        //  Start transitioning to flutter state
        upAbdomen.setMode(upAbdomen.Stop);
        rLeg.setMode(rLeg.Stop);
        lLeg.setMode(lLeg.Stop);
        
        scared = 0;
        changeMode(behaviour_flutter);
        
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////////////////// HELD
void heldMode(unsigned long dt) {
  if (!modeInit) {
    Serial.println("HELD MODE");
    //  Held! - Small movements
    modeInit = true;
  }
  //  CHECK OTHER STATES
  if (jostled) {
    //  Being held roughly => curl up ... await reorientation
  }
}

//////////////////////////////////////////////////////////////////////////////////////////  PLAYING
void playMode(unsigned long dt) {
  if (!modeInit) {
    Serial.println("PLAY MODE");
    //  both legs moving simulateneously, like trying to grab at the person interacting with it.
    
    modeInit = true;
  }
  //  CHECK OTHER STATES
}

void changeMode(int newMode) {
  modeInit = false;
  currentBehaviour = newMode;
}

//  Transition helper
int tH(int variable, int currentPosition, int desiredPosition) {
  float temp1 = 1000 - variable;
  temp1 = temp1/1000;
  int temp = currentPosition - temp1*(currentPosition - desiredPosition);
  Serial.print(temp);Serial.print(" ");Serial.println(temp1);
  return temp;
}
