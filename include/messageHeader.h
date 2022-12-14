#ifndef MESSAGEHEADER_H
#define MESSAGEHEADER_H

#include "constants.h" // For MAX_PATH_LENGTH and MAX_EXTENSION_LENGTH

#include <stdint.h>

struct MessageHeader {
	unsigned char encryptionMethod;				// Type of encryption. 0 = no encryption, 1 = CHACHA20
	uint32_t chunkSize;							// subChunk1Size + subChunk2Size + 6bytes(subchunk sizes)
	uint16_t subChunk1Size;						// Size of metadata subChunk, filenameLength + filename + fileExtensionLength + fileExtension + bitsPerSample
	uint16_t filenameLength;					// Length of filename in bytes
	char filename[MAX_PATH_LENGTH];				// (without fileExtension)
	unsigned char fileExtensionLength;			// Length of extension name in bytes
	char fileExtension[MAX_EXTENSION_LENGTH];	// File extension name stored as string
	unsigned char bitsPerSample;				// How many bits are encoded per sample (this char is encoded in the last bit of the first 8 samples)
	uint32_t subChunk2Size;						// The length of the data encrypted in the wave file (excluding the header) in bytes
};

int createMessageHeaderStruct(struct MessageHeader* pMessageHeader, char* pMessageFilepath, unsigned int messageFilepathLength, unsigned char encryptionMethod, unsigned char bitsPerSample);

int printMessageHeaderStruct(struct MessageHeader* pMessageHeader);

void messageStructToArr(char* destinationArr, struct MessageHeader* pMessageHeader);
#endif
