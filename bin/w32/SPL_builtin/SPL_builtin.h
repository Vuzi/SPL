#ifndef _H_SPL_BUILTIN_
#define _H_SPL_BUILTIN_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

typedef enum TYPE_VAR
{
    TYPE_CHAR, TYPE_INT, TYPE_FLOAT, TYPE_STRING
}TYPE_VAR;

char* clean (char *chaine);
void purger(void);

void out(int nb,...);
void in(int nb,...);

#endif

