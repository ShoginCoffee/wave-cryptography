#include "encoder.h"

void encodeMessage(char *data, unsigned int dataLength, int sampleSize, int bitsPerByte, char *message, unsigned int messageLength){
    // SIDENOTE: DEBUG PRINTF ARE COMMENTED OUT BUT ARE STILL IN THE FUNCTION, REMOVE WHEN NECESSARY.
    int i, t;
    int messageByte = 0, messageBit = 0;
    unsigned char bit;
    // loop all samples
    for(i = sampleSize - 1; i < dataLength; i += sampleSize){
        // for every sample
                                //printf("sample %d: \n\n", i);
        if(messageLength != messageByte){
            for(t = 0; t < bitsPerByte; t++){
                                //printf("changing bit %d: \n\n", t);
                // 1. take bit from message byte
                bit = (( *(message + messageByte) << messageBit) & 0xff) >> 7;
                                //printf("bit to encode: %d \n\n", bit);
                // 2. encode bit in data
                encodeBitInByte((data + i), bit, t);
                // 3. update messageData counters
                if(messageBit + 1 == 8) messageByte++;
                messageBit = (messageBit + 1) % 8;
                // 4. if message has been encoded: break
                if(messageByte == messageLength) break;
            }
        }
    }
}

/* THEO. Vad gör det här? funkar det eller är det gammalt?

#include <stdbool.h>
#include <stdio.h>
#include "wavHeader.h"


char * pReadInWaveData(FILE *pWavOriginal, struct WavHeader wavHeader)
{
    char waveData = malloc(wavHeader.subChunk2Size);
    char *pWaveData = &waveData;
    fread(pWaveData, wavHeader.subChunk2Size, 1, pWavOriginal);

    return pWaveData;
}

char changeBits(char whichBits, char bitValues, char *byte)
{
    //All other bits configuration except 1 are work in progress
    switch(whichBits)
    {
    //Last 1 bit
    case 1:
        //Is last bit true?             */
        /*switch(bitValues & 1) {
        case 0:
            return (bit) ? 1 : 0;
        case 1:
            return (bit) ? 0 : 1;
        }*/                             /*
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
*/

void encodeBitInByte(unsigned char *byte, int bit, int positionInByte){
    // 1. set positionInByte bit in "byte" to 0
    // 2. make new byte with all zeros except bit in positionInByte is set to "bit" value
    // 3. bitwise or the two bytes
    unsigned char byteCopy = *byte;
    *(byte) = (byteCopy & ~(1 << positionInByte)) | (bit << positionInByte);
}
