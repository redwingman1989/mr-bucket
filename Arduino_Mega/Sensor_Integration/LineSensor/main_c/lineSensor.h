/* Macros */
#define NUM_LINE_SENSOR_SENSORS      (8)
#define FRONT_LINE_SENSOR_PIN_OFFSET (FRONT_LN_SEN_SENSOR_1_PIN)
#define RIGHT_LINE_SENSOR_PIN_OFFSET (RIGHT_LN_SEN_SENSOR_1_PIN)
#define REAR_LINE_SENSOR_PIN_OFFSET  (REAR_LN_SEN_SENSOR_1_PIN)
#define LEFT_LINE_SENSOR_PIN_OFFSET  (LEFT_LN_SEN_SENSOR_1_PIN)
#define BLACK_LINE_MICROSEC_THRESHOLD_VALUE (900)

/* Line Sensor Array */
unsigned char frontLineSensorDischargeTimes[NUM_LINE_SENSOR_SENSORS];
unsigned char rightLineSensorDischargeTimes[NUM_LINE_SENSOR_SENSORS];
unsigned char rearLineSensorDischargeTimes[NUM_LINE_SENSOR_SENSORS];
unsigned char leftLineSensorDischargeTimes[NUM_LINE_SENSOR_SENSORS];

/* Enums */
enum lineID {
  INDETERMINATE,
  WHITE,
  BLACK,
  NUM_LINE_IDS
};

/* Structs */
struct lineUnderSensorStruct {
  union {
    struct {
      unsigned char front     :1;
      unsigned char right     :1;
      unsigned char rear      :1;
      unsigned char left      :1;
      unsigned char spare3    :1;
      unsigned char spare4    :1;
      unsigned char spare5    :1;
      unsigned char spare6    :1;
    };
    unsigned char allSensors;
  };
};

/* Global Variables */
lineUnderSensorStruct lineUnderSensor;
