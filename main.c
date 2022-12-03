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
	const char version[] = "1.0";


	// User inputs
	char containerFilepath[] = "../AudioFiles/BabyElephantWalk60.wav"; // !!! Change to real location after compiling code !!!
	char messageFilepath[] = "../demo.txt";
	unsigned char encryptionMehtod = 1;


	/* Get the directory where the executable is located
	char string[255];
	GetCurrentDirectory(255, &string);
	printf("%s\n", string);
	*/

	// GUI or CLI
	if (argc == 1) {
		// GUI menu
		// int nothing = GUIMenu();

		/* List Directories
		char path[] = ".";
		listDirectoryContents(path);
		*/
	}
	else {
		char sourceIndex = 0;
		char dataIndex = 0;
		char nameIndex = 0;
		char outputIndex = 0;
		char encryptionIndex = 0;

		// Goes througth argv and reads in the options given by the user
		int i = 0;
		while (i < argc) {
			if ((strcmp(argv[i], "-h") == 0) || (strcmp(argv[i], "-H") == 0) || (strcmp(argv[i], "--help") == 0)) {
				printf("usage: crypteo [--help] [--version] [--source <path>] [--data <path>] [--name <value>] [--output <path>] [--encryption <value>]\n\nHelp: documentation of usage of the program [--help] [-h] [-H]\n\nVersion: the version of the program [--version] [-v] [-V]\n\nSource: path to the container file that the data is is to be hidden within [--source] [-s] [-S]\n\nData: path to the message file that has the data that is to be hidden within the container file [--data] [-d] [-D]\n\nName: the name of the new file that has the data hidden within it (without file extension) [--name] [-n] [-N]\n\nOutput: path to the where the new file is to be created [--output] [-o] [-O]\n\nEncryption: if the hidden data is to be encrypted and by which algorithm, if no argument is given it defaults to no encryption. 0: no encryption, 1: chacha20. [--encryption] [-e] [-E]");
				return 0;
			}
			else if ((strcmp(argv[i], "-v") == 0) || (strcmp(argv[i], "-V") == 0) || (strcmp(argv[i], "--version") == 0)) {
				printf(version);
				return 0;
			}
			else if ((strcmp(argv[i], "-s") == 0) || (strcmp(argv[i], "-S") == 0) || (argv[i], "--source" == 0)) {
				sourceIndex = i + 1;
				i++;
			}
			else if ((strcmp(argv[i], "-d") == 0) || (strcmp(argv[i], "-D") == 0) || (argv[i], "--data" == 0)) {
				dataIndex = i + 1;
				i++;
			}
			else if ((strcmp(argv[i], "-n") == 0) || (strcmp(argv[i], "-N") == 0) || (argv[i], "--name" == 0)) {
				nameIndex = i + 1;
				i++;
			}
			else if ((strcmp(argv[i], "-o") == 0) || (strcmp(argv[i], "-O") == 0) || (argv[i], "--output" == 0)) {
				outputIndex = i + 1;
				i++;
			}
			else if ((strcmp(argv[i], "-e") == 0) || (strcmp(argv[i], "-E") == 0) || (argv[i], "--encryption" == 0)) {
				encryptionIndex = i + 1;
				i++;
			}

			i++;
		}


		/* Print out arguments passed to program
		printf("SOURCE: %s\n", argv[sourceIndex]);
		printf("DATA: %s\n", argv[dataIndex]);
		printf("NAME: %s\n", argv[nameIndex]);
		printf("OUTPUT: %s\n", argv[outputIndex]);
		printf("ENCRYPTION: %s\n", argv[encryptionIndex]);
		printf("\n");
		*/
	}


	// Make containerHeader and messageHeader

	// Declare and construct containerHeader
	struct ContainerHeader containerHeader;
	if (createContainerHeaderStruct(&containerHeader, containerFilepath) != NULL) {
		// printContainerHeaderStruct(&containerHeader);
	}
	else {
		return 0;
	}

	// Declare and construct messageHeader
	struct MessageHeader messageHeader;
	if (createMessageHeaderStruct(&messageHeader, messageFilepath, sizeof(messageFilepath), encryptionMehtod) != NULL) {
		// printMessageHeaderStruct(&messageHeader);
	}
	else {
		return 0;
	}


	// Read message and container file

	// Read in message file and message file length
	char* pMessageData = NULL;
	unsigned int messageDataLength = fileLength(messageFilepath);
	if (messageDataLength != 0) {
		pMessageData = (char*)malloc(messageDataLength);
		readInMessageData(pMessageData, messageFilepath);
	}
	else {
		printf("main: Data file couldn't be found at given filepath");
		free(pMessageData);
		return 0;
	}


	// Read in container file and container file length
	char* pContainerData = NULL;
	unsigned int containerDataLength = fileLength(containerFilepath);
	if (containerDataLength != 0) {
		pContainerData = (char*)malloc(containerDataLength);
		readInContainerData(pContainerData, containerFilepath, containerHeader.subChunk2Size);
	}
	else {
		printf("main: Container file couldn't be found at given filepath");
		free(pMessageData);
		free(pContainerData);
		return 0;
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

	// TODO: How to free allocated memory that is not always allocated
	free(pMessageData);
	free(pContainerData);
	return 0;
}
