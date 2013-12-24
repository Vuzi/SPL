#ifndef _CHARELEMSTRUCTTOOL_H_
#define _CHARELEMSTRUCTTOOL_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../args/args.h"
#include "charElemStruct.h"
#include "../progOperation/operationStructTool.h"

/* Prototypes */
Elem* getLastElem(Elem* last);
Elem* getElemByID(int ID, Elem* elem);
Elem* getElemByRef(char* str, Elem* elem);
Elem* initElem(Elem* newElem);
Elem* addElem(Elem* last);
void destroyElem(Elem* elem);
void destroyElemNoCl(Elem* elem);
Elem* destroyElemList(Elem* elem);
Elem* destroyElemNoClList(Elem* elem);

CodeLine* getLastCodeLine(CodeLine* last);
CodeLine* getCodeLineByID(int ID, CodeLine* codeLine);
CodeLine* getCodeLineByRef(char* str, CodeLine* codeLine);
CodeLine* initCodeLine(CodeLine* newCodeLine);
CodeLine* addCodeLine(CodeLine* last);
void destroyCodeLine(CodeLine* codeLine);
CodeLine* destroyCodeLineList(CodeLine* codeLine);

Operation* getLastOperation(Operation* last);
Operation* getOperationByID(int ID, Operation* operation);
Operation* initOperation(Operation* newOperation);
Operation* addOperation(Operation* last);
void destroyOperation(Operation* operation);
Operation* destroyOperationList(Operation* operation);

#endif
