#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "wave.h"

// takes char and prints bits
void printb(char val) {
  for (int i = 7; 0 <= i; i--) {
    printf("%c", (val & (1 << i)) ? '1' : '0');
  }
}

unsigned short charLEToShortBE(unsigned char charArray[2]) {
    return charArray[0] | charArray[1] << 8;
}

unsigned int charLEToIntBE(unsigned char charArray[4]) {
    return charArray[0] | charArray[1] << 8 | charArray[2] << 16 | charArray[3] << 24;
}

unsigned char * shortBEToCharLE (unsigned short num) {
    unsigned char* pCharArray = malloc(2);

    pCharArray[0] = num & 0xFF;
    pCharArray[1] = (num & 0xFF00) >> 8;

    return pCharArray;
}

unsigned char * intBEToCharLE (unsigned int num) {
    unsigned char* pCharArray = malloc(4);

    pCharArray[0] = num & 0xFF;
    pCharArray[1] = (num & 0xFF00) >> 8;
    pCharArray[2] = (num & 0xFF0000) >> 16;
    pCharArray[3] = (num & 0xFF000000) >> 24;

    return pCharArray;
}

// takes in file path and returns header struct
struct HEADER headerToStruct(FILE* pWavOriginal) {
    unsigned char buffer4[4];
    unsigned char buffer2[2];
    //FILE* pWavOriginal = fopen("../AudioFiles/BabyElephantWalk60.wav", "rb");

    // read header into struct
    int bytesRead = 0;
    struct HEADER header;

    // ChunkID  4 byte BE
    bytesRead = fread(header.ChunkID, sizeof(header.ChunkID), 1, pWavOriginal);
    printf("ChunkID: %.4s \n", header.ChunkID);

    // ChunkSize 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pWavOriginal);
    header.ChunkSize = charLEToIntBE(buffer4);
    printf("ChunkSize: %u \n", header.ChunkSize);

    // Format 4 byte BE
    bytesRead = fread(header.Format, sizeof(header.Format), 1, pWavOriginal);
    printf("Format: %.4s \n", header.Format);

    // Subchunk1ID 4 byte BE
    bytesRead = fread(header.Subchunk1ID, sizeof(header.Subchunk1ID), 1, pWavOriginal);
    printf("Subchunk1ID: %.4s \n", header.Subchunk1ID);

    // Subchunk1Size 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pWavOriginal);
    header.Subchunk1Size = charLEToIntBE(buffer4);
    printf("Subchunk1Size: %u \n", header.Subchunk1Size);

    // AudioFormat 2 byte LE converted to BE
    bytesRead = fread(buffer2, sizeof(buffer2), 1, pWavOriginal);
    header.AudioFormat = charLEToIntBE(buffer2);
    printf("AudioFormat: %u \n", header.AudioFormat);

    // NumChannels 2 byte LE converted to BE
    bytesRead = fread(buffer2, sizeof(buffer2), 1, pWavOriginal);
    header.NumChannels = charLEToIntBE(buffer2);
    printf("NumChannels: %u \n", header.NumChannels);

    // SampleRate 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pWavOriginal);
    header.SampleRate = charLEToIntBE(buffer4);
    printf("SampleRate: %u \n", header.SampleRate);

    // ByteRate 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pWavOriginal);
    header.ByteRate = charLEToIntBE(buffer4);
    printf("ByteRate: %u \n", header.ByteRate);

    // BlockAlign 2 byte LE converted to BE
    bytesRead = fread(buffer2, sizeof(buffer2), 1, pWavOriginal);
    header.BlockAlign = charLEToIntBE(buffer2);
    printf("BlockAlign: %u \n", header.BlockAlign);

    // BitsPerSample 2 byte LE converted to BE
    bytesRead = fread(buffer2, sizeof(buffer2), 1, pWavOriginal);
    header.BitsPerSample = charLEToIntBE(buffer2);
    printf("BitsPerSample: %u \n", header.BitsPerSample);

    // Subchunk2ID 4 byte BE
    bytesRead = fread(header.Subchunk2ID, sizeof(header.Subchunk2ID), 1, pWavOriginal);
    printf("Subchunk2ID: %.4s \n", header.Subchunk2ID);

    // Subchunk2Size 4 byte LE converted to BE
    bytesRead = fread(buffer4, sizeof(buffer4), 1, pWavOriginal);
    header.Subchunk2Size = charLEToIntBE(buffer4);
    printf("Subchunk2Size: %u \n", header.Subchunk2Size);

