#ifndef _COMPILER_H_
#define _COMPILER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#ifdef __linux__
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
#endif

#include "../args/args.h"
#include "../debug/debug.h"
#include "../parseFile/charFunc.h"
#include "../parseFile/charElemStructNew.h"
#include "../progStruct/progStruct.h"
#include "../progVar/varStructNew.h"
#include "../progFunc/funcStructNew.h"
#include "../progOperation/operationStructNew.h"
#include "../progTranslation/translation.h"

typedef enum STRUCT_CODE
{
    DONE, DONE_WARNING, ERROR_COMPIL
}STRUCT_CODE;

/* Prototypes */
void startCompil(FILE* f, char* fileOutput);
char* prepareCode(char* code);
void* compilCode(void* prog_void);
void constructCCode(Prog* prog);
void constructProgCode(Prog* prog);
void constructProg(Prog *prog);

#endif
