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


typedef struct packet {
  const unsigned int sync1   = SYNC_CHAR;
  const unsigned int sync2   = SYNC_CHAR;
  unsigned int pktId;
  const unsigned int dataLen = LEN_DATA;
  char    data[LEN_DATA];
  unsigned int crc;
} 
packet_t;

packet_t * irLSenPkt;
packet_t * irRSenPkt;
packet_t * ultSonLSenPkt;
packet_t * ultSonRSenPkt;

enum {
  SENSR_IR_01,
  NUM_PKT_IDS
};

void setup() {
  Serial.begin(9600);
  crcInit();
  /* Initialize packet: MIGHT NOT BE NEEDED IF STRUCT DEFINITION CAN SET CONST UINTs */
  //initPackets();
}

void loop() {
  while(Serial.available()) {
    char reading = Serial.read();
    if (reading == 'o') {
      execSendPacket();
    }
    else if (reading == 'x') {
      execSendBadPacket();
    }
  }
}

/*
void initPackets() {
  irLSenPkt->sync1 = irRSenPkt->sync1 = ultSonLSenPkt->sync1 = ultSonRSenPkt->sync1 =
  irLSenPkt->sync2 = irRSenPkt->sync2 = ultSonLSenPkt->sync2 = ultSonRSenPkt->sync2 = SYNC_CHAR;
  irLSenPkt->dataLen = irRSenPkt->dataLen = ultSonLSenPkt->dataLen = ultSonRSenPkt->dataLen = LEN_DATA;
}
*/

void execSendPacket() {

  char pktArr[TOT_PKT_LEN];

  /*Build a packet */
  buildPacket(pktArr);

  /*Send packet out serial port */
  for (int i = 0; i < TOT_PKT_LEN; i++) {
    Serial.write(pktArr[i]);
  }
}

void buildPacket(char * pktArr) {

  //char msgData[LEN_DATA];

  //sprintf(msgData, "I am BATMAN!!!!\0");
  /* 
   pkt->sync1 = SYNC_CHAR;
   pkt->sync2 = SYNC_CHAR;
   pkt->pktId = HI_MATT;
   pkt->dataLen = LEN_DATA;
   pkt->data    = msgData;
   pkt->crc     = calcCRC(msgData, LEN_DATA);
   */
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

  //char msgData[LEN_DATA];

  //sprintf(msgData, "Hi Matt!\0");
  /* 
   pkt->sync1 = SYNC_CHAR;
   pkt->sync2 = SYNC_CHAR;
   pkt->pktId = HI_MATT;
   pkt->dataLen = LEN_DATA;
   pkt->data    = msgData;
   pkt->crc     = calcCRC(msgData, LEN_DATA);
   */
  pktArr[SYNC_CHAR_1_ARR_LOC] = SYNC_CHAR;
  pktArr[SYNC_CHAR_2_ARR_LOC] = SYNC_CHAR;
  pktArr[PKT_ID_ARR_LOC] = SENSR_IR_01;
  pktArr[DATA_LEN_ARR_LOC] = LEN_DATA;
  sprintf(pktArr+4, "I am BATMAN!!!!\0");
  uint8_t crc = calcCRC((uint8_t *) pktArr, (int)(TOT_PKT_LEN-1));
  pktArr[CRC_ARR_LOC] = crc+1;
  //Serial.print(crc, HEX);

}

