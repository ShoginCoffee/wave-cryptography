#ifndef MENUGUI_H
#define MENUGUI_H
#include <stdio.h>
#include "menuGUI.h"

void getUserInputInt(int *pUserInput);

void help();

void printLogo();

void encodingMenu(char *filepathSecret, char *filepathWave, char *filepathNewWave);

int GUIMenu();

#endif // MENUGUI_H
