#ifndef HIDDENHEADER_H
#define HIDDENHEADER_H
#include <stdio.h>

struct HiddenHeader {
	unsigned int chunkSize;
	unsigned int subChunk1Size;             // Size of metadata subChunk, The length of the header in bytes
	unsigned int fileNameLength;            // Length of filename in bytes
	char fileName[244];                     // (without fileExtension) FilePath + FileName + FileExtension = 247 max. 247 - "C:\" = 244
	char fileExtension[10];                 // File extension name stored as string
	// ska vi ha encryption method? om den är krypterad är det lite svårt att veta...
	// (vi kan ha det i första första början av filen okrypterad, sen kommer allt annat)
	unsigned char encryptionMethod;         // Type of encryption. 0 = no encryption, 1 = CHACHA20
	unsigned int  subChunk2Size;            // The length of the data encrypted in the wave file (excluding the header) in bytes
};

struct HiddenHeader hiddenHeaderToStruct(char* pFilepath, char filePath, int filePathLength);
#endif
