
#include "operationStructNew.h"

/* ====================================================
                      getOpDebugBr
    --------------------------------------------------
    Arg : br   : bracket à traiter
          prog : Programme
    --------------------------------------------------
    Def : Traite les opérations et les vérifie
   ==================================================== */
void getOpDebugBr(Bracket *br, Prog *prog)
{
    CodeLine *cl = NULL;

    if(br)
    {
        cl = br->code;

        while(cl)
        {
            if(cl->link_b)
            {
                INC_DEBUG;

                if(cl->link_b->cond)
                {
                    if(g->g_debug)
                    {
                        DEBUG(INFO_LOG, "Condition a traiter : ");
                        debug_elemPrintLine(cl->link_b->cond, 1);
                    }

                    cl->link_b->cond_d = getOpDebugCL(cl->link_b->cond, prog, br);
                    cl->link_b->cond = destroyOperationList(cl->link_b->cond); /* Libération de l'élément traité */
                }
                getOpDebugBr(cl->link_b, prog);

                DEC_DEBUG;
            }

            else
            {
                if(g->g_debug && cl->op)
                {
                    DEBUG(INFO_LOG, "Ligne de code a traiter : ");
                    debug_elemPrintLine(cl->op, 1);
                }
                INC_DEBUG;

                cl->opDebug = getOpDebugCL(cl->op, prog, br);
                cl->op = destroyOperationList(cl->op); /* Libération de l'élément de la mémoire */

                DEC_DEBUG;
            }
            cl = cl->next;
        }
    }
}

/* ====================================================
                      getOpDebugCL
    --------------------------------------------------
    Arg : op   : Opération à traduire
          br   : bracket à traiter
          prog : Programme
    --------------------------------------------------
    Def : Traduit l'opération en OperationDebug et
          vérifie sa validité
   ==================================================== */
OperationDebug* getOpDebugCL(Operation *op, Prog *prog, Bracket* br)
{
    OperationDebug *opDebug = NULL;

    if (!op)
        return NULL;

    op = getLastOperation(op);

    /* On chercher en premier les références aux variables */
    opDebug = getOpDebugVarAll(opDebug, op, prog, br);

    /* Ici les fonctions, plus tard */
    opDebug = getOpDebugFuncAll(opDebug, op, prog, br);

    /* On cherche les parenthèses */
    opDebug = getOpDebugParenthAll(opDebug, op, prog, br);

    /* On cherche maintenant les ++/-- */
    opDebug = getOpDebugIncAll(opDebug, op);

    /* On cherche les +/- unaires */
    opDebug = getOpDebugUnaAll(opDebug, op);

    /* On cherche les ! */
    opDebug = getOpDebugExcAll(opDebug, op);

    /* On cherche les *, /, % */
    opDebug = getOpDebugCalcFirstAll(opDebug, op);

    /* On cherche les + et - */
    opDebug = getOpDebugCalcSecondAll(opDebug, op);

    /* On cherche les >, <, >=, <= et == */
    opDebug = getOpDebugCompAll(opDebug, op);

    /* On cherche les && et || */
    opDebug = getOpDebugAndOrAll(opDebug, op);

    /* On cherche les +=, -=, *=, /=, %= */
    opDebug = getOpDebugAssignCalcAll(opDebug, op);

    /* On cherche le = */
    opDebug = getOpDebugAssignAll(opDebug, op);

    /* Définit le ou les points de départ du calcul */
    getOpDebugStartOrder(opDebug, op);


    if(g->g_debugOp)
    {
        printf("\n========== Liste des operations ===========");
        debug_opPrintList(opDebug, 1);
        printf("\n===========================================");
    }

    if(opDebug)
        return opDebug->first;
    else
        return opDebug;
}

/* ====================================================
                      getOpDebugVarAll
    --------------------------------------------------
    Arg : opDebug : Opération resultante
          op      : Opération à traduire
          br      : bracket à traiter
          prog    : Programme
    --------------------------------------------------
    Def : Traite les variables dans op
   ==================================================== */
OperationDebug* getOpDebugVarAll(OperationDebug *opDebug, Operation *op, Prog *prog, Bracket* br)
{
    while(op)
    {
        if(op->type == OP_VAR) /* Si on trouve une variable */
            opDebug = getOpDebugVar(opDebug, op, prog, br);

        else if(op->type == OP_NAT_VAR) /* Si on trouve une variable naturelle */
            opDebug = getOpDebugNatVar(opDebug, op);

        op = op->previous;
    }

    return opDebug;
}

/* ====================================================
                      getOpDebugFuncAll
    --------------------------------------------------
    Arg : opDebug : Opération resultante
          op      : Opération à traduire
          br      : bracket à traiter
          prog    : Programme
    --------------------------------------------------
    Def : Traite les appels de fonctions dans op
   ==================================================== */
OperationDebug* getOpDebugFuncAll(OperationDebug *opDebug, Operation *op, Prog *prog, Bracket* br)
{
    while(op)
    {
        if(op->type == OP_FUNC_CALL) /* Si on trouve une fonction */
            opDebug = getOpDebugFunc(opDebug, op, prog, br);

        op = op->previous;
    }

    return opDebug;
}

/* ====================================================
                    getOpDebugParenthAll
    --------------------------------------------------
    Arg : opDebug : Opération resultante
          op      : Opération à traduire
          br      : bracket à traiter
          prog    : Programme
    --------------------------------------------------
    Def : Traite les parenthèses dans op
   ==================================================== */
OperationDebug* getOpDebugParenthAll(OperationDebug *opDebug, Operation *op, Prog *prog, Bracket* br)
{
    while(op)
    {
        if(op->type == OP_PARENTH)
            opDebug = getOpDebugParenth(opDebug, op, prog, br);

        op = op->previous;
    }

    return opDebug;
}

/* ====================================================
                    getOpDebugIncAll
    --------------------------------------------------
    Arg : opDebug : Opération resultante
          op      : Opération à traduire
    --------------------------------------------------
    Def : Traite les incrémentations (--/++) dans op
   ==================================================== */
