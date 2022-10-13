#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "wavHeader.h"
#include "hiddenHeader.h"
#include "chacha.h"
//#include "encoder.h"
//#include "hiddenDataHeaderStruct.h"
//#include "hiddenFile.h"

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
    //menus
    //menus();



    /* print header
    FILE* pWavOriginal = fopen("../AudioFiles/BabyElephantWalk60.wav", "rb");
    struct WavHeader wavHeader = headerToStruct(pWavOriginal);
    */
    for(int i= 0; i<204; i++){
        printf("a");
    }




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



    /* Theo stuff
    char * pWaveData = pReadInWaveData(pWavOriginal, header);
    char filePath[] = "C:\\Users\\mortaza.ebeid\\Documents\\prog\\c\\wave-cryptography\\WaveCrypt\\demo.txt";
    //Error is on the line below
    struct HIDDENDATAHEADER hiddenDataHeader = hiddenDataHeaderToStruct(&filePath);

    printf("Subchunk1Size: %d\n", hiddenDataHeader.Subchunk1Size);
    printf("Subchunk2Size: %d\n", hiddenDataHeader.Subchunk2Size);
    printf("FileName: %s\n", hiddenDataHeader.Filename);

    //for (int i = 0; i < header.Subchunk2Size; i++) {
    //    *pWaveData[i] += changeBits(1, *pWaveData[i], hiddenData[0])
    //}
    */



    return 0;
}
