#include "LineSensorManager.h"



const char * sensorIDs[] =
{
"CF: ",
"RF: ",
"CB: ",
"RB: "
};

const char * pairIDs[] =
{
"CENTER: ",
"RIGHT: ",
"BACK"
};
//Constructor, pass in pointer to array of line sensors
LineSensorManager::LineSensorManager(LineSensor **lineSensors) {
  this->lineSensors = lineSensors;
}

//Main executive, poll line sensors then perform some calculations
bool LineSensorManager::RunTick(uint16_t time, RobotState state) {

  //Poll the line sensors
  this->pollLineSensors();

  //Determine sensor hits
  this->determineSensorHits(LSL_CENTER_BACK);
  this->determineSensorHits(LSL_CENTER_FRONT);
  this->determineSensorHits(LSL_RIGHT_BACK);
  this->determineSensorHits(LSL_RIGHT_FRONT);

  //Determine sensor pair drive points
  this->determineLineDriveCommand(LSP_CENTER);
  this->determineLineDriveCommand(LSP_RIGHT);
  this->determineLineDriveCommand(LSP_BACK);

  //Return without error
  return true;
}

//Debug output
void LineSensorManager::DebugOutput(HardwareSerial *serialPort) {
  uint8_t i;

  for (i=0 ; i<NUM_OF_LINESENSORS; i++) {
    this->sensorDebugOutput((lineSensorLocations)i, serialPort);
    serialPort->print(" ");
  }
  serialPort->println();

  for (i=0 ; i<NUM_OF_LINESENSORS; i++) {
    this->sensorHitDebugOutput((lineSensorLocations)i, serialPort);
    serialPort->print(" ");
  }
  serialPort->println();

  for (i=0 ; i<NUM_OF_SENSORPAIRS; i++) {
    this->driveCommandDebugOutput((lineSensorPairs)i, serialPort);
    serialPort->print(" ");
  }
  serialPort->println();
}

void LineSensorManager::sensorDebugOutput(lineSensorLocations sensor, HardwareSerial *serialPort) {
  uint8_t i;
  uint8_t reading = lineSensors[sensor]->getLineSensorReadings();

  serialPort->print(sensorIDs[sensor]);
  serialPort->print(":");
  for (i=0 ; i<8; i++) serialPort->print((reading & (1 << i))!=0);

}

void LineSensorManager::sensorHitDebugOutput(lineSensorLocations sensor, HardwareSerial *serialPort) {
  serialPort->print(sensorIDs[sensor]);
  serialPort->print(":");

  if (sensorHits[sensor].hit) {
    serialPort->print(sensorHits[sensor].center.x, 2);
    serialPort->print(",");
    serialPort->print(sensorHits[sensor].center.y, 2);
  }
  else serialPort->print("Miss");
}

void LineSensorManager::driveCommandDebugOutput(lineSensorPairs sensorPair, HardwareSerial *serialPort) {
  serialPort->print(pairIDs[sensorPair]);
  serialPort->print(":");

  if (driveCommands[sensorPair].valid) {
    serialPort->print(driveCommands[sensorPair].offset.x, 2);
    serialPort->print(",");
    serialPort->print(driveCommands[sensorPair].offset.y, 2);
    serialPort->print("@");
    serialPort->print(radToDegrees(driveCommands[sensorPair].angle), 2);
    serialPort->write(0xA7); //Degree symbol
  }
  else serialPort->print("Invalid");
}

