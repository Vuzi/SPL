#ifndef _BRACKETSTRUCTNEW_H_
#define _BRACKETSTRUCTNEW_H_

#include <stdio.h>
#include <stdlib.h>

#include "../compiler/compiler.h"
#include "bracketStructTool.h"

/* Prototypes */
Bracket* getBracketFromElem(Elem *elem, Bracket *container);
void checkIf(LOOP_TYPE newType, LOOP_TYPE oldType);
void getBracketType(Bracket *br, Elem *elem);

#endif
