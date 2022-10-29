#ifndef UTIL_H
#define UTIL_H

unsigned int fileLength(char* pFilepath);

unsigned short charLEToShortBE(unsigned char charArray[2]);

unsigned int charLEToIntBE(unsigned char charArray[4]);

unsigned char* shortBEToCharLE(unsigned short num);

unsigned char* intBEToCharLE(unsigned int num);
#endif