OperationDebug* getOpDebugIncAll(OperationDebug *opDebug, Operation *op)
{
    while(op)
    {
        if(op->type == OP_MATHS && strlen(op->info) == 2)
        {
            if(op->info[0] == '+' && op->info[1] == '+') /* ++ */
                opDebug = getOpDebugInc(opDebug, op);

            else if(op->info[0] == '-' && op->info[1] == '-') /* -- */
                opDebug = getOpDebugInc(opDebug, op);
        }

        op = op->previous;
    }

    return opDebug;
}

/* ====================================================
                    getOpDebugUnaAll
    --------------------------------------------------
    Arg : opDebug : Opération resultante
          op      : Opération à traduire
    --------------------------------------------------
    Def : Traite les opérateur unaires (-/+) dans op
   ==================================================== */
OperationDebug* getOpDebugUnaAll(OperationDebug *opDebug, Operation *op)
{
    while(op)
    {
        if(op->type == OP_MATHS && strlen(op->info) == 1)
        {
            if(op->info[0] == '+') /* + */
                opDebug = getOpDebugUna(opDebug, op);

            else if(op->info[0] == '-') /* - */
                opDebug = getOpDebugUna(opDebug, op);
        }

        op = op->previous;
    }

    return opDebug;
}

/* ====================================================
                    getOpDebugExcAll
    --------------------------------------------------
    Arg : opDebug : Opération resultante
          op      : Opération à traduire
    --------------------------------------------------
    Def : Traite le point d'exclamation (!) dans op
   ==================================================== */
OperationDebug* getOpDebugExcAll(OperationDebug *opDebug, Operation *op)
{
    while(op)
    {
        if(op->type == OP_MATHS && strlen(op->info) == 1 && op->info[0] == '!')
            opDebug = getOpDebugExc(opDebug, op);

        op = op->previous;
    }

    return opDebug;
}

/* ====================================================
                    getOpDebugCalcFirstAll
    --------------------------------------------------
    Arg : opDebug : Opération resultante
          op      : Opération à traduire
    --------------------------------------------------
    Def : Traite le calculs prioritaires (*, /, %)
          dans op
   ==================================================== */
OperationDebug* getOpDebugCalcFirstAll(OperationDebug *opDebug, Operation *op)
{
    while(op)
    {
        if(op->type == OP_MATHS && strlen(op->info) == 1)
        {
            if(op->info[0] == '*') /* * */
                opDebug = getOpDebugCalc(opDebug, op);

            else if(op->info[0] == '/') /* / */
                opDebug = getOpDebugCalc(opDebug, op);

            else if(op->info[0] == '%') /* % */
                opDebug = getOpDebugCalc(opDebug, op);
        }

        op = op->previous;
    }

    return opDebug;
}

/* ====================================================
                    getOpDebugCalcSecondAll
    --------------------------------------------------
    Arg : opDebug : Opération resultante
          op      : Opération à traduire
    --------------------------------------------------
    Def : Traite le calculs secondaires (+/-) dans op
   ==================================================== */
OperationDebug* getOpDebugCalcSecondAll(OperationDebug *opDebug, Operation *op)
{
    while(op)
    {
        if(op->type == OP_MATHS && strlen(op->info) == 1)
        {
            if(op->info[0] == '+') /* + */
                opDebug = getOpDebugCalc(opDebug, op);

            else if(op->info[0] == '-') /* - */
                opDebug = getOpDebugCalc(opDebug, op);
        }

        op = op->previous;
    }

    return opDebug;
}

/* ====================================================
                    getOpDebugCompAll
    --------------------------------------------------
    Arg : opDebug : Opération resultante
          op      : Opération à traduire
    --------------------------------------------------
    Def : Traite le opérateurs de comparaisons (>/</..)
          dans op
   ==================================================== */
OperationDebug* getOpDebugCompAll(OperationDebug *opDebug, Operation *op)
{
    while(op)
    {
        if(op->type == OP_MATHS)
        {
            if(strlen(op->info) == 1 && (op->info[0] == '>' || op->info[0] == '<')) /* < ou > */
                opDebug = getOpDebugComp(opDebug, op);

            else if(strlen(op->info) == 2)
            {
                if(op->info[0] == '>' && op->info[1] == '=') /* >= */
                    opDebug = getOpDebugComp(opDebug, op);

                else if(op->info[0] == '<' && op->info[1] == '=') /* <= */
                    opDebug = getOpDebugComp(opDebug, op);

                else if(op->info[0] == '=' && op->info[1] == '=') /* == */
                    opDebug = getOpDebugComp(opDebug, op);

                else if(op->info[0] == '!' && op->info[1] == '=') /* != */
                    opDebug = getOpDebugComp(opDebug, op);
            }
        }

        op = op->previous;
    }

    return opDebug;
}

/* ====================================================
                    getOpDebugAndOrAll
    --------------------------------------------------
    Arg : opDebug : Opération resultante
          op      : Opération à traduire
    --------------------------------------------------
    Def : Traite le opérateurs de comparaisons (ET et OU)
          dans op
   ==================================================== */
OperationDebug* getOpDebugAndOrAll(OperationDebug *opDebug, Operation *op)
{
    while(op)
    {
        if(op->type == OP_MATHS)
        {
            if(strlen(op->info) == 1)
            {
                if(op->info[0] == '&') /* & */
                    opDebug = getOpDebugComp(opDebug, op);

                else if(op->info[0] == '|') /* | */
                    opDebug = getOpDebugComp(opDebug, op);
            }
            else if(strlen(op->info) == 2)
            {
                if(op->info[0] == '&' && op->info[1] == '&') /* && */
                    opDebug = getOpDebugComp(opDebug, op);

                else if(op->info[0] == '|' && op->info[1] == '|') /* || */
                    opDebug = getOpDebugComp(opDebug, op);
            }
        }

        op = op->previous;
    }

    return opDebug;
}

/* ====================================================
                    getOpDebugAssignCalcAll
    --------------------------------------------------
    Arg : opDebug : Opération resultante
          op      : Opération à traduire
    --------------------------------------------------
    Def : Traite le opérateurs d'assignation avec calcul
          de op
   ==================================================== */
