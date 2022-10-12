#include "chacha.h"

struct Block {
    uint32_t row1[4];
    uint32_t row2[4];
    uint32_t row3[4];
    uint32_t row4[4];
};

void quarterRound(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d){
    uint32_t temp;
    //1.  a += b; d ^= a; d <<<= 16;
    *a += *b;
    *d ^= *a;
    temp = *d;
    *d <<= 16;
    temp >>= (32 - 16);
    *d ^= temp;
    //2.  c += d; b ^= c; b <<<= 12;
    *c += *d;
    *b ^= *c;
    temp = *b;
    *b <<= 12;
    temp >>= (32 - 12);
    *b ^= temp;
    //3.  a += b; d ^= a; d <<<= 8;
    *a += *b;
    *d ^= *a;
    temp = *d;
    *d <<= 8;
    temp >>= (32 - 8);
    *d ^= temp;
    //4.  c += d; b ^= c; b <<<= 7;
    *c += *d;
    *b ^= *c;
    temp = *b;
    *b <<= 7;
    temp >>= (32 - 7);
    *b ^= temp;
}

void addBlock(struct Block *primaryBlock, struct Block *secondaryBlock){
    int i;
    for(i = 0; i < 4; i++){
        primaryBlock->row1[i] += secondaryBlock->row1[i];
    }
    for(i = 0; i < 4; i++){
        primaryBlock->row2[i] += secondaryBlock->row2[i];
    }
    for(i = 0; i < 4; i++){
        primaryBlock->row3[i] += secondaryBlock->row3[i];
    }
    for(i = 0; i < 4; i++){
        primaryBlock->row4[i] += secondaryBlock->row4[i];
    }
}

void chacha20Block(struct Block *Block){
    struct Block startBlock = *Block;
    int i;
    int t;
    // 20 rounds
    for(t = 0; t < 10; t++){
        // DOUBLE ROUND

        // column round
        for(i = 0; i < 4; i++){
            quarterRound(&Block->row1[i], &Block->row2[i], &Block->row3[i], &Block->row4[i]);
        }

        // diagonal round
        for(i = 0; i < 4; i++){
            quarterRound(&Block->row1[i], &Block->row2[(i + 1) % 4], &Block->row3[(i + 2) % 4], &Block->row4[(i + 3) % 4]);
        }
    }

    addBlock(Block, &startBlock);
}

uint8_t * serialize(struct Block *Block){
    uint8_t *serialized = malloc(64); // 512 bits

    int i;
    uint32_t currentInt;
    for(i = 0; i < 4; i++){
        currentInt = Block->row1[i];
        serialized[i*4] = currentInt & 0xFF;
        serialized[i*4 + 1] = (currentInt & 0xFF00) >> 8;
        serialized[i*4 + 2] = (currentInt & 0xFF0000) >> 16;
        serialized[i*4 + 3] = (currentInt & 0xFF000000) >> 24;
    }
    for(i = 0; i < 4; i++){
        currentInt = Block->row2[i];
        serialized[16 + i*4] = currentInt & 0xFF;
        serialized[16 + i*4 + 1] = (currentInt & 0xFF00) >> 8;
        serialized[16 + i*4 + 2] = (currentInt & 0xFF0000) >> 16;
        serialized[16 + i*4 + 3] = (currentInt & 0xFF000000) >> 24;
    }
    for(i = 0; i < 4; i++){
        currentInt = Block->row3[i];
        serialized[16*2 + i*4] = currentInt & 0xFF;
        serialized[16*2 + i*4 + 1] = (currentInt & 0xFF00) >> 8;
        serialized[16*2 + i*4 + 2] = (currentInt & 0xFF0000) >> 16;
        serialized[16*2 + i*4 + 3] = (currentInt & 0xFF000000) >> 24;
    }
    for(i = 0; i < 4; i++){
        currentInt = Block->row4[i];
        serialized[16*3 + i*4] = currentInt & 0xFF;
        serialized[16*3 + i*4 + 1] = (currentInt & 0xFF00) >> 8;
        serialized[16*3 + i*4 + 2] = (currentInt & 0xFF0000) >> 16;
        serialized[16*3 + i*4 + 3] = (currentInt & 0xFF000000) >> 24;
    }

    return serialized;
}

void xorStreams(uint8_t *primaryStream, int lower, int length, uint8_t *secondaryStream){
    for(int i = lower; i < lower + length; i++){
        primaryStream[i] ^= secondaryStream[i - lower];
    }
}

void populateBlock(struct Block *pBlock, uint32_t *pKey, uint32_t *pCounter, uint32_t *pNonce){
    pBlock->row1[0] = 0x61707865;
    pBlock->row1[1] = 0x3320646e;
    pBlock->row1[2] = 0x79622d32;
    pBlock->row1[3] = 0x6b206574;

    int i;
    for(i = 0; i < 4; i++){
        pBlock->row2[i] = *(pKey + i);
    }

    for(i = 0; i < 4; i++){
        pBlock->row3[i] = *(pKey + 4 + i);
    }

    pBlock->row4[0] = *(pCounter);
    for(i = 0; i < 3; i++){
        pBlock->row4[i + 1] = *(pNonce + i);
    }
}

void chacha20(uint8_t *cryptText, uint64_t cryptTextLength, uint32_t *pKey, uint32_t *pCounter, uint32_t *pNonce){
    int64_t bytesToCryptLeft = cryptTextLength;
    uint32_t step = 0;
    uint8_t *serializedBlock;
    struct Block workingBlock;
    int length;
    uint32_t counterCopy = *pCounter;

    do{
        // chacha20
        populateBlock(&workingBlock, pKey, &counterCopy, pNonce);
        chacha20Block(&workingBlock);
        serializedBlock = serialize(&workingBlock);
        length = bytesToCryptLeft % 64 == bytesToCryptLeft ? bytesToCryptLeft : 64;
        xorStreams(cryptText, step*64, length, serializedBlock);

        // increments and decrements
        bytesToCryptLeft -= 64;
        step++;
        counterCopy++;
    } while(bytesToCryptLeft > 0);
}
