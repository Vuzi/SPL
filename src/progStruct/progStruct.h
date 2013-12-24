#ifndef _PROGSTRUCT_H_
#define _PROGSTRUCT_H_

#include "../progVar/varStructTool.h"
#include "../progFunc/funcStructTool.h"

typedef struct Prog
{
    struct Func* func;
    struct Func* specialFunc;
    struct Var* globalVar;
    FILE* f;
    char* fileOutput;
    char* fileOutputCode;
    struct Elem* elem;
    int warning;

}Prog;

#endif
