#include "encoder.h"
#include "containerHeader.h"
#include "messageHeader.h"
#include "util.h"
#include "constants.h"
#include "chacha.h"

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <math.h>

int encodeMessageInWavFile(int encryptionMethod, char* resultFilepath, char* containerFilepath, char* messageFilepath, int encodingBitsPerSample, uint32_t* key, uint32_t* counter, uint32_t* nonce) {
	int output;
	
	// INITIALIZE HEADER STRUCTS (CONTAINER & MESSAGE)

	// Construct containerHeader
	struct ContainerHeader containerHeader;
	printf("container filepath: %s .\n", containerFilepath);
	output = createContainerHeaderStruct(&containerHeader, containerFilepath);
	printf("output: %d \n", output);
	if (output != 0) {
		return 1; // for now
	}
	printContainerHeaderStruct(&containerHeader);

	// Construct messageHeader
	struct MessageHeader messageHeader;
	output = createMessageHeaderStruct(&messageHeader, messageFilepath, strlen(messageFilepath), encryptionMethod, encodingBitsPerSample);
	if (output != 0) return 1;

	printMessageHeaderStruct(&messageHeader);
	printf("messageFilepath: [%s] \n", messageFilepath);
	printf("resultFilenameLength: %d \n", messageHeader.filenameLength);

	// Check if container file contains any data
	if (containerHeader.subChunk2Size < 1) {
		printf("encodeMessageInWavFile(): Container file does not have any data");
		return 1;
	}

	// Message data length
	uint32_t messageDataLength;
	output = fileLength(messageFilepath, &messageDataLength);
	if (output != 0) {
		return 1;
	}

	// Container data length
	uint32_t containerDataLength;
	output = fileLength(containerFilepath, &containerDataLength);
	if (output != 0) {
		return 1;
	}

	// create resultFile
	errno = 0;
	FILE* pResultFile = fopen(resultFilepath, "wb");

	if (pResultFile == 0) {
		printf("error in encodeMessageInWavFile(): \n");
		printf("Could not create file in filepath[%s]: \n%s \n", resultFilepath, strerror(errno));
		return 1;
	}

	// WRITE WAV HEADER TO RESULT

	char wavHeader[WAV_HEADER_SIZE];
	output = containerHeaderToArr(wavHeader, containerFilepath);
	if (output != 0) return 1;
	fwrite(wavHeader, WAV_HEADER_SIZE, 1, pResultFile);

	// INITIALIZE VARIABLES

	// encoder data arrays and constants
	const uint32_t MESSAGE_READ_BUFFER_SIZE = (CONTAINER_READ_BUFFER_SIZE / 8) * encodingBitsPerSample;
	char* pContainerData = (char*)malloc(CONTAINER_READ_BUFFER_SIZE);
	char* pMessageData = (char*)malloc(MESSAGE_READ_BUFFER_SIZE);
	const int sampleSize = (containerHeader.bitsPerSample / 8) / containerHeader.numChannels;
	const int encodingBitsPerSampleMessageHeader = 1; // how many bits to use per sample for encoding message header

	// message header size
	const int resultFilenameSize = messageHeader.filenameLength;
	const int resultFileExtensionSize = messageHeader.fileExtensionLength;
	printf("resultFilenameSize: %d \n", resultFilenameSize);
	printf("resultFileExtensionSize: %d \n", resultFileExtensionSize);


	const int encryptionMethodSize = sizeof(messageHeader.encryptionMethod);
	const int messageHeaderSize = sizeof(messageHeader.chunkSize) + sizeof(messageHeader.subChunk1Size) +
		sizeof(messageHeader.filenameLength) + sizeof(messageHeader.fileExtensionLength) +
		sizeof(messageHeader.bitsPerSample) + sizeof(messageHeader.subChunk2Size) +
		resultFilenameSize + resultFileExtensionSize;		// size of all variables in messageHeader except encryptionMethod

	// check if messagefile will not fit to encode in container
	int containerBytesForMessage = messageHeaderSize + encryptionMethodSize; // message header size in bytes
	containerBytesForMessage *= 8 * sampleSize; // bytes from container to encode message header
	containerBytesForMessage += ceil((float)(messageDataLength * 8) / encodingBitsPerSample);
	if (containerBytesForMessage > containerHeader.subChunk2Size) {
		printf("error in encodeMessageInWavFile()\n");
		printf("message too long for container and settings\n");
		printf("encoding %d bits per sample in container.\n", encodingBitsPerSample);
		printf("container has a sample size of %d bytes\n", sampleSize);
		printf("message header has size of %d bytes\n", encryptionMethodSize + messageHeaderSize);
		printf("message file has size of %ul bytes\n", messageDataLength);
		printf("container file has size of %ul bytes\n", containerHeader.subChunk2Size);
		printf("bytes needed to encode message %ul\n", containerBytesForMessage);
		return 1;
	}


	// ENCODE ENCRYPTIONMETHOD IN RESULT

	printf("encryptionMethod: \n");
	printf("encryptionMethodSize: %d \n", encryptionMethodSize);

	uint64_t containerBytesLeft = containerDataLength;
	uint64_t containerBytesRead = 0;

	printf("containerBytesLeft: %zu \n", containerBytesLeft);
	printf("containerBytesRead: %zu \n", containerBytesRead);

	uint32_t containerReadBufferSize = encryptionMethodSize * 8 * sampleSize; // how many bytes needed from container to encode encryptionMethod variable
	printf("containerReadBufferSize: %u \n", containerReadBufferSize);

	// ENCODE HEADER IN RESULT

	// read container data for encoding encryptionMethod from messageHeader
	output = readInContainerData(pContainerData, containerFilepath, containerReadBufferSize, containerBytesRead); // CONTAINER_BUFFER
	if (output != 0) return 1;
	containerBytesRead += containerReadBufferSize;
	containerBytesLeft -= containerReadBufferSize;

	printf("containerBytesLeft: %zu \n", containerBytesLeft);
	printf("containerBytesRead: %zu \n", containerBytesRead);

	printf("pContainerData: \n");
	for (int i = 0; i < containerReadBufferSize; i += 1) {
		printb(pContainerData[i]);
		printf(" ");
	}
	printf("\n");

	printf("pMessageData: \n");
	for (int i = 0; i < encryptionMethodSize; i++) {
		printb(messageHeader.encryptionMethod);
		printf(" ");
	}
	printf("\n");


	// encode encryptionMethod in container data
	encodeMessage(pContainerData, containerReadBufferSize, sampleSize, encodingBitsPerSampleMessageHeader, &(messageHeader.encryptionMethod), encryptionMethodSize);


	printf("pContainerData: \n");
	for (int i = 0; i < containerReadBufferSize; i += 1) {
		printb(pContainerData[i]);
		printf(" ");
	}
	printf("\n");

	// write encryptionMethod to result file
	fwrite(pContainerData, containerReadBufferSize, 1, pResultFile);




	printf("message header: \n");

	uint32_t messageReadBufferSize = messageHeaderSize; // size of message header variables in bytes w/o encryptionMethod
	containerReadBufferSize = messageReadBufferSize * 8 * sampleSize; // how many bytes to read from container to encode message header w/o encryptionMethod
	printf("messageReadBufferSize: %u \n", messageReadBufferSize);

	printf("messageHeaderSize: %d \n", messageHeaderSize);
	printf("containerReadBufferSize: %u \n", containerReadBufferSize);

	// read container data for message header data (w/o encryptionMethod)
	output = readInContainerData(pContainerData, containerFilepath, containerReadBufferSize, containerBytesRead); // CONTAINER_BUFFER
	if (output != 0) return 1;
	containerBytesLeft -= containerReadBufferSize;
	containerBytesRead += containerReadBufferSize;

	// read message header data from struct to array
	messageStructToArr(pMessageData, &messageHeader); // reads in encryptionMethod too

	// if encryption
	printf("encryptionMethod: %u \n", messageHeader.encryptionMethod);
	if (encryptionMethod) {	// 1 = chacha20
		chacha20(pMessageData + encryptionMethodSize, messageReadBufferSize, key, counter, nonce);
	}


	printf("pContainerData: \n");
	for (int i = 1; i < containerReadBufferSize; i += 2) {
		printb(pContainerData[i]);
		printf(" ");
	}
	printf("\n");

	printf("pMessageData: \n");
	for (int i = 1; i < messageReadBufferSize + 1; i++) {
		printb(pMessageData[i]);
		printf(" ");
	}
	printf("\n");



	// encode message header in container data
	encodeMessage(pContainerData, containerReadBufferSize, sampleSize, encodingBitsPerSampleMessageHeader, pMessageData + encryptionMethodSize, messageReadBufferSize);

	printf("pContainerData: \n");
	for (int i = 1; i < containerReadBufferSize; i += 2) {
		printb(pContainerData[i]);
		printf(" ");
	}
	printf("\n");


	// write message header to result file w/o encryptionMethod
	fwrite(pContainerData + encryptionMethodSize, containerReadBufferSize, 1, pResultFile);

	printf("containerBytesLeft: %zu \n", containerBytesLeft);
	printf("containerBytesRead: %zu \n", containerBytesRead);

	printf("message: \n");

	// ENCODE MESSAGE FILE IN RESULT

	uint64_t messageBytesLeft = messageDataLength;
	uint64_t messageBytesRead = 0;

	while (containerBytesLeft > 0) {
		// update readability-improving variables
		containerBytesRead = containerDataLength - containerBytesLeft;
		messageBytesRead = messageDataLength - messageBytesLeft;

		// setup container/message read buffer sizes (to not over read/encode)
		if (containerBytesLeft >= CONTAINER_READ_BUFFER_SIZE) {
			containerReadBufferSize = CONTAINER_READ_BUFFER_SIZE;
		}
		else {
			containerReadBufferSize = containerBytesLeft;
		}

		if (messageBytesLeft >= MESSAGE_READ_BUFFER_SIZE) {
			messageReadBufferSize = MESSAGE_READ_BUFFER_SIZE;
		}
		else if (messageBytesLeft <= 0) {
			messageReadBufferSize = 0; // is this needed? Probably i think
			messageBytesLeft = 0;
		}
		else {
			messageReadBufferSize = messageBytesLeft;
		}

		// DEBUGGG
		printf("containerBytesLeft: %zu \n", containerBytesLeft);
		printf("containerReadBufferSize: %lu \n", containerReadBufferSize);
		printf("messageBytesLeft: %zu \n", messageBytesLeft);
		printf("messageReadBufferSize: %lu \n", messageReadBufferSize);

		// read chunk from container
		output = readInContainerData(pContainerData, containerFilepath, containerReadBufferSize, containerBytesRead); // CONTAINER_BUFFER
		if (output != 0) return 1;

		/* DEBUGGG
		printf("pContainerData: \n");
		for (int i = 1; i < messageReadBufferSize * 8 * 2; i += 2) {
			printb(pContainerData[i]);
			printf(" ");
		}
		printf("\n");
		*/

		// read chunk from message
		output = readInMessageData(pMessageData, messageFilepath, messageReadBufferSize, messageBytesRead); // MESSAGE_BUFFER
		if (output != 0) return 1;

		/* DEBUGGG
		printf("pMessageData: \n");
		for (int i = 0; i < messageReadBufferSize; i++) {
			printb(pMessageData[i]);
			printf(" ");
		}
		printf("\n");
		*/

		// encrypt message
		if (encryptionMethod) { // chacha20
			chacha20(pMessageData, messageReadBufferSize, key, counter, nonce);
		}

		// encode message in container data
		encodeMessage(pContainerData, containerReadBufferSize, sampleSize, encodingBitsPerSample, pMessageData, messageReadBufferSize);

		/* DEBUGGG
		printf("pContainerData: \n");
		for (int i = 1; i < messageReadBufferSize * 8 * 2; i += 2) {
			printb(pContainerData[i]);
			printf(" ");
		}
		printf("\n");
		*/

		// write result to result file
		fwrite(pContainerData, containerReadBufferSize, 1, pResultFile);

		// update counters
		containerBytesLeft -= containerReadBufferSize;
		messageBytesLeft -= messageReadBufferSize;
	}
	free(pMessageData);
	free(pContainerData);

	return 0;
}

