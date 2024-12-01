#ifndef CONSOLE_H
#define CONSOLE_H

#include <stddef.h>

void clearScreen();

void printBorder(char t, size_t length);

void printTitle(const char *title, int pad);

#endif  // CONSOLE_H
