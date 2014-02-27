
void packetizeMessage(void) {
   message[DATA_0] = Robot.xCoordinate;
   message[DATA_1] = Robot.Ultra_X[LEFT];
   message[DATA_2] = Robot.Ultra_X[RIGHT]; 
   message[DATA_3] = (uint8_t)loadedBall.redValue;
   message[DATA_4] = (uint8_t)loadedBall.greenValue;
   message[DATA_5] = (uint8_t)loadedBall.blueValue;
   message[DATA_6] = maxLoopTime;
}


void transmitMessage(void) {
   Serial.write(message, MESSAGE_LEN);
}
