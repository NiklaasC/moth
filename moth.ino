//  The Moth Project
//  Copyright (C) 2016, The Ensemble

//  INCLUDES
#include "proximity.h"
#include <Servo.h>
//  IMU
//#include <Wire.h>
//#include <SPI.h>
//#include <LSM9DS1_Registers.h>
//#include <LSM9DS1_Types.h>
//#include <SparkFunLSM9DS1.h>

//  TIMING
unsigned long previousUpdate = 0;
const int updateInterval = 1000/30;

//  SENSORS
Proximity iR( A3 );
//  Acceleration
//  LSM9DS1 imu;

//  SERVOS
const int totalServos = 3;
Servo servos[totalServos];

//  REFERENCE CONSTANTS
//const byte RIGHTLEG     = 0;
//const byte LEFTLEG      = 1;
//const byte UPABDOMEN  = 2;
//const byte SIDEABDOMEN  = 3;
//const byte WINGS      = 4;

//  SERVO CONSTANTS
const byte PIN                = 0;
const byte MECHANICALMINIMUM  = 1;
const byte MECHANICALMAXIMUM  = 2;
const byte INVERTED           = 3;

//  MOVEMENT CONSTANTS
const byte POSITION           = 0;
const byte MIDPOINT           = 1;
const byte RANGE              = 2;
const byte SOURCE             = 3;
const byte TARGET             = 4;
const byte PROGRESS           = 5;
const byte TIME               = 6;
const byte DIRECTION          = 7;
const byte MAXRANGE           = 8;
const byte ISMOVING           = 9;

//  Servo Data
//  [servo][pin,mechanicalMinimum,mechanicalMaximum,Inverted]
const int servoConfig[totalServos][4] = {
//  pin   min   max   inv
  { 8,    50,   95,   0 },
  { 9,   85,   130,  1 },
  //  { 10,,,},
  { 11,   50,   130,  0 }
  //  { 12,,,}
};
//  [servo][position,midPoint,range,source,target,progress,time,direction]
float servoData[totalServos][10] = {
//  pos mid     rng   src   trg     k   time  dir maxRng, isMoving
  { 70, 72.5,   20,   62.5, 82.5,   0,  3000, 1,  20,     0 },
  { 70, 107.5,  20,   97.5, 117.5,  0,  3000, 1,  20,     0 },
  // {,,,,,,,},
  { 70, 90,     40,   70,   110,    0,  5000, 1,  40,     1 }
  // {,,,,,,,}
};

//  ANCILLARY CONSTANTS
//  PI
const float toDeg = 180/PI;
const float toRad = PI/180;

//  BEHAVIOUR

//  All cooldowns or accumulators are number of seconds to get from 1 to 0 or vice versa
//  Modifiers can be modified themselves by other variables.

//  attractMode
//  A slowly increasing measure of activity. When it gets to 1, it will busk for attention.
//  Any interaction will reset this value to 0.
float attractMode = 0;    //  Aka busking!
float attractModeAccumulator = 60;

//  scared
//  Set to 1 when the moth has been frightened (fright) - has highest priority!
//  Reset to 0 over time ... used as a threshold for certain behaviours.
float scared = 0;
float scaredCooldown = 3;

//  excitement
//  A measure of how much it has been interacted with in a pleasent way.
float excitement = 0.5;

//  satisfaction
//  A meausure of satisfaction with itself. Will be set when picked up and turned over?
float satisfaction = 0.5;
float satisfactionAccumulator = 8;

//  twitchiness
//  A general measurement of restlessness.
float twitchiness = 1;
float twitchinessCooldown = 8;

//  breathing
//  Breathing rate ... effected by a lot of things ... should tend towards 0.5 under no stimulus.
//  Effectively time per breath ... so 0.1 breathing is fast! and 1 is normal and slow.
float breathing = 1;

//  DEBUG - hrhr - STATES
bool MECHMINSTATE = false;
bool MECHMAXSTATE = false;

//  SETUP
void setup() {
  //  Initialize serial communication at 9600 bits per second:
  Serial.begin( 9600 );
  Serial.println( "Serial Initialised" );

  for( byte count = 0; count < totalServos; count += 1 ) {
    //  Attach servos to pins
    servos[count].attach(servoConfig[count][PIN]);
  }
}

// LOOP
void loop() {
  unsigned long currentMillis = millis();

  //  Update Sensors and Logic
  if( currentMillis - previousUpdate > updateInterval ) {
    unsigned long dt = currentMillis - previousUpdate;
    previousUpdate = currentMillis;

    iR.update();

    //  Serial.print(iR.getAverage());
    //  Serial.print(" ");
    //  Serial.println(iR.getDifferentialAverage());

    updateMoth( dt );
  }

  //  Update Servos on every loop to maintain smooth movement.
  //  Updating the movement 30 fps is creating jerky movement - more insect-like?
  //  Keeping it in ... ! WIN!

  delay( 1 );
}

