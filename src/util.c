#include "util.h"
#include "constants.h"
#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <errno.h>


int fileLength(char* pFilepath, uint32_t* pFileLength) {
	errno = 0;
	FILE* pFile = fopen(pFilepath, "rb");

    if (pFile == NULL) {
		printf("error in fileLength(): \n");
		printf("Could not access file in filepath[%s]: \n%s \n", pFilepath, strerror(errno));
        return 1;
    }

	fseek(pFile, 0, SEEK_END);
	unsigned int fileLength = ftell(pFile);

	fclose(pFile);
	*(pFileLength) = fileLength;
	return 0;
}

int listDirectoryContents(char* sDir) {
	WIN32_FIND_DATA fdFile;
	HANDLE hFind = NULL;

	char sPath[MAX_PATH_LENGTH];

	// Specify a file mask.		*.* = We want everything
	sprintf(sPath, "%s\\*.*", sDir);

	if ((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
	{
		printf("Path not found: [%s]\n", sDir);
		return 1;
	}

	do
	{
		// Find first file will always return "." and ".." as the first two directories
		if (strcmp(fdFile.cFileName, ".") != 0
			&& strcmp(fdFile.cFileName, "..") != 0)
		{
			// Is the entity a File or Folder?
			if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) // return 1 if file
			{
				printf(LIGHTBLUE_TEXT);
				printf("%s/    ", fdFile.cFileName);
			}
			else {
				printf(RESET_COLOR);
				printf("%s    ", fdFile.cFileName);
			}
		}
	} while (FindNextFile(hFind, &fdFile)); //Find the next file.

	FindClose(hFind);

	printf(RESET_COLOR);
	return 0;
}

uint16_t charLEToShortBE(unsigned char charArray[2]) {
    return charArray[0] | charArray[1] << 8;
}

uint32_t charLEToIntBE(unsigned char charArray[4]) {
    return charArray[0] | charArray[1] << 8 | charArray[2] << 16 | charArray[3] << 24;
}

void shortBEToCharLE(uint16_t num, unsigned char* destinationCharLE) {
    destinationCharLE[0] = num & 0xFF;
    destinationCharLE[1] = (num & 0xFF00) >> 8;
}

void intBEToCharLE(uint32_t num, unsigned char* destinationCharLE) {
    destinationCharLE[0] = num & 0xFF;
    destinationCharLE[1] = (num & 0xFF00) >> 8;
    destinationCharLE[2] = (num & 0xFF0000) >> 16;
    destinationCharLE[3] = (num & 0xFF000000) >> 24;
}
