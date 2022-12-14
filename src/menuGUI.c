#include "menuGUI.h"

void getUserInputInt(int *pUserInput){
    int output = scanf("%d", pUserInput);
    while(getchar() != '\n'); // gets rid of unwanted chars in stdin
}


void help(){
    system("cls");
    printf("information placeholder.");
    printf("\n");
    printf("press Enter to go back.\n\n");
    int output = getchar();
    while(getchar() != '\n'); // gets rid of unwanted chars in stdin
}

void printLogo(){
    printf("                                           /$$                        \n                                          | $$                        \n  /$$$$$$$  /$$$$$$  /$$   /$$  /$$$$$$  /$$$$$$    /$$$$$$   /$$$$$$ \n /$$_____/ /$$__  $$| $$  | $$ /$$__  $$|_  $$_/   /$$__  $$ /$$__  $$\n| $$      | $$  \\__/| $$  | $$| $$  \\ $$  | $$    | $$$$$$$$| $$  \\ $$\n| $$      | $$      | $$  | $$| $$  | $$  | $$ /$$| $$_____/| $$  | $$\n|  $$$$$$$| $$      |  $$$$$$$| $$$$$$$/  |  $$$$/|  $$$$$$$|  $$$$$$/\n \\_______/|__/       \\____  $$| $$____/    \\___/   \\_______/ \\______/ \n                     /$$  | $$| $$                                    \n                    |  $$$$$$/| $$                                    \n                     \\______/ |__/                                    ");
}

void encodingMenu(char *filepathSecret, char *filepathWave, char *filepathNewWave){
    int userInput;
    do {
        userInput = 0;
        system("cls");
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

        switch(userInput){
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

    } while(userInput != 5);
}

int GUIMenu(){
    // for encoding
    char filepathSecret[150] = "", filepathWave[150] = "", filepathNewWave[150] = "";
    // for decoding
    char filepathEncoded[150] = "", filepathDecodedSecret[150] = "";

    int userInput;
    do {
        userInput = 0;
        system("cls"); // clears screen for windows
        printLogo();

        printf("\n1: Encoding mode");
        printf("\n2: Decoding mode");
        printf("\n3: Help");
        printf("\n4: Exit\n");

        getUserInputInt(&userInput);

        switch(userInput) {
            case 1:
                // encoding mode menu
                encodingMenu(&filepathSecret, &filepathWave, &filepathNewWave);
                break;
            case 2:
                // decoding mode menu
                break;
            case 3:
                help();
                break;
        }
    } while(userInput != 4);

    return 0;
}
