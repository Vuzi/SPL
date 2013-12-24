
#include "charElemStructTool.h"

/* ====================================================
                        getLastElem
    --------------------------------------------------
    Arg : last : élément de la liste chainée
    --------------------------------------------------
    Def : Retourne le dernier élément de la liste chainée
          ou NULL si impossible
   ==================================================== */
Elem* getLastElem(Elem* last)
{
    if (!last)
        return NULL;

    while(last->next != NULL)
        last = last->next;

    return last;
}

/* ====================================================
                        getLastCodeLine
    --------------------------------------------------
    Arg : last : élément de la liste chainée
    --------------------------------------------------
    Def : Retourne le dernier élément de la liste chainée
          ou NULL si impossible
   ==================================================== */
CodeLine* getLastCodeLine(CodeLine* last)
{
    if (!last)
        return NULL;

    while(last->next != NULL)
        last = last->next;

    return last;
}


/* ====================================================
                        getLastOperation
    --------------------------------------------------
    Arg : last : élément de la liste chainée
    --------------------------------------------------
    Def : Retourne le dernier élément de la liste chainée
          ou NULL si impossible
   ==================================================== */
Operation* getLastOperation(Operation* last)
{
    if (!last)
        return NULL;

    while(last->next != NULL)
        last = last->next;

    return last;
}

/* ====================================================
                        getElemByID
    --------------------------------------------------
    Arg : ID   : ID de fonction à chercher
          last : élément de la liste chainée
    --------------------------------------------------
    Def : Retourne le premier élément dont l'ID
          correspond, ou NULL s'il n'existe pas
   ==================================================== */
Elem* getElemByID(int ID, Elem* elem)
{
    if (!elem)
        return NULL;

    elem = elem->first;

    while(elem)
    {
        if (elem->ID == ID)
            return elem;
        elem = elem->next;
    }

    return NULL;
}

/* ====================================================
                        getCodeLineByID
    --------------------------------------------------
    Arg : ID   : ID de fonction à chercher
          last : élément de la liste chainée
    --------------------------------------------------
    Def : Retourne le premier élément dont l'ID
          correspond, ou NULL s'il n'existe pas
   ==================================================== */
CodeLine* getCodeLineByID(int ID, CodeLine* codeLine)
{
    if (!codeLine)
        return NULL;

    codeLine = codeLine->first;

    while(codeLine)
    {
        if (codeLine->ID == ID)
            return codeLine;
        codeLine = codeLine->next;
    }

    return NULL;
}

/* ====================================================
                        getOperationByID
    --------------------------------------------------
    Arg : ID   : ID de fonction à chercher
          last : élément de la liste chainée
    --------------------------------------------------
    Def : Retourne le premier élément dont l'ID
          correspond, ou NULL s'il n'existe pas
   ==================================================== */
Operation* getOperationByID(int ID, Operation* operation)
{
    if (!operation)
        return NULL;

    operation = operation->first;

    while(operation)
    {
        if (operation->ID == ID)
            return operation;
        operation = operation->next;
    }

    return NULL;
}

/* ====================================================
                      getElemByRef
    --------------------------------------------------
    Arg : str   : code a tester
          elem  : liste chainée
    --------------------------------------------------
    Def : Test si la référence pointée par str renvoit
          vers le code d'un élément de la liste
   ==================================================== */
Elem* getElemByRef(char* str, Elem* elem)
{
    if(!str || !elem)
        return NULL;

    return getElemByID(atoi(str+2), elem);
}

/* ====================================================
                    getCodeLineByRef
    --------------------------------------------------
    Arg : str      : code a tester
          codeLine : liste chainée
    --------------------------------------------------
    Def : Test si la référence pointée par str renvoit
          vers le code d'un élément de la liste
   ==================================================== */
CodeLine* getCodeLineByRef(char* str, CodeLine* codeLine)
{
    if(!str || !codeLine)
        return NULL;

    return getCodeLineByID(atoi(str+2), codeLine);
}

/* ====================================================
                        initElem
    --------------------------------------------------
    Arg : newElem : fonction à initialiser
    --------------------------------------------------
    Def : Initialise les valeurs de la structure et
          renvoit un pointeur vers celle-ci
   ==================================================== */
