#ifndef HIDDENDATAHEADERSTRUCT_H
#define HIDDENDATAHEADERSTRUCT_H

char * pReadInHiddenFile(FILE* pHiddenFile, char fileName) {
    fseek(pHiddenFile, 0L, SEEK_END);
    int hiddenFileSize = ftell(pHiddenFile);
    //printf("SIZE: %d\n", hiddenFileSize);
    //waveDataWithHiddenFile = malloc() //Storleken av vår egna heder beror på filnamnets längd? Hur gör vi?

    return 0;
}

struct HIDDENDATAHEADER hiddenDataHeaderToStruct(char * pFilePath) {
    struct HIDDENDATAHEADER hiddenDataHeader;

    char fileName[] = "demo.txt";

    FILE * pDataFile = fopen(pFilePath, "rb");
    fseek(pDataFile, 0, SEEK_END);
    long dataSize = ftell(pDataFile);
    printf("DataSize: %d \n", dataSize);
    memcpy(&hiddenDataHeader.Filename, &fileName, sizeof(fileName));
    hiddenDataHeader.Subchunk1Size = 6 + sizeof(fileName);
    hiddenDataHeader.Subchunk2Size = ftell(pDataFile);

    return hiddenDataHeader;
}

#endif
