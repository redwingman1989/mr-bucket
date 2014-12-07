
void packetizeMessage(void) {
   message[DATA_0]  = 0x00; //Robot.xCoordinate;
   message[DATA_1]  = 0x00; //Robot.Ultra_X[LEFT];
   message[DATA_2]  = 0x00; //Robot.Ultra_X[RIGHT]; 
   message[DATA_3]  = 0x00; //(uint8_t)loadedBall.redValue;
   message[DATA_4]  = magBuffer[MR_; //(uint8_t)loadedBall.greenValue;
   message[DATA_5]  = 0x00; //(uint8_t)loadedBall.blueValue;
   message[DATA_6]  = magData.data_y >> 8;
   message[DATA_7]  = magData.data_y;
   message[DATA_8]  = magData.data_x >> 8;
   message[DATA_9]  = magData.data_x;
   message[DATA_10] = maxLoopTime;
}


void transmitMessage(void) {
   Serial.write(message, MESSAGE_LEN);
}
