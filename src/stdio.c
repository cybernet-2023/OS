#include "stdio.h"
#include "types.h"
#include "string.h"

int curX, curY;

uint8_t inb(uint16_t port)
{
    uint8_t data;
    __asm__ __volatile__("inb %1, %0" : "=a"(data) : "d"(port));
    return data;
}

void outb(uint16_t port, uint8_t data)
{
    __asm__ __volatile__("outb %0, %1" : : "a" (data), "d" (port));
}

void clearLine(uint8_t from, uint8_t to)
{
    uint16_t i = maxColumns * from * maxDepth;
    string vidmem = (string) 0xb8000;
    for(i; i < (maxColumns * (to + 1) * maxDepth); i++)
    {
        vidmem[i] = 0x0;
    }
}

void updateCursor()
{
    unsigned temp;

    temp = curY * maxColumns + curX;
    outb(0x3D4, 14);
    outb(0x3D5, temp >> 8);
    outb(0x3D4, 15);
    outb(0x3d5, temp);
}

void clearScreen()
{
    clearLine(0, maxRows);
    curX = 0;
    curY = 0;
    updateCursor();
}

void scrollUp(uint8_t lineNum)
{
    string vidmem = (string) 0xb8000;
    uint16_t i = 0;
    for(i; i < maxColumns * (maxRows - 1) * 2; i++)
    {
        vidmem[i] = vidmem[i + maxColumns * 2 * lineNum];
    }
    clearLine(maxRows - 1 - lineNum, maxRows - 1);
    if((curY - lineNum) < 0)
    {
        curX = 0;
        curY = 0;
    }
    else
    {
        curY -= lineNum;
    }
    updateCursor();
}

void newLineCheck()
{
    if(curY >= (maxRows - 1))
    {
        scrollUp(1);
    }
}

void putc(char c)
{
    string vidmem = (string) 0xb8000;
    switch(c)
    {
        case (0x08):
            if(curX > 0)
            {
                curX--;
                vidmem[(curY * maxColumns + curX) * maxDepth] = 0x02;
            }
            break;
        
        case (0x09):
            curX = (curX + 8) & ~(8 - 1);
            break;
        
        case ('\r'):
            curX = 0;
            break;
        
        case ('\n'):
            curX = 0;
            curY++;
            break;
        
        default:
            vidmem[((curY * maxColumns + curX)) * maxDepth] = c;
            vidmem[((curY * maxColumns + curX)) * maxDepth+1] = 0x02;
            curX++;
            break;
    }
    if(curX >= maxColumns)
    {
        curX = 0;
        curY++;
    }
    newLineCheck();
    updateCursor();
}

void puts(char* str)
{
    uint16_t i = 0;
    for(i; i < strLength(str); i++)
    {
        putc(str[i]);
    }
}