#ifndef _VARSTRUCT_H_
#define _VARSTRUCT_H_

#include "varEnum.h"

typedef union Value
{
    int i; /* Si c'est un entier */
    char c;
    float f;
}Value;

typedef struct Var
{
    int ID;            /* Identifiant */

    char isRef;        /* S'il s'agit d'une référence ou non */

    char* name;        /* Nom dans le code */
    char* finalName;   /* Nom utilisé lors de la compilation en C */

    VAR_TYPE type;      /* Type de la valeur */
    Value* val;         /* Valeur de base de l'entier lors de son initialisation */

    struct Var* next;
    struct Var* first;

}Var;

#endif
