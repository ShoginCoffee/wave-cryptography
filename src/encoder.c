#include "encoder.h"
#include "containerHeader.h"
#include "util.h"
#include "constants.h"

#include <stdio.h>
#include <errno.h>
#include <string.h>

void encodeMessage(char* containerData, int containerDataLength, int sampleSize, int bitsPerSample, char* message, unsigned int messageLength) {
	// SIDENOTE: DEBUG PRINTF ARE COMMENTED OUT BUT ARE STILL IN THE FUNCTION, REMOVE WHEN NECESSARY.
    int i, t;
    int messageByte = 0, messageBit = 0;
    unsigned char bit;
    // loop all samples
    for(i = sampleSize - 1; i < containerDataLength; i += sampleSize){
        // for every sample
        //printf("sample %d: \n\n", i);
        if(messageLength != messageByte){
            for(t = 0; t < bitsPerSample; t++){
                                //printf("changing bit %d: \n\n", t);
                // 1. take bit from message byte
                bit = (( *(message + messageByte) << messageBit) & 0xff) >> 7;
                                //printf("bit to encode: %d \n\n", bit);
                // 2. encode bit in data
                encodeBitInByte((containerData + i), bit, t);
                // 3. update messageData counters
                if(messageBit + 1 == 8) messageByte++;
                messageBit = (messageBit + 1) % 8;
                // 4. if message has been encoded: break
                if(messageByte == messageLength) break;
            }
        }
    }
}

void encodeBitInByte(unsigned char* byte, int bit, int positionInByte) {
	// 1. set positionInByte bit in "byte" to 0
	// 2. make new byte with all zeros except bit in positionInByte is set to "bit" value
	// 3. bitwise or the two bytes
	unsigned char byteCopy = *byte;
	*(byte) = (byteCopy & ~(1 << positionInByte)) | (bit << positionInByte);
}

//Takes in the filepath and returns a pointer to a char array with the data
int readInMessageData(char* pMessageData, char* pFilepath) {
	errno = 0;
	FILE* pMessageFile = fopen(pFilepath, "rb");

	if (pMessageFile == NULL) {
		printf("error in readInMessageData(): \n");
		printf("Could not access file in filepath[%s]: \n%s \n", pFilepath, strerror(errno));
		return 1;
	}

	uint32_t messageLength;
	int output = fileLength(pFilepath, &messageLength);
	if (output != 0) {
		printf("error in readInMessageData() \n"); // REMOVE BEFORE RELEASE
		return 1;
	}
	
	/*
	printf((messageLength == 0) ? "\nmessageLength is null" : "\nmessageLength is not null");
	printf((pMessageData == 0) ? "\npMessageData is null" : "\npMessageData is not null");
	printf((pMessageFile == NULL) ? "\npMessageFile is null" : "\npMessageFile is not null");
	*/

	fread(pMessageData, messageLength, 1, pMessageFile);

	// printf((pMessageFile == NULL) ? "\npMessageFile is null" : "\npMessageFile is not null");

	fclose(pMessageFile);
	return 0;
}

int readInContainerData(char* pContainerData, char* pFilepath, uint32_t subChunk2Size) {
	errno = 0;
	FILE* pFile = fopen(pFilepath, "rb");

	if (pFile == NULL) {
		printf("error in readInContainerData(): \n");
		printf("Could not access file in filepath[%s]: \n%s \n", pFilepath, strerror(errno));
		return 1;
	}

	char buffer[WAV_HEADER_SIZE];
	fread(buffer, sizeof(buffer), 1, pFile); // Moving the "cursor" to where the data starts

	fread(pContainerData, subChunk2Size, 1, pFile);

	fclose(pFile);
	return 0;
}

