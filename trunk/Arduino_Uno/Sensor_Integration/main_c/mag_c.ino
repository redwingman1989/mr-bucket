


uint16_t getHeading() {
  static double filter[MAG_FILTER_SIZE];
  static unsigned int index;
  static uint16_t prev_sHeading;
  uint16_t sHeading;
  double result, x, y;
  
  getMagData();
  
  x = (double)magData.data_x;
  y = (double)magData.data_y;
  
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
  
  index++;
  if(index == MAG_FILTER_SIZE)
    index = 0;
  
  for(int i = 0; i < MAG_FILTER_SIZE; i++) {
    result += filter[i];
  };
  
  result /= MAG_FILTER_SIZE;
  
  //result *= 100;
    
  sHeading = (uint16_t) result;
  
  return sHeading;
}

void getMagData(void) {
  /* Set the register address to the start of the data registers */
  Wire.beginTransmission(MAG_ADDR);
  Wire.write(MR_DATA_START_REG);
  Wire.endTransmission();  
  
  /* Request all Data bytes from magnetometer */
  Wire.requestFrom(MAG_ADDR, MAG_DATA_SIZE);
  
  /* store the received bytes in in the magData structure */
  if(MAG_DATA_SIZE <= Wire.available()) {
    magData.data_x = Wire.read();
    magData.data_x = (magData.data_x << 8) | Wire.read(); 
    magData.data_z = Wire.read();
    magData.data_z = (magData.data_z << 8) | Wire.read(); 
    magData.data_y = Wire.read();
    magData.data_y = (magData.data_y << 8) | Wire.read(); 
  };
}
