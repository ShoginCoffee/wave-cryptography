#ifndef ENCODER_H
#define ENCODER_H

void encodeMessage(char* targetData, int sampleSize, int bitsPerByte, char* message, unsigned int messageLength);

void encodeBitInByte(unsigned char* byte, int bit, int positionInByte);

char* readInMessageData(char* pFilepath);

char* readInTargetData(char* pFilepath, int subChunk2Size);

#endif
