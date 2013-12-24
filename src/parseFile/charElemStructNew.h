#ifndef _CHARELEMSTRUCTNEW_H_
#define _CHARELEMSTRUCTNEW_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../args/args.h"
#include "charFunc.h"
#include "charElemStructTool.h"

/* Prototypes */
char validOperand(char* start, char* stop);

Elem* structureCode(char* code, Elem* container);
CodeLine* structureOperation(char* code, Elem *elem);
Operation* structureOperationLine(char* code);
Operation* structureOperationLineVarFunc(char** tmp, Operation *op);
Operation* structureOperationLineIntFloat(char** tmp, Operation *op);
Operation* structureOperationLineChar(char** tmp, Operation *op);
Operation* structureOperationLineParenth(char** tmp, Operation *op);
Operation* structureOperationLineOp(char** tmp, Operation *op);

#endif
