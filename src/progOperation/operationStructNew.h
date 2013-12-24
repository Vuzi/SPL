#ifndef _OPERATIONSTRUCTNEW_H_
#define _OPERATIONSTRUCTNEW_H_

#include "operationStructTool.h"
#include "../progStruct/progStruct.h"
#include "../compiler/compiler.h"

/* Prototypes */
void getOpDebugBr(Bracket *br, Prog* prog);
OperationDebug* getOpDebugCL(Operation *op, Prog *prog, Bracket *br);

OperationDebug* getOpDebugVarAll(OperationDebug *opDebug, Operation *op, Prog *prog, Bracket* br);
OperationDebug* getOpDebugParenthAll(OperationDebug *opDebug, Operation *op, Prog *prog, Bracket* br);
OperationDebug* getOpDebugFuncAll(OperationDebug *opDebug, Operation *op, Prog *prog, Bracket* br);
OperationDebug* getOpDebugIncAll(OperationDebug *opDebug, Operation *op);
OperationDebug* getOpDebugUnaAll(OperationDebug *opDebug, Operation *op);
OperationDebug* getOpDebugExcAll(OperationDebug *opDebug, Operation *op);
OperationDebug* getOpDebugCalcFirstAll(OperationDebug *opDebug, Operation *op);
OperationDebug* getOpDebugCalcSecondAll(OperationDebug *opDebug, Operation *op);
OperationDebug* getOpDebugCompAll(OperationDebug *opDebug, Operation *op);
OperationDebug* getOpDebugAndOrAll(OperationDebug *opDebug, Operation *op);
OperationDebug* getOpDebugAssignCalcAll(OperationDebug *opDebug, Operation *op);
OperationDebug* getOpDebugAssignAll(OperationDebug *opDebug, Operation *op);

OperationDebug* getOpDebugVar(OperationDebug *opDebug, Operation *op, Prog* prog, Bracket *br);
OperationDebug* getOpDebugNatVar(OperationDebug *opDebug, Operation *op);
OperationDebug* getOpDebugParenth(OperationDebug *opDebug, Operation *op, Prog *prog, Bracket *br);
OperationDebug* getOpDebugFunc(OperationDebug *opDebug, Operation *op, Prog *prog, Bracket* br);
OperationDebug* getOpDebugInc(OperationDebug *opDebug, Operation *op);
OperationDebug* getOpDebugUna(OperationDebug *opDebug, Operation *op);
OperationDebug* getOpDebugExc(OperationDebug *opDebug, Operation *op);
OperationDebug* getOpDebugCalc(OperationDebug *opDebug, Operation *op);
OperationDebug* getOpDebugComp(OperationDebug *opDebug, Operation *op);
OperationDebug* getOpDebugAssign(OperationDebug *opDebug, Operation *op);

void getOpDebugStartOrder(OperationDebug *opDebug, Operation *op);
char getSpeChar(char *str);
#endif
