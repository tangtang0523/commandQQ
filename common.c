#include "common.h"

void itoa(int i, char*string)
{
    if (i < 10)
        *string++ = '0' + 0;

    int power,j;
    j = i;

    for(power = 1; j >= 10; j /= 10)
        power *= 10;

    for(; power > 0; power /= 10)
    {
        *string++ = '0' + i/power;
        i %= power;
    }
    *string='\0';
}
