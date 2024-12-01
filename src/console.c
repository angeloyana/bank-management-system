#include "console.h"

#include <stdio.h>
#include <stdlib.h>

void clearScreen() {
#ifdef _WIN23
  system("cls");
#else
  system("clear");
#endif
}

void printBorder(char t, size_t length) {
  for (size_t i = 0; i < length; i++) {
    printf("%c", t);
  }
  printf("\n");
}

void printTitle(const char *title, int pad) {
  size_t titleLength = strlen(title);
  printBorder('=', titleLength + pad * 2);
  printf("%*s\n", (int)titleLength + pad, title);
  printBorder('=', titleLength + pad * 2);
}
