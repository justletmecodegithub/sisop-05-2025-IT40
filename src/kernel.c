#include "shell.h"
#include "kernel.h"

int main() {
  clearScreen();
  shell();
}

void printString(char *str) {
  while (*str) {
    interrupt(0x10, 0x0E00 | *str, 0, 0, 0);
    str++;
  }
}

void readString(char *buf) {
  int i = 0;
  while (1) {
    int c = interrupt(0x16, 0, 0, 0, 0);
    if (c == '\r') {
      buf[i] = '\0';
      printString("\n");
      return;
    } else if (c == '\b' && i > 0) {
      i--;
      printString("\b \b");
    } else if (c >= 32 && c <= 126) {
      buf[i++] = c;
      interrupt(0x10, 0x0E00 | c, 0, 0, 0);
    }
  }
}

void clearScreen() {
  // Clear screen by scrolling
  interrupt(0x10, 0x0600, 0, 0, 0);
  // Set cursor position to top-left
  interrupt(0x10, 0x0200, 0, 0, 0);
}
