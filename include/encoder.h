#ifndef ENCODER_H
#define ENCODER_H

void encodeMessage(char* containerData, int sampleSize, int bitsPerByte, char* message, unsigned int messageLength);

void encodeBitInByte(unsigned char* byte, int bit, int positionInByte);

char* readInMessageData(char* pMessageData, char* pFilepath);

char* readInContainerData(char* pContainerData, char* pFilepath, int subChunk2Size);

#endif
