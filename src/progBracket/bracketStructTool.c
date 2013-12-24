
#include "bracketStructTool.h"

/* ====================================================
                        getLastBracket
    --------------------------------------------------
    Arg : last : élément de la liste chainée
    --------------------------------------------------
    Def : Retourne le dernier élément de la liste chainée
          ou NULL si impossible
   ==================================================== */
Bracket* getLastBracket(Bracket* last)
{
    if (!last)
        return NULL;

    while(last->next != NULL)
        last = last->next;

    return last;
}


/* ====================================================
                        getBracketByID
    --------------------------------------------------
    Arg : ID   : ID de fonction à chercher
          last : élément de la liste chainée
    --------------------------------------------------
    Def : Retourne le premier élément dont l'ID
          correspond, ou NULL s'il n'existe pas
   ==================================================== */
Bracket* getBracketByID(int ID, Bracket* bracket)
{
    if (!bracket)
        return NULL;

    bracket = bracket->first;

    while(bracket)
    {
        if (bracket->ID == ID)
            return bracket;
        bracket = bracket->next;
    }

    return NULL;
}


/* ====================================================
                        initBracket
    --------------------------------------------------
    Arg : newBracket : fonction à initialiser
    --------------------------------------------------
    Def : Initialise les valeurs de la structure et
          renvoit un pointeur vers celle-ci
   ==================================================== */
Bracket* initBracket(Bracket* newBracket)
{
    if (!newBracket)
        return NULL;

    newBracket->name = NULL;
    newBracket->cond = NULL;
    newBracket->cond_d = NULL;
    newBracket->type = L_NONE;
    newBracket->code = NULL;

    newBracket->var = NULL;

    newBracket->content = NULL;
    newBracket->container = NULL;

    newBracket->func = NULL;

    newBracket->next = NULL;
    newBracket->first = NULL;

    return newBracket;
}

/* ====================================================
                        addBracket
    --------------------------------------------------
    Arg : last : élément quelconque de la liste
    --------------------------------------------------
    Def : Ajoute un élément à une liste chainée, et
          retourne un pointeur vers le nouvel dernier
          élément de celle-ci
   ==================================================== */
Bracket* addBracket(Bracket* last)
{
    Bracket* newBracket = initBracket(malloc(sizeof(Bracket)));

    if (last)
    {
        last = getLastBracket(last); /* On vérifie qu'il s'agit bien du dernier */
        newBracket->ID = last->ID + 1; /* ID suivant */
        newBracket->container = last->container;
        newBracket->func = last->func;

        newBracket->first = last->first; /* premier de la liste */
        last->next = newBracket; /* dernier de la liste */
    }
    else
    {
        newBracket->ID = 1; /* premier ID de la liste, 0 étant le 'main' */
        newBracket->first = newBracket; /* premier de la liste */
    }

    return newBracket;
}

/* ====================================================
                      destroyBracket
    --------------------------------------------------
    Arg : last : élément de la liste
    --------------------------------------------------
    Def : Détruit l'élément en le libérant de la mémoire
   ==================================================== */
void destroyBracket(Bracket* bracket)
{
    if (!bracket)
        return;

    if(bracket->name)
        free(bracket->name);

    if(bracket->content)
        destroyBracketList(bracket->content);

    if(bracket->var)
        destroyVarList(bracket->var);

    if(bracket->cond)
        destroyOperationList(bracket->cond);

    if(bracket->code)
        destroyCodeLineList(bracket->code);

    if(bracket->cond_d)
        destroyOpDebugList(bracket->cond_d);

    free(bracket);
}

/* ====================================================
                      destroyBracketList
    --------------------------------------------------
    Arg : last : élément de la liste
    --------------------------------------------------
    Def : Détruit la liste de la mémoire
   ==================================================== */
Bracket* destroyBracketList(Bracket* bracket)
{
    Bracket* toDestroy = NULL;

    if (!bracket)
        return NULL;

    bracket = bracket->first;

    while(bracket)
    {
        toDestroy = bracket;
        bracket = bracket->next;

        destroyBracket(toDestroy);
    }

    return NULL;
}

