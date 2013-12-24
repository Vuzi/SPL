#ifndef _OPERATIONSTRUCTTOOL_H_
#define _OPERATIONSTRUCTTOOL_H_

#include "operationStruct.h"

/* Prototypes */
OperationDebug* getLastOpDebug(OperationDebug* last);
OperationDebug* getOpDebugByID(int ID, OperationDebug* op);
OperationDebug* initOpDebug(OperationDebug* newOp);
OperationDebug* addOpDebug(OperationDebug* last);
void destroyOpDebug(OperationDebug* opDebug);
OperationDebug* destroyOpDebugList(OperationDebug* opDebug);

#endif
