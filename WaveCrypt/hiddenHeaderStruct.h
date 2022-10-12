#ifndef HIDDENDATAHEADERSTRUCT_H
#define HIDDENDATAHEADERSTRUCT_H

struct HiddenHeader {
    unsigned int ChunkSize;
    unsigned int Subchunk1Size;           // The length of the header in bytes
    unsigned int FileNameLength;
    char Filename[244];                     // FilePath + FileName + FileExtension = 247. 247 - "C:\" = 244
    char fileExtension[6]
    unsigned char method
    unsigned int  Subchunk2Size;           // The length of the data encrypted in the wave file (excluding the header) in bytes
};

char * pReadInHiddenFile(FILE* pHiddenFile, char fileName);

struct HIDDENDATAHEADER hiddenDataHeaderToStruct(char * pFilePath);
#endif
