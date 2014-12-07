
void packetizeMessage(void) {
   message[DATA_0]  = 0x00; //Robot.xCoordinate;
   message[DATA_1]  = UltraTime[RIGHT]/148;
   message[DATA_2]  = UltraTime[LEFT]/148; 
   message[DATA_3]  = 0x00;
   message[DATA_4]  = 0x00;
   message[DATA_5]  = (uint8_t)loadedBall.redValue;
   message[DATA_6]  = (uint8_t)loadedBall.greenValue;
   message[DATA_7]  = (uint8_t)loadedBall.blueValue;
   message[DATA_8]  = Robot.heading >> 8; //Heading MSB
   message[DATA_9]  = Robot.heading;      //Heading LSB
   message[DATA_10] = maxLoopTime;
}


void transmitMessage(void) {
   Serial.write(message, MESSAGE_LEN);
}