OperationDebug* getOpDebugAssignCalcAll(OperationDebug *opDebug, Operation *op)
{
    while(op)
    {
        if(op->type == OP_MATHS && strlen(op->info) == 2)
        {
            if(op->info[0] == '+' && op->info[1] == '=') /* += */
                opDebug = getOpDebugAssign(opDebug, op);

            else if(op->info[0] == '-' && op->info[1] == '=') /* -= */
                opDebug = getOpDebugAssign(opDebug, op);

            else if(op->info[0] == '*' && op->info[1] == '=') /* *= */
                opDebug = getOpDebugAssign(opDebug, op);

            else if(op->info[0] == '/' && op->info[1] == '=') /* /= */
                opDebug = getOpDebugAssign(opDebug, op);

            else if(op->info[0] == '%' && op->info[1] == '=') /* %= */
                opDebug = getOpDebugAssign(opDebug, op);
        }

        op = op->previous;
    }

    return opDebug;
}

/* ====================================================
                    getOpDebugAssignAll
    --------------------------------------------------
    Arg : opDebug : Opération resultante
          op      : Opération à traduire
    --------------------------------------------------
    Def : Traite le opérateurs d'assignation de op
   ==================================================== */
OperationDebug* getOpDebugAssignAll(OperationDebug *opDebug, Operation *op)
{
    while(op)
    {
        if(op->type == OP_MATHS && strlen(op->info) == 1 && op->info[0] == '=')
            opDebug = getOpDebugAssign(opDebug, op);

        op = op->previous;
    }

    return opDebug;
}

/* ====================================================
                    getOpDebugStartOrder
    --------------------------------------------------
    Arg : opDebug : Opération resultante
          op      : Opération à traduire
          br      : bracket à traiter
          prog    : Programme
    --------------------------------------------------
    Def : Définit l'ordre de début des opérations
   ==================================================== */
void getOpDebugStartOrder(OperationDebug *opDebug, Operation *op)
{
    int i = -1, cpt = 1;
    char sep = 1;

    if(!opDebug)
        return;

    op = op->first;

    while(op)
    {
        /* S'il est d'une nouvelle opération */
        if((op->type == OP_REF)&& op->ID != i)
        {
            if((opDebug = getOpDebugByID(op->ID, opDebug)) && sep)
            {
                sep = 0;
                i = opDebug->ID;
                opDebug->startOrder = cpt++;
            }
            else if(!sep)
            {
                PRINTFNV(ERROR_LOG, "Des elements du code sont à la suite sans opérations. Pour ecrire plusieurs elements a la suite il est necessaire d'utiliser le signe \",\" : \n");
                LOG(ERROR_LOG, "Des éléments du code sont à la suite sans opérations. Pour écrire plusieurs éléments à la suite il est nécessaire d'utiliser le signe \",\" ");

                debug_elemPrintLine(op->first, 1);

                pthread_exit((void*)ERROR_COMPIL);
            }
            else
            {
                PRINTFNV(ERROR_LOG, "Une erreur est survenue lors du traitement du code");
                LOG(ERROR_LOG, "Une erreur est survenue lors du traitement du code");

                debug_elemPrintLine(op, 1);

                pthread_exit((void*)ERROR_COMPIL);
            }
        }
        /* Un séparateur */
        else if (op->type != OP_REF && op->info && strlen(op->info) == 1 && op->info[0] == ',')
        {
            sep = 1;
        }
        /* Autre chose qu'un séparateur */
        else if(op->type != OP_REF)
        {
            PRINTFNV(ERROR_LOG, "Un element du code n'a pas pu etre traite : \n");
            LOG(ERROR_LOG, "Un élément du code n'a pas pu être traité ");

            debug_elemPrintLine(op, 1);

            pthread_exit((void*)ERROR_COMPIL);
        }

        op = op->next;
    }
}

/* ====================================================
                    getOpDebugAssign
    --------------------------------------------------
    Arg : opDebug : Opération resultante
          op      : Opération à traduire
    --------------------------------------------------
    Def : Traite un opérateur d'assignement
   ==================================================== */
OperationDebug* getOpDebugAssign(OperationDebug *opDebug, Operation *op)
{
    int i = 0;
    Operation *oldOp = op;
    OperationDebug* opLeft = NULL, *opRight = NULL;

    /* Il doit forcément y avoir quelque chose à droite et à gauche */
    if(!op->previous || op->previous->type != OP_REF || !op->next || op->next->type != OP_REF)
    {
        PRINTFNV(ERROR_LOG, "Un operateur d'affectation (%s) n'est pas correctement utilise : \n", op->info);
        LOG(ERROR_LOG, "Un operateur d'affectation (%s) n'est pas correctement utilisé", op->info);

        debug_elemPrintLine(op->first, 1);

        pthread_exit((void*)ERROR_COMPIL);
    }

    else
    {
        if(g->g_debugOp)
            DEBUG(INFO_LOG, "Operateur d'affectation (%s)", op->info);

        opLeft = getOpDebugByID(op->previous->ID, opDebug);
        opRight = getOpDebugByID(op->next->ID, opDebug);

        if((!opLeft || !opRight) || (opLeft->varType == TYPE_VOID || opRight->varType == TYPE_VOID))
        {
            PRINTFNV(ERROR_LOG, "Une operation d'affectation (%s) utilise un element de type void (vide) : \n", op->info);
            LOG(ERROR_LOG, "Une opération d'affectation (%s) utilise un élément de type void (vide)", op->info);

            debug_elemPrintLine(op->first, 1);

            pthread_exit((void*)ERROR_COMPIL);
        }

        /* L'élément de gauche doit être une variable */
        /* Plus tard, également tester les types des 2 élémens */
        else if(opLeft->type != OP_VAR)
        {
            PRINTFNV(ERROR_LOG, "Une operation d'affectation (%s) est effectuee sur autre chose qu'une variable : \n", op->info);
            LOG(ERROR_LOG, "Une opération d'affectation (%s) est effectuée sur autre chose qu'une variable ", op->info);

            debug_elemPrintLine(op->first, 1);

            pthread_exit((void*)ERROR_COMPIL);
        }

        else
        {
            opDebug = addOpDebug(opDebug);
            opDebug->type = OP_ASSIGN;

            opDebug->elem1 = malloc(sizeof(OpElem));
            opDebug->elem1->ref = opLeft;
            opDebug->elem1Type = ELEM_REF;

            opDebug->elem2 = malloc(sizeof(OpElem));
            opDebug->elem2->ref = opRight;
            opDebug->elem2Type = ELEM_REF;

            /* Type définitif */
            opDebug->varType = opLeft->type;

            /* Cast */
            if(opLeft->varType != opRight->varType)
            {
                opRight->cast = opLeft->varType;
            }

            /* Opérateur */
            opDebug->operand = malloc(sizeof(char)*strlen(op->info));
            strcpy(opDebug->operand, op->info);
            opDebug->operand[strlen(op->info)] = '\0';

            /* On met la référence dans l'opération */
            op->type = OP_REF;
            op->ID = opDebug->ID;

            i = op->next->ID;
            op = op->next;

            /* On met la référence dans les éléments suivants */
            while(op && op->ID == i && op->type == OP_REF)
            {
                op->type = OP_REF;
                op->ID = opDebug->ID;

                op = op->next;
            }

            i = oldOp->previous->ID;
            op = oldOp->previous;

            /* On met la référence dans les éléments suivants */
            while(op && op->ID == i && op->type == OP_REF)
            {
                op->type = OP_REF;
                op->ID = opDebug->ID;

                op = op->previous;
            }
        }
    }

    return opDebug;
}

