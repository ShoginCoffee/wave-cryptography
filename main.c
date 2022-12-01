#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>

#include "containerHeader.h"
#include "messageHeader.h"
#include "chacha.h"
#include "menuGUI.h"
#include "encoder.h"
#include "util.h"

#define LIGHTBLUE_TEXT "\033[34;1m"
#define RESET_COLOR "\033[0m"

void printb(char binary) {
	for (int i = 0; i < 8; i++) {
		printf("%x", ((binary << i) & 0xff) >> 7);
	}
}

int listDirectoryContents(char* sDir) {
	WIN32_FIND_DATA fdFile;
	HANDLE hFind = NULL;

	char sPath[2048];

	//Specify a file mask. *.* = We want everything!
	sprintf(sPath, "%s\\*.*", sDir);

	if ((hFind = FindFirstFile(sPath, &fdFile)) == INVALID_HANDLE_VALUE)
	{
		printf("Path not found: [%s]\n", sDir);
		return 0;
	}

	do
	{
		//Find first file will always return "."
		//    and ".." as the first two directories.
		if (strcmp(fdFile.cFileName, ".") != 0
			&& strcmp(fdFile.cFileName, "..") != 0)
		{
			//Is the entity a File or Folder?
			if (fdFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
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

	FindClose(hFind); //Always, Always, clean things up!

	printf(RESET_COLOR);
	return 1;
}


int main(char argc, char* argv[]) {


	// printf("Numer of arguments: %d\n", argc);

	int i = 0;
	while (i < argc) {
		// printf("%s\n", argv[i]);
		if (strcmp(argv[i], "--help") == 0) {
			printf("HELP MESSAGE");
			return 0;
		}
		else if (strcmp(argv[i], "¨-s") == 0) {

		}
		else if (strcmp(argv[i], "¨-o") == 0) {

		}

		i++;
	}

	/* Get the directory where the executable is located
	char string[255];
	GetCurrentDirectory(255, &string);
	printf("%s\n", string);
	*/


	/* List Directories
	char path[] = ".";
	listDirectoryContents(path);
	*/



	/* GUI menu
	int nothing = GUIMenu();
	*/


	// Filepaths
	char containerFilepath[] = "../AudioFiles/BabyElephantWalk60.wav"; // !!! Change to real location after compiling code !!!
	char messageFilepath[] = "../demo.txt";



	// Make containerHeader and messageHeader

	// Read in container file and construct containerHeader
	struct ContainerHeader containerHeader;
	createContainerHeaderStruct(&containerHeader, containerFilepath);
	// printContainerHeaderStruct(&containerHeader);


	// Read in message file and construct messageHeader
	struct MessageHeader messageHeader;
	createMessageHeaderStruct(&messageHeader, messageFilepath, sizeof(messageFilepath));
	// printMessageHeaderStruct(&messageHeader);



	// Read message and container file

	// Read in message file and message file length
	unsigned int messageDataLength = fileLength(messageFilepath);
	char* pMessageData = (char*)malloc(messageDataLength);
	readInMessageData(pMessageData, messageFilepath);

	// Read in container file and container file length
	unsigned int containerDataLength = fileLength(containerFilepath);
	if (containerDataLength != 0) {
		char* pContainerData = (char*)malloc(containerDataLength);
		readInContainerData(pContainerData, containerFilepath, containerHeader.subChunk2Size);
	}
	else {
		printf("main: Container file couldn't be found at given filepath");
	}




	/*
	// encoder
	unsigned char data[16] = {[0 ... 15] = 0x00};
	unsigned char message[] = {0xff, 0xff};
	int sampleSize = 2;
	int bitsPerByte = 2;

	printf("input data: \n");
	for(int i = 0; i < sizeof(data); i++){
		printb(data[i]);
		printf("\n");
	}
	printf("\n");

	printf("input message: \n");
	for(int i = 0; i < sizeof(message); i++){
		printb(message[i]);
		printf("\n");
	}
	printf("\n");

	printf("sample size: %d \n", sampleSize);
	printf("bits per byte: %d \n\n", bitsPerByte);

	encodeMessage(&data, sizeof(data), sampleSize, bitsPerByte, &message, sizeof(message));


	printf("output data: \n");
	for(int i = 0; i < sizeof(data); i++){
		printb(data[i]);
		printf("\n");
	}
	*/


	/*
	// chacha20 test
	uint8_t plainText[] = "Ladies and Gentlemen of the class of '99: If I could offer you only one tip for the future, sunscreen would be it.";
	int key[] = {
		0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c,
		0x13121110, 0x17161514, 0x1b1a1918, 0x1f1e1d1c
		};
	int counter[] = {0x00000001};
	int nonce[] = {0x00000000, 0x4a000000, 0x00000000};

	//Printing original array
	for(int b = 0; b < 114; b++){
		printf("%x ", plainText[b]);
		if((b + 1) % 16 == 0 && b != 0) printf("\n");
	}

	chacha20(&plainText, 114, &key, &counter, &nonce);
	printf("\n\n");

	//Printing encrypted array
	for(int a = 0; a < 114; a++) {
		printf("%x ", plainText[a]);
		if((a + 1) % 16 == 0 && a != 0) printf("\n");
	}

	chacha20(&plainText, 114, &key, &counter, &nonce);
	printf("\n\n");

	//Printing decrypted array
	for(int a = 0; a < 114; a++) {
		printf("%x ", plainText[a]);
		if((a + 1) % 16 == 0 && a != 0) printf("\n");
	}
	*/

	//free(pMessageData);
	//free(pTargetData);
	return 0;
}
