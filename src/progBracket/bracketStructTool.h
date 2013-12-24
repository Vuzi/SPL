#ifndef _BRACKETSTRUCTTOOL_H_
#define _BRACKETSTRUCTTOOL_H_

#include <stdio.h>
#include <stdlib.h>

#include "bracketStruct.h"
#include "../progVar/varStructTool.h"
#include "../progOperation/operationStructTool.h"
#include "../parseFile/charElemStructTool.h"

/* Prototypes */
Bracket* getLastBracket(Bracket* last);
Bracket* getBracketByID(int ID, Bracket* bracket);
Bracket* initBracket(Bracket* newBracket);
Bracket* addBracket(Bracket* last);
void destroyBracket(Bracket* bracket);
Bracket* destroyBracketList(Bracket* bracket);

#endif
