//  Sensors
//  Reads sensor data and sets flags for events

//  Flags for behaviour
boolean upsidedown          = true;
boolean orientationChange   = false;
boolean jostled             = false;
boolean startled            = false;
boolean playing             = false;

int settledInterval         = 1000 * 3;
unsigned long changeTime    = 0;

int jostleInterval          = 1000 * 8;
unsigned long jostleTime    = 0;

int startledInterval        = 1000 * 8;
unsigned long startledTime  = 0;

int playInterval        = 1000 * 3;
unsigned long playTime  = 0;
boolean playTimerSet = false;

float orientationBound = 0.88;
int iRStartle = 50;
float jostledBound = 0.03;

//  Recoil
//  A state for when the moth is picked up, startled or jostled.
//  The tail will have to be curled a bit to get the moth to read as flat on it's belly.
//  After a period of time - Have the moth try to flip itself on it's back - version 2?

void updateSensors() {
  if (millis() - sensorTime >= sensorInterval) {
    sensorTime = millis();
    
    iR.update();
    
    iMU.update();
    
    playStatus();
    jostleStatus();
    startledStatus();
    orientationStatus();
    
  }
}

//  If the IR sensor is over a threshold - for a while - set PLAYING!
void playStatus() {
  //  If the theshold is broken
  if (iR.getAverage() > 500) {
    //  start the timer to set playing to true
    if (!playTimerSet) {
      playTime = millis();
      playTimerSet = true;
    }
  } else {
    if (playing) {
      playing = false;
      Serial.println("NOT PLAYING!");
    }
    playTimerSet = false;
  }
  
  if ((millis() - playTime >= playInterval ) && playTimerSet) {
    if (!playing) {
      playing = true;
      Serial.println("PLAYING!");
    }
  }
}

//  Checks for disturbances to the moth from accelerometers - sets the jostled flag
void jostleStatus() {
  if (abs(iMU.getJostle()) >= jostledBound) {
    if (!jostled) {
      Serial.println("FLAG: Recoil");
      jostled = true;
    }
    jostleTime = millis();
  }
  if (jostled) {
    if (millis() - jostleTime >= jostleInterval) {
      jostled = false;
      Serial.println("FLAG: Un-Recoil");
    }
  }
}

//  Check for fast movements towards the moth

void startledStatus() {
  if (iR.getDifferential() > iRStartle) {
    if (!startled) {
      startled = true;
      playing = false;
      Serial.print("FLAG: Startled");
    }
    startledTime = millis();
  }
  if (startled) {
    if (millis() - startledTime >= startledInterval) {
      startled = false;
      Serial.println("FLAG: Un-Startled");
    }
  }
}

//  Checks for orientation
//  Checks to see if the moth's orientation has changed     - bool orientationChange
//  Checks to see whether the moth is upside-down           - bool upsidedown
void orientationStatus() {
  if (iMU.getOrientation() < orientationBound && iMU.getOrientation() > -orientationBound && !orientationChange) {
    //  set flag
    orientationChange = true;
    playing = false;
    Serial.println("FLAG: Orientation Unknown");
    changeTime = millis();
  }
  if ((iMU.getOrientation() > orientationBound || iMU.getOrientation() < -orientationBound) && orientationChange) {
    if (millis() - changeTime >= settledInterval) {
      
      if( iMU.getOrientation() > orientationBound) {
        upsidedown = false;
        orientationChange = false;
        Serial.println("FLAG: Orientation: Rightway-up!");
      } else {
        upsidedown = true;
        orientationChange = false;
        Serial.println("FLAG: Orientation: Upside-down!");
      }
    }
  }
}
