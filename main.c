#include "containerHeader.h"
#include "messageHeader.h"
#include "chacha.h"
#include "menuGUI.h"
#include "encoder.h"
#include "util.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

void printb(char binary) {
	for (int i = 0; i < 8; i++) {
		printf("%x", ((binary << i) & 0xff) >> 7);
	}
}

int main(char argc, char* argv[]) {
	int output;
	const char version[] = "1.0";

	char* containerFilepath = "../AudioFiles/BabyElephantWalk60.wav"; // !!! Change to real location after compiling code !!!
	char* messageFilepath = "../demo.txt";
	unsigned char encryptionMehtod = 1;

	// User inputs
	char* pContainerFilepath; // = "../AudioFiles/BabyElephantWalk60.wav"; // !!! Change to real location after compiling code !!!
	char* pMessageFilepath; // = "../demo.txt";
	char* resultName;
	char* pResultFilepath;
	// unsigned char encryptionMehtod; // = 1;


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

		// Check if a container and message file have been input
		if ((sourceIndex == 0) || (dataIndex == 0)) {
			if (sourceIndex == 0) {
				printf("Container file is required.\n");
				return 0;
			}
			printf("Message file is required.\n");
			return 0;
		}
		else {
			pContainerFilepath = (char*)malloc(sizeof(argv[sourceIndex]));
			strcpy(pContainerFilepath, &argv[sourceIndex]);

			pMessageFilepath = (char*)malloc(sizeof(argv[dataIndex]));
			strcpy(pMessageFilepath, &argv[dataIndex]);
		}

		// Check if name for output file has been input else set it to the name of the input file
		if (nameIndex == 0) {
			//TODO: set resultName to input file name
		}
		else {
			resultName = (char*)malloc(sizeof(argv[nameIndex]));
			strcpy(resultName, &argv[nameIndex]);
		}

		// Check if output filepath has been input else set it to the filepath of the input file
		if (outputIndex == 0) {
			//TODO: set pResultFilepath to the same as input
		}
		else {
			pResultFilepath = (char*)malloc(sizeof(argv[outputIndex]));
			strcpy(pResultFilepath, &argv[outputIndex]);
		}

		// Check if encryption method has been input else set it to 0 (no encryption)
		if (encryptionIndex == 0) {
			encryptionMehtod = 0;
		}
		else {
			encryptionMehtod = atoi(argv[encryptionIndex]);
		}
	}

	// List Directories
	char path[] = ".";
	output = listDirectoryContents(path);
	if (output != 0) {
		return 1;
	}
	printf("\n\n");

	// Make containerHeader and messageHeader

	// Declare and construct containerHeader
	struct ContainerHeader containerHeader;
	output = createContainerHeaderStruct(&containerHeader, containerFilepath);
	if (output != 0) {
		return 1; // for now
	}
	printContainerHeaderStruct(&containerHeader);

	// Declare and construct messageHeader
	struct MessageHeader messageHeader;
	output = createMessageHeaderStruct(&messageHeader, messageFilepath, strlen(messageFilepath), encryptionMehtod);
	if (output != 0) {
		return 1; // for now
	}
	printMessageHeaderStruct(&messageHeader);

	// Read message and container file

	// Read in message file and message file length
	
	uint32_t messageDataLength;
	output = fileLength(messageFilepath, &messageDataLength);
	if (output != 0) {
		return 1;
	}

	char* pMessageData = (char*)malloc(messageDataLength);
	output = readInMessageData(pMessageData, messageFilepath);
	if (output != 0) {
		free(pMessageData);
		return 1;
	}
	
	printf("\n%u \n", messageDataLength);
	for (int i = 0; i < messageDataLength; i++) {
		printf("%x ", pMessageData[i]);
	}
	
	printf("\n");

	// Read in container file and container file length
	if (containerHeader.subChunk2Size < 1) {
		printf("main: Container file does not have any data");
		free(pMessageData);
		return 1;
	}

	uint32_t containerDataLength;
	output = fileLength(containerFilepath, &containerDataLength);
	if (output != 0) {
		free(pMessageData);
		return 1;
	}

	char* pContainerData = (char*)malloc(containerDataLength);
	output = readInContainerData(pContainerData, containerFilepath, containerHeader.subChunk2Size);
	
	if (output != 0) {
		free(pMessageData);
		free(pContainerData);
		return 1;
	}

	printf("\n%u \n", containerDataLength);
	for (int i = 0; i < 32; i++) {
		printf("%x ", pContainerData[i]);
	}

	printf("\n");


	
	// encoder
	printf("\n\nENCODER: \n\n");

	unsigned char data[16];
	for (int i = 0; i < sizeof(data); i++) {
		data[i] = 0x00;
	}


	unsigned char message[] = {0xff, 0xff};
	int sampleSize = 2;
	int bitsPerSample = 2;

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
	printf("bits per sample: %d \n\n", bitsPerSample);

	encodeMessage(data, sizeof(data), sampleSize, bitsPerSample, message, sizeof(message));


	printf("output data: \n");
	for(int i = 0; i < sizeof(data); i++){
		printb(data[i]);
		printf("\n");
	}
	
	
	printf("\n");
	
	// chacha20 test
	uint8_t plainText[] = "Ladies and Gentlemen of the class of '99: If I could offer you only one tip for the future, sunscreen would be it.";
	uint32_t key[] = {
		0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c,
		0x13121110, 0x17161514, 0x1b1a1918, 0x1f1e1d1c
		};
	uint32_t counter[] = {0x00000001};
	uint32_t nonce[] = {0x00000000, 0x4a000000, 0x00000000};

	
	//Printing original array
	for(int b = 0; b < strlen(plainText); b++) {
		printf("%x ", plainText[b]);
		if((b + 1) % 16 == 0 && b != 0) printf("\n");
	}
	

	chacha20(plainText, strlen(plainText), key, counter, nonce);
	printf("\n\n");

	
	//Printing encrypted array
	for(int a = 0; a < strlen(plainText); a++) {
		printf("%x ", plainText[a]);
		if((a + 1) % 16 == 0 && a != 0) printf("\n");
	}
	

	counter[0] = 0x00000001;

	chacha20(plainText, strlen(plainText), key, counter, nonce);
	printf("\n\n");

	
	//Printing decrypted array
	for(int a = 0; a < strlen(plainText); a++) {
		printf("%x ", plainText[a]);
		if((a + 1) % 16 == 0 && a != 0) printf("\n");
	}
	

	// TODO: How to free allocated memory that is not always allocated
	free(pMessageData);
	free(pContainerData);
	return 0;
}
