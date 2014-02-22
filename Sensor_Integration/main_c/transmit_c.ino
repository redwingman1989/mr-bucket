
void packetizeMessage(void) {
   message[DATA_0] = Robot.xCoordinate;
   message[DATA_1] = Robot.Ultra_X[LEFT];
   message[DATA_2] = Robot.Ultra_X[RIGHT]; 
}


void transmitMessage(void) {
   Serial.write(message, MESSAGE_LEN);
}
