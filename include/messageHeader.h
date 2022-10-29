#ifndef MESSAGEHEADER_H
#define MESSAGEHEADER_H
#include <stdio.h>

struct MessageHeader {
	unsigned char id;						// identifies that the file is encrypted with crypteo
	unsigned int chunkSize;					// The size of the entire message - 5 bytes (id & chunkSize)
	unsigned int subChunk1Size;             // Size of metadata subChunk, The length of the header in bytes
	unsigned int filenameLength;            // Length of filename in bytes
	char filename[244];                     // (without fileExtension) FilePath + FileName + FileExtension = 247 max. 247 - "C:\" = 244
	unsigned int fileExtensionLength;		// Length of extension name in bytes
	char fileExtension[10];                 // File extension name stored as string
	// ska vi ha encryption method? om den �r krypterad �r det lite sv�rt att veta...
	// (vi kan ha det i f�rsta f�rsta b�rjan av filen okrypterad, sen kommer allt annat)
	unsigned char encryptionMethod;         // Type of encryption. 0 = no encryption, 1 = CHACHA20
	unsigned int  subChunk2Size;            // The length of the data encrypted in the wave file (excluding the header) in bytes
};

struct MessageHeader createMessageHeaderStruct(char* pMessageFilepath, char* pMessageFilename, unsigned int messageFilenameLength);
#endif
