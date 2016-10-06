//  behaviour
//  Variables in the range of 0 to 1000

//  int scared = 0;
//  int scaredAdjust = 10;
//  int happy = 500;
//  int happyAdjust = 0;
//  max breath = 24 breaths in 20 seconds ~1 breath (in and out) per second i cycle in 1000 - 1 cycle per second
//  min breath = 6 in 30 seconds ~1 breath in 5 seconds 1 cycle in 5000 millis or 0.2 cycles per second
//  breath = 0.2 -> 1 ... with 0.2 being
float breath = 0.5;
//int breathAdjust = 0;

void updateBehaviour() {
  if ( millis() - behaviourTime >= behaviourInterval ) {
    behaviourTime = millis();
    //  Update behaviour
    //  Adjust emotions
  }
}

void attractMode() {
  
}
