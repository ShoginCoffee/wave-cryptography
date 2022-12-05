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


	// List Directories
	char path[] = ".";
	output = listDirectoryContents(path);
	if (output != 0) {
		return 1;
	}
	printf("\n\n");
	



	/* GUI menu
	int nothing = GUIMenu();
	*/


	// User inputs
	char containerFilepath[] = "../AudioFiles/BabyElephantWalk60.wav"; // !!! Change to real location after compiling code !!!
	char messageFilepath[] = "../demo.txt";
	unsigned char encryptionMehtod = 1;


	// Make containerHeader and messageHeader

	// Read in container file and construct containerHeader
	struct ContainerHeader containerHeader;
	output = createContainerHeaderStruct(&containerHeader, containerFilepath);
	if (output != 0) {
		return 1; // for now
	}
	printContainerHeaderStruct(&containerHeader);


	// Read in message file and construct messageHeader
	struct MessageHeader messageHeader;
	output = createMessageHeaderStruct(&messageHeader, messageFilepath, sizeof(messageFilepath), encryptionMehtod);
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
		return 1;
	}

	uint32_t containerDataLength;
	output = fileLength(containerFilepath, &containerDataLength);
	if (output != 0) {
		return 1;
	}

	char* pContainerData = (char*)malloc(containerDataLength);
	output = readInContainerData(pContainerData, containerFilepath, containerHeader.subChunk2Size);
	
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
	for(int b = 0; b < sizeof(plainText); b++) {
		printf("%x ", plainText[b]);
		if((b + 1) % 16 == 0 && b != 0) printf("\n");
	}
	

	chacha20(plainText, sizeof(plainText), key, counter, nonce);
	printf("\n\n");

	
	//Printing encrypted array
	for(int a = 0; a < sizeof(plainText); a++) {
		printf("%x ", plainText[a]);
		if((a + 1) % 16 == 0 && a != 0) printf("\n");
	}
	

	counter[0] = 0x00000001;

	chacha20(plainText, sizeof(plainText), key, counter, nonce);
	printf("\n\n");

	
	//Printing decrypted array
	for(int a = 0; a < sizeof(plainText); a++) {
		printf("%x ", plainText[a]);
		if((a + 1) % 16 == 0 && a != 0) printf("\n");
	}
	

	free(pMessageData);
	free(pContainerData);
	return 0;
}
