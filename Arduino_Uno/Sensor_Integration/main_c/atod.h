#ifndef _ATOD_H
#define _ATOD_H

enum analogSignals {
  ANALOG_RED,        //A0
  ANALOG_GREEN,      //A1
  ANALOG_BLUE,       //A2
  ANALOG_SPARE,      //A3
  NUM_ANALOGS        //A4 and A5 are reserved for I2C Bus (A4 = SDA, A5 = SCL)
};

#define RED_SCALER     (1)
#define GREEN_SCALER   (1.25)
#define BLUE_SCALER    (1.75)

#endif
