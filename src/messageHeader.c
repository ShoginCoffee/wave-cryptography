#include "messageHeader.h"
#include <stdio.h>
#include "util.h"
#include <string.h>

struct MessageHeader createMessageHeaderStruct(char* pMessageFilepath, char* pMessageFilename, unsigned int messageFilenameLength) {
	struct MessageHeader header;
 
	// Finding FileExtension and its length
 	char* pMessageFileExtension = strrchr(pMessageFilename, 46) + 1;
	unsigned int messageFileExtensionLength = messageFilenameLength - (pMessageFileExtension - pMessageFilename);

	header.id = 119;

	header.subChunk1Size = 18 + (messageFilenameLength - messageFileExtensionLength) + messageFileExtensionLength; 

	header.filenameLength = messageFilenameLength - messageFileExtensionLength - 1;

	memcpy(&header.filename, pMessageFilename, (messageFilenameLength - messageFileExtensionLength - 1));

	header.fileExtensionLength = messageFileExtensionLength;

	memcpy(&header.fileExtension, pMessageFileExtension, messageFileExtensionLength);

	header.encryptionMethod = 1;

	header.subChunk2Size = fileLength(pMessageFilepath);

	header.chunkSize = header.subChunk1Size + header.subChunk2Size - 5;

	printf("id: %c\n", header.id);
	printf("chunkSize: %d\n", header.chunkSize);
	printf("subChunk1Size: %d\n", header.subChunk1Size);
	printf("filenameLength: %d\n", header.filenameLength);
	printf("filename: %s\n", header.filename);
	printf("fileExtensionLength: %d\n", header.fileExtensionLength);
	printf("fileExtension: %s\n", header.fileExtension);
	printf("encryptionMethod: %d\n", header.encryptionMethod);
	printf("subChunk2Size: %d\n", header.subChunk2Size);

	printf("\n");

	return header;
}

