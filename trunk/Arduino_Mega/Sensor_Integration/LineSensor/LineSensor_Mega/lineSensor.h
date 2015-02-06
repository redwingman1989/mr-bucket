#ifndef _LINESENSOR_H
#define _LINESENSOR_H

#define PORTA_PIN_0 (22)
#define PORTB_PIN_0 (53)
#define PORTC_PIN_0 (37)
#define PORTL_PIN_0 (49)

typedef union readings_t {
   unsigned char allReadings;
     struct {
       unsigned char sensor1 : 1;
       unsigned char sensor2 : 1;
       unsigned char sensor3 : 1;
       unsigned char sensor4 : 1;
       unsigned char sensor5 : 1;
       unsigned char sensor6 : 1;
       unsigned char sensor7 : 1;
       unsigned char sensor8 : 1;
     } individualReadings;
 } readings;

class LineSensor {
  public:
    //Constructor
    LineSensor(unsigned char pin1, 
               unsigned char pin2,
               unsigned char pin3,
               unsigned char pin4,
               unsigned char pin5,
               unsigned char pin6,
               unsigned char pin7,
               unsigned char pin8); 
               
    
    // Methods
    void beginCheck();
    
  private:
    // Pin Numbers
    unsigned char pinSen1;
    unsigned char pinSen2;
    unsigned char pinSen3;
    unsigned char pinSen4;
    unsigned char pinSen5;
    unsigned char pinSen6;
    unsigned char pinSen7;
    unsigned char pinSen8;
    // Readings from the Line Sensor
    readings sensorReadings;
    // Start Cap Charge Time;
    unsigned short startCapChargeTime;
    
};


#endif
