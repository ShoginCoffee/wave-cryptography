#include "messageHeader.h"
#include <stdio.h>
#include "util.h"
#include <string.h>

struct MessageHeader* createMessageHeaderStruct(struct MessageHeader* pMessageHeader,char* pMessageFilepath, unsigned int messageFilepathLength) {
	pMessageHeader->subChunk2Size = fileLength(pMessageFilepath);

	if (pMessageHeader->subChunk2Size == NULL) {
		printf("Message file couldn't be found at given filepath");
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

	pMessageHeader->filenameLength = messageFilenameLength - messageFileExtensionLength - 1;

	memcpy(&(pMessageHeader->filename), pMessageFilename, (messageFilenameLength - messageFileExtensionLength - 1));

	pMessageHeader->fileExtensionLength = messageFileExtensionLength;

	memcpy(&(pMessageHeader->fileExtension), pMessageFileExtension, messageFileExtensionLength);

	pMessageHeader->encryptionMethod = 1;

	pMessageHeader->chunkSize = pMessageHeader->subChunk1Size + pMessageHeader->subChunk2Size - 5;

	printf("id: %c\n", pMessageHeader->id);
	printf("chunkSize: %d\n", pMessageHeader->chunkSize);
	printf("subChunk1Size: %d\n", pMessageHeader->subChunk1Size);
	printf("filenameLength: %d\n", pMessageHeader->filenameLength);
	printf("filename: %s\n", pMessageHeader->filename);
	printf("fileExtensionLength: %d\n", pMessageHeader->fileExtensionLength);
	printf("fileExtension: %s\n", pMessageHeader->fileExtension);
	printf("encryptionMethod: %d\n", pMessageHeader->encryptionMethod);
	printf("subChunk2Size: %d\n", pMessageHeader->subChunk2Size);

	printf("\n");

	return pMessageHeader;
}

