#include <stdio.h>
#include <stdlib.h>

int main()
{
    int littleEndian = 0x78563412;
    int byteArray[sizeof(littleEndian)];
    int bigEndian;

    for (int i = (2 * sizeof(littleEndian)) - 1; i > 0; i -= 2) {
        byteArray[i] = (littleEndian & (pow(16, i) + pow(16,i-1)));
    };

    for (int i = 0; i < sizeof(littleEndian); i++) {
        bigEndian = (bigEndian | byteArray[i]);
    };



    char string[10];
    sprintf(string, "%X", bigEndian);
    printf(string);
    return 0;
}