/* ====================================================
                    getOpDebugComp
    --------------------------------------------------
    Arg : opDebug : Opération resultante
          op      : Opération à traduire
    --------------------------------------------------
    Def : Traite un opérateur de comparaison
   ==================================================== */
OperationDebug* getOpDebugComp(OperationDebug *opDebug, Operation *op)
{
    int i = 0;
    Operation *oldOp = op;
    OperationDebug* opLeft = NULL, *opRight = NULL;

    /* Il doit forcément y avoir un signe à gauche (previous) et à droite (next) */
    if(!op->previous || op->previous->type != OP_REF || !op->next || op->next->type != OP_REF)
    {
        PRINTFNV(ERROR_LOG, "Un operateur de comparaison (%s) n'est pas correctement utilise : \n", op->info);
        LOG(ERROR_LOG, "Un operateur de comparaison (%s) n'est pas correctement utilisé", op->info);

        debug_elemPrintLine(op->first, 1);

        pthread_exit((void*)ERROR_COMPIL);
    }

    else
    {
        if(g->g_debugOp)
            DEBUG(INFO_LOG, "Operateur de comparaison (%s)", op->info);

        opLeft = getOpDebugByID(op->previous->ID, opDebug);
        opRight = getOpDebugByID(op->next->ID, opDebug);

        if((!opLeft || !opRight) || (opLeft->varType == TYPE_VOID || opRight->varType == TYPE_VOID))
        {
            PRINTFNV(ERROR_LOG, "Une operation de comparaison (%s) utilise un element de type void (vide) : \n", op->info);
            LOG(ERROR_LOG, "Une opération de comparaison (%s) utilise un élément de type void (vide)", op->info);

            debug_elemPrintLine(op->first, 1);

            pthread_exit((void*)ERROR_COMPIL);
        }

        else
        {
            opDebug = addOpDebug(opDebug);
            opDebug->type = OP_MATHS;

            opDebug->elem1 = malloc(sizeof(OpElem));
            opDebug->elem1->ref = opLeft;
            opDebug->elem1Type = ELEM_REF;

            opDebug->elem2 = malloc(sizeof(OpElem));
            opDebug->elem2->ref = opRight;
            opDebug->elem2Type = ELEM_REF;

            /* Type définitif */
            opDebug->varType = getNewType(opLeft->varType, opRight->varType);

            /* Cast */
            if(opDebug->varType != opLeft->varType )
            {
                opLeft->cast = opDebug->varType;
            }

            if(opDebug->varType != opRight->varType)
            {
                opRight->cast = opDebug->varType;
            }

            /* Opérateur */
            opDebug->operand = malloc(sizeof(char)*strlen(op->info));
            strcpy(opDebug->operand, op->info);
            opDebug->operand[strlen(op->info)] = '\0';

            /* On met la référence dans l'opération */
            op->type = OP_REF;
            op->ID = opDebug->ID;

            i = op->next->ID;
            op = op->next;

            /* On met la référence dans les éléments suivants */
            while(op && op->ID == i && op->type == OP_REF)
            {
                op->type = OP_REF;
                op->ID = opDebug->ID;

                op = op->next;
            }

            i = oldOp->previous->ID;
            op = oldOp->previous;

            /* On met la référence dans les éléments suivants */
            while(op && op->ID == i && op->type == OP_REF)
            {
                op->type = OP_REF;
                op->ID = opDebug->ID;

                op = op->previous;
            }
        }

    }

    return opDebug;
}

/* ====================================================
                    getOpDebugCalc
    --------------------------------------------------
    Arg : opDebug : Opération resultante
          op      : Opération à traduire
    --------------------------------------------------
    Def : Traite un opérateur de calcul
   ==================================================== */
