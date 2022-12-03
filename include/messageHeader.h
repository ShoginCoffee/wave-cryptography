#ifndef MESSAGEHEADER_H
#define MESSAGEHEADER_H
#include <stdio.h>

struct MessageHeader {
	unsigned char id;						// identifies that the file is encrypted with crypteo
	unsigned char encryptionMethod;         // Type of encryption. 0 = no encryption, 1 = CHACHA20
	unsigned int chunkSize;					// subChunk1Size + subChunk2Size
	unsigned int subChunk1Size;             // Size of metadata subChunk, The length of the header in bytes - 5 bytes (id & chunkSize)
	unsigned int filenameLength;            // Length of filename in bytes
	char filename[244];                     // (without fileExtension) FilePath + FileName + FileExtension = 247 max. 247 - "C:\" = 244
	unsigned int fileExtensionLength;		// Length of extension name in bytes
	char fileExtension[10];                 // File extension name stored as string
	// ska vi ha encryption method? om den är krypterad är det lite svårt att veta...
	// (vi kan ha det i första första början av filen okrypterad, sen kommer allt annat)
	unsigned int subChunk2Size;            // The length of the data encrypted in the wave file (excluding the header) in bytes
};

int createMessageHeaderStruct(struct MessageHeader* pMessageHeader, char* pMessageFilepath, unsigned int messageFilepathLength, unsigned char encryptionMethod);

int printMessageHeaderStruct(struct MessageHeader* pMessageHeader);
#endif
