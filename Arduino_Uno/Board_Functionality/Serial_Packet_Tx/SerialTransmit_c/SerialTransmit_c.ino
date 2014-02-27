#include <stdio.h>
#include "crc.h"



#define SYNC_CHAR       (0xAA) // Decimal 170
#define LEN_DATA        (0x10) // Decimal 16
#define LEN_HDR_AND_CRC (0x05) // Decimal 5
#define TOT_PKT_LEN     (LEN_DATA + LEN_HDR_AND_CRC) // Decimal 21

#define SYNC_CHAR_1_ARR_LOC (0)
#define SYNC_CHAR_2_ARR_LOC (1)
#define PKT_ID_ARR_LOC      (2)
#define DATA_LEN_ARR_LOC    (4)
#define CRC_ARR_LOC         (TOT_PKT_LEN - 1)

enum pkIds {
  LEFT_IR,
  RIGHT_IR,
  LEFT_ULTRASONIC,
  RIGHT_ULTRASONIC,
  COLOR_SENSOR,
  NUM_PKT_IDS
};

typedef struct packet {
  const unsigned int sync1;
  const unsigned int sync2;
  unsigned int       pktId;
  const unsigned int dataLen;
  char               data[LEN_DATA];
  unsigned int       crc;
} 
packet_t;

packet_t * irLSenPkt;
irLSenPkt.sync1   = SYNC_CHAR;
irLSenPkt.sync2   = SYNC_CHAR;
irLSenPkt.pktId   = LEFT_IR;
irLSenPkt.dataLen = LEN_DATA;

packet_t * irRSenPkt;
irRSenPkt.sync1   = SYNC_CHAR;
irRSenPkt.sync2   = SYNC_CHAR;
irRSenPkt.pktId   = RIGHT_IR;
irRSenPkt.dataLen = LEN_DATA;

packet_t * ultSonLSenPkt;
ultSonLSenPkt.sync1   = SYNC_CHAR;
ultSonLSenPkt.sync2   = SYNC_CHAR;
ultSonLSenPkt.pktId   = LEFT_ULTRASONIC;
ultSonLSenPkt.dataLen = LEN_DATA;

packet_t * ultSonRSenPkt;
ultSonRSenPkt.sync1   = SYNC_CHAR;
ultSonRSenPkt.sync2   = SYNC_CHAR;
ultSonRSenPkt.pktId   = RIGHT_ULLTRASONIC;
ultSonRSenPkt.dataLen = LEN_DATA;

packet_t * colorSenPkt;
colorSenPkt.sync1   = SYNC_CHAR;
colorSenPkt.sync2   = SYNC_CHAR;
colorSenPkt.pktId   = COLOR_SENSOR;
colorSenPkt.dataLen = LEN_DATA;



void setup() {
  Serial.begin(115200);
  crcInit();
}

void loop() {
  /* While there is data to read */
  while(Serial.available()) {
    /* Read in one char and see if it is a value to trigger a message */
    char reading = Serial.read();
    /* If the char is 'o' send a packet with a good CRC */
    if (reading == 'o') {
      execSendPacket();
    }
    /* If the tchar is 'x' send a packet with a bad CRC */
    else if (reading == 'x') {
      execSendBadPacket();
    }
  }
}

void execSendPacket() {
  packet_t * packetToSend = NULL;
  
  while(Serial.available()) {
    char reading = Serial.read();
    switch (reading) {
      case '0':
        packetToSend = irLSenPkt;
        break;
        
      case '1':
        packetToSend = irRSenPkt;
        break;
        
      case '2':
        packetToSend = ultSonLSenPkt;
        break;
        
      case '3':
        packetToSend = ultSonRSenPkt;
        break;
        
      case '4':
        packetToSent = colorSenPkt;
        break;
        
      default:
        Serial.println("Error reading packet specifier.\n");
        break;
    }    
  }

  /*Build a packet */
  buildPacket(packetToSend);

  /*Send packet out serial port */
  for (int i = 0; i < sizeof(packet_t); i++) {
    Serial.write((unsigned char)pktArr[i]);
  }
}

void buildPacket(char * pktArr) {

  pktArr[SYNC_CHAR_1_ARR_LOC] = SYNC_CHAR;
  pktArr[SYNC_CHAR_2_ARR_LOC] = SYNC_CHAR;
  pktArr[PKT_ID_ARR_LOC] = SENSR_IR_01;
  pktArr[DATA_LEN_ARR_LOC] = LEN_DATA;
  sprintf(pktArr+4, "I am BATMAN!!!!\0");
  uint8_t crc = calcCRC((uint8_t *) pktArr, (int)(TOT_PKT_LEN-1));
  pktArr[CRC_ARR_LOC] = crc;
  //Serial.print(crc, HEX);

}

void execSendBadPacket() {

  char pktArr[LEN_DATA+5];

  /*Build a packet */
  buildBadPacket(pktArr);

  /*Send packet out serial port */
  for (int i = 0; i < LEN_DATA+5; i++) {
    Serial.write(pktArr[i]);
  }
}

void buildBadPacket(char * pktArr) {

  sprintf(pktArr+4, "I am BATMAN!!!!\0");
  uint8_t crc = calcCRC((uint8_t *) pktArr, (int)(TOT_PKT_LEN-1));
  pktArr[CRC_ARR_LOC] = crc+1;
  //Serial.print(crc, HEX);

}

