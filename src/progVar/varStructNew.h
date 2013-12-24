#ifndef _VARSTRUCTNEW_H_
#define _VARSTRUCTNEW_H_

#include <stdio.h>
#include <stdlib.h>

#include "../compiler/compiler.h"
#include "varStructTool.h"
#include "../parseFile/charElemStruct.h"

/* Prototypes */
VAR_TYPE getNewType(VAR_TYPE a, VAR_TYPE b);
VAR_TYPE typeFormChar(char* code);

void getVarName(Operation* op, Var* var);
void getVarType(Operation* op, Var* var);
void getVarValue(Operation* op, Var* var);

Var* getVarFromElem(Elem* elem, Var* varList);
void checkVar(Var* var, Elem *elem);

#endif