void sweepServos( unsigned long dt ) {
  for( byte count = 0; count < totalServos; count += 1 ) {
    if( servoData[count][ISMOVING] == 1 ) {
      servoData[count][PROGRESS] += dt * servoData[count][DIRECTION];

      //  Update sweep bounds
      if( servoConfig[count][INVERTED] == false ) {
        servoData[count][SOURCE] = servoData[count][MIDPOINT] - servoData[count][RANGE];
        servoData[count][TARGET] = servoData[count][MIDPOINT] + servoData[count][RANGE];
      } else {
        servoData[count][SOURCE] = servoData[count][MIDPOINT] + servoData[count][RANGE];
        servoData[count][TARGET] = servoData[count][MIDPOINT] - servoData[count][RANGE];
      }

      if( servoData[count][PROGRESS] >= servoData[count][TIME] ) {
        servoData[count][PROGRESS] = servoData[count][TIME];
        servoData[count][DIRECTION] = -1;
      }
      if( servoData[count][PROGRESS] <= 0 ) {
        servoData[count][PROGRESS] = 0;
        servoData[count][DIRECTION] = 1;
        servoData[count][ISMOVING] = 0;
      }
      servoData[count][POSITION] = servoData[count][SOURCE] + ((servoData[count][TARGET] - servoData[count][SOURCE]) * sinusoidalInOut( servoData[count][PROGRESS]/servoData[count][TIME] ));
    }
  }
  moveServos();
}

//  Moves servos to their current position
void moveServos() {
  for( byte count = 0; count < totalServos; count += 1 ) {
    servoData[count][POSITION] = constrain( servoData[count][POSITION], servoConfig[count][MECHANICALMINIMUM], servoConfig[count][MECHANICALMAXIMUM] );
    servos[count].write( servoData[count][POSITION] );
  }
}

void updateMoth( unsigned long dt ) {
  //  CHECK FOR DEBUGGING STATES
  if( MECHMINSTATE || MECHMAXSTATE ) {
    if( MECHMINSTATE ) {
      for( byte count = 0; count < totalServos; count += 1 ) {
        servoData[count][POSITION] = servoConfig[count][MECHANICALMINIMUM];
        moveServos();
      }
    } else {
      for( byte count = 0; count < totalServos; count += 1 ) {
        servoData[count][POSITION] = servoConfig[count][MECHANICALMAXIMUM];
        moveServos();
      }
    }
  } else {
    //  Otherwise do normal autonomous behaviour

    //  UPDATE COOLDOWNS/ACCUMULATORS HERE?
    //  So they can be overridden during the update?
    if( scared > 0 ) {
      scared = scared - float (dt) / 1000 / scaredCooldown;
    }
    if( satisfaction < 1 && scared < 0.1 ) {
      satisfaction = satisfaction + float( dt ) / 1000 / satisfactionAccumulator;
    }
    twitchiness = twitchiness - float( dt ) / 1000 / twitchinessCooldown;

    //  SENSOR INPUT
    if( iR.getDifferentialAverage() > 50 ) {
      scared = 1;
      satisfaction = 0;
    }

    if( twitchiness <= 0 ) {
      twitchiness = 1;
      twitchinessCooldown = random(4,9);
      twitch();
    }

    for( byte count = 0; count < totalServos; count += 1 ) {
      if( scared >=  0.05 ) {
        //  If the moth is startled it curls up into a ball
        servoData[count][MIDPOINT] = servoConfig[count][MECHANICALMINIMUM];
        servoData[count][RANGE] = 0;
      } else {
        //  Set midpoint
        if( servoConfig[count][INVERTED] == 1 ) {
          servoData[count][MIDPOINT] = servoConfig[count][MECHANICALMINIMUM] - 22.5 * satisfaction;
        } else {
          servoData[count][MIDPOINT] = servoConfig[count][MECHANICALMINIMUM] + 22.5 * satisfaction;
        }
        //  Set range
        servoData[count][RANGE] = servoData[count][MAXRANGE] * satisfaction;
      }
    }
    sweepServos( dt );
  }
}

//  DO NOT CALL RECURSIVELY ... FIND ANOTHER SOLUTION
//  GENERALLY AVOID ON ARDUINO
void twitch() {
  long temp = random( totalServos );
  if( servoData[temp][ISMOVING] == 0 ) {
    servoData[temp][ISMOVING] = 1;
  } else {
    twitch();
  }
}
