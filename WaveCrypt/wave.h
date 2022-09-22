// BE - big endian  /  LE - little endian

// WAVE file header format
struct HEADER {
    unsigned char riff[4];              // BE RIFF string
    unsigned int overall_size;          // LE (file length - 8) bytes
    unsigned char wave[4];              // WAVE string
    unsigned char fmt_chunk_marker[4];  // fmt string with trailing null char
    unsigned int length_of_fmt;         // length of remaining fmt data (should be 16 bytes)
    unsigned int format_type;           // format type. 1=PCM, (3=IEEE float, 6=8bit A law, 7=8bit mu law)
    unsigned int channels;              // no.of channels. 1=mono, 2=stereo
    unsigned int sample_rate;           // sampling rate (blocks per second). ex. 44100
    unsigned int byterate;              // bytes per second: SampleRate * NumChannels * BytesPerSample
    unsigned int block_align;           // size of a block: NumChannels * BytesPerSample
    unsigned int bits_per_sample;       // bits per sample, 8=8bits, 16=16 bits etc
    unsigned char data_chunk_header [4];// DATA string
    unsigned int data_size;             // data chunk size: NumSamples * NumChannels * BytesPerSample - (size of the next chunk that will be read)
};
