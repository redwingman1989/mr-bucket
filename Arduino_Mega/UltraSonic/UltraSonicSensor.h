#ifndef _ULTRASONICSENSOR_H
#define _ULTRASONICSENSOR_H

/* Macros */

/* Front Facing UltraSonic */
#define ULTRA_FRONT_TRIG_PIN (7)
#define ULTRA_FRONT_ECHO_PIN (18)

/* Left Facing UltraSonic */
#define ULTRA_LEFT_TRIG_PIN  (8)
#define ULTRA_LEFT_ECHO_PIN  (19)

/* Right Facing UltraSonic */
#define ULTRA_RIGHT_TRIG_PIN (9)
#define ULTRA_RIGHT_ECHO_PIN (2)

class UltraSonicSensor {
  public:
    /* Constructor */
    UltraSonicSensor(uint8_t triggerPin,
                     uint8_t echoPin);

    /* Deconstructor */
    ~UltraSonicSensor();

  private:
    /* Trigger Pin - Triggers Pulse */
    uint8_t pinTrigger;

    /* Echo Pin - Return Echo to Sensor */
    uint8_t pinEcho;

    /* Trigger Pin set high at this time in us */
    uint32_t triggerStartTime;
};

#endif // ULTRASONIC_H_INCLUDED
