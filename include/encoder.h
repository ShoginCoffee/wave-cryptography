#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>

void encodeMessage(char* containerData, int dataLength, int sampleSize, int bitsPerByte, char* message, unsigned int messageLength);

void encodeBitInByte(unsigned char* byte, int bit, int positionInByte);

int readInMessageData(char* pMessageData, char* pFilepath);

int readInContainerData(char* pContainerData, char* pFilepath, uint32_t subChunk2Size);

#endif
