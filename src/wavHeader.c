#include "wavHeader.h"
#include "util.h"

// takes in file path and returns header struct
struct WavvHeader* createTargetHeaderStruct(struct WavHeader* pTargetHeader, char* pTargetFilepath) {
    FILE* pFile = fopen(pTargetFilepath, "rb");

    if (pFile == NULL) {
        printf("Target file couldn't be found at given filepath\n");
        return NULL;
    }

    unsigned char buffer4[4];
    unsigned char buffer2[2];

    // read header into struct
    int bytesRead = 0;
    struct WavHeader header;

    // ChunkID  4 byte BE
    bytesRead = fread(pTargetHeader->chunkID, sizeof(pTargetHeader->chunkID), 1, pFile);

    // ChunkSize 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1,pFile);
    pTargetHeader->chunkSize = charLEToIntBE(buffer4);

    // Format 4 byte BE
    bytesRead = fread(pTargetHeader->format, sizeof(pTargetHeader->format), 1, pFile);

    // SubChunk1ID 4 byte BE
    bytesRead = fread(pTargetHeader->subChunk1ID, sizeof(pTargetHeader->subChunk1ID), 1, pFile);

    // SubChunk1Size 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pFile);
    pTargetHeader->subChunk1Size = charLEToIntBE(buffer4);

    // AudioFormat 2 byte LE converted to BE
    bytesRead = fread(buffer2, sizeof(buffer2), 1, pFile);
    pTargetHeader->audioFormat = charLEToIntBE(buffer2);

    // NumChannels 2 byte LE converted to BE
    bytesRead = fread(buffer2, sizeof(buffer2), 1, pFile);
    pTargetHeader->numChannels = charLEToIntBE(buffer2);

    // SampleRate 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pFile);
    pTargetHeader->sampleRate = charLEToIntBE(buffer4);

    // ByteRate 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pFile);
    pTargetHeader->byteRate = charLEToIntBE(buffer4);

    // BlockAlign 2 byte LE converted to BE
    bytesRead = fread(buffer2, sizeof(buffer2), 1, pFile);
    pTargetHeader->blockAlign = charLEToIntBE(buffer2);

    // BitsPerSample 2 byte LE converted to BE
    bytesRead = fread(buffer2, sizeof(buffer2), 1, pFile);
    pTargetHeader->bitsPerSample = charLEToIntBE(buffer2);

    // SubChunk2ID 4 byte BE
    bytesRead = fread(pTargetHeader->subChunk2ID, sizeof(pTargetHeader->subChunk2ID), 1, pFile);

    // SubChunk2Size 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pFile);
    pTargetHeader->subChunk2Size = charLEToIntBE(buffer4);

    fclose(pFile);
    return pTargetHeader;
}

// takes in pointer to a targetHeaderStruct and prints it to the console
int printTargetHeaderStruct(struct WavHeader* pTargetHeader) {
    if (pTargetHeader == NULL) {
        printf("targetrHeaderStruct couldn't be found at given memory address (pointer)\n");
        return 1;
    }

    printf("chunkID: %.4s \n", pTargetHeader->chunkID);
    printf("chunkSize: %u \n", pTargetHeader->chunkSize);
    printf("format: %.4s \n", pTargetHeader->format);
    printf("subchunk1ID: %.4s \n", pTargetHeader->subChunk1ID);
    printf("subchunk1Size: %u \n", pTargetHeader->subChunk1Size);
    printf("audioFormat: %u \n", pTargetHeader->audioFormat);
    printf("numChannels: %u \n", pTargetHeader->numChannels);
    printf("sampleRate: %u \n", pTargetHeader->sampleRate);
    printf("byteRate: %u \n", pTargetHeader->byteRate);
    printf("blockAlign: %u \n", pTargetHeader->blockAlign);
    printf("bitsPerSample: %u \n", pTargetHeader->bitsPerSample);
    printf("subchunk2ID: %.4s \n", pTargetHeader->subChunk2ID);
    printf("subchunk2Size: %u \n", pTargetHeader->subChunk2Size);

    printf("\n");

    return 0;
}
