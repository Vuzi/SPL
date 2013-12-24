#ifndef _FUNCSTRUCTTOOL_H_
#define _FUNCSTRUCTTOOL_H_

#include <stdio.h>
#include <stdlib.h>

#include "funcStruct.h"

#include "../progBracket/bracketStructTool.h"

/* Prototypes */
Func* getLastFunc(Func* last);
Func* getFuncByID(int ID, Func* func);
Func* getFuncByName(char* name, Func* func);
Func* createSpecialFunc(void);
Func* initFunc(Func* newFunc);
Func* addFunc(Func* last);
void destroyFunc(Func* func);
Func* destroyFuncList(Func* func);

#endif
