#ifndef CHACHA_H
#define CHACHA_H
#include <stdint.h>

struct Block {
    uint32_t row1[4];
    uint32_t row2[4];
    uint32_t row3[4];
    uint32_t row4[4];
};

void quarterRound(uint32_t *a, uint32_t *b, uint32_t *c, uint32_t *d);

void addBlock(struct Block *primaryBlock, struct Block *secondaryBlock);

void chacha20Block(struct Block *Block);

uint8_t * serialize(struct Block *Block);

void xorStreams(uint8_t *primaryStream, int lower, int length, uint8_t *secondaryStream);

void populateBlock(struct Block *pBlock, uint32_t *pKey, uint32_t *pCounter, uint32_t *pNonce);

void chacha20(uint8_t *cryptText, uint64_t cryptTextLength, uint32_t *pKey, uint32_t *pCounter, uint32_t *pNonce);
#endif // CHACHA_H
