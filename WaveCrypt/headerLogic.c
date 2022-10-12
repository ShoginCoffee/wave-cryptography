#include "headerStruct.h"
#include "headerLogic.h"

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
    bytesRead = fread(header.ChunkID, sizeof(header.ChunkID), 1, pWavOriginal);
    printf("ChunkID: %.4s \n", header.ChunkID);

    // ChunkSize 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pWavOriginal);
    header.ChunkSize = charLEToIntBE(buffer4);
    printf("ChunkSize: %u \n", header.ChunkSize);

    // Format 4 byte BE
    bytesRead = fread(header.Format, sizeof(header.Format), 1, pWavOriginal);
    printf("Format: %.4s \n", header.Format);

    // Subchunk1ID 4 byte BE
    bytesRead = fread(header.Subchunk1ID, sizeof(header.Subchunk1ID), 1, pWavOriginal);
    printf("Subchunk1ID: %.4s \n", header.Subchunk1ID);

    // Subchunk1Size 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pWavOriginal);
    header.Subchunk1Size = charLEToIntBE(buffer4);
    printf("Subchunk1Size: %u \n", header.Subchunk1Size);

    // AudioFormat 2 byte LE converted to BE
    bytesRead = fread(buffer2, sizeof(buffer2), 1, pWavOriginal);
    header.AudioFormat = charLEToIntBE(buffer2);
    printf("AudioFormat: %u \n", header.AudioFormat);

    // NumChannels 2 byte LE converted to BE
    bytesRead = fread(buffer2, sizeof(buffer2), 1, pWavOriginal);
    header.NumChannels = charLEToIntBE(buffer2);
    printf("NumChannels: %u \n", header.NumChannels);

    // SampleRate 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pWavOriginal);
    header.SampleRate = charLEToIntBE(buffer4);
    printf("SampleRate: %u \n", header.SampleRate);

    // ByteRate 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pWavOriginal);
    header.ByteRate = charLEToIntBE(buffer4);
    printf("ByteRate: %u \n", header.ByteRate);

    // BlockAlign 2 byte LE converted to BE
    bytesRead = fread(buffer2, sizeof(buffer2), 1, pWavOriginal);
    header.BlockAlign = charLEToIntBE(buffer2);
    printf("BlockAlign: %u \n", header.BlockAlign);

    // BitsPerSample 2 byte LE converted to BE
    bytesRead = fread(buffer2, sizeof(buffer2), 1, pWavOriginal);
    header.BitsPerSample = charLEToIntBE(buffer2);
    printf("BitsPerSample: %u \n", header.BitsPerSample);

    // Subchunk2ID 4 byte BE
    bytesRead = fread(header.Subchunk2ID, sizeof(header.Subchunk2ID), 1, pWavOriginal);
    printf("Subchunk2ID: %.4s \n", header.Subchunk2ID);

    // Subchunk2Size 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pWavOriginal);
    header.Subchunk2Size = charLEToIntBE(buffer4);
    printf("Subchunk2Size: %u \n", header.Subchunk2Size);

    fclose(pWavOriginal);
    return header;
}
