#ifndef WAVHEADER_H
#define WAVHEADER_H

#include <stdio.h>
#include <stdint.h>

// BE - Big Endian  /  LE - Little Endian
// 2 - 2 bytes long /  4 - 4 bytes long

// WAVE file header format
struct ContainerHeader {
	char chunkID[4];			// 4BE RIFF string
	uint32_t chunkSize;			// 4LE (file length - 8) bytes
	char format[4];				// 4BE WAVE string
	char subChunk1ID[4];		// 4BE fmt string with trailing null char
	uint32_t subChunk1Size;		// 4LE length of remaining fmt data (should be 16 bytes)
	uint16_t audioFormat;		// 2LE format type. 1=PCM, (3=IEEE float, 6=8bit A law, 7=8bit mu law)
	uint16_t numChannels;		// 2LE no.of channels. 1=mono, 2=stereo
	uint32_t sampleRate;		// 4LE sampling rate (blocks per second). ex. 44100
	uint32_t byteRate;			// 4LE bytes per second: SampleRate * NumChannels * BytesPerSample
	uint16_t blockAlign;		// 2LE size of a block: NumChannels * BytesPerSample
	uint16_t bitsPerSample;		// 2LE bits per sample, 8=8bits, 16=16 bits etc. ONLY ONE CHANNEL
	char subChunk2ID[4];		// 4BE DATA string
	uint32_t subChunk2Size;		// 4LE data chunk size: NumSamples * NumChannels * BytesPerSample - (size of the next chunk that will be read)
};

int createContainerHeaderStruct(struct ContainerHeader* pContainerHeader, char* pContainerFilepath);

int printContainerHeaderStruct(struct ContainerHeader* pContainerHeader);

int containerHeaderToArr(char* destinationArr, char* pContainerFilepath);
#endif
