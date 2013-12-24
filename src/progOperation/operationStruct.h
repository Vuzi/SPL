#ifndef _OPERATIONSTRUCT_H_
#define _OPERATIONSTRUCT_H_

#include "../progBracket/bracketStructTool.h"

typedef enum OP_ELEM_TYPE
{
    ELEM_INT, ELEM_FLOAT, ELEM_CHAR, ELEM_REF, ELEM_VAR, ELEM_BR
}OP_ELEM_TYPE;

typedef union OpElem
{
    Var* var;
    int i;
    char c;
    float f;
    struct OperationDebug* ref;
    struct Bracket* br;

}OpElem;

typedef struct OperationDebug
{
    int ID;
    int startOrder;

    OP_TYPE type;

    VAR_TYPE varType;
    VAR_TYPE cast;

    OpElem *elem1;
    OP_ELEM_TYPE elem1Type;

    char* operand;

    OpElem *elem2;
    OP_ELEM_TYPE elem2Type;

    Func* func;

    struct OperationDebug* next;
    struct OperationDebug* first;

}OperationDebug;

#endif