    fclose(pWavOriginal);
    return header;
}

void createNewWave(){
    FILE* pWavEncoded = fopen("../AudioFiles/wavEncoded.wav", "wb");
    fclose(pWavEncoded);
}

void getUserInputInt(int *pUserInput) {
    scanf("%d", pUserInput);
    while(getchar() != '\n');
}

void help(){
    printf("\n");
    printf("information placeholder.");
    printf("\n");
    printf("press Enter to go back.\n\n");
    getchar();
}

int mainMenu(){
    // prints logo
    printf("                                           /$$                        \n                                          | $$                        \n  /$$$$$$$  /$$$$$$  /$$   /$$  /$$$$$$  /$$$$$$    /$$$$$$   /$$$$$$ \n /$$_____/ /$$__  $$| $$  | $$ /$$__  $$|_  $$_/   /$$__  $$ /$$__  $$\n| $$      | $$  \\__/| $$  | $$| $$  \\ $$  | $$    | $$$$$$$$| $$  \\ $$\n| $$      | $$      | $$  | $$| $$  | $$  | $$ /$$| $$_____/| $$  | $$\n|  $$$$$$$| $$      |  $$$$$$$| $$$$$$$/  |  $$$$/|  $$$$$$$|  $$$$$$/\n \\_______/|__/       \\____  $$| $$____/    \\___/   \\_______/ \\______/ \n                     /$$  | $$| $$                                    \n                    |  $$$$$$/| $$                                    \n                     \\______/ |__/                                    ");

    int userInput;
    do {
        printf("\n1: Encoding mode");
        printf("\n2: Decoding mode");
        printf("\n3: Help");
        printf("\n4: Exit\n");

        getUserInputInt(&userInput);
    } while(userInput < 1 || userInput > 4);

    return (userInput == 4 ? 0 : userInput);
}

int encodingMenu(char *filepathSecret, char *filepathWave, char *filepathNewWave){
    int userInput;
    do {
        printf("\n");
        if (filepathSecret[0] == '\0') printf("\nno filepath to file that needs to be encoded into wave file");
        if (filepathWave[0] == '\0') printf("\nno filepath to wave file that will be encoded into");
        if (filepathNewWave[0] == '\0') printf("\nno filepath/directory used for export wave");
        //  TEMPORARY NOTES:
        //      check if directory exists:
        //          DIR dir* = opendir(directory string)
        //          if (ENOENT == errno)
        //          errno.h

        // OPTIONS
        printf("\n\n1: Change filepath to secret file");
        printf("\n2: Change filepath to wave file that will be encoded into");
        printf("\n3: Change filepath/directory used for export wave");
        printf("\n4: Help");
        printf("\n5: Back\n");

        getUserInputInt(&userInput);
    } while(userInput < 1 || userInput > 5);

    return (userInput == 5 ? 0 : userInput);
}

void menus(){
    // for encoding
    char filepathSecret[150] = "", filepathWave[150] = "", filepathNewWave[150] = "";
    // for decoding
    char filepathEncoded[150] = "", filepathDecodedSecret[150] = "";

    int userInput;
    do {
        userInput = mainMenu();

        switch(userInput) {
        case 0:
            break;
        case 1:
            // encoding mode menu
            userInput = encodingMenu(&filepathSecret, &filepathWave, &filepathNewWave);

            switch(userInput){
            case 0:
                userInput = 1; // back button pressed (not exit). To stay in main menu do-while loop
                break;
            case 1:
                //change filepath to secret file
                break;
            case 2:
                //change filepath to wave
                break;
            case 3:
                //change filepath for export
                break;
            case 4:
                help();
                break;
            }

            break;
        case 2:
            // decoding mode menu
            // userInput = encodingMenu(filepathEncoded, filepathDecodedSecret);
            break;
        case 3:
            // help menu
            help();
            break;
        default:
            printf("ERROR IN SWTICH STATEMENT IN menus() FUNCTION. userInput VARIABLE OUT OF BOUNDS");
        }
    } while(userInput != 0);
}

int main(){
    //headerToStruct();
    //createNewWave();

    //return menus();
    FILE* pWavOriginal = fopen("../AudioFiles/BabyElephantWalk60.wav", "rb");
    struct HEADER header = headerToStruct(pWavOriginal);


    return 0;
}