//Determine the hit location for a sensor array
void LineSensorManager::determineSensorHits(lineSensorLocations location) {
  uint8_t i;
  bool activeHit = false; //Flag for in first contiguous hit section
  uint8_t hitStart = 0; //Tracks the start of the hit
  uint8_t hitCount = 0; //Tracks number of hits in contiguous section

  //Grab pointer for output
  sensorHit_t *output = &sensorHits[location];

  //Get sensor reading
  uint8_t reading = this->lineSensors[location]->getLineSensorReadings();

  //Keep right most reading pair.  Sensor 1 is right most for all arrays
  for (i=0; i<NUM_SENSORS_PER_ARRAY; i++) {
    //Check if sensor is detecting
    //If detected hit and active hit is not active then record start point
    //For all hits increment the hit count
    if (reading & (1 << i)) {
      if (!activeHit) {
        activeHit = true;
        hitStart = i;
      }
      hitCount++;
    }
    //If no hit is detected and there was an active hit, break out because we are done
    else if (activeHit) break;
    //Else no hit, just continue looping
  }
  //Set sensor hit in the output
  output->hit = (hitCount);

  //If the hit count is non-zero, calculate sensor hit center point
  if (hitCount) {
    //Reset center to zero
    output->center.x = 0;
    output->center.y = 0;
    //Loop from hit start to end and sum x and y
    for (i=hitStart; i<hitStart+hitCount; i++) {
      output->center.x+=sensorPositions[location][i].x;
      output->center.y+=sensorPositions[location][i].y;
    }
    //Calc average x and y
    output->center.x /= hitCount;
    output->center.y /= hitCount;
  }
}

//Calculate the line drive commands
void LineSensorManager::determineLineDriveCommand(lineSensorPairs sensorPair) {
  //Get pointer to drive command
  lineDriveCommand_t *output = &driveCommands[sensorPair];
  point_t A = sensorHits[sensorPairs[sensorPair].sensorA].center;
  point_t B = sensorHits[sensorPairs[sensorPair].sensorB].center;

    //Set output as valid, both sensors bad will invalidate it
  output->valid = false;

  if (sensorHits[sensorPairs[sensorPair].sensorA].hit) {
    if (sensorHits[sensorPairs[sensorPair].sensorB].hit) {
      //Both hit
      //Offset is midpoint
      output->offset.x = (A.x + B.x) * 0.5;
      output->offset.y = (A.y + B.y) * 0.5;
      //Calc angle
      output->angle = atan2(A.y-B.y, A.x-B.x);
      output->valid = true;
    }
    else {
      //Only A hit
      output->offset.x = A.x;
      output->offset.y = A.y;
      output->angle = atan2(A.y, A.x);
    }
  }
  else if (sensorHits[sensorPairs[sensorPair].sensorB].hit) {
    //Only B hit
    output->offset.x = B.x;
    output->offset.y = B.y;
    output->angle = atan2(B.y, A.x);
  }
  else {
    //None hit - invalidate sensor drive commnad and zero out
    output->valid = false;
  }

}

//Poll all the sensors
void LineSensorManager::pollLineSensors(void) {

  //Set as outputs and charge the line sensors
  this->lineSensors[LSL_CENTER_BACK]->startCharging();
  this->lineSensors[LSL_CENTER_FRONT]->startCharging();
  this->lineSensors[LSL_RIGHT_BACK]->startCharging();
  this->lineSensors[LSL_RIGHT_FRONT]->startCharging();

  //Delay
  delayMicroseconds(CHARGE_DELAY_US);

  //Stop charging and set as inputs
  this->lineSensors[LSL_CENTER_BACK]->stopCharging();
  this->lineSensors[LSL_CENTER_FRONT]->stopCharging();
  this->lineSensors[LSL_RIGHT_BACK]->stopCharging();
  this->lineSensors[LSL_RIGHT_FRONT]->stopCharging();

  //Delay
  delayMicroseconds(READING_DELAY_US);

  //Get data from line sensors
  this->lineSensors[LSL_CENTER_BACK]->takeReading();
  this->lineSensors[LSL_CENTER_FRONT]->takeReading();
  this->lineSensors[LSL_RIGHT_BACK]->takeReading();
  this->lineSensors[LSL_RIGHT_FRONT]->takeReading();
}

//Get the drive command for the given sensor pair
lineDriveCommand_t LineSensorManager::getLineDriveCommand(lineSensorPairs sensorPair) {
  return this->driveCommands[sensorPair];
}
