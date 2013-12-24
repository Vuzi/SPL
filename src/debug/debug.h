#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../parseFile/charElemStructTool.h"
#include "../progBracket/bracketStructTool.h"
#include "../progOperation/operationStructTool.h"

/* Prototypes */
void debug_bracketPrint(Bracket* br, int lvl);

void debug_funcOnly(Func* func);
void debug_func(Func* func);

void debug_varOnly(Var* var);
void debug_var(Var* var);

void debug_elem(Elem *elem);
void debug_elemPrint(Elem *elem, int lvl);
void debug_elemPrintLine(Operation *op, int lvl);

void debug_opPrintList(OperationDebug *op, int lvl);
void debug_opPrint(OperationDebug *op, int lvl);

#endif
