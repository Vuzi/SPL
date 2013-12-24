#ifndef _FUNCSTRUCTNEW_H_
#define _FUNCSTRUCTNEW_H_

#include <stdio.h>
#include <stdlib.h>

#include "../compiler/compiler.h"
#include "funcStructTool.h"
#include "../progBracket/bracketStructNew.h"
#include "../parseFile/charElemStruct.h"

/* Prototypes */
Func* getFuncFromElem(Elem* elem, Func* funcList);
Func* getMainFromElem(Elem* elem, Func* funcList);

void getFuncName(Operation* op, Func* func);
void getFuncType(Operation* op, Func* func);
void getFuncArgs(Elem *elem, Func* func);
void getFuncCode(Elem *elem, Func* func);
void checkFunc(Func* func, Elem* elem);

#endif
