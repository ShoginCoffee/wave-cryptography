#include "messageHeader.h"
#include <stdio.h>
#include "util.h"
#include <string.h>

int createMessageHeaderStruct(struct MessageHeader* pMessageHeader,char* pMessageFilepath, unsigned int messageFilepathLength) {
	pMessageHeader->subChunk2Size = fileLength(pMessageFilepath);

	if (pMessageHeader->subChunk2Size == NULL) {
		printf("Message file couldn't be found at given filepath\n");
		return NULL;
	}

	// Find message file name and its length
	char* pMessageFilename = strrchr(pMessageFilepath, '/') + 1;
	unsigned int messageFilenameLength = messageFilepathLength - (pMessageFilename - pMessageFilepath + 1);
	

	// Finding FileExtension and its length
 	char* pMessageFileExtension = strrchr(pMessageFilename, 46) + 1;
	unsigned int messageFileExtensionLength = messageFilenameLength - (pMessageFileExtension - pMessageFilename);
	

	pMessageHeader->id = 119;

	pMessageHeader->subChunk1Size = 18 + messageFilenameLength + messageFileExtensionLength;

	pMessageHeader->filenameLength = messageFilenameLength - messageFileExtensionLength - 1; // the 1 because of the . before filetype

	memcpy(&(pMessageHeader->filename), pMessageFilename, messageFilenameLength - messageFileExtensionLength - 1);
	//printf("%s", pMessageHeader->filename);

	pMessageHeader->fileExtensionLength = messageFileExtensionLength;

	memcpy(&(pMessageHeader->fileExtension), pMessageFileExtension, messageFileExtensionLength);

	pMessageHeader->encryptionMethod = 1;

	pMessageHeader->chunkSize = pMessageHeader->subChunk1Size + pMessageHeader->subChunk2Size - 5;

	return 0;
}

int printMessageHeaderStruct(struct MessageHeader* pMessageHeader) {
	printf("id: %u\n", pMessageHeader->id); // unsigned char
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
	
	printf("encryptionMethod: %u\n", pMessageHeader->encryptionMethod); // unsigned char
	printf("subChunk2Size: %u\n", pMessageHeader->subChunk2Size); // unsigned int

	printf("\n");

	return 0;
}
