#ifndef WAVHEADER_H
#define WAVHEADER_H
#include <stdio.h>
// BE - Big Endian  /  LE - Little Endian
// 2 - 2 bytes long /  4 - 4 bytes long

// WAVE file header format
struct ContainerHeader {
	char chunkID[4];                        // 4BE RIFF string
	unsigned int chunkSize;                 // 4LE (file length - 8) bytes
	char format[4];                         // 4BE WAVE string
	char subChunk1ID[4];                    // 4BE fmt string with trailing null char
	unsigned int subChunk1Size;             // 4LE length of remaining fmt data (should be 16 bytes)
	unsigned short audioFormat;             // 2LE format type. 1=PCM, (3=IEEE float, 6=8bit A law, 7=8bit mu law)
	unsigned short numChannels;             // 2LE no.of channels. 1=mono, 2=stereo
	unsigned int sampleRate;                // 4LE sampling rate (blocks per second). ex. 44100
	unsigned int byteRate;                  // 2LE bytes per second: SampleRate * NumChannels * BytesPerSample
	unsigned short blockAlign;              // 2LE size of a block: NumChannels * BytesPerSample
	unsigned short bitsPerSample;           // 2LE bits per sample, 8=8bits, 16=16 bits etc
	char subChunk2ID[4];                    // 4BE DATA string
	unsigned int subChunk2Size;             // 4LE data chunk size: NumSamples * NumChannels * BytesPerSample - (size of the next chunk that will be read)
};

struct ContainervHeader* createContainerHeaderStruct(struct ContainerHeader* pContainerHeader, char* pContainerFilepath);

int printContainerHeaderStruct(struct ContainerHeader* pContainerHeader);
#endif
