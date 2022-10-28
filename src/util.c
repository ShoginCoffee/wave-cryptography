#include <stdio.h>

int fileLength(char* filepath) {
	FILE* pFile = fopen(filepath, "rb");

	fseek(pFile, 0, SEEK_END);
	int fileLength = ftell(pFile);

	fclose(pFile);
	return fileLength;
}
