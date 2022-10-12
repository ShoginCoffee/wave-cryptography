#ifndef HEADERLOGIC_H
#define HEADERLOGIC_H
#include <stdio.h>


unsigned short charLEToShortBE(unsigned char charArray[2]);

unsigned int charLEToIntBE(unsigned char charArray[4]);

unsigned char * shortBEToCharLE (unsigned short num);

unsigned char * intBEToCharLE (unsigned int num);

struct WavHeader headerToStruct(FILE* pWavOriginal);
#endif
