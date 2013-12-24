#ifndef _VARSTRUCTTOOL_H_
#define _VARSTRUCTTOOL_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "varStruct.h"

/* Prototypes */
Var* getLastVar(Var* last);
Var* getVarByID(int ID, Var* var);
Var* getVarByName(char* name, Var* var);
Var* initVar(Var* newVar);
Var* addVar(Var* last);
void destroyVar(Var* var);
Var* destroyVarList(Var* var);

#endif
