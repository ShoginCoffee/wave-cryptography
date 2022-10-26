#ifndef ENCODER_H
#define ENCODER_H

void encodeMessage(char* data, unsigned int dataLength, int sampleSize, int bitsPerByte, char* message, unsigned int messageLength);

void encodeBitInByte(unsigned char* byte, int bit, int positionInByte);

char* pFileData(char* pFilepath);

#endif
