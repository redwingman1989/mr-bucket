#ifndef _BUTTONS_H
#define _BUTTONS_H
#include "System\RunableModule.h"

#define MAX_BUTTONS 8
#define LBUTT 14
#define RBUTT 15

enum buttonPosition {
  BP_UP,
  BP_DOWN,
  BP_NUM_BUTT_POS
};

class Button {
  private:
    union {
      struct {
        uint8_t pos        : 1;
        uint8_t prevPos    : 1;
        uint8_t H2LTran    : 1;
        uint8_t L2HTran    : 1;
        uint8_t L2Hcount   : 4;
      };
      uint8_t buttonStatus;
    };

    uint8_t pinAddr;

  public:
    Button();
    bool init(uint8_t);
    void update();
    uint8_t getAddress();
    bool getPos();
    uint8_t getTrans();
    uint8_t getCount();

};

class ButtonManager : public RunableModule {
  private:
    Button   buttons[MAX_BUTTONS];
    uint8_t  numButtons;
    uint8_t  buttPoses;

  public:
    ButtonManager();
    bool init();
    int8_t addButton(uint8_t);
    uint8_t readButtons();
    bool RunTick(uint16_t , RobotState);
    void DebugOutput(HardwareSerial*);
};

#endif
