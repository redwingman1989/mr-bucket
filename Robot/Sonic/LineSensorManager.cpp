#include "LineSensorManager.h"
#include "System/RunableModule.h"
#include "LineSensor.h"

sensorHit_t LineSensorManager::determineSensorHits(lineSensorLocations location) {
  //Get sensor reading
  uint8_t i;
  uint8_t hitStart = 0;
  bool activeHit = false;
  uint8_t hitCount = 0;
  sensorHit_t output;
  output.hit = false;
  output.center.x = 0;
  output.center.y = 0;
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
  }
  //Set sensor hit in the output
  output.hit = (hitCount);

  //Get sensor hit center point
  for (i=hitStart; i<hitStart+hitCount; i++) {
    output.center.x+=sensorPositions[location][i].x;
    output.center.y+=sensorPositions[location][i].y;
  }

  return output;
}
