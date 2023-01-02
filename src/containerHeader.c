#include "containerHeader.h"
#include "util.h"
#include "constants.h"

#include <errno.h>
#include <string.h>
#include <stdlib.h>

int createContainerHeaderStruct(struct ContainerHeader* pContainerHeader, char* pContainerFilepath) {
    // WAV file validity

    uint32_t containerFileSize;
    int output = fileLength(pContainerFilepath, &containerFileSize);

    if (output != 0) {
        printf("error in createContainerHeaderStruct() \n"); // REMOVE BEFORE RELEASE
        return 1;
    }

    // is this even necessary ?
    if (containerFileSize < WAV_HEADER_SIZE) {
        printf("error in createContainerHeaderStruct() \n"); // REMOVE BEFORE RELEASE
        printf("invalid WAV file: invalid header length \n");
        return 1;
    }

    // open WAV
    errno = 0;
    FILE* pFile = fopen(pContainerFilepath, "rb");

    if (pFile == NULL) {
        printf("error in createContainerHeaderStruct() \n"); // REMOVE BEFORE RELEASE
        printf("Could not access file in filepath[%s]: \n%s \n", pContainerFilepath, strerror(errno));
        return 1;
    }

    // initialize variables
    unsigned char buffer4[4];
    unsigned char buffer2[2];
    char stringBuffer[5];
    stringBuffer[4] = '\0';
    uint32_t containerBytesLeft = containerFileSize;

    // READ HEADER INTO STRUCT

    // ChunkID 4 byte BE
    int elementsRead = fread(stringBuffer, sizeof(pContainerHeader->chunkID), 1, pFile);    
    containerBytesLeft -= sizeof(pContainerHeader->chunkID);

    if (strcmp("RIFF", stringBuffer) != 0) {
        printf("error in createContainerHeaderStruct(): \n");
        printf("Invalid WAV file, CHUNKID must be 'RIFF' not [%s] \n", stringBuffer);
        return 1;
    }

    memcpy(pContainerHeader->chunkID, stringBuffer, 4);


    // ChunkSize 4 byte LE converted to BE
    containerBytesLeft -= sizeof(pContainerHeader->chunkSize);
    pContainerHeader->chunkSize = containerBytesLeft;

    fseek(pFile, sizeof(pContainerHeader->chunkSize), SEEK_CUR);


    // Format 4 byte BE
    elementsRead = fread(stringBuffer, sizeof(pContainerHeader->format), 1, pFile);
    containerBytesLeft -= sizeof(pContainerHeader->format);

    if (strcmp("WAVE", stringBuffer) != 0) { // Check if file is of type WAVE
        printf("error in createContainerHeaderStruct(): \n");
        printf("Invalid WAV file, FORMAT must be 'WAVE' not [%s] \n", stringBuffer);
        return 1;
    }

    memcpy(pContainerHeader->format, stringBuffer, 4);


    // SubChunk1ID 4 byte BE
    output = seekSubChunk("fmt ", &containerBytesLeft, pContainerHeader->subChunk1ID, pFile);
    if (output != 0) return 1;

    
    // SubChunk1Size 4 byte LE converted to BE
    elementsRead = fread(buffer4, sizeof(buffer4), 1, pFile);
    containerBytesLeft -= sizeof(buffer4);

    if (elementsRead != 1) {
        printf("error in createContainerHeaderStruct(): \n");
        printf("failed to read from file: \n");
        return 1;
    }

    pContainerHeader->subChunk1Size = charLEToIntBE(buffer4);


    // AudioFormat 2 byte LE converted to BE
    elementsRead = fread(buffer2, sizeof(buffer2), 1, pFile);
    containerBytesLeft -= sizeof(buffer2);

    pContainerHeader->audioFormat = charLEToIntBE(buffer2);
    
    if (pContainerHeader->audioFormat != 1) { // Check if audioFormat is of type PCM
        printf("error in createContainerHeaderStruct(): \n");
        printf("WAV file in filepath[%s]: \n", pContainerFilepath);
        printf("audio format is not of type PCM (audiformat in header must be equal to 1) \n");
        return 1;
    }


    // NumChannels 2 byte LE converted to BE
    elementsRead = fread(buffer2, sizeof(buffer2), 1, pFile);
    containerBytesLeft -= sizeof(buffer2);

    pContainerHeader->numChannels = charLEToIntBE(buffer2);


    // SampleRate 4 byte LE converted to BE
    elementsRead = fread(buffer4, sizeof(buffer4), 1, pFile);
    containerBytesLeft -= sizeof(buffer4);

    pContainerHeader->sampleRate = charLEToIntBE(buffer4);


    // ByteRate 4 byte LE converted to BE
    elementsRead = fread(buffer4, sizeof(buffer4), 1, pFile);
    containerBytesLeft -= sizeof(buffer4);

    pContainerHeader->byteRate = charLEToIntBE(buffer4);

    
    // BlockAlign 2 byte LE converted to BE
    elementsRead = fread(buffer2, sizeof(buffer2), 1, pFile);
    containerBytesLeft -= sizeof(buffer2);

    pContainerHeader->blockAlign = charLEToIntBE(buffer2);

    
    // BitsPerSample 2 byte LE converted to BE
    elementsRead = fread(buffer2, sizeof(buffer2), 1, pFile);
    containerBytesLeft -= sizeof(buffer2);

    pContainerHeader->bitsPerSample = charLEToIntBE(buffer2);

    
    // SubChunk2ID 4 byte BE
    output = seekSubChunk("data", &containerBytesLeft, pContainerHeader->subChunk2ID, pFile);
    if (output != 0) return 1;

    // SubChunk2Size 4 byte LE converted to BE
    containerBytesLeft -= sizeof(pContainerHeader->subChunk2Size);
    pContainerHeader->subChunk2Size = containerBytesLeft;

    fseek(pFile, sizeof(pContainerHeader->subChunk2Size), SEEK_CUR);


    fclose(pFile);
    return 0;
}

