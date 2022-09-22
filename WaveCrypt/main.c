#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "wave.h"

// takes char and prints bits
void printb(char val) {
  for (int i = 7; 0 <= i; i--) {
    printf("%c", (val & (1 << i)) ? '1' : '0');
  }
}


/*
// takes in file path and returns header struct
struct HEADER headerToStruct(FILE* pWavOriginal) {
    unsigned char buffer4[4];
    unsigned char buffer2[2];
    FILE* pWavOriginal = fopen("../AudioFiles/BabyElephantWalk60.wav", "rb");

    if (pWavOriginal == NULL) {
        printf("error opening file.");
        return 1;
    }

    // read header into struct
    int read = 0;
    struct HEADER header;

    // RIFF
    read = fread(header.riff, sizeof(header.riff), 1, pWavOriginal);
    printf("1 - 4: %s \n", header.riff);

    // overall_size
    read = fread(buffer[4], sizeof(buffer[4]), 1, pWavOriginal);


    fclose(pWavOriginal);
    return header;
}
*/


void createNewWave() {
    FILE* pWavEncoded = fopen("../AudioFiles/wavEncoded.wav", "wb");
    fclose(pWavEncoded);
}

int main(){
    //headerToStruct();
    //createNewWave();

    unsigned char little[] = {0xdd, 0xcc, 0xbb, 0xaa};
    int i;
    for (i = 0; i < 4; i++){
        printf("%x", little[i]);
    }


    int big = little[3] << 24 | little[2] << 16 | little[1] << 8 | little[0];
    printf("\n%x", big);

    return 0;
}
