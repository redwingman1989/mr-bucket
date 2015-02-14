#include <Wire.h>
#include <math.h>
#include "Magnetometer.h"

/* 
  Function: Magnetometer(uint8_t)
  
  Description: Magnetometer constructor. Builds instance of Magnetometer and 
    sets I2C address of instance
 
  Arguments:
    "a" - Magnetometer instance address (should be 0x1E per data sheet) 
*/
Magnetometer::Magnetometer(uint8_t a) {
  addr = a;
};



uint16_t Magnetometer::calcHeading() {
  double result, x, y, delta;
  
  /* update sensor data */
  updateXYZ();
  
  x = (double)data_x;
  y = (double)data_y;
  
  if(y > 0) 
    filter[index] = 90 - (atan(x/y) * (180/PI));
  else if(y < 0) 
    filter[index] = 270 - (atan(x/y) * (180/PI));
  else {
    if (x < 0)
      filter[index] = 180;
    else
      filter[index] = 0;
  };
  
  rawHeading = filter[index];
  
  /* Address corner case when transitioning from 0 to 359 deg and vice versa */
  if (index > 0) {
    delta = filter[index] - filter[index-1];
  }
  else {
    delta = filter[index] - filter[MAG_FILTER_SIZE - 1];
  }
  
  if(abs(delta) > 180) {
    for(uint8_t i = 0; i < MAG_FILTER_SIZE; i++)
      filter[i] = rawHeading;
  }
  
  
  if(++index == MAG_FILTER_SIZE)
    index = 0;
  
  for(uint8_t i = 0; i < MAG_FILTER_SIZE; i++) {
    result += filter[i];
  };
  
  result /= MAG_FILTER_SIZE;
    
  filteredHeading = (uint16_t) result;

};

void Magnetometer::readMagnetometer(uint8_t startReg,uint8_t dataSize) {
  /* Set the register address to the start of the data registers */
  Wire.beginTransmission(addr);
  Wire.write(startReg);
  Wire.endTransmission();  
  
  /* Request all Data bytes from magnetometer */
  Wire.requestFrom(addr, dataSize);
  
  /* store the received bytes in in the magData structure */
  if(dataSize <= Wire.available()) {
	for (int i=0; i<dataSize; i++) {
		data[(startReg+i)] = Wire.read();
	}
  };
};

void Magnetometer::updateXYZ(void) {
  readMagnetometer(MR_DATA_START_REG, 6); 
  
  data_x = x_msb;
  data_x = (data_x << 8) | x_lsb;
  data_y = y_msb;
  data_y = (data_y << 8) | y_lsb;
  data_z = z_msb;
  data_z = (data_z << 8) | z_lsb;
};

void Magnetometer::init() {
  Wire.begin();
  cfg_A.data_rate   = DR_75_HZ;
  cfg_A.avg         = SA_8;
  cfg_B.gain        = G_0_88_GA;
  mode.mode_sel     = MM_CMM;
  sendByte(MR_CFG_REG_A, data[MR_CFG_REG_A]);
  sendByte(MR_CFG_REG_B, data[MR_CFG_REG_B]);
  sendByte(MR_MODE_REG, data[MR_MODE_REG]);
};


/* Macro Definitions */
void Magnetometer::sendByte(uint8_t reg,uint8_t data) { 
  Wire.beginTransmission(addr); 
  Wire.write(reg); 
  Wire.write(data); 
  Wire.endTransmission(); 
};

uint16_t Magnetometer::getFiltHead() {
  return filteredHeading;
};

uint16_t Magnetometer::getRawHead() {
  return rawHeading;
};
