#ifndef MESSAGEHEADER_H
#define MESSAGEHEADER_H

#include "constants.h" // For MAX_PATH_LENGTH and MAX_EXTENSION_LENGTH

#include <stdint.h>

struct MessageHeader {
	unsigned char encryptionMethod;				// Type of encryption. 0 = no encryption, 1 = CHACHA20
	uint32_t chunkSize;							// subChunk1Size + subChunk2Size + 6bytes(subchunk sizes)
	uint16_t subChunk1Size;						// Size of metadata subChunk, filenameLength+filename+fileExtensionLength+fileExtension
	uint16_t filenameLength;					// Length of filename in bytes
	char filename[MAX_PATH_LENGTH];				// (without fileExtension)
	unsigned char fileExtensionLength;			// Length of extension name in bytes
	char fileExtension[MAX_EXTENSION_LENGTH];	// File extension name stored as string
	uint32_t subChunk2Size;						// The length of the data encrypted in the wave file (excluding the header) in bytes
};

int createMessageHeaderStruct(struct MessageHeader* pMessageHeader, char* pMessageFilepath, unsigned int messageFilepathLength, unsigned char encryptionMethod);

int printMessageHeaderStruct(struct MessageHeader* pMessageHeader);
#endif