int read4ByteElement(FILE* pFile, uint32_t* pContainerBytesLeft, uint32_t* destElement) {
    char buffer[4];
    int elementsRead = fread(buffer, sizeof(buffer), 1, pFile);

    if (elementsRead != 1) {
        printf("error in createContainerHeaderStruct(): \n");
        printf("failed to read from file \n");
        return 1;
    }

    *(pContainerBytesLeft) -= sizeof(buffer);
    *(destElement) = charLEToIntBE(buffer);
    
    return 0;
}

int read2ByteElement(FILE* pFile, uint32_t* pContainerBytesLeft, uint32_t* destElement) {
    char buffer[2];
    int elementsRead = fread(buffer, sizeof(buffer), 1, pFile);

    if (elementsRead != 1) {
        printf("error in createContainerHeaderStruct(): \n");
        printf("failed to read from file \n");
        return 1;
    }

    *(pContainerBytesLeft) -= sizeof(buffer);
    *(destElement) = charLEToIntBE(buffer);

    return 0;
}

int seekSubChunk(char seekedSubChunkID[], uint32_t* bytesLeft, char* destID, FILE* pFile) {
    const int stringLength = 4;
    char stringBuffer[5];
    stringBuffer[4] = '\0';
    
    char buffer4[4];
    uint32_t tempSubchunkSizeStorage;
    int elementsRead;
    int output;


    elementsRead = fread(stringBuffer, stringLength, 1, pFile);

    *(bytesLeft) -= stringLength;

    while (strcmp(seekedSubChunkID, stringBuffer) != 0) {

        elementsRead = fread(buffer4, sizeof(buffer4), 1, pFile);

        *(bytesLeft) -= sizeof(buffer4);

        tempSubchunkSizeStorage = charLEToIntBE(buffer4);

        if (*(bytesLeft) < 0 || *(bytesLeft) < tempSubchunkSizeStorage) {
            printf("error in createContainerHeaderStruct(): \n");
            printf("Invalid WAV file, WAV file must contain a [%s] SUBCHUNKID \n", seekedSubChunkID);
            return 1;
        }

        fseek(pFile, tempSubchunkSizeStorage, SEEK_CUR);

        *(bytesLeft) -= tempSubchunkSizeStorage;

        elementsRead = fread(stringBuffer, stringLength, 1, pFile);

        *(bytesLeft) -= stringLength;
    }

    memcpy(destID, stringBuffer, 4);
    return 0;
}

int byteReadErrorCheck(int elementsRead, int bytesNeeded) {
    if (elementsRead != bytesNeeded) {
        printf("error in createContainerHeaderStruct(): \n");
        printf("failed to read from file: \n");
        printf("needed to read %d bytes, but program read %d bytes instead \n", bytesNeeded, elementsRead);
        return 1;
    }

    return 0;
}

// takes in pointer to a containerHeaderHeaderStruct and prints it to the console
int printContainerHeaderStruct(struct ContainerHeader* pContainerHeader) {
    if (pContainerHeader == NULL) {
        printf("printContainerHeaderStruct(): \n");
        printf("invalid pointer, null \n");
        return 1;
    }

    printf("\nCONTAINER HEADER: \n");

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

    return 0;
}

int containerHeaderToArr(char* destinationArr, char* pContainerFilepath, unsigned int headerLength) {
    // WAV file validity
    
    uint32_t containerFileSize;
    int output = fileLength(pContainerFilepath, &containerFileSize);

    if (output != 0) {
        printf("error in containerHeaderToArr() \n"); // REMOVE BEFORE RELEASE
        return 1;
    }

    if (containerFileSize < headerLength) {
        printf("error in containerHeaderToArr() \n"); // REMOVE BEFORE RELEASE
        printf("invalid WAV file: invalid header length \n");
        return 1;
    }

    errno = 0;
    FILE* pFile = fopen(pContainerFilepath, "rb");

    if (pFile == NULL) {
        printf("error in containerHeaderToArr() \n"); // REMOVE BEFORE RELEASE
        printf("Could not access file in filepath[%s]: \n%s \n", pContainerFilepath, strerror(errno));
        return 1;
    }

    char* containerHeaderArr = malloc(headerLength);
    output = fread(containerHeaderArr, headerLength, 1, pFile);

    memcpy(destinationArr, containerHeaderArr, headerLength);
    
    fclose(pFile);
    free(containerHeaderArr);
    return 0;
}