OperationDebug* getOpDebugCalc(OperationDebug *opDebug, Operation *op)
{
    int i = 0;
    Operation *oldOp = op;
    OperationDebug* opLeft = NULL, *opRight = NULL;

    if((!op->next || !op->previous) || (op->next->type != OP_REF || op->previous->type != OP_REF)) /* S'il n'y a pas au moins 2 éléments autour */
    {
        PRINTFNV(ERROR_LOG, "Un operateur mathematique (%s) n'est pas correctement utilise : \n", op->info);
        LOG(ERROR_LOG, "Un operateur mathematique (%s) n'est pas correctement utilisé", op->info);

        debug_elemPrintLine(op->first, 1);

        pthread_exit((void*)ERROR_COMPIL);
    }

    else
    {
        if(g->g_debugOp)
            DEBUG(INFO_LOG, "Operateur de calcul (%s)", op->info);

        opLeft = getOpDebugByID(op->previous->ID, opDebug);
        opRight = getOpDebugByID(op->next->ID, opDebug);

        if((!opLeft || !opRight) || (opLeft->varType == TYPE_VOID || opRight->varType == TYPE_VOID))
        {
            PRINTFNV(ERROR_LOG, "Une operation mathematique (%s) utilise un element de type void (vide) : \n", op->info);
            LOG(ERROR_LOG, "Une opération mathematique (%s) utilise un élément de type void (vide)", op->info);

            debug_elemPrintLine(op->first, 1);

            pthread_exit((void*)ERROR_COMPIL);
        }

        else
        {
            opDebug = addOpDebug(opDebug);
            opDebug->type = OP_MATHS;

            opDebug->elem1 = malloc(sizeof(OpElem));
            opDebug->elem1->ref = opLeft;
            opDebug->elem1Type = ELEM_REF;

            opDebug->elem2 = malloc(sizeof(OpElem));
            opDebug->elem2->ref = opRight;
            opDebug->elem2Type = ELEM_REF;

            /* Type définitif */
            opDebug->varType = getNewType(opLeft->varType, opRight->varType);

            /* Cast */
            if(opDebug->varType != opLeft->varType )
            {
                opLeft->cast = opDebug->varType;
            }

            if(opDebug->varType != opRight->varType)
            {
                opRight->cast = opDebug->varType;
            }

            /* Opérateur */
            opDebug->operand = malloc(sizeof(char)*2);
            opDebug->operand[0] = op->info[0];
            opDebug->operand[1] = '\0';

            /* On met la référence dans l'opération */
            op->type = OP_REF;
            op->ID = opDebug->ID;

            i = op->next->ID;
            op = op->next;

            /* On met la référence dans les éléments suivants */
            while(op && op->ID == i && op->type == OP_REF)
            {
                op->type = OP_REF;
                op->ID = opDebug->ID;

                op = op->next;
            }

            i = oldOp->previous->ID;
            op = oldOp->previous;

            /* On met la référence dans les éléments suivants */
            while(op && op->ID == i && op->type == OP_REF)
            {
                op->type = OP_REF;
                op->ID = opDebug->ID;

                op = op->previous;
            }
        }

    }

    return opDebug;
}

/* ====================================================
                    getOpDebugExc
    --------------------------------------------------
    Arg : opDebug : Opération resultante
          op      : Opération à traduire
    --------------------------------------------------
    Def : Traite un opérateur d'inversion logique
   ==================================================== */
OperationDebug* getOpDebugExc(OperationDebug *opDebug, Operation *op)
{
    int i = 0;
    OperationDebug *opTest = NULL;

    /* Il n'y a rien après */
    if(!op->next || op->next->type != OP_REF)
    {
        PRINTFNV(ERROR_LOG, "Un operateur (%s) n'affecte aucune variable : \n", op->info);
        LOG(ERROR_LOG, "Un opérateur (%s) n'affecte aucune variable", op->info);

        debug_elemPrintLine(op->first, 1);

        pthread_exit((void*)ERROR_COMPIL);
    }

    else
    {
        opTest = getOpDebugByID(op->next->ID, opDebug);

        /* S'il affecte un type void */
        if(!opTest || opTest->varType == TYPE_VOID)
        {
            PRINTFNV(ERROR_LOG, "Un operateur (%s) est utilise sur un element de type void (vide) : \n", op->info);
            LOG(ERROR_LOG, "Un operateur (%s) est utilisé sur un élément de type void (vide)", op->info);

            debug_elemPrintLine(op->first, 1);

            pthread_exit((void*)ERROR_COMPIL);
        }
        /* Un opérateur unaire n'a pas de variable à gauche (previous) */
        else if(!op->previous || op->previous->type != OP_REF)
        {
            if(g->g_debugOp)
                DEBUG(INFO_LOG, "Operateur d'inversion (%s)", op->info);

            opDebug = addOpDebug(opDebug);
            opDebug->type = OP_MATHS;

            opDebug->elem2 = malloc(sizeof(OpElem));
            opDebug->elem2->ref = opTest;

            opDebug->elem2Type = ELEM_REF;
            opDebug->varType = opDebug->elem2->ref->varType;

            /* Opérateur */
            opDebug->operand = malloc(sizeof(char)*2);
            opDebug->operand[0] = '!';
            opDebug->operand[1] = '\0';

            /* On met la référence dans le ! */
            op->type = OP_REF;
            op->ID = opDebug->ID;

            i = op->next->ID;
            op = op->next;

            /* On met la référence dans les éléments suivants */
            while(op && op->ID == i && op->type == OP_REF)
            {
                op->type = OP_REF;
                op->ID = opDebug->ID;

                op = op->next;
            }
        }
        else
        {
            PRINTFNV(ERROR_LOG, "Un operateur (%s) est mal positionne, impossible de definir son impact : \n", op->info);
            LOG(ERROR_LOG, "Un opérateur (%s) est mal positionné, impossible de définir son impact : \n", op->info);

            debug_elemPrintLine(op->first, 1);

            pthread_exit((void*)ERROR_COMPIL);
        }
    }

    return opDebug;
}

/* ====================================================
                    getOpDebugUna
    --------------------------------------------------
    Arg : opDebug : Opération resultante
          op      : Opération à traduire
    --------------------------------------------------
    Def : Traite un opérateur unaire
   ==================================================== */
