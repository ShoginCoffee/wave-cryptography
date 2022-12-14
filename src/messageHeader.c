#include "messageHeader.h"
#include "util.h"

#include <stdio.h>
#include <string.h>
#include <errno.h>

int createMessageHeaderStruct(struct MessageHeader* pMessageHeader, char* pMessageFilepath, unsigned int messageFilepathLength, unsigned char encryptionMethod, unsigned char bitsPerSample) {
	uint32_t subChunk2Size;
	int output = fileLength(pMessageFilepath, &subChunk2Size);

	if (output != 0) {
		printf("error in createMessageHeaderStruct() \n"); // REMOVE BEFORE RELEASE
		return 1;
	}
	
	// Find message file name and its length
	char* pFileName = strrchr(pMessageFilepath, '/') + 1;
	unsigned int filenameAndExtensionLength = messageFilepathLength - (pFileName - pMessageFilepath);

	// Finding FileExtension and its length
	char* pMessageFileExtension = strrchr(pFileName, '.') + 1;
	unsigned char fileExtensionLength = filenameAndExtensionLength - (pMessageFileExtension - pFileName);

	// Size of the static part of MESSAGE after subChunk1Size
	const unsigned int MESSAGE_HEADER_SUBCHUNK1_STATIC_SIZE = sizeof(pMessageHeader->filenameLength) + sizeof(pMessageHeader->fileExtensionLength);
	// filenameLength and metadata/subchunk1 size in message header
	uint16_t filenameLength = filenameAndExtensionLength - fileExtensionLength - 1; // the 1 because of the . before filetype
	uint16_t subChunk1Size = MESSAGE_HEADER_SUBCHUNK1_STATIC_SIZE + filenameLength + fileExtensionLength;
	
	pMessageHeader->subChunk1Size = subChunk1Size;
	pMessageHeader->subChunk2Size = subChunk2Size;
	
	pMessageHeader->filenameLength = filenameLength;
	memcpy(pMessageHeader->filename, pFileName, filenameLength);


	pMessageHeader->fileExtensionLength = fileExtensionLength;
	memcpy(pMessageHeader->fileExtension, pMessageFileExtension, fileExtensionLength);

	pMessageHeader->bitsPerSample = bitsPerSample;
	pMessageHeader->encryptionMethod = encryptionMethod;

	const unsigned int SUBCHUNK_SIZES = sizeof(pMessageHeader->subChunk1Size) + sizeof(pMessageHeader->subChunk2Size);
	pMessageHeader->chunkSize = pMessageHeader->subChunk1Size + pMessageHeader->subChunk2Size + SUBCHUNK_SIZES;

	return 0;
}

int printMessageHeaderStruct(struct MessageHeader* pMessageHeader) {
	printf("encryptionMethod: %u\n", pMessageHeader->encryptionMethod);	// unsigned char
	printf("chunkSize: %u\n", pMessageHeader->chunkSize);				// uint32_t
	printf("subChunk1Size: %u\n", pMessageHeader->subChunk1Size);		// uint16_t
	printf("filenameLength: %u\n", pMessageHeader->filenameLength);		// uint16_t
	
	unsigned int filenameLength = pMessageHeader->filenameLength;
	printf("filename: ");
	for (int i = 0; i < filenameLength; i++) {
		printf("%c", *(pMessageHeader->filename + i));					// char[MAX_PATH_LENGTH]
	}
	printf("\n");

	printf("fileExtensionLength: %u\n", pMessageHeader->fileExtensionLength);// unsigned char

	unsigned int fileExtensionLength = pMessageHeader->fileExtensionLength;
	printf("fileExtension: ");
	for (int i = 0; i < fileExtensionLength; i++) {
		printf("%c", *(pMessageHeader->fileExtension + i));				// char[MAX_EXTENSION_LENGTH]
	}
	printf("\n");

	printf("bitsPerSample: %u\n", pMessageHeader->bitsPerSample);		// unsigned char

	printf("subChunk2Size: %u\n", pMessageHeader->subChunk2Size);		// uint32_t

	printf("\n");

	return 0;
}

void messageStructToArr(char* destinationArr, struct MessageHeader* pMessageHeader) {
	int bytesWritten = 0;
	destinationArr[bytesWritten] = pMessageHeader->encryptionMethod;	// unsigned char
	bytesWritten += 1;
	*((uint32_t*)(destinationArr + bytesWritten)) = pMessageHeader->chunkSize;	// uint32_t
	bytesWritten += 4;
	*((uint16_t*)(destinationArr + bytesWritten)) = pMessageHeader->subChunk1Size;	// uint16_t
	bytesWritten += 2;
	*((uint32_t*)(destinationArr + bytesWritten)) = pMessageHeader->filenameLength;	// uint16_t
	bytesWritten += 2;
	
	unsigned int filenameLength = pMessageHeader->filenameLength;
	for (int i = 0; i < filenameLength; i++) {
		destinationArr[bytesWritten + i] = pMessageHeader->filename[i];			// char[MAX_PATH_LENGTH]
	}
	bytesWritten += filenameLength;

	destinationArr[bytesWritten] = pMessageHeader->fileExtensionLength;
	bytesWritten += 1;

	unsigned int fileExtensionLength = pMessageHeader->fileExtensionLength;
	for (int i = 0; i < fileExtensionLength; i++) {
		destinationArr[bytesWritten + i] = pMessageHeader->fileExtension[i];	// char[MAX_EXTENSION_LENGTH]
	}
	bytesWritten += fileExtensionLength;

	destinationArr[bytesWritten] = pMessageHeader->bitsPerSample;	// unsigned char
	bytesWritten += 1;
	
	*((uint32_t*)(destinationArr + bytesWritten)) = pMessageHeader->subChunk2Size;	// uint32_t
}
