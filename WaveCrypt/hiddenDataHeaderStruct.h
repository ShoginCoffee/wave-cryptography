
struct HIDDENDATAHEADER {
    unsigned short Subchunk1Size;           // The length of the header in bytes
    unsigned char Filename[64];             // The name of the file including the file type
    unsigned short Subchunk2Size;           // The length of the data encrypted in the wave file (excluding the header) in bytes
};