Elem* initElem(Elem* newElem)
{
    if (!newElem)
        return NULL;

    newElem->name = NULL;
    newElem->cond = NULL;

    newElem->code = NULL;

    newElem->content = NULL;
    newElem->container = NULL;

    newElem->next = NULL;
    newElem->first = NULL;

    return newElem;
}

/* ====================================================
                        initCodeLine
    --------------------------------------------------
    Arg : newCodeLine : fonction à initialiser
    --------------------------------------------------
    Def : Initialise les valeurs de la structure et
          renvoit un pointeur vers celle-ci
   ==================================================== */
CodeLine* initCodeLine(CodeLine* newCodeLine)
{
    if (!newCodeLine)
        return NULL;

    newCodeLine->code = NULL;

    newCodeLine->link = NULL;
    newCodeLine->link_b = NULL;

    newCodeLine->op = NULL;
    newCodeLine->opDebug = NULL;

    newCodeLine->next = NULL;
    newCodeLine->first = NULL;

    return newCodeLine;
}

/* ====================================================
                        initOperation
    --------------------------------------------------
    Arg : newOperation : fonction à initialiser
    --------------------------------------------------
    Def : Initialise les valeurs de la structure et
          renvoit un pointeur vers celle-ci
   ==================================================== */
Operation* initOperation(Operation* newOperation)
{
    if (!newOperation)
        return NULL;

    newOperation->info = NULL;
    newOperation->link = NULL;

    newOperation->type = OP_NONE;

    newOperation->first = NULL;
    newOperation->next = NULL;
    newOperation->previous = NULL;

    return newOperation;
}

/* ====================================================
                        addElem
    --------------------------------------------------
    Arg : last : élément quelconque de la liste
    --------------------------------------------------
    Def : Ajoute un élément à une liste chainée, et
          retourne un pointeur vers le nouvel dernier
          élément de celle-ci
   ==================================================== */
Elem* addElem(Elem* last)
{
    Elem* newElem = initElem(malloc(sizeof(Elem)));

    if (last)
    {
        last = getLastElem(last); /* On vérifie qu'il s'agit bien du dernier */
        newElem->ID = last->ID + 1; /* ID suivant */
        newElem->container = last->container;
        newElem->first = last->first; /* premier de la liste */
        last->next = newElem; /* dernier de la liste */
    }
    else
    {
        newElem->ID = 1; /* premier ID de la liste, 0 étant le 'main' */
        newElem->first = newElem; /* premier de la liste */
    }

    return newElem;
}

/* ====================================================
                        addCodeLine
    --------------------------------------------------
    Arg : last : élément quelconque de la liste
    --------------------------------------------------
    Def : Ajoute un élément à une liste chainée, et
          retourne un pointeur vers le nouvel dernier
          élément de celle-ci
   ==================================================== */
CodeLine* addCodeLine(CodeLine* last)
{
    CodeLine* newCodeLine = initCodeLine(malloc(sizeof(CodeLine)));

    if (last)
    {
        last = getLastCodeLine(last); /* On vérifie qu'il s'agit bien du dernier */
        newCodeLine->ID = last->ID + 1; /* ID suivant */
        newCodeLine->first = last->first; /* premier de la liste */
        last->next = newCodeLine; /* dernier de la liste */
    }
    else
    {
        newCodeLine->ID = 1; /* premier ID de la liste, 0 étant le 'main' */
        newCodeLine->first = newCodeLine; /* premier de la liste */
    }

    return newCodeLine;
}

/* ====================================================
                        addOperation
    --------------------------------------------------
    Arg : last : élément quelconque de la liste
    --------------------------------------------------
    Def : Ajoute un élément à une liste chainée, et
          retourne un pointeur vers le nouvel dernier
          élément de celle-ci
   ==================================================== */
Operation* addOperation(Operation* last)
{
    Operation* newOperation = initOperation(malloc(sizeof(Operation)));

    if (last)
    {
        last = getLastOperation(last); /* On vérifie qu'il s'agit bien du dernier */
        newOperation->ID = last->ID + 1; /* ID suivant */
        newOperation->first = last->first; /* premier de la liste */
        last->next = newOperation; /* dernier de la liste */
        newOperation->previous = last;
    }
    else
    {
        newOperation->ID = 1; /* premier ID de la liste, 0 étant le 'main' */
        newOperation->first = newOperation; /* premier de la liste */
    }

    return newOperation;
}

