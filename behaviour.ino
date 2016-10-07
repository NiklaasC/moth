//  behaviour
//  Variables in the range of 0 to 1000

//  Specific state for thrashing about ... busking
boolean attractMode = false;
boolean attractTime = millis();
boolean attractInterval = 60000; // 60 seconds between busking

boolean scared = false;
//  int scaredAdjust = 10;
//  int happy = 500;
//  int happyAdjust = 0;
//  max breath = 24 breaths in 20 seconds ~1 breath (in and out) per second i cycle in 1000 - 1 cycle per second
//  min breath = 6 in 30 seconds ~1 breath in 5 seconds 1 cycle in 5000 millis or 0.2 cycles per second
//  breath = 0.2 -> 1 ... with 0.2 being
float breath = 0.5;
//  int breathAdjust = 0;

void updateBehaviour() {
  if (millis() - behaviourTime >= behaviourInterval) {
    behaviourTime = millis();
    //  Update behaviour
    //  Adjust emotions
    
    //  Abdomen code
    //  Abdomen up-and-down is an indication of breathing so this should nearly always be going.
    //  Behaviour changes the amplitude (range) and position
    //  If the moth is in attractMode
    upAbdomen.breath();
    
    //  Side to side movement of abdomen ... sign of happyness? Like a tail wagging?
    //  Extended periods of close ... but not scary interaction = tail wagging!
    
    
    //  Wing code
    //  Keep the wings closed unless the moth is stable and the rightway-up!
    if (!orientationChange && !upsidedown) {
      //  Slowly open wings!
      //  Occational flutter
    } else {
      //  Fold wings / keep wings folded back
      
    }
  }
}

void attractMode() {
  
}

//  bored -> attractMode - looks like it struggles to get onto it's back
//  ... fast alternating movements with left and right ... and pushing itself up with it's tail
//  upabdomen = always breathing
//  more excited = breathing faster
//
//  wings extend and twitch when it is on it's front in a known orientationStatus
//  otherwise
