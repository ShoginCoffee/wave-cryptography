#ifndef ENCODER_H
#define ENCODER_H

#include <stdint.h>

int encodeMessageInWavFile(int encryptionMethod, char* resultFilepath, char* containerFilepath, char* messageFilepath, int encodingBitsPerSample, uint32_t* key, uint32_t* counter, uint32_t* nonce);

void encodeMessage(char* containerData, int dataLength, int sampleSize, int bitsPerByte, char* message, unsigned int messageLength);

void encodeBitInByte(unsigned char* byte, int bit, int positionInByte);

int readInMessageData(char* pMessageData, char* pFilepath, uint32_t bufferSize, uint32_t offsetBytes);

int readInContainerData(char* pContainerData, char* pFilepath, uint32_t bufferSize, uint32_t offsetBytes);

#endif
