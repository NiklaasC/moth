//  Sensors
//  Reads sensor data and sets flags for events

//  Flags for behaviour
boolean upsidedown          = true;
boolean orientationChange   = false;
boolean jostled             = false;
boolean startled            = false;

int settledInterval         = 1000 * 3;
unsigned long changeTime    = 0;

int jostleInterval          = 1000 * 8;
unsigned long jostleTime    = 0;

int startledInterval        = 1000 * 8;
unsigned long startledTime  = 0;


//  Recoil
//  A state for when the moth is picked up, startled or jostled.
//  The tail will have to be curled a bit to get the moth to read as flat on it's belly.
//  After a period of time - Have the moth try to flip itself on it's back - version 2?

void updateSensors() {
  if (millis() - sensorTime >= sensorInterval) {
    sensorTime = millis();
    
    iR.update();
    iMU.update();
    
    jostleStatus();
    startledStatus();
    orientationStatus();
    
  }
}

//  Checks for disturbances to the moth from accelerometers - sets the jostled flag
void jostleStatus() {
  if (abs(iMU.getJostle()) >= 0.03) {
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
  if (iR.getDifferential() > 50) {
    if (!startled) {
      startled = true;
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
  if (iMU.getOrientation() < 0.9 && iMU.getOrientation() > -0.9 && !orientationChange) {
    //  set flag
    orientationChange = true;
    Serial.println("FLAG: Orientation Unknown");
    changeTime = millis();
  }
  if ((iMU.getOrientation() > 0.9 || iMU.getOrientation() < -0.9) && orientationChange) {
    if (millis() - changeTime >= settledInterval) {
      
      if( iMU.getOrientation() > 0.9) {
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
