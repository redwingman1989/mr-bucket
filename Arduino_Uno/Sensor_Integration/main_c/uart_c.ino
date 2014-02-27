#include "uart.h"

void serialEvent() {
  iFlags.uartRx = 1; 
}


void processMessage_Matt() {
  static int i = 0;
  static int startTime = 0;
  
  if(i==0)
    startTime = millis();
  
  while (Serial.available()) {
    serialBuf[i] = (uint8_t)Serial.read();
    
    switch (i) {
    case SYNC_CHAR_1:
    case SYNC_CHAR_2:
      if (serialBuf[i] != 0xAA)
          i = 0;     
      break;
    default:
      break;
    }
    i++;
    if (i == MESSAGE_LEN)
      break;
  }
  
  if (i == MESSAGE_LEN) {
   // Serial.print("Time Lapsed: ");
   // Serial.print((millis()-startTime),DEC);
   // Serial.println("ms");
    if (calcCRC(serialBuf, MESSAGE_LEN) == 0) {
   //   Serial.println("Valid Packet");
   //   Serial.write(serialBuf, MAX_LEN);
   //   Serial.write('\n');
    }
    else {
   //   Serial.println("Invalid Packet");
   //   Serial.write('\n');
    }
    i = 0;
    //message_length = 0;
  } 
  /* Increment Buffer Index */
}


//void processMessage() {
//  static int i = 0;
//  int bytesWaiting = 0;
//  int bytesReadIntoBuf = 0;
//  static int bytesInBuf = 0;
////  static int startTime = millis();
////  static int message_length = 0;
//  
//  /* Check for data timeout, if timeout expires look for next sync char pair */
///*  if (millis() - startTime > 15) {
//    i = 0;
//  }
//*/
//  bytesWaiting = Serial.available();
//  if (bytesWaiting > 0) {
//    bytesReadIntoBuf = Serial.readBytesUntil('\0', serialBuf, bytesWaiting);
//    bytesInBuf += bytesReadIntoBuf;
//  }
//  if (bytesReadIntoBuf >= MAX_LEN) {
//    parsePacketInBuf(bytesInBuf);
//  else {
//    /* Currently do not have the entire packet, reset flag to re-check later */
//    iFlags.uartRx = 1;
//    return;
//  }
//  
//  
//  
//  
//  
//  
//  
//  
//  while (Serial.available()) {
//    
//    /* Capture time of start char */
//    //if (i==0)
//    //  startTime = millis();
//    /* Read the char that is ready to be ready */
//    serialBuf[i] = (uint8_t)Serial.read();
//  }
//  /*
//  
//  */
//  /*
//  if (i == MAX_LEN-1) {
//    if (calcCRC(serialBuf, MAX_LEN) == 0) {
//      Serial.println("Valid Packet");
//      Serial.write(serialBuf, MAX_LEN);
//    }
//    else {
//      Serial.println("Invalid Packet");
//    }
//    i = 0;
//    */
//    //message_length = 0;
//  } 
//  /* Increment Buffer Index */
////  else
////    i++;
//}
//
//void parsePacketInBuf(int bytesInbuf) {
//  /* Local Variables */
//  static int i = 0;
//  int parserState = SYNC_CHAR_1;
//  int dataLen = 0;
//  /
//  /* "Parse" all data in the buffer */
//  while (i < bytesInBuf) {
//    switch(parserState) {
//      case PARSE_SYNC_1:
//      case PARSE_SYNC_2:
//        if (serialBuf[i] == 0xAA)
//          /* Increment Parser to next state, be it SYNC_CHAR_2 or MESSAGE_ID */
//          parserState++;
//        else
//          parserState = SYNC_CHAR_1;
//        break;
//       case PARSE_MSG_ID:
//         /* Set the Message ID to whatever variable we want... */
//         parserState = DATA_LEN;
//         break;
//       case PARSE_DATA_LEN:
//         dataLen = serialBuf[i];
//         parserState = DATA;
//         break;
//       case PARSE_DATA:
//         dataLen--;
//         if (dataLen == 0)
//           parserState = PARSE_CRC;
//         break;
//       case PARSE_CRC:
//         
//    }
//    /* Increment buf iterator, used in while loop */
//    i++;
//  }
//}
