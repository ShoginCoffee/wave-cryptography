#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "headerLogic.h"
#include "chacha.h"
#include "encoder.h"

void createNewWave(){
    FILE* pWavEncoded = fopen("../AudioFiles/wavEncoded.wav", "wb");
    fclose(pWavEncoded);
}

void getUserInputInt(int *pUserInput) {
    scanf("%d", pUserInput);
    while(getchar() != '\n');
}

void help(){
    system("cls");
    printf("information placeholder.");
    printf("\n");
    printf("press Enter to go back.\n\n");
    getchar();
}

void printLogo() {
    printf("                                           /$$                        \n                                          | $$                        \n  /$$$$$$$  /$$$$$$  /$$   /$$  /$$$$$$  /$$$$$$    /$$$$$$   /$$$$$$ \n /$$_____/ /$$__  $$| $$  | $$ /$$__  $$|_  $$_/   /$$__  $$ /$$__  $$\n| $$      | $$  \\__/| $$  | $$| $$  \\ $$  | $$    | $$$$$$$$| $$  \\ $$\n| $$      | $$      | $$  | $$| $$  | $$  | $$ /$$| $$_____/| $$  | $$\n|  $$$$$$$| $$      |  $$$$$$$| $$$$$$$/  |  $$$$/|  $$$$$$$|  $$$$$$/\n \\_______/|__/       \\____  $$| $$____/    \\___/   \\_______/ \\______/ \n                     /$$  | $$| $$                                    \n                    |  $$$$$$/| $$                                    \n                     \\______/ |__/                                    ");
}

int mainMenu(){
    system("cls");
    printLogo();
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
    system("cls");
    int userInput;
    do {
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
    //menus();

    //menus and print header
    //return menus();
    FILE* pWavOriginal = fopen("../AudioFiles/BabyElephantWalk60.wav", "rb");
    struct HEADER header = headerToStruct(pWavOriginal);


    /* quarterRound test
    unsigned int a = 0x11111111;
    unsigned int b = 0x01020304;
    unsigned int c = 0x9b8d6f43;
    unsigned int d = 0x01234567;


    quarterRound(&a, &b, &c, &d);

    printf("%x \n", a);
    printf("%x \n", b);
    printf("%x \n", c);
    printf("%x \n", d);
    */


    /*
    4c 61 64 69 65 73 20 61 6e 64 20 47 65 6e 74 6c  Ladies and Gentl
    65 6d 65 6e 20 6f 66 20 74 68 65 20 63 6c 61 73  emen of the clas
    73 20 6f 66 20 27 39 39 3a 20 49 66 20 49 20 63  s of '99: If I c
    6f 75 6c 64 20 6f 66 66 65 72 20 79 6f 75 20 6f  ould offer you o
    6e 6c 79 20 6f 6e 65 20 74 69 70 20 66 6f 72 20  nly one tip for
    74 68 65 20 66 75 74 75 72 65 2c 20 73 75 6e 73  the future, suns
    63 72 65 65 6e 20 77 6f 75 6c 64 20 62 65 20 69  creen would be i
    74 2e
    */

    /*
    // chacha20 test
    uint8_t plainText[] = "Ladies and Gentlemen of the class of '99: If I could offer you only one tip for the future, sunscreen would be it.";
    int key[] = {
        0x03020100, 0x07060504, 0x0b0a0908, 0x0f0e0d0c,
        0x13121110, 0x17161514, 0x1b1a1918, 0x1f1e1d1c
        };
    int counter[] = {0x00000001};
    int nonce[] = {0x00000000, 0x4a000000, 0x00000000};

    //Printing original array
    for(int b = 0; b < 114; b++){
        printf("%x ", plainText[b]);
        if((b + 1) % 16 == 0 && b != 0) printf("\n");
    }

    chacha20(&plainText, 114, &key, &counter, &nonce);
    printf("\n\n");

    //Printing encrypted array
    for(int a = 0; a < 114; a++) {
        printf("%x ", plainText[a]);
        if((a + 1) % 16 == 0 && a != 0) printf("\n");
    }

    chacha20(&plainText, 114, &key, &counter, &nonce);
    printf("\n\n");

    //Printing decrypted array
    for(int a = 0; a < 114; a++) {
        printf("%x ", plainText[a]);
        if((a + 1) % 16 == 0 && a != 0) printf("\n");
    }
    */
    char fileName[] = "demo.txt";
    char * pWaveData = pReadInWaveData(pWavOriginal, header);
    FILE* pHiddenFile = fopen("demo.txt", "rb");


    int i;
    /*for (i = 0; i < header.Subchunk2Size; i++) {
        *pWaveData[i] += changeBits(1, *pWaveData[i], hiddenData[0])
    }*/


    return 0;
}
