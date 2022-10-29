#include <stdio.h>
#include "encoder.h"
#include "wavHeader.h"
#include "util.h"

void encodeMessage(char* targetData, int sampleSize, int bitsPerByte, char* message, unsigned int messageLength) {
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
			encodeBitInByte((targetData + i), bit, t);
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

char* readInMessageData(char* pFilepath) {
	//Takes in the filepath and returns a pointer to an char array of the data

	FILE* pMessageFile = fopen(pFilepath, "rb");
	unsigned int messageLength = fileLength(pFilepath);

	char* pMessageData = (char*)malloc(messageLength);

	printf((messageLength == 0) ? "\nmessageLength is null" : "\nmessageLength is not null");
	printf((pMessageData == 0) ? "\npMessageData is null" : "\npMessageData is not null");
	printf((pMessageFile == NULL) ? "\npMessageFile is null" : "\npMessageFile is not null");

	fread(pMessageData, messageLength, 1, pMessageFile);

	printf((pMessageFile == NULL) ? "\npMessageFile is null" : "\npMessageFile is not null");

	fclose(pMessageFile);
	return pMessageData;
}

char* readInTargetData(char* pFilepath, int subChunk2Size) {
	FILE* pTargetFile = fopen(pFilepath, "rb");

	char* pTargetData = (char*)malloc(subChunk2Size);
	char buffer[44];

	fread(buffer, 44, 1, pTargetFile);
	fread(pTargetData, subChunk2Size, 1, pTargetFile);

	fclose(pTargetFile);
	return pTargetData;
}

