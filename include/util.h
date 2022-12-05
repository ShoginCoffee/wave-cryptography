#ifndef UTIL_H
#define UTIL_H

#include <stdint.h>

int fileLength(char* pFilepath, uint32_t* pFileLength);

int listDirectoryContents(char* sDir);

uint16_t charLEToShortBE(unsigned char charArray[2]);

uint32_t charLEToIntBE(unsigned char charArray[4]);

void shortBEToCharLE(uint16_t num, unsigned char* destinationCharLE);

void intBEToCharLE(uint32_t num, unsigned char* destinationCharLE);
#endif
