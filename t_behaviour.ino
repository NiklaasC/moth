//  behaviour
//  Variables in the range of 0 to 1000

//  Specific state for thrashing about ... busking
boolean attract = false;
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
    //  Realign reality
    //  Superimpose alternate belief-system
    //  Reticulating Splines
    
    //  Abdomen code
    //  Abdomen up-and-down is an indication of breathing so this should nearly always be going.
    //  Behaviour changes the amplitude (range) and position
    //  If the moth is in attractMode
    //upAbdomen.setBounds(15,20);
    //upAbdomen.setDuration(4000);              //  Get from breathing code
    //upAbdomen.setMode(upAbdomen.Breath);
    
    rLeg.setBounds(30,40);
    rLeg.setDuration(1600);              //  Get from breathing code
    rLeg.setTwitchInterval(6000);
    rLeg.setMode(rLeg.Twitch);
    
    //  Side to side movement of abdomen ... sign of happyness? Like a tail wagging?
    //  Extended periods of close ... but not scary interaction = tail wagging!
    
    /*
    //  Wing code
    //  Keep the wings closed unless the moth is stable and the rightway-up!
    if (!orientationChange && !upsidedown) {
      //  Slowly open wings!
      if (wings.getFinishMoveStatus()) {
        //  Done with opening wings ... flutter!
        wings.setBounds(1100,200);
        wings.setDuration(1000);                    //  GET THIS FROM BREATHING CODE!
        wings.setMode(wings.Breath);
      } else {
        //  Open wings
        wings.setBounds(1000,0);
        wings.setDuration(3000);                    //  Nice and slow!
        wings.setMode(wings.Move);
      }
      //  Occational flutter
    } else {
      //  Fold wings / keep wings folded back
      //  This will snap the wings back into position
      wings.setBounds(1500,0);
      wings.setMode(wings.Move);
    } */
  }
}

void attractMode() {
  
}

//  bored -> attractMode - looks like it struggles to get onto it's back
//  ... fast alternating movements with left and right ... and pushing itself up with it's tail?
//  stop breathing update
//  sideways movement with tail.

//  upabdomen = always breathing
//  more excited = breathing faster
//
//  wings extend and twitch when it is on it's front in a known orientationStatus
//  otherwise