void encodeMessage(char* containerData, int containerDataLength, int sampleSize, int bitsPerSample, char* message, unsigned int messageLength) {
	// SIDENOTE: DEBUG PRINTF ARE COMMENTED OUT BUT ARE STILL IN THE FUNCTION, REMOVE WHEN NECESSARY.
    int i, t;
    int messageByte = 0, messageBit = 0;
    unsigned char bit;
    // loop all samples
    for(i = sampleSize - 1; i < containerDataLength; i += sampleSize){
        // for every sample
        if(messageLength != messageByte){
            for(t = 0; t < bitsPerSample; t++){
                // 1. take bit from message byte
                bit = (( *(message + messageByte) << messageBit) & 0xff) >> 7;
                // 2. encode bit in data
                encodeBitInByte((containerData + i), bit, t);
				// 3. update messageData counters
                if(messageBit + 1 == 8) messageByte++;
                messageBit = (messageBit + 1) % 8;
                // 4. if message has been encoded: break
                if(messageByte == messageLength) break;
            }
        }
    }
}

void encodeBitInByte(unsigned char* byte, int bit, int positionInByte) {
	// 1. set positionInByte bit in "byte" to 0
	// 2. make new byte with all zeros except bit in positionInByte is set to "bit" value
	// 3. bitwise or the two bytes
	unsigned char byteCopy = *byte;
	*(byte) = (byteCopy & ~(1 << positionInByte)) | (bit << positionInByte);
}

