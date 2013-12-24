#ifndef _BRACKETSTRUCT_H_
#define _BRACKETSTRUCT_H_

#include "bracketEnum.h"
#include "../parseFile/charElemStruct.h"
#include "../progVar/varStructTool.h"
#include "../progFunc/funcStructTool.h"

typedef struct Bracket
{
    int ID;            /* Identifiant */

    char* name;        /* Nom dans le code */
    Operation* cond;    /* Condition */
    struct OperationDebug* cond_d;
    LOOP_TYPE type;     /* Type */

    CodeLine *code;

    struct Var* var;           /* Variables déclarées dans la boucle */

    struct Bracket* content;
    struct Bracket* container;

    struct Func* func;        /* Fonction le contenant */

    struct Bracket* next;
    struct Bracket* first;

}Bracket;


#endif
