#include "hiddenHeader.h"
#include <stdio.h>

struct HiddenHeader hiddenHeaderToStruct(char* pFilepath, char filePath, int filePathLength) {
	struct HiddenHeader hiddenHeader;

	char fileName[] = "demo";
	char fileExtension[] = "txt";

	FILE* pDataFile = fopen(pFilepath, "rb");
	fseek(pDataFile, 0, SEEK_END);


	hiddenHeader.subChunk1Size = 267;

	hiddenHeader.subChunk2Size = ftell(pDataFile);

	hiddenHeader.chunkSize = hiddenHeader.subChunk1Size + hiddenHeader.subChunk2Size;

	hiddenHeader.fileNameLength = sizeof(fileName);

	memcpy(&hiddenHeader.fileName, &fileName, sizeof(fileName));

	memcpy(&hiddenHeader.fileExtension, &fileExtension, sizeof(fileExtension));

	hiddenHeader.encryptionMethod = 1;

	return hiddenHeader;
}

