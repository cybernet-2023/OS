#pragma once

#include "types.h"

#define maxColumns 80
#define maxRows 25
#define maxDepth 2

void outb(uint16_t port, uint8_t data);
uint8_t inb(uint16_t port);
void updateCursor();
void putc(char c);
void puts(char* str);
void clearScreen();
void scrollUp(uint8_t lineNum);
void newLineCheck();