#pragma once

#include "System/RunableModule.h"
#include "LineSensor.h"
#include "System/RunableModule.h"
#include <math.h>

const uint8_t NUM_OF_LINESENSORS = 4;
const uint8_t NUM_OF_SENSORPAIRS = 3;
const uint8_t NUM_SENSORS_PER_ARRAY = 8;

const unsigned int CHARGE_DELAY_US = 10;
const unsigned int READING_DELAY_US = 300;

inline double radToDegrees(double a) { return a * 180.0 / M_PI;}

enum lineSensorLocations {
  LSL_CENTER_FRONT,
  LSL_RIGHT_FRONT,
  LSL_CENTER_BACK,
  LSL_RIGHT_BACK
};


enum lineSensorPairs {
  LSP_CENTER,
  LSP_RIGHT,
  LSP_BACK
};


typedef struct {
  lineSensorLocations sensorA;
  lineSensorLocations sensorB;
} sensorPairSensors;

const sensorPairSensors sensorPairs[NUM_OF_SENSORPAIRS] =
{
  {LSL_CENTER_FRONT, LSL_CENTER_BACK},
  {LSL_RIGHT_FRONT, LSL_RIGHT_BACK},
  {LSL_RIGHT_BACK, LSL_CENTER_BACK}
};

struct  point_t{
  double x;
  double y;
};

typedef struct {
  bool valid;
  point_t offset;
  double angle;
} lineDriveCommand_t;

typedef struct {
  bool hit;
  point_t center;
} sensorHit_t;

class LineSensorManager : public RunableModule {
  public:
  LineSensorManager(LineSensor **lineSensors);
  bool RunTick();
  void DebugOutput(HardwareSerial *serialPort);
  lineDriveCommand_t getLineDriveCommand(lineSensorPairs sensorPair);

  private:
  void determineSensorHits(lineSensorLocations sensor);
  void determineLineDriveCommand(lineSensorPairs sensorPair);
  void pollLineSensors(void);

  void sensorDebugOutput(lineSensorLocations sensor, HardwareSerial *serialPort);
  void sensorHitDebugOutput(lineSensorLocations sensor, HardwareSerial *serialPort);
  void driveCommandDebugOutput(lineSensorPairs sensorPair, HardwareSerial *serialPort);

  LineSensor **lineSensors;
  sensorHit_t sensorHits[NUM_OF_LINESENSORS];
  lineDriveCommand_t driveCommands[NUM_OF_SENSORPAIRS];

};

//Individual sensor reference geometry in inches from the robot center
//From the top, +X is to the right, +Y is to the front
const point_t sensorPositions[NUM_OF_LINESENSORS][NUM_SENSORS_PER_ARRAY] = {
{{-1.8125, 2.9},
{-1.4375, 2.9},
{-1.0625, 2.9},
{-0.6875, 2.9},
{-0.3125, 2.9},
{0.0625, 2.9},
{0.4375, 2.9},
{0.8125, 2.9}},
{{2.1875, 2.9},
{2.5625, 2.9},
{2.9375, 2.9},
{3.3125, 2.9},
{3.6875, 2.9},
{4.0625, 2.9},
{4.4375, 2.9},
{4.8125, 2.9}},
{{-1.42808, -0.64384},
{-1.162914957, -0.9090050429},
{-0.8977499141, -1.174170086},
{-0.6325848712, -1.439335129},
{-0.3674198282, -1.704500172},
{-0.1022547853, -1.969665215},
{0.1629102577, -2.234830258},
{0.4280753006, -2.499995301}},
{{2.57192, -0.64384},
{2.837085043, -0.9090050429},
{3.102250086, -1.174170086},
{3.367415129, -1.439335129},
{3.632580172, -1.704500172},
{3.897745215, -1.969665215},
{4.162910258, -2.234830258},
{4.428075301, -2.499995301}}
};

//Reference geometry of the center of the line sensor along the sensor array axis
//Same coordinate system as individual sensor geometry
const point_t sensorCenters[NUM_OF_LINESENSORS] = {
{-.50, 2.9},
{3.5, 2.9},
{-.5, -1.57192},
{3.5, -1.57192}
};
