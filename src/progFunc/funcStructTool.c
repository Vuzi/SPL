
#include "funcStructTool.h"

/* ====================================================
                        getLastFunc
    --------------------------------------------------
    Arg : last : élément de la liste chainée
    --------------------------------------------------
    Def : Retourne le dernier élément de la liste chainée
          ou NULL si impossible
   ==================================================== */
Func* getLastFunc(Func* last)
{
    if (!last)
        return NULL;

    while(last->next != NULL)
        last = last->next;

    return last;
}


/* ====================================================
                        getFuncByID
    --------------------------------------------------
    Arg : ID   : ID de fonction à chercher
          last : élément de la liste chainée
    --------------------------------------------------
    Def : Retourne le premier élément dont l'ID
          correspond, ou NULL s'il n'existe pas
   ==================================================== */
Func* getFuncByID(int ID, Func* func)
{
    if (!func)
        return NULL;

    func = func->first;

    while(func)
    {
        if (func->ID == ID)
            return func;
        func = func->next;
    }

    return NULL;
}


/* ====================================================
                        getFuncByName
    --------------------------------------------------
    Arg : name : nom de fonction à chercher
          last : élément de la liste chainée
    --------------------------------------------------
    Def : Retourne le premier élément dont le nom
          correspond, ou NULL s'il n'existe pas
   ==================================================== */
Func* getFuncByName(char* name, Func* func)
{
    if (!func || !name)
        return NULL;

    func = func->first;

    while(func)
    {
        if (!strcmp(name, func->name))
            return func;
        func = func->next;
    }

    return NULL;
}

/* ====================================================
                        initFunc
    --------------------------------------------------
    Arg : newFunc : fonction à initialiser
    --------------------------------------------------
    Def : Initialise les valeurs de la structure et
          renvoit un pointeur vers celle-ci
   ==================================================== */
Func* initFunc(Func* newFunc)
{
    if (!newFunc)
        return NULL;

    newFunc->name = NULL;
    newFunc->finalName = NULL;

    newFunc->type = TYPE_UNDEFINED;
    newFunc->arg = NULL;

    newFunc->content = NULL;

    newFunc->next = NULL;
    newFunc->first = NULL;

    return newFunc;
}

/* ====================================================
                         createSpecialFunc
    --------------------------------------------------
    Arg : -
    --------------------------------------------------
    Def : Permet de créer les fonctions built-in du
          langage
   ==================================================== */
Func* createSpecialFunc(void)
{
    Func* funcReturn = initFunc(malloc(sizeof(Func)));
    Func* out = initFunc(malloc(sizeof(Func)));
    Func* in = initFunc(malloc(sizeof(Func)));

    funcReturn->ID = -2;
    out->ID = -1;
    in->ID = -3;

    out->name = malloc((1+strlen("out"))*sizeof(char));
    strcpy(out->name, "out");

    out->finalName = malloc((1+strlen("out"))*sizeof(char));
    strcpy(out->finalName, "out");

    funcReturn->name = malloc((1+strlen("return"))*sizeof(char));
    strcpy(funcReturn->name, "return");

    funcReturn->finalName = malloc((1+strlen("return"))*sizeof(char));
    strcpy(funcReturn->finalName, "return");

    in->name = malloc((1+strlen("in"))*sizeof(char));
    strcpy(in->name, "in");

    in->finalName = malloc((1+strlen("in"))*sizeof(char));
    strcpy(in->finalName, "in");

    out->type = TYPE_VOID;
    funcReturn->type = TYPE_VOID;
    in->type = TYPE_VOID;

    out->next = funcReturn;
    funcReturn->next = in;
    in->first = funcReturn->first = out->first = out;

    return out;
}

/* ====================================================
                        addFunc
    --------------------------------------------------
    Arg : last : élément quelconque de la liste
    --------------------------------------------------
    Def : Ajoute un élément à une liste chainée, et
          retourne un pointeur vers le nouvel dernier
          élément de celle-ci
   ==================================================== */
Func* addFunc(Func* last)
{
    Func* newFunc = initFunc(malloc(sizeof(Func)));

    if (last)
    {
        last = getLastFunc(last); /* On vérifie qu'il s'agit bien du dernier */
        newFunc->ID = last->ID + 1; /* ID suivant */

        newFunc->first = last->first; /* premier de la liste */
        last->next = newFunc; /* dernier de la liste */
    }
    else
    {
        newFunc->ID = 1; /* premier ID de la liste, 0 étant le 'main' */
        newFunc->first = newFunc; /* premier de la liste */
    }

    return newFunc;
}

/* ====================================================
                      destroyFunc
    --------------------------------------------------
    Arg : last : élément de la liste
    --------------------------------------------------
    Def : Détruit l'élément en le libérant de la mémoire
   ==================================================== */
void destroyFunc(Func* func)
{
    if (!func)
        return;

    if(func->name)
        free(func->name);

    if(func->finalName)
        free(func->finalName);

    if(func->content)
        destroyBracketList(func->content);

    if(func->arg)
        destroyVarList(func->arg);

    free(func);
}

/* ====================================================
                      destroyFuncList
    --------------------------------------------------
    Arg : last : élément de la liste
    --------------------------------------------------
    Def : Détruit la liste de la mémoire
   ==================================================== */
Func* destroyFuncList(Func* func)
{
    Func* toDestroy = NULL;

    if (!func)
        return NULL;

    func = func->first;

    while(func)
    {
        toDestroy = func;
        func = func->next;

        destroyFunc(toDestroy);
    }

    return NULL;
}