/* ====================================================
                      destroyElem
    --------------------------------------------------
    Arg : last : élément de la liste
    --------------------------------------------------
    Def : Détruit l'élément en le libérant de la mémoire
   ==================================================== */
void destroyElem(Elem* elem)
{
    if (!elem)
        return;

    if(elem->name)
        free(elem->name);

    if(elem->cond)
        destroyOperationList(elem->cond);

    if(elem->code)
        destroyCodeLineList(elem->code);

    if(elem->content)
        destroyElemList(elem->content);

    free(elem);
}

/* ====================================================
                      destroyElemNoCl
    --------------------------------------------------
    Arg : last : élément de la liste
    --------------------------------------------------
    Def : Détruit l'élément en le libérant de la mémoire
          sans détruire les CodeLine
   ==================================================== */
void destroyElemNoCl(Elem* elem)
{
    if (!elem)
        return;

    if(elem->name)
        free(elem->name);

    if(elem->content)
        destroyElemNoClList(elem->content);

    free(elem);
}

/* ====================================================
                      destroyCodeLine
    --------------------------------------------------
    Arg : last : élément de la liste
    --------------------------------------------------
    Def : Détruit l'élément en le libérant de la mémoire
   ==================================================== */
void destroyCodeLine(CodeLine* codeLine)
{
    if (!codeLine)
        return;

    if(codeLine->code)
        free(codeLine->code);

    if(codeLine->op)
        destroyOperationList(codeLine->op);

    if(codeLine->opDebug)
        destroyOpDebugList(codeLine->opDebug);

    free(codeLine);
}

/* ====================================================
                      destroyOperation
    --------------------------------------------------
    Arg : last : élément de la liste
    --------------------------------------------------
    Def : Détruit l'élément en le libérant de la mémoire
   ==================================================== */
void destroyOperation(Operation* operation)
{
    if (!operation)
        return;

    if(operation->info)
        free(operation->info);

    if(operation->link)
        destroyOperationList(operation->link);

    free(operation);
}

/* ====================================================
                      destroyElemList
    --------------------------------------------------
    Arg : last : élément de la liste
    --------------------------------------------------
    Def : Détruit la liste de la mémoire
   ==================================================== */
Elem* destroyElemList(Elem* elem)
{
    Elem* toDestroy = NULL;

    if (!elem)
        return NULL;

    elem = elem->first;

    while(elem)
    {
        toDestroy = elem;
        elem = elem->next;

        destroyElem(toDestroy);
    }

    return NULL;
}

/* ====================================================
                      destroyElemNoClList
    --------------------------------------------------
    Arg : last : élément de la liste
    --------------------------------------------------
    Def : Détruit la liste de la mémoire
   ==================================================== */
Elem* destroyElemNoClList(Elem* elem)
{
    Elem* toDestroy = NULL;

    if (!elem)
        return NULL;

    elem = elem->first;

    while(elem)
    {
        toDestroy = elem;
        elem = elem->next;

        destroyElemNoCl(toDestroy);
    }

    return NULL;
}

/* ====================================================
                      destroyCodeLineList
    --------------------------------------------------
    Arg : last : élément de la liste
    --------------------------------------------------
    Def : Détruit la liste de la mémoire
   ==================================================== */
CodeLine* destroyCodeLineList(CodeLine* codeLine)
{
    CodeLine* toDestroy = NULL;

    if (!codeLine)
        return NULL;

    codeLine = codeLine->first;

    while(codeLine)
    {
        toDestroy = codeLine;
        codeLine = codeLine->next;

        destroyCodeLine(toDestroy);
    }

    return NULL;
}

/* ====================================================
                      destroyOperationList
    --------------------------------------------------
    Arg : last : élément de la liste
    --------------------------------------------------
    Def : Détruit la liste de la mémoire
   ==================================================== */
Operation* destroyOperationList(Operation* operation)
{
    Operation* toDestroy = NULL;

    if (!operation)
        return NULL;

    operation = operation->first;

    while(operation)
    {
        toDestroy = operation;
        operation = operation->next;

        destroyOperation(toDestroy);
    }

    return NULL;
}
