// BE - Big Endian  /  LE - Little Endian
// 2 - 2 bytes long /  4 - 4 bytes long

// WAVE file header format
struct HEADER {
    unsigned char ChunkID[4];               // 4BE RIFF string
    unsigned int ChunkSize;                 // 4LE (file length - 8) bytes
    unsigned char Format[4];                // 4BE WAVE string
    unsigned char Subchunk1ID[4];           // 4BE fmt string with trailing null char
    unsigned int Subchunk1Size;             // 4LE length of remaining fmt data (should be 16 bytes)
    unsigned short AudioFormat;             // 2LE format type. 1=PCM, (3=IEEE float, 6=8bit A law, 7=8bit mu law)
    unsigned short NumChannels;             // 2LE no.of channels. 1=mono, 2=stereo
    unsigned int SampleRate;                // 4LE sampling rate (blocks per second). ex. 44100
    unsigned int ByteRate;                  // 2LE bytes per second: SampleRate * NumChannels * BytesPerSample
    unsigned short BlockAlign;              // 2LE size of a block: NumChannels * BytesPerSample
    unsigned short BitsPerSample;           // 2LE bits per sample, 8=8bits, 16=16 bits etc
    unsigned char Subchunk2ID[4];           // 4BE DATA string
    unsigned int Subchunk2Size;             // 4LE data chunk size: NumSamples * NumChannels * BytesPerSample - (size of the next chunk that will be read)
};
