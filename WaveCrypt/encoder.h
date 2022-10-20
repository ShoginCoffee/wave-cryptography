#ifndef ENCODER_H
#define ENCODER_H
#include <stdbool.h>
#include <stdio.h>

char * pReadInWaveData(FILE* pWavOriginal, struct HEADER header);

char changeBits(char whichBits, char byte, bool bit);
#endif

