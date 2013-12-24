#ifndef _CHARFUNCSTRUCT_H_
#define _CHARFUNCSTRUCT_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "charElemEnum.h"

#include "../args/args.h"

typedef struct Elem
{
    int ID;
    char* name;

    struct Operation* cond;

    struct CodeLine* code;

    struct Elem* content;
    struct Elem* container;

    struct Elem* next;
    struct Elem* first;

}Elem;

typedef struct CodeLine
{
    int ID;
    char* code;

    struct Elem* link;
    struct Bracket* link_b;
    struct Operation *op;
    struct OperationDebug *opDebug;

    struct CodeLine* next;
    struct CodeLine* first;

}CodeLine;

typedef struct Operation
{
    int ID;

    OP_TYPE type;

    char* info;
    struct Operation *link;

    struct Operation *next;
    struct Operation *previous;
    struct Operation *first;

}Operation;

#endif
