#include <stdio.h>

unsigned int fileLength(char* pFilepath) {
    FILE* pFile = fopen(pFilepath, "rb");

    if (pFile == NULL) {
        return 0;
    }

	fseek(pFile, 0, SEEK_END);
	unsigned int fileLength = ftell(pFile);

	fclose(pFile);
	return fileLength;
}

unsigned short charLEToShortBE(unsigned char charArray[2]) {
    return charArray[0] | charArray[1] << 8;
}

unsigned int charLEToIntBE(unsigned char charArray[4]) {
    return charArray[0] | charArray[1] << 8 | charArray[2] << 16 | charArray[3] << 24;
}

unsigned char* shortBEToCharLE(unsigned short num) {
    unsigned char* pCharArray = malloc(2);

    pCharArray[0] = num & 0xFF;
    pCharArray[1] = (num & 0xFF00) >> 8;

    return pCharArray;
}

unsigned char* intBEToCharLE(unsigned int num) {
    unsigned char* pCharArray = malloc(4);

    pCharArray[0] = num & 0xFF;
    pCharArray[1] = (num & 0xFF00) >> 8;
    pCharArray[2] = (num & 0xFF0000) >> 16;
    pCharArray[3] = (num & 0xFF000000) >> 24;

    return pCharArray;
}
