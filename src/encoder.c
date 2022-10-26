#include <stdio.h>
#include "encoder.h"
#include "wavHeader.h"

void encodeMessage(char* data, unsigned int dataLength, int sampleSize, int bitsPerByte, char* message, unsigned int messageLength) {
	// SIDENOTE: DEBUG PRINTF ARE COMMENTED OUT BUT ARE STILL IN THE FUNCTION, REMOVE WHEN NECESSARY.
	int i = sampleSize - 1;
	int t;
	int messageByte = 0, messageBit = 0;
	unsigned char bit;

	// loop over all the bytes of the message
	while (messageByte > messageLength) {

		//Change the bit of the byte to the value from the message
		for (t = 0; t < bitsPerByte; t++) {
			//printf("changing bit %d: \n\n", t);
			// 1. take bit from message byte
			bit = ((*(message + messageByte) << messageBit) & 0xff) >> 7;
			//printf("bit to encode: %d \n\n", bit);
			// 2. encode bit in data
			encodeBitInByte((data + i), bit, t);
			// 3. update messageData counters
			if (messageBit + 1 == 8) messageByte++;
			messageBit = (messageBit + 1) % 8;
			// 4. if message has been encoded: break
			if (messageByte == messageLength) break;
		}

		i += sampleSize;
	}
}

void encodeBitInByte(unsigned char* byte, int bit, int positionInByte) {
	// 1. set positionInByte bit in "byte" to 0
	// 2. make new byte with all zeros except bit in positionInByte is set to "bit" value
	// 3. bitwise or the two bytes
	unsigned char byteCopy = *byte;
	*(byte) = (byteCopy & ~(1 << positionInByte)) | (bit << positionInByte);
}

char* pHiddenData(char* pFilepath) {
	//Takes in the filepath and returns a pointer to an char array of the data

	FILE* pHiddenFile = fopen(pFilepath, "rb");


	fseek(pHiddenFile, 0, SEEK_END);
	char* pHiddenData = (char*)malloc(ftell(pHiddenFile));

	fread(pHiddenData, ftell(pHiddenFile), 1, pHiddenFile); //Is it better to use ftell twice or to store ftell in an integer?

	fclose(pHiddenFile);
	return pHiddenData;
}

char* pWaveData(char* pFilepath, int subChunk2Size) {
	FILE* pWavOriginal = fopen(pFilepath, "rb");

	char* pWavData = (char*)malloc(subChunk2Size);
	char buffer[44];

	fread(buffer, 44, 1, pWavOriginal);
	fread(pWavData, subChunk2Size, 1, pWavOriginal);

	fclose(pWavOriginal);
	return pWavData;
}
