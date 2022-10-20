#include "hiddenHeader.h"
#include <stdio.h>

char *pReadInHiddenFile(FILE *pHiddenFile, char fileName) {
    fseek(pHiddenFile, 0L, SEEK_END);
    int hiddenFileSize = ftell(pHiddenFile);
    //printf("SIZE: %d\n", hiddenFileSize);
    //waveDataWithHiddenFile = malloc() //Storleken av vår egna heder beror på filnamnets längd? Hur gör vi?

    return 0;
}

struct HiddenHeader hiddenHeaderToStruct(char *pFilePath, char *filePath,  int filePathLength, char *dataArray[], int dataArrayLength) {
    struct HiddenHeader hiddenHeader;

    char fileName[] = "demo";
    char fileExtension[] = "txt";

    FILE *pDataFile = fopen(pFilePath, "rb");
    fseek(pDataFile, 0, SEEK_END);
    long dataSize = ftell(pDataFile);
    printf("DataSize: %d \n", dataSize);

    hiddenHeader.subChunk1Size = 267;

    hiddenHeader.subChunk2Size = dataSize;

    hiddenHeader.chunkSize = hiddenHeader.subChunk1Size + hiddenHeader.subChunk2Size;

    hiddenHeader.fileNameLength = sizeof(fileName);

    memcpy(&hiddenHeader.fileName, &fileName, sizeof(fileName));

    memcpy(&hiddenHeader.fileExtension, &fileExtension, sizeof(fileExtension));

    hiddenHeader.encryptionMethod = 1;

    return hiddenHeader;
}