//Takes in the filepath and returns a pointer to a char array with the data
int readInMessageData(char* pMessageData, char* pFilepath, uint32_t bufferSize, uint32_t offsetBytes) {
	errno = 0;
	FILE* pMessageFile = fopen(pFilepath, "rb");

	if (pMessageFile == NULL) {
		printf("error in readInMessageData(): \n");
		printf("Could not access file in filepath[%s]: \n%s \n", pFilepath, strerror(errno));
		return 1;
	}
	
	/*
	printf((messageLength == 0) ? "\nmessageLength is null" : "\nmessageLength is not null");
	printf((pMessageData == 0) ? "\npMessageData is null" : "\npMessageData is not null");
	printf((pMessageFile == NULL) ? "\npMessageFile is null" : "\npMessageFile is not null");
	*/

	if (offsetBytes > 0) {
		char* buffer = malloc(offsetBytes);
		fread(buffer, offsetBytes, 1, pMessageFile); // Moving the "cursor" to where the data starts
		free(buffer);
	}

	fread(pMessageData, bufferSize, 1, pMessageFile);

	


	// printf((pMessageFile == NULL) ? "\npMessageFile is null" : "\npMessageFile is not null");

	fclose(pMessageFile);
	return 0;
}

int readInContainerData(char* pContainerData, char* pFilepath, uint32_t bufferSize, uint32_t offsetBytes) {
	errno = 0;
	FILE* pFile = fopen(pFilepath, "rb");

	if (pFile == NULL) {
		printf("error in readInContainerData(): \n");
		printf("Could not access file in filepath[%s]: \n%s \n", pFilepath, strerror(errno));
		return 1;
	}

	if (offsetBytes > 0) {
		char* buffer = malloc(WAV_HEADER_SIZE + offsetBytes);
		fread(buffer, WAV_HEADER_SIZE + offsetBytes, 1, pFile); // Moving the "cursor" to where the data starts
		free(buffer);
	}
	else {
		char buffer[WAV_HEADER_SIZE];
		fread(buffer, WAV_HEADER_SIZE, 1, pFile); // Moving the "cursor" to where the data starts
	}

	fread(pContainerData, bufferSize, 1, pFile);

	fclose(pFile);
	return 0;
}

