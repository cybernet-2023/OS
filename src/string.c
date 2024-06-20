#include "string.h"

uint16_t strLength(string str)
{
    uint16_t i = 1;
    while(str[i++]);
    return --i;
}