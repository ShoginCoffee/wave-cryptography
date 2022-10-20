#include "wavHeader.h"

unsigned short charLEToShortBE(unsigned char charArray[2]) {
    return charArray[0] | charArray[1] << 8;
}

unsigned int charLEToIntBE(unsigned char charArray[4]) {
    return charArray[0] | charArray[1] << 8 | charArray[2] << 16 | charArray[3] << 24;
}

unsigned char * shortBEToCharLE (unsigned short num) {
    unsigned char* pCharArray = malloc(2);

    pCharArray[0] = num & 0xFF;
    pCharArray[1] = (num & 0xFF00) >> 8;

    return pCharArray;
}

unsigned char * intBEToCharLE (unsigned int num) {
    unsigned char* pCharArray = malloc(4);

    pCharArray[0] = num & 0xFF;
    pCharArray[1] = (num & 0xFF00) >> 8;
    pCharArray[2] = (num & 0xFF0000) >> 16;
    pCharArray[3] = (num & 0xFF000000) >> 24;

    return pCharArray;
}

// takes in file path and returns header struct
struct WavHeader headerToStruct(FILE* pWavOriginal) {
    unsigned char buffer4[4];
    unsigned char buffer2[2];

    // read header into struct
    int bytesRead = 0;
    struct WavHeader header;

    // ChunkID  4 byte BE
    bytesRead = fread(header.chunkID, sizeof(header.chunkID), 1, pWavOriginal);
    printf("ChunkID: %.4s \n", header.chunkID);

    // ChunkSize 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pWavOriginal);
    header.chunkSize = charLEToIntBE(buffer4);
    printf("ChunkSize: %u \n", header.chunkSize);

    // Format 4 byte BE
    bytesRead = fread(header.format, sizeof(header.format), 1, pWavOriginal);
    printf("Format: %.4s \n", header.format);

    // Subchunk1ID 4 byte BE
    bytesRead = fread(header.subChunk1ID, sizeof(header.subChunk1ID), 1, pWavOriginal);
    printf("Subchunk1ID: %.4s \n", header.subChunk1ID);

    // Subchunk1Size 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pWavOriginal);
    header.subChunk1Size = charLEToIntBE(buffer4);
    printf("Subchunk1Size: %u \n", header.subChunk1Size);

    // AudioFormat 2 byte LE converted to BE
    bytesRead = fread(buffer2, sizeof(buffer2), 1, pWavOriginal);
    header.audioFormat = charLEToIntBE(buffer2);
    printf("AudioFormat: %u \n", header.audioFormat);

    // NumChannels 2 byte LE converted to BE
    bytesRead = fread(buffer2, sizeof(buffer2), 1, pWavOriginal);
    header.numChannels = charLEToIntBE(buffer2);
    printf("NumChannels: %u \n", header.numChannels);

    // SampleRate 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pWavOriginal);
    header.sampleRate = charLEToIntBE(buffer4);
    printf("SampleRate: %u \n", header.sampleRate);

    // ByteRate 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pWavOriginal);
    header.byteRate = charLEToIntBE(buffer4);
    printf("ByteRate: %u \n", header.byteRate);

    // BlockAlign 2 byte LE converted to BE
    bytesRead = fread(buffer2, sizeof(buffer2), 1, pWavOriginal);
    header.blockAlign = charLEToIntBE(buffer2);
    printf("BlockAlign: %u \n", header.blockAlign);

    // BitsPerSample 2 byte LE converted to BE
    bytesRead = fread(buffer2, sizeof(buffer2), 1, pWavOriginal);
    header.bitsPerSample = charLEToIntBE(buffer2);
    printf("BitsPerSample: %u \n", header.bitsPerSample);

    // Subchunk2ID 4 byte BE
    bytesRead = fread(header.subChunk2ID, sizeof(header.subChunk2ID), 1, pWavOriginal);
    printf("Subchunk2ID: %.4s \n", header.subChunk2ID);

    // Subchunk2Size 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pWavOriginal);
    header.subChunk2Size = charLEToIntBE(buffer4);
    printf("Subchunk2Size: %u \n", header.subChunk2Size);

    fclose(pWavOriginal);
    return header;
}
