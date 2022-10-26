#include "encoder.h"

void encodeMessage(char *data, unsigned int dataLength, int sampleSize, int bitsPerByte, char *message, unsigned int messageLength){
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


void encodeBitInByte(unsigned char *byte, int bit, int positionInByte){
    // 1. set positionInByte bit in "byte" to 0
    // 2. make new byte with all zeros except bit in positionInByte is set to "bit" value
    // 3. bitwise or the two bytes
    unsigned char byteCopy = *byte;
    *(byte) = (byteCopy & ~(1 << positionInByte)) | (bit << positionInByte);
}
