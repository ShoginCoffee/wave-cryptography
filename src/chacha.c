#include "chacha.h"

void zeroBlock(struct Block* block) {
	for (unsigned int i = 0; i < 4; i++) {
		block->row1[i] = 0;
		block->row2[i] = 0;
		block->row3[i] = 0;
		block->row4[i] = 0;
	}
}

void quarterRound(uint32_t* a, uint32_t* b, uint32_t* c, uint32_t* d) {
	uint32_t temp;

	// 1. a += b; d ^= a; d <<<= 16;
	*a += *b;
	*d ^= *a;
	temp = *d;
	*d <<= 16;
	temp >>= (32 - 16);
	*d ^= temp;

	// 2. c += d; b ^= c; b <<<= 12;
	*c += *d;
	*b ^= *c;
	temp = *b;
	*b <<= 12;
	temp >>= (32 - 12);
	*b ^= temp;

	// 3.  a += b; d ^= a; d <<<= 8;
	*a += *b;
	*d ^= *a;
	temp = *d;
	*d <<= 8;
	temp >>= (32 - 8);
	*d ^= temp;

	// 4.  c += d; b ^= c; b <<<= 7;
	*c += *d;
	*b ^= *c;
	temp = *b;
	*b <<= 7;
	temp >>= (32 - 7);
	*b ^= temp;
}

void addBlock(struct Block* primaryBlock, struct Block* secondaryBlock) {
	for (unsigned int i = 0; i < 4; i++) {
		primaryBlock->row1[i] += secondaryBlock->row1[i];
		primaryBlock->row2[i] += secondaryBlock->row2[i];
		primaryBlock->row3[i] += secondaryBlock->row3[i];
		primaryBlock->row4[i] += secondaryBlock->row4[i];
	}
}

void chacha20Block(struct Block* block) {
	struct Block startBlock = *block;

	// 20 rounds
	for (unsigned int t = 0; t < 10; t++) {
		// DOUBLE ROUND

		// column round
		for (unsigned int i = 0; i < 4; i++) {
			quarterRound(&block->row1[i], &block->row2[i], &block->row3[i], &block->row4[i]);
		}

		// diagonal round
		for (unsigned int i = 0; i < 4; i++) {
			quarterRound(&block->row1[i], &block->row2[(i + 1) % 4], &block->row3[(i + 2) % 4], &block->row4[(i + 3) % 4]);
		}
	}

	addBlock(block, &startBlock);
	zeroBlock(&startBlock);
}

void serializeBlock(uint8_t* serializeDestination, struct Block* sourceBlock) {
	uint32_t currentInt;
	for (unsigned int i = 0; i < 4; i++) {
		*((uint32_t*)serializeDestination + i) = sourceBlock->row1[i];
		*((uint32_t*)serializeDestination + 4 + i) = sourceBlock->row2[i];
		*((uint32_t*)serializeDestination + 8 + i) = sourceBlock->row3[i];
		*((uint32_t*)serializeDestination + 12 + i) = sourceBlock->row4[i];
	}
}

void xorStreams(uint8_t* primaryStream, int lower, int length, uint8_t* secondaryStream) {
	for (int i = lower; i < lower + length; i++) {
		primaryStream[i] ^= secondaryStream[i - lower];
	}
}

void populateBlock(struct Block* pBlock, uint32_t* pKey, uint32_t* pCounter, uint32_t* pNonce) {
	// THE CHACHA20 constant “expand 32-byte k” in first row of block
	pBlock->row1[0] = 0x61707865;
	pBlock->row1[1] = 0x3320646e;
	pBlock->row1[2] = 0x79622d32;
	pBlock->row1[3] = 0x6b206574;

	unsigned int i;
	for (i = 0; i < 4; i++) {
		pBlock->row2[i] = *(pKey + i);
	}

	for (i = 0; i < 4; i++) {
		pBlock->row3[i] = *(pKey + 4 + i);
	}

	pBlock->row4[0] = *(pCounter);
	for (i = 0; i < 3; i++) {
		pBlock->row4[i + 1] = *(pNonce + i);
	}
}

void chacha20(uint8_t* cryptText, uint64_t cryptTextLength, uint32_t* pKey, uint32_t* pCounter, uint32_t* pNonce) {
    
	// variable initialization
    int64_t bytesToCryptLeft = cryptTextLength;
    uint32_t step = 0;
    struct Block workingBlock;
    int length;
    uint8_t serialized[64];

    // chacha20 magic
    do{
        // chacha20
        populateBlock(&workingBlock, pKey, pCounter, pNonce);
        chacha20Block(&workingBlock);
        serializeBlock(&serialized, &workingBlock);
        length = bytesToCryptLeft % 64 == bytesToCryptLeft ? bytesToCryptLeft : 64;
        xorStreams(cryptText, step*64, length, serialized);

        // increments and decrements (counters)
        bytesToCryptLeft -= 64;
        step++;
        (*pCounter)++;
    } while(bytesToCryptLeft > 0);

	zeroBlock(&workingBlock);
}
