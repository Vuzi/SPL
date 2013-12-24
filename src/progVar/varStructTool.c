
#include "varStructTool.h"

/* ====================================================
                        getLastVar
    --------------------------------------------------
    Arg : last : élément de la liste chainée
    --------------------------------------------------
    Def : Retourne le dernier élément de la liste chainée
          ou NULL si impossible
   ==================================================== */
Var* getLastVar(Var* last)
{
    if (!last)
        return NULL;

    while(last->next != NULL)
        last = last->next;

    return last;
}


/* ====================================================
                        getVarByID
    --------------------------------------------------
    Arg : ID   : ID de fonction à chercher
          last : élément de la liste chainée
    --------------------------------------------------
    Def : Retourne le premier élément dont l'ID
          correspond, ou NULL s'il n'existe pas
   ==================================================== */
Var* getVarByID(int ID, Var* var)
{
    if (!var)
        return NULL;

    var = var->first;

    while(var)
    {
        if (var->ID == ID)
            return var;
        var = var->next;
    }

    return NULL;
}

/* ====================================================
                        getVarByName
    --------------------------------------------------
    Arg : name : nom de fonction à chercher
          last : élément de la liste chainée
    --------------------------------------------------
    Def : Retourne le premier élément dont le nom
          correspond, ou NULL s'il n'existe pas
   ==================================================== */
Var* getVarByName(char* name, Var* var)
{
    if (!var || !name)
        return NULL;

    var = var->first;

    while(var)
    {
        if (!strcmp(name, var->name))
            return var;
        var = var->next;
    }

    return NULL;
}


/* ====================================================
                        initVar
    --------------------------------------------------
    Arg : newElem : fonction à initialiser
    --------------------------------------------------
    Def : Initialise les valeurs de la structure et
          renvoit un pointeur vers celle-ci
   ==================================================== */
Var* initVar(Var* newVar)
{
    if (!newVar)
        return NULL;

    newVar->isRef = 0;

    newVar->name = NULL;
    newVar->finalName = NULL;

    newVar->type = TYPE_UNDEFINED;
    newVar->val = NULL;

    newVar->next = NULL;
    newVar->first = NULL;

    return newVar;
}

/* ====================================================
                        addVar
    --------------------------------------------------
    Arg : last : élément quelconque de la liste
    --------------------------------------------------
    Def : Ajoute un élément à une liste chainée, et
          retourne un pointeur vers le nouvel dernier
          élément de celle-ci
   ==================================================== */
Var* addVar(Var* last)
{
    Var* newVar = initVar(malloc(sizeof(Var)));

    if (last)
    {
        last = getLastVar(last); /* On vérifie qu'il s'agit bien du dernier */
        newVar->ID = last->ID + 1; /* ID suivant */

        newVar->first = last->first; /* premier de la liste */
        last->next = newVar; /* dernier de la liste */
    }
    else
    {
        newVar->ID = 1; /* premier ID de la liste, 0 étant le 'main' */
        newVar->first = newVar; /* premier de la liste */
    }

    return newVar;
}

/* ====================================================
                      destroyVar
    --------------------------------------------------
    Arg : last : élément de la liste
    --------------------------------------------------
    Def : Détruit l'élément en le libérant de la mémoire
   ==================================================== */
void destroyVar(Var* var)
{
    if (!var)
        return;

    if(var->name)
        free(var->name);

    if(var->finalName)
        free(var->finalName);

    if(var->val)
        free(var->val);

    free(var);
}

/* ====================================================
                      destroyVarList
    --------------------------------------------------
    Arg : last : élément de la liste
    --------------------------------------------------
    Def : Détruit la liste de la mémoire
   ==================================================== */
Var* destroyVarList(Var* var)
{
    Var* toDestroy = NULL;

    if (!var)
        return NULL;

    var = var->first;

    while(var)
    {
        toDestroy = var;
        var = var->next;

        destroyVar(toDestroy);
    }

    return NULL;
}

