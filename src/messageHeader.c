#include "messageHeader.h"
#include <stdio.h>
#include "util.h"
#include <string.h>

// Size of the static part of MESSAGE after subChunk1Size
const size_t MESSAGE_HEADER_STATIC_SIZE = 8;

int createMessageHeaderStruct(struct MessageHeader* pMessageHeader, char* pMessageFilepath, unsigned int messageFilepathLength, unsigned char encryptionMethod) {
	unsigned int subChunk2Size = fileLength(pMessageFilepath);

	if (subChunk2Size == NULL) {
		printf("createMessageHeaderStruct: Message file couldn't be found at given filepath\n");
		return NULL;
	}

	// Find message file name and its length
	char* pFileName = strrchr(pMessageFilepath, '/') + 1;
	unsigned int fileNameAndExtensionLength = messageFilepathLength - (pFileName - pMessageFilepath + 1);
	

	// Finding FileExtension and its length
	char* pMessageFileExtension = strrchr(pFileName, '.') + 1;


	unsigned int fileExtensionLength = fileNameAndExtensionLength - (pMessageFileExtension - pFileName);
	unsigned int fileNameLength = fileNameAndExtensionLength - fileExtensionLength - 1; // the 1 because of the . before filetype
	unsigned int subChunk1Size = MESSAGE_HEADER_STATIC_SIZE + fileNameLength + fileExtensionLength;

	pMessageHeader->subChunk1Size = subChunk1Size;
	pMessageHeader->subChunk2Size = subChunk2Size;
	
	pMessageHeader->filenameLength = fileNameLength;
	memcpy(pMessageHeader->filename, pFileName, fileNameLength);

	pMessageHeader->fileExtensionLength = fileExtensionLength;
	memcpy(pMessageHeader->fileExtension, pMessageFileExtension, fileExtensionLength);

	pMessageHeader->encryptionMethod = encryptionMethod;

	pMessageHeader->chunkSize = pMessageHeader->subChunk1Size + pMessageHeader->subChunk2Size;

	return 0;
}

int printMessageHeaderStruct(struct MessageHeader* pMessageHeader) {
	printf("encryptionMethod: %u\n", pMessageHeader->encryptionMethod); // unsigned char
	printf("chunkSize: %u\n", pMessageHeader->chunkSize); // unsigned int
	printf("subChunk1Size: %u\n", pMessageHeader->subChunk1Size); // unsigned int
	printf("filenameLength: %u\n", pMessageHeader->filenameLength); // unsigned int
	
	unsigned int filenameLength = pMessageHeader->filenameLength;
	printf("filename: "); // string or char[]
	for (int i = 0; i < filenameLength; i++) {
		printf("%c", *(pMessageHeader->filename + i));
	}
	printf("\n");

	printf("fileExtensionLength: %u\n", pMessageHeader->fileExtensionLength); // unsigned int

	unsigned int fileExtensionLength = pMessageHeader->fileExtensionLength;
	printf("fileExtension: "); // string or char[]
	for (int i = 0; i < fileExtensionLength; i++) {
		printf("%c", *(pMessageHeader->fileExtension + i));
	}
	printf("\n");
	
	printf("subChunk2Size: %u\n", pMessageHeader->subChunk2Size); // unsigned int

	printf("\n");

	return 0;
}
