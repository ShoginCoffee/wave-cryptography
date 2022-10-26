#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include "wavHeader.h"
#include "hiddenHeader.h"
#include "chacha.h"
#include "menuGUI.h"
#include "encoder.h"
//#include "hiddenDataHeaderStruct.h"
//#include "hiddenFile.h"

void createNewWave(){
    FILE* pWavEncoded = fopen("../AudioFiles/wavEncoded.wav", "wb");
    fclose(pWavEncoded);
}

void printb(char binary){
    for(int i = 0; i < 8; i++){
        printf("%x", ((binary << i) & 0xff) >> 7);
    }
}

int main(){
    //meenu
    int nothing = GUIMenu();



    /* print header
    FILE* pWavOriginal = fopen("../AudioFiles/BabyElephantWalk60.wav", "rb");
    struct WavHeader wavHeader = headerToStruct(pWavOriginal);
    */


    /*
    // encoder
    unsigned char data[16] = {[0 ... 15] = 0x00};
    unsigned char message[] = {0xff, 0xff};

    printf("input data: \n");
    for(int i = 0; i < sizeof(data); i++){
        printb(data[i]);
        printf("\n");
    }
    printf("\n");

    encodeMessage(&data, sizeof(data), 2, 2, &message, 2);


    printf("output data: \n");
    for(int i = 0; i < sizeof(data); i++){
        printb(data[i]);
        printf("\n");
    }
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


    return 0;
}