OperationDebug* getOpDebugUna(OperationDebug *opDebug, Operation *op)
{
    int i = 0;
    OperationDebug *opTest = NULL;

    /* Il n'y a rien après */
    if(!op->next || op->next->type != OP_REF)
    {
        PRINTFNV(ERROR_LOG, "Un operateur (%s) n'affecte aucune variable : \n", op->info);
        LOG(ERROR_LOG, "Un opérateur (%s) n'affecte aucune variable", op->info);

        debug_elemPrintLine(op->first, 1);

        pthread_exit((void*)ERROR_COMPIL);
    }

    else
    {
        opTest = getOpDebugByID(op->next->ID, opDebug);

        /* S'il affecte un type void */
        if(!opTest || opTest->varType == TYPE_VOID)
        {
            PRINTFNV(ERROR_LOG, "Un operateur (%s) est utilise sur un eléement de type void (vide) : \n", op->info);
            LOG(ERROR_LOG, "Un operateur (%s) est utilisé sur un élément de type void (vide)", op->info);

            debug_elemPrintLine(op->first, 1);

            pthread_exit((void*)ERROR_COMPIL);
        }
        /* Un opérateur unaire n'a pas de variable à gauche (previous), et ne peut concerner q'une variable (naturelle ou pas) */
        else if((!op->previous || op->previous->type != OP_REF) && (opTest->type == OP_VAR || opTest->type == OP_NAT_VAR))
        {
            if(g->g_debugOp)
                DEBUG(INFO_LOG, "Operateur unaire (%s)", op->info);

            opDebug = addOpDebug(opDebug);
            opDebug->type = OP_MATHS;

            opDebug->elem2 = malloc(sizeof(OpElem));
            opDebug->elem2->ref = opTest;

            opDebug->elem2Type = ELEM_REF;
            opDebug->varType = opDebug->elem2->ref->varType;

            /* Opérateur */
            opDebug->operand = malloc(sizeof(char)*2);
            opDebug->operand[0] = op->info[0];
            opDebug->operand[1] = '\0';

            /* On met la référence dans le +/- */
            op->type = OP_REF;
            op->ID = opDebug->ID;

            i = op->next->ID;
            op = op->next;

            /* On met la référence dans les éléments suivants */
            while(op && op->ID == i && op->type == OP_REF)
            {
                op->type = OP_REF;
                op->ID = opDebug->ID;

                op = op->next;
            }
        }
    }

    return opDebug;
}

/* ====================================================
                    getOpDebugInc
    --------------------------------------------------
    Arg : opDebug : Opération resultante
          op      : Opération à traduire
    --------------------------------------------------
    Def : Traite un opérateur d'incrémentation
   ==================================================== */
OperationDebug* getOpDebugInc(OperationDebug *opDebug, Operation *op)
{
    int i = 0;
    OperationDebug *opTest = NULL;

    opDebug = addOpDebug(opDebug);
    opDebug->type = OP_MATHS;

    /* Si une variable est située à droite (++a) */
    if(op->next && op->next->type == OP_REF && (opTest = getOpDebugByID(op->next->ID, opDebug)) && opTest->type == OP_VAR )
    {
        if(g->g_debugOp)
            DEBUG(INFO_LOG, "Operation d'incrementation/decrementation (%s)", op->info);

        /* Element à droite */
        opDebug->elem2 = malloc(sizeof(OpElem));
        opDebug->elem2->ref = opTest;

        opDebug->elem2Type = ELEM_REF;
        opDebug->varType = opDebug->elem2->ref->varType;

        /* Opérateur */
        opDebug->operand = malloc(sizeof(char)*3);
        strncpy(opDebug->operand, op->info, 2);
        opDebug->operand[2] = '\0';

        /* On met la référence dans le ++/-- */
        op->type = OP_REF;
        op->ID = opDebug->ID;

        i = op->next->ID;
        op = op->next;

        /* On met la référence dans les éléments suivants */
        while(op && op->ID == i && op->type == OP_REF)
        {
            op->type = OP_REF;
            op->ID = opDebug->ID;

            op = op->next;
        }
    }
    /* Si une variable est située à gauche (a++) */
    else if(op->previous && op->previous->type == OP_REF && (opTest = getOpDebugByID(op->previous->ID, opDebug)) && opTest->type == OP_VAR )
    {
        if(g->g_debugOp)
            DEBUG(INFO_LOG, "Operation d'incrementation/decrementation (%s)", op->info);

        /* Element à gauche */
        opDebug->elem1 = malloc(sizeof(OpElem));
        opDebug->elem1->ref = opTest;

        opDebug->elem1Type = ELEM_REF;
        opDebug->varType = opDebug->elem1->ref->varType;

        /* Opérateur */
        opDebug->operand = malloc(sizeof(char)*3);
        strncpy(opDebug->operand, op->info, 2);
        opDebug->operand[2] = '\0';

        /* On met la référence dans le ++/-- */
        op->type = OP_REF;
        op->ID = opDebug->ID;

        i = op->previous->ID;
        op = op->previous;

        /* On met la référence dans les éléments suivants */
        while(op && op->ID == i && op->type == OP_REF)
        {
            op->type = OP_REF;
            op->ID = opDebug->ID;

            op = op->previous;
        }
    }
    /* D'aucun coté */
    else
    {
        PRINTFNV(ERROR_LOG, "Une incrementation est effectuee sur autre chose qu'une variable : \n");
        LOG(ERROR_LOG, "Une incrementation est effectuée sur autre chose qu'une variable");

        debug_elemPrintLine(op->first, 1);

        pthread_exit((void*)ERROR_COMPIL);
    }

    return opDebug;
}

/* ====================================================
                    getOpDebugVar
    --------------------------------------------------
    Arg : opDebug : Opération resultante
          op      : Opération à traduire
          br      : bracket à traiter
          prog    : Programme
    --------------------------------------------------
    Def : Traite une variable
   ==================================================== */
OperationDebug* getOpDebugVar(OperationDebug *opDebug, Operation *op, Prog* prog, Bracket *br)
{
    Bracket *brRef = br;
    Var *var = prog->globalVar;

    while(br && !(var = getVarByName(op->info, br->var))) /* On cherche en remontant dans la fonction */
        br = br->container;

    br = brRef;

    if(!var && !(var = getVarByName(op->info, br->func->arg)) && !(var = getVarByName(op->info, prog->globalVar))) /* Arguments et var globales */
    {
        PRINTFNV(ERROR_LOG, "Un nom de variable ne renvoit vers aucune variable declaree : %s\n", op->info);
        LOG(ERROR_LOG, "Un nom de variable ne renvoit vers aucune variable déclarée : %s", op->info);

        debug_elemPrintLine(op->first, 1);

        pthread_exit((void*)ERROR_COMPIL);
    }

    if(g->g_debugOp)
            DEBUG(INFO_LOG, "Variable (%s)", op->info);

    opDebug = addOpDebug(opDebug);

    opDebug->type = OP_VAR;
    opDebug->varType = var->type;

    opDebug->elem1 = malloc(sizeof(OpElem));
    opDebug->elem1->var = var;
    opDebug->elem1Type = ELEM_VAR;

    op->type = OP_REF;
    op->ID = opDebug->ID;

    return opDebug;
}

