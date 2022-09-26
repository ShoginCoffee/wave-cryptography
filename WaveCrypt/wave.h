// BE - Big Endian  /  LE - Little Endian
// 2 - 2 bytes long /  4 - 4 bytes long

// WAVE file header format
struct HEADER {
    unsigned char riff[4];              // 4BE RIFF string
    unsigned int overall_size;          // 4LE (file length - 8) bytes
    unsigned char wave[4];              // 4BE WAVE string
    unsigned char fmt_chunk_marker[4];  // 4BE fmt string with trailing null char
    unsigned int length_of_fmt;         // 4LE length of remaining fmt data (should be 16 bytes)
    unsigned short format_type;         // 2LE format type. 1=PCM, (3=IEEE float, 6=8bit A law, 7=8bit mu law)
    unsigned short channels;            // 2LE no.of channels. 1=mono, 2=stereo
    unsigned short sample_rate;         // 2LE sampling rate (blocks per second). ex. 44100
    unsigned short byterate;            // 2LE bytes per second: SampleRate * NumChannels * BytesPerSample
    unsigned short block_align;         // 2LE size of a block: NumChannels * BytesPerSample
    unsigned short bits_per_sample;     // 2LE bits per sample, 8=8bits, 16=16 bits etc
    unsigned char data_chunk_header [4];// 4BE DATA string
    unsigned int data_size;             // 4LE data chunk size: NumSamples * NumChannels * BytesPerSample - (size of the next chunk that will be read)
};
