#include "util.h"
#include <stdio.h>
#include <stdlib.h>


void ErrorIf(bool condition, const char* msg)
{
    if (condition)
    {
        perror(msg);
        exit(EXIT_FAILURE);
    }
}