#include <Wire.h>

/* Macro Values */
#define MAGMTR_I2C_ADDR     (0x1E) // Magnetometer 7 Bit I2C Address
#define CFG_REGISTER_A      (0x00) // Address to Magnetometer Config Register A
#define CFG_REGISTER_B      (0x01) // Address to Magnetometer Config Register B
#define MODE_REGISTER       (0x02) // Address to Magnetometer Mode Register
#define DATA_REGISTER_START (0x03) // Address to Magnetometer Output Data
#define NUM_OUT_DATA_BYTES  (0x06) // 6 Data Bytes (2 per axis: 2-X, 2-Z, 2-Y)

/* Function:    setupMagnetometer
 *  Paramters:   void
 *  Returns:     void
 *  Description: Initialize I2C Interface for the Magnetometer.
 */
void setupMagnetometer() {
  Wire.begin();
  Wire.beginTransmission(MAGMTR_I2C_ADDR);
  Wire.write(MODE_REGISTER);
  Wire.write(0x00);
  Wire.endTransmission();
}

void i2cExec() {
  /* Local Variables to hold axis data */
  int x, y, z;
  char outString[100];
  
  /* Init Local Variables to 0 */
  //x = y = z = 0;
  
  /* Move Magnetometer Address Pointer to Sensor Data section */
  Wire.beginTransmission(MAGMTR_I2C_ADDR);
  Wire.write(DATA_REGISTER_START);
  Wire.endTransmission();
  
  /* Read data from each axis, 2 registers per axis*/
  Wire.requestFrom(MAGMTR_I2C_ADDR, NUM_OUT_DATA_BYTES);
  if (NUM_OUT_DATA_BYTES <= Wire.available()) {
    //Serial.println("----- Found data!\n");
    x = Wire.read() << 8; // X MSB
    x |= Wire.read();     // X LSB
    z = Wire.read() << 8; // Z MSB
    z |= Wire.read();     // Z LSB
    y = Wire.read() << 8; // Y MSB
    y |= Wire.read();     // Y LSB
  }
  
  /* Print out data */
  sprintf(outString, "X: %d\nY: %d\nZ: %d\n*********\n", x, y, z);
  Serial.print(outString);
  //Serial.print("X: ");
  //Serial.println(x);
  
  delay(15);
}
