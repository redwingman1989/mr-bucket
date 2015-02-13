#ifndef _ULTRASONICSENSOR_H
#define _ULTRASONICSENSOR_H

/* Macros */

/* Front Facing UltraSonic */
#define ULTRA_FRONT_TRIG_PIN (7)
#define ULTRA_FRONT_ECHO_PIN (18) /* PD3 = INT3 */

/* Left Facing UltraSonic */
#define ULTRA_LEFT_TRIG_PIN  (8)
#define ULTRA_LEFT_ECHO_PIN  (19) /* PD2 = INT2 */

/* Right Facing UltraSonic */
#define ULTRA_RIGHT_TRIG_PIN (9)
#define ULTRA_RIGHT_ECHO_PIN (2) /* PE4 = INT4 */

/* Enumerations */
enum ultraSonicPins {
  TRIGGER_PIN,
  ECHO_PIN,
  NUM_ULTRA_PINS
};

/* Front Facing UltraSonic Sensor Pin Mappings */
static const uint8_t frontUltraSonic[NUM_ULTRA_PINS] = {ULTRA_FRONT_TRIG_PIN, ULTRA_FRONT_ECHO_PIN};

/* Left Facing UltraSonic Sensor Pin Mappings */
static const uint8_t leftUltraSonic[NUM_ULTRA_PINS] = {ULTRA_LEFT_TRIG_PIN, ULTRA_LEFT_ECHO_PIN};

/* Right Facing UltraSonic Sensor Pin Mappings */
static const uint8_t rightUltraSonic[NUM_ULTRA_PINS] = {ULTRA_RIGHT_TRIG_PIN, ULTRA_RIGHT_ECHO_PIN};


class UltraSonicSensor {
  public:
    /* Constructor */
    UltraSonicSensor(const uint8_t * inPinMap);

    /* Deconstructor */
    ~UltraSonicSensor();

    /* Initialize Echo Interrupt */
    void initEchoInterrupt();

    /* Get the Trigger Pin */
    uint8_t getTriggerPin();

    /* Get the Echo Pin */
    uint8_t getEchoPin();

    /* Configure External Interrupt Control Register */
    void configureExtIntCtrlRegister(uint8_t * ptrRegister);

  private:
    /* Sensor Pin Numbers */
    const uint8_t * pinMap;

    /* Trigger Pin set high at this time in us */
    uint32_t triggerStartTime;

    /* External Interrupt Number for this UltraSonic Sensor */
    uint8_t extInterruptNumber;
};

#endif // _ULTRASONICSENSOR_H
