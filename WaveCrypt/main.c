#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "wave.h"

// takes char and prints bits
void printb(char val) {
  for (int i = 7; 0 <= i; i--) {
    printf("%c", (val & (1 << i)) ? '1' : '0');
  }
}

// takes in file path and returns header struct
struct HEADER headerToStruct(FILE* pWavOriginal) {
    unsigned char buffer4[4];
    unsigned char buffer2[2];
    //FILE* pWavOriginal = fopen("../AudioFiles/BabyElephantWalk60.wav", "rb");

    // read header into struct
    int bytesRead = 0;
    struct HEADER header;

    // RIFF 4 byte BE
    bytesRead = fread(header.riff, sizeof(header.riff), 1, pWavOriginal);
    printf("1 - 4: %s \n", header.riff);

    // overall_size 4 byte LE
    bytesRead = fread(buffer4[4], sizeof(buffer4[4]), 1, pWavOriginal);

    //

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
    menus();

    return 0;
}
