//  Sensors
//  Reads sensor data and sets flags for events

//  Flags for behaviour
boolean upsidedown = true;
boolean orientationChange = false;
unsigned long changeTime = 0;
int settledInterval = 1000 * 8;

//  Recoil
//  A state for when the moth is picked up, startled or jostled.
//  A curled state that when placed on a table reads as flat
//  The tailed will have to be curled a bit to get the moth to read as flat on it's belly.
//  After a period of time - Have the moth try to flip itself on it's back

void updateSensors() {
  if ( millis() - sensorTime >= sensorInterval ) {
    sensorTime = millis();
    
    iR.update();
    iMU.update();
    
    
    
    
    
    //  Check for being picked up
    //  Needs another check or different method!
    if (abs(iMU.getOrientationDifferential()) >= 0.03) {
      Serial.println("Recoil");
      //  Testing external editor setting!
    }
    
    
    
    
    //  orientationStatus();
    //  Add to IMU?
    //  Check orientation!
    //  Check to see if the moth's orientation has changed
    //  Does the moth know it's orientation?
    //  Is it upright?
    //  Upside-down?
    if (iMU.getOrientation() < 0.9 && iMU.getOrientation() > -0.9 && !orientationChange) {
      //  set flag
      orientationChange = true;
      Serial.println("Orientation Unknown");
      changeTime = millis();
    }
    if ((iMU.getOrientation() > 0.9 || iMU.getOrientation() < -0.9) && orientationChange ) {
      if (millis() - changeTime >= settledInterval) {
        
        if( iMU.getOrientation() > 0.9) {
          upsidedown = false;
          orientationChange = false;
          Serial.println("Rightway Up!");
        } else {
          upsidedown = true;
          orientationChange = false;
          Serial.println("Upside Down!");
        }
      }
    }
  }
}