/* ====================================================
                    getOpDebugFunc
    --------------------------------------------------
    Arg : opDebug : Opération resultante
          op      : Opération à traduire
          br      : bracket à traiter
          prog    : Programme
    --------------------------------------------------
    Def : Traite une fonction
   ==================================================== */
OperationDebug* getOpDebugFunc(OperationDebug *opDebug, Operation *op, Prog* prog, Bracket *br)
{
    Func *func = NULL;
    Var *var = NULL;
    OperationDebug *opTest = NULL;
    int i = 1;

    if(!(func = getFuncByName(op->info, prog->func))) /* On cherche en remontant dans la fonction */
        func = getFuncByName(op->info, prog->specialFunc);

    if(!func) /* Arguments et var globales */
    {
        PRINTFNV(ERROR_LOG, "Un nom de fonction ne renvoit vers aucune fonction declaree : %s\n", op->info);
        LOG(ERROR_LOG, "Un nom de fonction ne renvoit vers aucune fonction déclarée : %s", op->info);

        debug_elemPrintLine(op->first, 1);

        pthread_exit((void*)ERROR_COMPIL);
    }

    if(g->g_debugOp)
            DEBUG(INFO_LOG, "Fonction (%s)", op->info);

    opDebug = addOpDebug(opDebug);

    opDebug->type = OP_FUNC_CALL;
    opDebug->varType = func->type;

    INC_DEBUG;
    opDebug->elem1 = malloc(sizeof(OpElem));
    opTest = opDebug->elem1->ref = getOpDebugCL(op->link, prog, br);
    DEC_DEBUG;

    opDebug->func = func;

    opDebug->elem1Type = ELEM_REF;

    op->type = OP_REF;
    op->ID = opDebug->ID;

    /* Si ce n'est pas une fonction built-in */
    if(func->ID > 0)
    {
        while(opTest)
        {
            if(opTest->startOrder == i)
            {
                 if(!(var = getVarByID(i, func->arg)))
                 {
                    PRINTFNV(ERROR_LOG, "Une fonction (%s) est appelee avec trop d'arguments : \n", func->name);
                    LOG(ERROR_LOG, "Une fonction (%s) est appelée avec trop d'arguments", func->name);

                    debug_elemPrintLine(op->first, 1);

                    pthread_exit((void*)ERROR_COMPIL);
                 }

                 else
                 {
                     /* Ici tester que les types soient compatibles */
                     if(opTest->varType == TYPE_VOID)
                     {
                        PRINTFNV(ERROR_LOG, "Une fonction (%s) est appelee avec un arguments (%d) d'un type incorrect : \n", func->name, i);
                        LOG(ERROR_LOG, "Une fonction (%s) est appelée avec un arguments (%d) d'un type incorrect ", func->name, i);

                        debug_elemPrintLine(op->first, 1);

                        pthread_exit((void*)ERROR_COMPIL);
                     }
                     else
                     {
                         if(var->isRef)
                         {
                             if(opTest->type != OP_VAR || opTest->varType != var->type)
                             {
                                PRINTFNV(ERROR_LOG, "Une fonction (%s) est appelee avec un arguments (%d) incorrect pour une reference. Celui-ci doit etre une variable et etre du meme type que la reference: \n", func->name, i);
                                LOG(ERROR_LOG, "Une fonction (%s) est appelee avec un arguments (%d) incorrect pour une reference. Celui-ci doit etre une variable et être du même type que la référence", func->name, i);

                                debug_elemPrintLine(op->first, 1);

                                pthread_exit((void*)ERROR_COMPIL);
                             }
                         }

                         if (opTest->varType != var->type)
                            opTest->cast = var->type;

                         if (g->g_debugOp)
                            DEBUG(INFO_LOG, "Arg %d passe avec succes", i);
                     }


                 }

                 opTest = opTest->first;
                 i++;
            }
            else
                opTest = opTest->next;
        }

        i--;

        if((!func->arg && i > 0) || (func->arg && getLastVar(func->arg)->ID > i))
        {
            if(func->arg)
            {
                PRINTFNV(ERROR_LOG, "Une fonction (%s) est appelee avec %d arguments alors qu'elle en necessite %d : \n", func->name, i, getLastVar(func->arg)->ID);
                LOG(ERROR_LOG, "Une fonction (%s) est appelee avec %d arguments alors qu'elle en necessite %d : \n", func->name, i, getLastVar(func->arg)->ID);
            }
            else
            {
                PRINTFNV(ERROR_LOG, "Une fonction (%s) est appelee avec %d arguments alors qu'elle en necessite 0 : \n", func->name, i);
                LOG(ERROR_LOG, "Une fonction (%s) est appelee avec %d arguments alors qu'elle en necessite 0 : \n", func->name, i);
            }

            debug_elemPrintLine(op->first, 1);

            pthread_exit((void*)ERROR_COMPIL);
        }
    }
    /* Seul return possède une limitation */
    else if(func->ID == -2)
    {
        while(opTest)
        {
            if(br->func->type == TYPE_VOID && i > 0)
            {
                PRINTFNV(ERROR_LOG, "Une fonction de type void ne peut pas renvoyer des résultats : \n");
                LOG(ERROR_LOG, "Une fonction de type void ne peut pas renvoyer des résultats");

                debug_elemPrintLine(op->first, 1);

                pthread_exit((void*)ERROR_COMPIL);
            }
            else if(opTest->startOrder == i)
            {
                if(opTest->varType == TYPE_VOID)
                {
                    PRINTFNV(ERROR_LOG, "Une fonction (%s) est appelee avec un arguments (%d) d'un type incorrect : \n", func->name, i);
                    LOG(ERROR_LOG, "Une fonction (%s) est appelée avec un arguments (%d) d'un type incorrect ", func->name, i);

                    debug_elemPrintLine(op->first, 1);

                    pthread_exit((void*)ERROR_COMPIL);
                 }
                 else if(i > 1)
                 {
                    PRINTFNV(ERROR_LOG, "Une fonction (%s) est appelee avec trop d'arguments : \n", func->name);
                    LOG(ERROR_LOG, "Une fonction (%s) est appelée avec trop d'arguments ", func->name);

                    debug_elemPrintLine(op->first, 1);

                    pthread_exit((void*)ERROR_COMPIL);
                 }
                 else
                 {
                     if (opTest->varType != br->func->type)
                        opTest->cast = br->func->type;

                     if (g->g_debugOp)
                         DEBUG(INFO_LOG, "Arg %d passe avec succes", i);
                 }

                 opTest = opTest->first;
                 i++;
            }
            opTest = opTest->next;
        }
    }
    /* Fonction in() qui necessite des références */
    else if (func->ID == -3)
    {

        while(opTest)
        {
            if(opTest->startOrder == i)
            {
                if(opTest->type != OP_VAR)
                {
                    PRINTFNV(ERROR_LOG, "Une fonction (%s) est appelee avec un arguments (%d) incorrect pour une reference. Celui-ci doit etre une variable et etre du meme type que la reference: \n", func->name, i);
                    LOG(ERROR_LOG, "Une fonction (%s) est appelee avec un arguments (%d) incorrect pour une reference. Celui-ci doit etre une variable et être du même type que la référence", func->name, i);

                    debug_elemPrintLine(op->first, 1);

                    pthread_exit((void*)ERROR_COMPIL);
                }
                 else if (g->g_debugOp)
                         DEBUG(INFO_LOG, "Arg %d passe avec succes", i);

                 opTest = opTest->first;
                 i++;
            }
            else
                opTest = opTest->next;
        }
    }



    return opDebug;
}

