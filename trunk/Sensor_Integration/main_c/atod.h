#ifndef _ATOD_H
#define _ATOD_H

enum analogSignals {
  ANALOG_LEFT_IR,
  ANALOG_RIGHT_IR,
  ANALOG_SPARE,
  ANALOG_RED,
  ANALOG_GREEN,
  ANALOG_BLUE,
  NUM_ANALOGS
};

#define RED_SCALER     (1)
#define GREEN_SCALER   (1.25)
#define BLUE_SCALER    (1.75)

#endif
