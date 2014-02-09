#include "uart.h"

#define MAX_LEN (21)
#define DATA_LEN (16)

extern int start_time;

void serialEvent() {
 iFlags.uartRx = 1; 
}

void processMessage () {
  int bytesWaiting = 0;
  int bytesReadIntoBuf = 0;
  int bytesInBuf = 0;
  start_time = millis();
  
  bytesWaiting = Serial.available();
  if (bytesWaiting > 0) {
    Serial.println("Bytes Were Waiting.\n");
    bytesReadIntoBuf = Serial.readBytesUntil('\0', serialBuf, bytesWaiting);
    bytesInBuf = bytesReadIntoBuf;
  }
  if (bytesReadIntoBuf >= MAX_LEN) {
    parsePacketInBuf(bytesInBuf);
    
  }
  else {
    /* Currently do not have the entire packet, reset flag to re-check later */
    iFlags.uartRx = 1;
    return;
  }
}

void parsePacketInBuf(int bytesInBuf) {
  /* Local Variables */
  int i       = 0;
  static int parserState = SYNC_CHAR_1;
  static int msgId   = 0;
  static int dataLen = 0;
  static int crc     = 0;
  static char dataBuf[DATA_LEN]
  
  /* "Parse" all data in the buffer */
  while (i < bytesInBuf) {
    switch(parserState) {
      case PARSE_SYNC_1:
      case PARSE_SYNC_2:
        if (serialBuf[i] == 0xAA)
          /* Increment Parser to next state, be it SYNC_CHAR_2 or MESSAGE_ID */
          parserState++;
        else
          parserState = SYNC_CHAR_1;
        break;
       case PARSE_MSG_ID:
         /* Set the Message ID to whatever variable we want... */
         msgId = serialBuf[i];
         parserState = DATA_LEN;
         break;
       case PARSE_DATA_LEN:
         dataLen = serialBuf[i];
         parserState = PARSE_DATA;
         break;
       case PARSE_DATA:
         for (int j = 0; j < dataLen; j++, i++) {
           dataBuf[j] = serialBuf[i];
           Serial.println(dataBuf, dataLen);
         }
         parserState = PARSE_CRC;
         break;
       case PARSE_CRC:
         /* Verify CRC was good */
         crc = serialBuf[i];
         if (calcCRC((serialBuf + PARSE_MSG_ID), dataLen) == 0) {
           /* Data is good, distribute the data */
           distributeData();
         }
         parserState = PARSE_SYNC_1;
    }
    /* Increment buf iterator, used in while loop */
    i++;
  }
}

void distributeData() {
  Serial.println("Distributing Data");
  return;
}
