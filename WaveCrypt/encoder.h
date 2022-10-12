#ifndef WAVHEADERSTRUCT_H
#define WAVHEADERSTRUCT_H
#include <stdbool.h>


char * pReadInWaveData(FILE* pWavOriginal, struct HEADER header);

char changeBits(char whichBits, char byte, bool bit);
#endif

