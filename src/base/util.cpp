#include "util.h"
#include <stdio.h>
#include <stdlib.h>

void errif(bool err, const char* errmsg)
{
    if(err)
    {
        perror(errmsg);
        exit(EXIT_FAILURE);
    }
}