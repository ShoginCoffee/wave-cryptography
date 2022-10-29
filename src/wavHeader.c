#include "wavHeader.h"
#include "util.h"

// takes in file path and returns header struct
struct WavHeader createTargetHeaderStruct(char* pTargetFilepath) {
    FILE* pFile = fopen(pTargetFilepath, "rb");

    unsigned char buffer4[4];
    unsigned char buffer2[2];

    // read header into struct
    int bytesRead = 0;
    struct WavHeader header;

    // ChunkID  4 byte BE
    bytesRead = fread(header.chunkID, sizeof(header.chunkID), 1, pFile);

    // ChunkSize 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1,pFile);
    header.chunkSize = charLEToIntBE(buffer4);

    // Format 4 byte BE
    bytesRead = fread(header.format, sizeof(header.format), 1, pFile);

    // SubChunk1ID 4 byte BE
    bytesRead = fread(header.subChunk1ID, sizeof(header.subChunk1ID), 1, pFile);

    // SubChunk1Size 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pFile);
    header.subChunk1Size = charLEToIntBE(buffer4);

    // AudioFormat 2 byte LE converted to BE
    bytesRead = fread(buffer2, sizeof(buffer2), 1, pFile);
    header.audioFormat = charLEToIntBE(buffer2);

    // NumChannels 2 byte LE converted to BE
    bytesRead = fread(buffer2, sizeof(buffer2), 1, pFile);
    header.numChannels = charLEToIntBE(buffer2);

    // SampleRate 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pFile);
    header.sampleRate = charLEToIntBE(buffer4);

    // ByteRate 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pFile);
    header.byteRate = charLEToIntBE(buffer4);

    // BlockAlign 2 byte LE converted to BE
    bytesRead = fread(buffer2, sizeof(buffer2), 1, pFile);
    header.blockAlign = charLEToIntBE(buffer2);

    // BitsPerSample 2 byte LE converted to BE
    bytesRead = fread(buffer2, sizeof(buffer2), 1, pFile);
    header.bitsPerSample = charLEToIntBE(buffer2);

    // SubChunk2ID 4 byte BE
    bytesRead = fread(header.subChunk2ID, sizeof(header.subChunk2ID), 1, pFile);

    // SubChunk2Size 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pFile);
    header.subChunk2Size = charLEToIntBE(buffer4);

    printf("chunkID: %.4s \n", header.chunkID);
    printf("chunkSize: %u \n", header.chunkSize);
    printf("format: %.4s \n", header.format);
    printf("subchunk1ID: %.4s \n", header.subChunk1ID);
    printf("subchunk1Size: %u \n", header.subChunk1Size);
    printf("audioFormat: %u \n", header.audioFormat);
    printf("numChannels: %u \n", header.numChannels);
    printf("sampleRate: %u \n", header.sampleRate);
    printf("byteRate: %u \n", header.byteRate);
    printf("blockAlign: %u \n", header.blockAlign);
    printf("bitsPerSample: %u \n", header.bitsPerSample);
    printf("subchunk2ID: %.4s \n", header.subChunk2ID);
    printf("subchunk2Size: %u \n", header.subChunk2Size);


    printf("\n");

    fclose(pFile);
    return header;
}
