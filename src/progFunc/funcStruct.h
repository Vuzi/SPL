#ifndef _FUNCSTRUCT_H_
#define _FUNCSTRUCT_H_

#include "../progVar/varStruct.h"
#include "../progBracket/bracketStruct.h"

typedef struct Func
{
    int ID;            /* Identifiant */

    char* name;        /* Nom dans le code */
    char* finalName;   /* Nom utilisé lors de la compilation en C */

    VAR_TYPE type;      /* Type de la valeur de retour */
    Var* arg;           /* Arguments de la fonction */

    Operation *code;

    struct Bracket* content;

    struct Func* next;
    struct Func* first;

}Func;

#endif
