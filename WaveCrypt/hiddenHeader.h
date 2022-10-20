#ifndef HIDDENHEADER_H
#define HIDDENHEADER_H
#include <stdio.h>

struct HiddenHeader {
    unsigned int chunkSize;
    unsigned int subChunk1Size;           // The length of the header in bytes
    unsigned int fileNameLength;
    char fileName[244];                     // FilePath + FileName + FileExtension = 247. 247 - "C:\" = 244
    char fileExtension[6];
    unsigned char encryptionMethod;        // 0 = no encryption, 1 = CHACHA20
    unsigned int  subChunk2Size;           // The length of the data encrypted in the wave file (excluding the header) in bytes
};

char * pReadInHiddenFile(FILE* pHiddenFile, char fileName);

struct HiddenHeader hiddenHeaderToStruct(char *pFilePath, int filePathLength, char *dataArray[], int dataArrayLength);
#endif
