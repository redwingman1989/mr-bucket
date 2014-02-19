
void packetizeMessage(void) {
   message[DATA_0] = Robot.Ultra_X[RIGHT];
   message[DATA_1] = Robot.Ultra_X[LEFT]; 
}


void transmitMessage(void) {
   Serial.write(message, MESSAGE_LEN);
}