/* ====================================================
                    getOpDebugNatVar
    --------------------------------------------------
    Arg : opDebug : Opération resultante
          op      : Opération à traduire
    --------------------------------------------------
    Def : Traite une variable naturelle
   ==================================================== */
OperationDebug* getOpDebugNatVar(OperationDebug *opDebug, Operation *op)
{
    opDebug = addOpDebug(opDebug);

    opDebug->type = OP_NAT_VAR;

    /* Selection du type */
    if(*(op->info) == '\'')
    {
        /* char */
        if(g->g_debugOp)
            DEBUG(INFO_LOG, "Var naturelle (caractere)(%s)", op->info);

        /* Type */
        opDebug->varType = TYPE_CHAR;

        if(strlen(op->info) < 3 && strlen(op->info) > 4)
        {
            PRINTFNV(ERROR_LOG, "Un caractere est mal declare : %s", op->info);
            LOG(ERROR_LOG, "Un caractere est mal declaré : %s", op->info);

            pthread_exit((void*)ERROR_COMPIL);
        }

        opDebug->elem1 = malloc(sizeof(OpElem));

        if(*(op->info + 1) != '\\')
            opDebug->elem1->c = *(op->info + 1);
        else
            opDebug->elem1->c = getSpeChar(op->info + 1);

        opDebug->elem1Type = ELEM_CHAR;
    }
    else if(strchr(op->info, '.'))
    {
        /* flottant */
        if(g->g_debugOp)
            DEBUG(INFO_LOG, "Var naturelle (flottant)(%f)", (float)atof(op->info));

        /* Type */
        opDebug->varType = TYPE_FLOAT;

        opDebug->elem1 = malloc(sizeof(OpElem));
        opDebug->elem1->f = (float)atof(op->info);
        opDebug->elem1Type = ELEM_FLOAT;
    }
    else
    {
        /* int */
        if(g->g_debugOp)
            DEBUG(INFO_LOG, "Var naturelle (entier)(%d)", atoi(op->info));

        /* Type */
        opDebug->varType = TYPE_INT;

        opDebug->elem1 = malloc(sizeof(OpElem));
        opDebug->elem1->i = atoi(op->info);
        opDebug->elem1Type = ELEM_INT;
    }



    op->type = OP_REF;
    op->ID = opDebug->ID;

    return opDebug;
}

/* ====================================================
                    getOpDebugParenth
    --------------------------------------------------
    Arg : opDebug : Opération resultante
          op      : Opération à traduire
          br      : bracket à traiter
          prog    : Programme
    --------------------------------------------------
    Def : Traite une parenthèse
   ==================================================== */
OperationDebug* getOpDebugParenth(OperationDebug *opDebug, Operation *op, Prog *prog, Bracket *br)
{
    if(g->g_debugOp)
            DEBUG(INFO_LOG, "Parentheses");
    INC_DEBUG;

    /* On ajoute à la liste */
    opDebug = addOpDebug(opDebug);

    opDebug->type = OP_PARENTH;

    /* On ajoute l'élément traité (récursivité) */
    INC_DEBUG;
    opDebug->elem1 = malloc(sizeof(OpElem));
    opDebug->elem1->ref = getOpDebugCL(op->link, prog, br);
    opDebug->elem1Type = ELEM_REF;
    DEC_DEBUG;

    opDebug->varType = opDebug->elem1->ref->type;

    op->type = OP_REF;
    op->ID = opDebug->ID;

    DEC_DEBUG;

    return opDebug;
}

char getSpeChar(char *str)
{
    if(*(str + 1) == '\'')
    {
        PRINTFNV(ERROR_LOG, "Un caractere est mal declare : %s", str);
        LOG(ERROR_LOG, "Un caractere est mal déclaré : %s", str);

        pthread_exit((void*)ERROR_COMPIL);
    }

    switch(*(str + 1))
    {
        case 'n' :
            return '\n';
        case 't' :
            return '\t';
        case 'r' :
            return '\r';
        case 'v' :
            return '\v';
        case 'b' :
            return '\b';
        case '0' :
            return '\0';
        default :
            return *(str + 1);
    }
}
