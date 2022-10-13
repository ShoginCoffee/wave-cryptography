#ifndef WAVHEADER_H
#define WAVHEADER_H
#include <stdbool.h>


char * pReadInWaveData(FILE *pWavOriginal, struct HEADER header)
{
    char waveData = malloc(header.Subchunk2Size);
    char * pWaveData = &waveData;
    fread(pWaveData, header.Subchunk2Size, 1, pWavOriginal);

    return pWaveData;
}

char changeBits(char whichBits, char bitValues, char *byte)
{
    //All other bits configuration except 1 are work in progress
    switch(whichBits)
    {
    //Last 1 bit
    case 1:
        //Is last bit true?
        switch(byte & 1) {
        case 0:
            return (bit) ? 1 : 0;
        case 1:
            return (bit) ? 0 : 1;
        }
        break;
    //Last 2 bits
    case 3:
        break;
    //Last 3 bits
    case 7:
        break;
    //Last 4 bits
    case 15:
        break;
    //Last 5 bits
    case 31:
        break;
    //Last 6 bits
    case 63:
        break;
    //Last 7 bits
    case 127:
        break;
    //Last 8 bits
    case 255:
        break;
    //Error
    default:
        return 1;
        break;
    }
}
#endif

