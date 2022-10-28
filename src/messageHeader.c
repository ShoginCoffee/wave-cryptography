#include "messageHeader.h"
#include <stdio.h>
#include "util.h"

struct MessageHeader MessageHeaderToStruct(char* pFilepath, char filePath, int filePathLength) {
	struct MessageHeader messageHeader;

	char fileName[] = "demo";
	char fileExtension[] = "txt";

	messageHeader.subChunk1Size = 267;

	messageHeader.subChunk2Size = fileLength(pFilepath);

	messageHeader.chunkSize = messageHeader.subChunk1Size + messageHeader.subChunk2Size;

	messageHeader.fileNameLength = sizeof(fileName);

	memcpy(&messageHeader.fileName, &fileName, sizeof(fileName));

	memcpy(&messageHeader.fileExtension, &fileExtension, sizeof(fileExtension));

	messageHeader.encryptionMethod = 1;

	return messageHeader;
}

