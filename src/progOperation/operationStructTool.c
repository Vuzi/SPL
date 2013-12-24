
#include "operationStructTool.h"

/* ====================================================
                        getLastOpDebug
    --------------------------------------------------
    Arg : last : élément de la liste chainée
    --------------------------------------------------
    Def : Retourne le dernier élément de la liste chainée
          ou NULL si impossible
   ==================================================== */
OperationDebug* getLastOpDebug(OperationDebug* last)
{
    if (!last)
        return NULL;

    while(last->next != NULL)
        last = last->next;

    return last;
}


/* ====================================================
                        OperationDebug
    --------------------------------------------------
    Arg : ID  : ID de fonction à chercher
          op  : élément de la liste chainée
    --------------------------------------------------
    Def : Retourne le premier élément dont l'ID
          correspond, ou NULL s'il n'existe pas
   ==================================================== */
OperationDebug* getOpDebugByID(int ID, OperationDebug* op)
{
    if (!op)
        return NULL;

    op = op->first;

    while(op)
    {
        if (op->ID == ID)
            return op;
        op = op->next;
    }

    return NULL;
}


/* ====================================================
                        initOpDebug
    --------------------------------------------------
    Arg : newOp : Operation à initialiser
    --------------------------------------------------
    Def : Initialise les valeurs de la structure et
          renvoit un pointeur vers celle-ci
   ==================================================== */
OperationDebug* initOpDebug(OperationDebug* newOp)
{
    if (!newOp)
        return NULL;

    newOp->startOrder = 0;

    newOp->type = OP_NONE;
    newOp->varType = TYPE_UNDEFINED;
    newOp->cast = TYPE_UNDEFINED;

    newOp->elem1 = NULL;
    newOp->operand = NULL;
    newOp->elem2 = NULL;

    newOp->func = NULL;

    newOp->next = NULL;
    newOp->first = NULL;

    return newOp;
}

/* ====================================================
                        addOpDebug
    --------------------------------------------------
    Arg : last : élément quelconque de la liste
    --------------------------------------------------
    Def : Ajoute un élément à une liste chainée, et
          retourne un pointeur vers le nouvel dernier
          élément de celle-ci
   ==================================================== */
OperationDebug* addOpDebug(OperationDebug* last)
{
    OperationDebug* newOp = initOpDebug(malloc(sizeof(OperationDebug)));

    if (last)
    {
        last = getLastOpDebug(last); /* On vérifie qu'il s'agit bien du dernier */
        newOp->ID = last->ID + 1; /* ID suivant */

        newOp->first = last->first; /* premier de la liste */
        last->next = newOp; /* dernier de la liste */
    }
    else
    {
        newOp->ID = 1; /* premier ID de la liste, 0 étant le 'main' */
        newOp->first = newOp; /* premier de la liste */
    }

    return newOp;
}


/* ====================================================
                      destroyOpDebug
    --------------------------------------------------
    Arg : last : élément de la liste
    --------------------------------------------------
    Def : Détruit l'élément en le libérant de la mémoire
   ==================================================== */
void destroyOpDebug(OperationDebug* opDebug)
{
    if (!opDebug)
        return;

    if(opDebug->type == OP_FUNC_CALL || opDebug->type == OP_PARENTH)
    {
        if(opDebug->elem1 && opDebug->elem1->ref)
            destroyOpDebug(opDebug->elem1->ref);
    }

    if(opDebug->elem1)
        free(opDebug->elem1);

    if(opDebug->elem2)
        free(opDebug->elem2);

    if(opDebug->operand)
        free(opDebug->operand);

    free(opDebug);
}

/* ====================================================
                      destroyOpDebugList
    --------------------------------------------------
    Arg : last : élément de la liste
    --------------------------------------------------
    Def : Détruit la liste de la mémoire
   ==================================================== */
OperationDebug* destroyOpDebugList(OperationDebug* opDebug)
{
    OperationDebug* toDestroy = NULL;

    if (!opDebug)
        return NULL;

    opDebug = opDebug->first;

    while(opDebug)
    {
        toDestroy = opDebug;
        opDebug = opDebug->next;

        destroyOpDebug(toDestroy);
    }

    return NULL;
}
