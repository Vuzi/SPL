#ifndef _CHARFUNCENUM_H_
#define _CHARFUNCENUM_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef enum OP_TYPE
{
    OP_PARENTH, OP_FUNC_CALL, OP_VAR, OP_NAT_VAR, OP_MATHS, OP_NONE, OP_REF, OP_BR, OP_ASSIGN
}OP_TYPE;

#endif
