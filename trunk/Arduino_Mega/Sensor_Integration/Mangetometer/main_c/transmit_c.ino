
void updateMessageBuf(uint8_t side) {
  int * temp;
  temp = (int *)&message[side][DATA_0];
  *temp = (int)(UltraTime[side]/14.8);
  
  message[side][CRC_IDX] = calcCRC(message[side],CRC_IDX);
}

void transmitMessage(uint8_t side) {
   Serial.write(message[side], MESSAGE_LEN);
}
