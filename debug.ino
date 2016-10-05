//  debug
//  Outputs in two flavours:
//  1.  General status readout every second
//      The type of readout can be set via serial input
//  2.  Immediate output via serial input

//  Capital letters output via flavour 1. Lower case outputs data in flavour 2.

//  modes
//  D/d General debug output

void debug() {
  char incomingByte;
  if ( millis() - debugTime <= debugInterval ) {
    //  Regular debug output
    if ( debugMode == 'D' ) {
      debugIMU();
    }
  }
  if ( Serial.available() > 0 ) {
    //  Immediate debug output
    incomingByte = Serial.read();
    //  "d" command - general debug output
    if ( incomingByte == 'd') {
      iMU.debugIMU();
    }
    if ( incomingByte == 'D') {
      debugMode = 'D';
    }
  }
}

//  rawData
//  Outputs
void rawData() {
  ;
}
