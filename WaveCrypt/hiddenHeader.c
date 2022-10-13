#include "hiddenHeader.h"
#ifndef HIDDENHEADER_H
#define HIDDENHEADER_H

char *pReadInHiddenFile(FILE *pHiddenFile, char fileName) {
    fseek(pHiddenFile, 0L, SEEK_END);
    int hiddenFileSize = ftell(pHiddenFile);
    //printf("SIZE: %d\n", hiddenFileSize);
    //waveDataWithHiddenFile = malloc() //Storleken av vår egna heder beror på filnamnets längd? Hur gör vi?

    return 0;
}

struct HiddenHeader hiddenHeaderToStruct(char *pFilePath, int filePathLength, char *dataArray[], int dataArrayLength) {
    struct HiddenHeader hiddenHeader;

    char fileName[] = "demo";
    char fileExtension[] = "txt";

    hiddenHeader.fileNameLength = sizeof(fileName);

    memcpy(&hiddenHeader.Filename, &fileName, sizeof(fileName));

    memcpy(&hiddenHeader.fileExtension, &fileExtension, sizeof(fileExtension));

    hiddenHead



    FILE *pDataFile = fopen(pFilePath, "rb");
    fseek(pDataFile, 0, SEEK_END);
    long dataSize = ftell(pDataFile);
    printf("DataSize: %d \n", dataSize);

    hiddenHeader.Subchunk1Size = 6 + sizeof(fileName);
    hiddenHeader.Subchunk2Size = ftell(pDataFile);

    return hiddenHeader;
}

#endif
