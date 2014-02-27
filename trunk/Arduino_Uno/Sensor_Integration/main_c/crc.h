#ifndef _CRC_H
#define _CRC_H

typedef uint8_t crc;

#define POLYNOMIAL 0xD8  /* 11011000 */
#define WIDTH  (8 * sizeof(crc))
#define TOPBIT (1 << (WIDTH - 1))

crc crcTable[256];

#endif

