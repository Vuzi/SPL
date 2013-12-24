#ifndef _TRANSLATION_H_
#define _TRANSLATION_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "../args/args.h"
#include "../debug/debug.h"
#include "../parseFile/charFunc.h"
#include "../parseFile/charElemStructNew.h"
#include "../progStruct/progStruct.h"
#include "../progVar/varStructNew.h"
#include "../progFunc/funcStructNew.h"
#include "../progOperation/operationStructNew.h"

/* Prototypes */
void translateProgToC(Prog* prog, FILE *f);
void translateVarList(Var* var, FILE *f, int lvl);
void translateVar(Var* var, FILE *f);
void translateFuncProto(Func* func, FILE *f);
void translateFunc(Func* func, FILE *f);
void translateCl(CodeLine *cl, FILE *f, int lvl);
void translateOperationStart(OperationDebug *op, FILE *f, char isBuiltIn, Func *isFunc);
void translateOperation(OperationDebug *op, FILE *f);

#endif
