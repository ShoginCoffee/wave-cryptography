#ifndef WAVHEADER_H
#define WAVHEADER_H
#include <stdbool.h>


char * pReadInWaveData(FILE* pWavOriginal, struct HEADER header);

char changeBits(char whichBits, char byte, bool bit);
#endif

