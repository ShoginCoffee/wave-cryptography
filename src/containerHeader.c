#include "containerHeader.h"
#include "util.h"

// takes in file path and returns header struct
struct ContainervHeader* createContainerHeaderStruct(struct ContainerHeader* pContainerHeader, char* pContainerFilepath) {
    FILE* pFile = fopen(pContainerFilepath, "rb");

    if (pFile == NULL) {
        printf("createContainerHeaderStruct: Container file couldn't be found at given filepath\n");
        return NULL;
    }

    unsigned char buffer4[4];
    unsigned char buffer2[2];

    // read header into struct
    int bytesRead = 0;

    // ChunkID  4 byte BE
    bytesRead = fread(pContainerHeader->chunkID, sizeof(pContainerHeader->chunkID), 1, pFile);

    // ChunkSize 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1,pFile);
    pContainerHeader->chunkSize = charLEToIntBE(buffer4);

    // Format 4 byte BE
    bytesRead = fread(pContainerHeader->format, sizeof(pContainerHeader->format), 1, pFile);

    // SubChunk1ID 4 byte BE
    bytesRead = fread(pContainerHeader->subChunk1ID, sizeof(pContainerHeader->subChunk1ID), 1, pFile);

    // SubChunk1Size 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pFile);
    pContainerHeader->subChunk1Size = charLEToIntBE(buffer4);

    // AudioFormat 2 byte LE converted to BE
    bytesRead = fread(buffer2, sizeof(buffer2), 1, pFile);
    pContainerHeader->audioFormat = charLEToIntBE(buffer2);

    // NumChannels 2 byte LE converted to BE
    bytesRead = fread(buffer2, sizeof(buffer2), 1, pFile);
    pContainerHeader->numChannels = charLEToIntBE(buffer2);

    // SampleRate 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pFile);
    pContainerHeader->sampleRate = charLEToIntBE(buffer4);

    // ByteRate 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pFile);
    pContainerHeader->byteRate = charLEToIntBE(buffer4);

    // BlockAlign 2 byte LE converted to BE
    bytesRead = fread(buffer2, sizeof(buffer2), 1, pFile);
    pContainerHeader->blockAlign = charLEToIntBE(buffer2);

    // BitsPerSample 2 byte LE converted to BE
    bytesRead = fread(buffer2, sizeof(buffer2), 1, pFile);
    pContainerHeader->bitsPerSample = charLEToIntBE(buffer2);

    // SubChunk2ID 4 byte BE
    bytesRead = fread(pContainerHeader->subChunk2ID, sizeof(pContainerHeader->subChunk2ID), 1, pFile);

    // SubChunk2Size 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pFile);
    pContainerHeader->subChunk2Size = charLEToIntBE(buffer4);

    fclose(pFile);
    return pContainerHeader;
}

// takes in pointer to a containerHeaderHeaderStruct and prints it to the console
int printContainerHeaderStruct(struct ContainerHeader* pContainerHeader) {
    if (pContainerHeader == NULL) {
        printf("printContainerHeaderStruct: containerHeaderStruct couldn't be found at given memory address (pointer)\n");
        return 1;
    }

    printf("chunkID: %.4s \n", pContainerHeader->chunkID);
    printf("chunkSize: %u \n", pContainerHeader->chunkSize);
    printf("format: %.4s \n", pContainerHeader->format);
    printf("subchunk1ID: %.4s \n", pContainerHeader->subChunk1ID);
    printf("subchunk1Size: %u \n", pContainerHeader->subChunk1Size);
    printf("audioFormat: %u \n", pContainerHeader->audioFormat);
    printf("numChannels: %u \n", pContainerHeader->numChannels);
    printf("sampleRate: %u \n", pContainerHeader->sampleRate);
    printf("byteRate: %u \n", pContainerHeader->byteRate);
    printf("blockAlign: %u \n", pContainerHeader->blockAlign);
    printf("bitsPerSample: %u \n", pContainerHeader->bitsPerSample);
    printf("subchunk2ID: %.4s \n", pContainerHeader->subChunk2ID);
    printf("subchunk2Size: %u \n", pContainerHeader->subChunk2Size);

    printf("\n");

    return 0;
}
