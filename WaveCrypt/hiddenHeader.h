#ifndef HIDDENHEADER_H
#define HIDDENHEADER_H
#include <stdio.h>

struct HiddenHeader {
    unsigned int chunkSize;
    unsigned int subchunk1Size;           // The length of the header in bytes
    unsigned int fileNameLength;
    char fileName[244];                     // FilePath + FileName + FileExtension = 247. 247 - "C:\" = 244
    char fileExtension[6];
    unsigned char encryptionMethod;
    unsigned int  subchunk2Size;           // The length of the data encrypted in the wave file (excluding the header) in bytes
};

char * pReadInHiddenFile(FILE* pHiddenFile, char fileName);

struct HiddenHeader hiddenHeaderToStruct(char * pFilePath);
#endif
