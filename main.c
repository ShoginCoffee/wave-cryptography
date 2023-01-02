#include "containerHeader.h"
#include "messageHeader.h"
#include "chacha.h"
#include "menuGUI.h"
#include "encoder.h"
#include "util.h"
#include "constants.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

// returns 1 if ANY of the strings match the arg ELSE returns 0
int argCmp(int numberOfStrings, const char** compareStrings, char* compareArg) {
	int cmpResult;
	for (int i = 0; i < numberOfStrings; i++) {
		cmpResult = strcmp(compareArg, compareStrings[i]);
		if (cmpResult == 0) return 1;
	}

	return 0;
}

int main(char argc, char* argv[]) {
	int output;
	const char version[] = "1.0";


	// User inputs
	char* containerFilepath;
	char* messageFilepath;
	char* resultName;
	char* resultDirectory;
	unsigned char encryptionMethod;
	unsigned int chachaBlock[3];
	unsigned int isDecode;


	// Argument variations
	const ARGUMENT_VARIATIONS = 3;
	char const* helpArguments[] = { "-h", "-H", "--help" };
	char const* versionArguments[] = { "-v", "-V", "--version" };
	char const* containerArguments[] = { "-c", "-C", "--container" };
	char const* messageArguments[] = { "-m", "-M", "--message" };
	char const* resultNameArguments[] = { "-n", "-N", "--name" };
	char const* resultDirectoryArguments[] = { "-r", "-R", "--result" };
	char const* encryptionArguments[] = { "-s", "-E", "--encryption" };
	char const* chachaArguments[] = { "--chacha", "--chacha", "--chacha" };
	char const* decodeArguments[] = { "--decode", "--decode", "--decode" };


	// GUI or CLI
	if (argc == 1) {
		printf("GUI");
		return 0;

		// GUI menu
		// int nothing = GUIMenu();

		/* List Directories
		char path[] = ".";
		listDirectoryContents(path);
		*/
	}
	else {
		char containerIndex = 0;
		char messageIndex = 0;
		char nameIndex = 0;
		char resultIndex = 0;
		char encryptionIndex = 0;
		char chachaIndex = 0;
		char decodeIndex = 0;

		// Goes througth argv and reads in the options given by the user
		int i = 0;
		while (i < argc) {
			printf("arg: %s \n", argv[i]);
			if (argCmp(ARGUMENT_VARIATIONS, helpArguments, argv[i])) {
				printf("usage: crypteo [--help] [--version] [--container <path>] [--message <path>] [--name <value>] [--result <path>] [--encryption <value>] [--chacha <value> <value> <value>]\n\nHelp: documentation of usage of the program [--help] [-h] [-H]\n\nVersion: the version of the program [--version] [-v] [-V]\n\nContainer: path to the container file that the data is is to be hidden within [--container] [-c] [-C]\n\nMessage: path to the message file that has the data that is to be hidden within the container file [--message] [-m] [-M]\n\nName: the name of the new file that has the data hidden within it (without file extension) [--name] [-n] [-N]\n\nResult: Directory where the new file is to be created [--result] [-r] [-R]\n\nEncryption: if the hidden data is to be encrypted and by which algorithm, if no argument is given it defaults to no encryption. 0: no encryption, 1: chacha20. [--encryption] [-e] [-E]\n\nChacha: takes in the key, counter and nonce for the chacha encryption. These should be inputed in hexadecimal. [--encryption] [-e] [-E]");
				return 0;
			}
			else if (argCmp(ARGUMENT_VARIATIONS, versionArguments, argv[i])) {
				printf(version);
				return 0;
			}
			else if (argCmp(ARGUMENT_VARIATIONS, containerArguments, argv[i])) {
				if (argc > i + 1) {
					containerIndex = i + 1;
				}
			}
			else if (argCmp(ARGUMENT_VARIATIONS, messageArguments, argv[i])) {
				if (argc > i + 1) {
					messageIndex = i + 1;
				}
			}
			else if (argCmp(ARGUMENT_VARIATIONS, resultNameArguments, argv[i])) {
				if (argc > i + 1) {
					nameIndex = i + 1;
				}
			}
			else if (argCmp(ARGUMENT_VARIATIONS, resultDirectoryArguments, argv[i])) {
				if (argc > i + 1) {
					resultIndex = i + 1;
				}
			}
			else if (argCmp(ARGUMENT_VARIATIONS, encryptionArguments, argv[i])) {
				if (argc > i + 1) {
					encryptionIndex = i + 1;
				}
			}
			else if (argCmp(1, chachaArguments, argv[i])) {
				if (argc > i + 3) {
					chachaIndex = i + 1;
				}
			}
			else if (argCmp(1, decodeArguments, argv[i])) {
				decodeIndex = i;
			}

			i++;
		}


		// Check if a container and message file have been input
		if ((containerIndex == 0) || (messageIndex == 0)) {
			if (containerIndex == 0) {
				printf("Container file is required.\n");
				return 0;
			}
			printf("Message file is required.\n");
			return 0;
		}
		else {
			containerFilepath = (char*)malloc(strlen(argv[containerIndex]));
			strcpy(containerFilepath, argv[containerIndex]);

			messageFilepath = (char*)malloc(strlen(argv[messageIndex]));
			strcpy(messageFilepath, argv[messageIndex]);
		}

		// Check if name for output file has been input; else set it to the name of the input file
		if (nameIndex == 0) {
			char resultNamePrefix[] = "enc_";
			char* containerFilenameAndExtension = strrchr(containerFilepath, '/') + 1;
			char* containerExtension = strrchr(containerFilenameAndExtension, '.');
			size_t containerFilenameLength = containerExtension - containerFilenameAndExtension;
			printf("filenameLength: %zu \n", containerFilenameLength);
			resultName = (char*)malloc(strlen(resultNamePrefix) + containerFilenameLength + 1); // trailing \0 char
			memcpy(resultName, resultNamePrefix, strlen(resultNamePrefix));
			memcpy(resultName + strlen(resultNamePrefix), containerFilenameAndExtension, containerFilenameLength);
			resultName[containerFilenameLength + strlen(resultNamePrefix)] = '\0';
		}
		else {
			resultName = (char*)malloc(strlen(argv[nameIndex]));
			strcpy(resultName, argv[nameIndex]);
		}

		// Check if output filepath has been input else set it to the filepath of the input file
		if (resultIndex == 0) {
			resultDirectory = (char*)malloc(strlen(argv[containerIndex]));
			strcpy(resultDirectory, argv[containerIndex]);
			// TODO: remove file from filepath
			char* pFileName = strrchr(argv[containerIndex], '/') + 1;
			int pFilepathLength = pFileName - argv[containerIndex];
			resultDirectory[pFilepathLength] = '\0';
		}
		else {
			resultDirectory = (char*)malloc(strlen(argv[resultIndex]));
			strcpy(resultDirectory, argv[resultIndex]);
			if (resultDirectory[strlen(resultDirectory) - 1] != '/') {
				strcat(resultDirectory, "/");
			}
		}

		// Check if encryption method has been input else set it to 0 (no encryption)
		if (encryptionIndex == 0) {

			encryptionMethod = 0;
		}
		else {
			encryptionMethod = atoi(argv[encryptionIndex]);

			if (encryptionMethod == 1) {
				// Check if chacha block have been input
				if (chachaIndex == 0) {
					printf("Chacha arguments are required");
					return  0;
				}
				else {
					for (int i = 0; i < 3; i++) {
						chachaBlock[i] = atoi(argv[chachaIndex + i]);
					}
				}


			}


		}

		// Check if user wants to decode or encode
		isDecode = decodeIndex == 0;


		/* Print out arguments passed to program */
		printf("Variables: \n");
		printf("container filepath:	%s \n", containerFilepath);
		printf("message filepath:	%s \n", messageFilepath);
		printf("result directory:	%s \n", resultDirectory);
		printf("result name:		%s \n", resultName);
		printf("encryption method:	%d \n", encryptionMethod);
		printf("chacha key:	        %d \n", chachaBlock[0]);
		printf("chacha counter:	        %d \n", chachaBlock[1]);
		printf("chacha nonce:	        %d \n", chachaBlock[2]);
		printf("\n");

		/*// Get the directory where the executable is located
		char string[255];
		GetCurrentDirectory(255, &string);
		printf("%s\n", string);
		*/


		// TEMP. TODO: MAKE ARGUMENTS
		uint32_t key[] = {
		0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c,
		0x13121110, 0x17161514, 0x1b1a1918, 0x1f1e1d1c
		};
		uint32_t counter[] = { 0x00000001 };
		uint32_t nonce[] = { 0x00000000, 0x4a000000, 0x00000000 };


		int encodingBitsPerSample = 2; // TEMP. TODO: MAKE ARGUMENT

		// resultFullPath is filepath + name + extension for result file
		int resultFullPathLength = strlen(resultDirectory) + strlen(resultName) + strlen(".wav");

		/*
		if (resultDirectory[strlen(resultDirectory) - 1] != '/' && resultDirectory[strlen(resultDirectory) - 1] != '\\') {
			resultFullPathLength++;			// REMOVE IF: a '/' is guranteed in resultDirectory
		}*/

		char* resultFullPath = malloc(resultFullPathLength);
		strcpy(resultFullPath, resultDirectory);

		/*
		if (resultDirectory[strlen(resultDirectory) - 1] != '/' && resultDirectory[strlen(resultDirectory) - 1] != '\\') {
			strcat(resultFullPath, "/");	// REMOVE IF: a '/' is guranteed in resultDirectory
		}*/

		strcat(resultFullPath, resultName);
		strcat(resultFullPath, ".wav");

		printf("resultFullPath: %s \n", resultFullPath);

		output = encodeMessageInWavFile(encryptionMethod, resultFullPath, containerFilepath, messageFilepath, encodingBitsPerSample, key, counter, nonce);
		if (output != 0) return 1;

		return 0;
	}


	/*
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
	output = createContainerHeaderStruct(&containerHeader, pContainerFilepath);
	if (output != 0) {
		return 1; // for now
	}
	printContainerHeaderStruct(&containerHeader);

	// Declare and construct messageHeader
	struct MessageHeader messageHeader;
	output = createMessageHeaderStruct(&messageHeader, messageFilepath, strlen(messageFilepath), encryptionMethod);
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
	output = readInMessageData(pMessageData, messageFilepath, messageDataLength, 0);
	if (output != 0) {
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
	output = fileLength(pContainerFilepath, &containerDataLength);
	if (output != 0) {
		return 1;
	}

	char* pContainerData = malloc(containerDataLength);
	output = readInContainerData(pContainerData, pContainerFilepath, containerHeader.subChunk2Size, 0);

	if (output != 0) {
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


	unsigned char message[] = { 0xff, 0xff };
	int sampleSize = 2;
	int encodingBitsPerSample = 2;

	printf("input data: \n");
	for (int i = 0; i < sizeof(data); i++) {
		printb(data[i]);
		printf("\n");
	}
	printf("\n");

	printf("input message: \n");
	for (int i = 0; i < sizeof(message); i++) {
		printb(message[i]);
		printf("\n");
	}
	printf("\n");

	printf("sample size: %d \n", sampleSize);
	printf("bits per sample: %d \n\n", encodingBitsPerSample);

	encodeMessage(data, sizeof(data), sampleSize, encodingBitsPerSample, message, sizeof(message));


	printf("output data: \n");
	for (int i = 0; i < sizeof(data); i++) {
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
	uint32_t counter[] = { 0x00000001 };
	uint32_t nonce[] = { 0x00000000, 0x4a000000, 0x00000000 };


	//Printing original array
	for (int b = 0; b < strlen(plainText); b++) {
		printf("%x ", plainText[b]);
		if ((b + 1) % 16 == 0 && b != 0) printf("\n");
	}


	chacha20(plainText, strlen(plainText), key, counter, nonce);
	printf("\n\n");


	//Printing encrypted array
	for (int a = 0; a < strlen(plainText); a++) {
		printf("%x ", plainText[a]);
		if ((a + 1) % 16 == 0 && a != 0) printf("\n");
	}


	counter[0] = 0x00000001;

	chacha20(plainText, strlen(plainText), key, counter, nonce);
	printf("\n\n");


	//Printing decrypted array
	for (int a = 0; a < strlen(plainText); a++) {
		printf("%x ", plainText[a]);
		if ((a + 1) % 16 == 0 && a != 0) printf("\n");
	}
	*/

	return 0;
}
