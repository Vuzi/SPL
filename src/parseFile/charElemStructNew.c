
#include "charElemStructNew.h"

/* ====================================================
                  valideOperand
    --------------------------------------------------
    Arg : start : début de la chaine
          stop  : fin de la chaine
    --------------------------------------------------
    Def : renvoit 1 en cas d'opérateur valide, ou 0 si
          un opérateur est invalide.
   ==================================================== */
char validOperand(char* start, char* stop)
{
    if(stop-start == 0)
    {
        if (start[0] == '-')
            return 1;
        else if (start[0] == '+')
            return 1;
        else if (start[0] == '=')
            return 1;
        else if (start[0] == '/')
            return 1;
        else if (start[0] == '*')
            return 1;
        else if (start[0] == '%')
            return 1;
        else if (start[0] == '>')
            return 1;
        else if (start[0] == '<')
            return 1;
        else if (start[0] == '&')
            return 1;
        else if (start[0] == '|')
            return 1;
        else if (start[0] == ',')
            return 1;
        else if (start[0] == '!')
            return 1;
    }
    else if(stop-start == 1)
    {
        if (start[1] == '=')
        {
            if (start[0] == '-') /* -= */
                return 1;
            else if (start[0] == '+') /* += */
                return 1;
            else if (start[0] == '=') /* == */
                return 1;
            else if (start[0] == '/') /* /= */
                return 1;
            else if (start[0] == '*') /* *= */
                return 1;
            else if (start[0] == '%') /* %= */
                return 1;
            else if (start[0] == '>') /* >= */
                return 1;
            else if (start[0] == '<') /* <= */
                return 1;
            else if (start[0] == '!') /* != */
                return 1;
        }
        else if (start[0] == '+' && start[1] == '+') /* ++ */
            return 1;
        else if (start[0] == '-' && start[1] == '-') /* -- */
            return 1;
        else if (start[0] == '&' && start[1] == '&') /* && */
            return 1;
        else if (start[0] == '|' && start[1] == '|') /* || */
            return 1;
    }

    return 0;
}

/* ====================================================
                    structureCode
    --------------------------------------------------
    Arg : code      : Code à traiter
          container : Elément contenant (Peu être nul)
    --------------------------------------------------
    Def : Permet de structure le code dans la mémoire
          en l'analysant
   ==================================================== */
Elem* structureCode(char* code, Elem* container)
{
    Elem *elem = NULL;

    char *nextBracket = NULL, *contentBracket = NULL, *tmp = NULL, *parenth = NULL, *close_parenth = NULL;
    char rep[255];

    /* Tant qu'on trouve des paires de crochets */
    do
    {
        tmp = nextBracket = strchr(code, '[');
        contentBracket = getBracket(nextBracket, '[', ']');

        /* Si on trouve une paire de  crochets */
        if(contentBracket)
        {
            /* Ajout d'un élément à la liste */
            elem = addElem(elem);
            elem->container = container;

            /* Récursif pour traiter l'intérieur du bracket */
            elem->content = structureCode(contentBracket, elem);

            /* S'il y a une déclaration avant */
            if(!(nextBracket == code || nextBracket[-1] == ';'))
            {
                /* On cherche une déclaration avant le code */
                while(--tmp >= code && *tmp != ';');

                parenth = tmp;
                tmp++;

                /* On cherche une "(" avant le code */
                while(++parenth < nextBracket && *parenth != '(');

                /* On trouve une parenthèse */
                if(*parenth == '(')
                {
                    /* Si elle existe et se ferme avant les crochets */
                    if (!(close_parenth = getClosingBracket(parenth, '(', ')')) || close_parenth >= nextBracket)
                    {
                        PRINTFNV(ERROR_LOG, "Impossible de trouver la parenthèse fermante : %s", parenth);
                        LOG(ERROR_LOG, "Impossible de trouver la parenthèse fermante : %s", parenth);

                        pthread_exit((void*)ERROR_COMPIL);
                    }
                    else
                    {
                        /* On récupère le code et le nom */
                        elem->cond = structureOperationLine(getBracket(parenth, '(', ')'));

                        parenth--;

                        elem->name = malloc(sizeof(char)*(parenth-tmp+2));
                        strncpy(elem->name, tmp, parenth-tmp+1);
                        elem->name[parenth-tmp+1] = '\0';
                    }
                }
                else
                {
                    /* On récupère uniquement le nom */
                    nextBracket--;

                    elem->name = malloc(sizeof(char)*(nextBracket-tmp+2));
                    strncpy(elem->name, tmp, nextBracket-tmp+1);
                    elem->name[nextBracket-tmp+1] = '\0';
                }
            }

            /* On test la validité du nom */
            if( !isValidName(elem->name))
            {
                PRINTFNV(ERROR_LOG, "Un mot clef utilisé dans le programme comporte autre chose que les caractères autorisés (a-zA-Z_) : %s  Cette erreur peut également venir d'une déclaration mal ouverte", elem->name);
                LOG(ERROR_LOG, "Un mot clef utilisé dans le programme comporte autre chose que les caractères autorisés (a-zA-Z_) : %s  Cette erreur peut également venir d'une déclaration mal ouverte", elem->name);

                pthread_exit((void*)ERROR_COMPIL);
            }

            sprintf(rep, "#E%d;", elem->ID);

            if (elem->name)
                code = strInsert(code, tmp+1, strlen(elem->name), rep);
            else
                code = strInsert(code, tmp+1, 0, rep);
        }

    }while(contentBracket);

    /* Ici, découper en ligne de code et les insérer dans container->code */
    container->code = structureOperation(code, elem);

    /* On termine en supprimant le code */
    free(code);

    if (elem)
        return elem->first;
    else
        return elem;
}

/* ====================================================
                  structureOperation
    --------------------------------------------------
    Arg : code      : Code à traiter
          container : Elément d'où vient la ligne de code
    --------------------------------------------------
    Def : Permet de décomposer les différentes lignes de
          code du programme en une série de listes
          chainées
   ==================================================== */
CodeLine* structureOperation(char* code, Elem *elem)
{
    char* tmp = code, *tmp_end = NULL;
    CodeLine *cl = NULL;

    if(!code || *(code+1) == '\0')
        return NULL;

    if(elem)
        elem = elem->first;

    while((tmp_end = strchr(tmp, ';')))
    {
        cl = addCodeLine(cl);

        /* Cas d'un référencement de boucle */
        if(strncmp(tmp, "#E", 2) == 0)
        {
            if(!(cl->link = getElemByID(atoi(tmp+2), elem)))
            {
                PRINTFNV(ERROR_LOG, "Une erreur inconnue est survenue lors de la compilation : %s | %d", tmp,  atoi(tmp+2));
                LOG(ERROR_LOG, "Une erreur inconnue est survenue lors de la compilation : %s | %d", tmp,  atoi(tmp+2));

                pthread_exit((void*)ERROR_COMPIL);
            }
        }

        /* Du code 'simple' */
        else
        {
            cl->code = malloc(sizeof(char)*(tmp_end-tmp+1));
            strncpy(cl->code, tmp, tmp_end-tmp);
            cl->code[tmp_end-tmp] = '\0';

            cl->op = structureOperationLine(cl->code);

            free(cl->code);
            cl->code = NULL;
        }

        tmp = tmp_end+1;
    }

    /* On doit terminer à la fin */
    if((!cl && strlen(code) > 0) || (*tmp != '\0'))
    {
        PRINTFNV(ERROR_LOG, "Une declaration n'est pas terminee par un \";\" ");
        LOG(ERROR_LOG, "Une déclaration n'est pas terminée par un \";\" ");

        pthread_exit((void*)ERROR_COMPIL);
    }

    /* On retourne la liste des opérations */
    if(cl)
        return cl->first;
    else
        return cl;
}

/* ====================================================
                  structureOperationLine
    --------------------------------------------------
    Arg : tmp : ligne de code à traiter
    --------------------------------------------------
    Def : Permet de décomposer la ligne de code en suite
          d'opérations
   ==================================================== */
Operation* structureOperationLine(char* tmp)
{
    Operation *op = NULL;

    while(tmp[0] != '\0')
    {

        if((*tmp >= 'a' && *tmp <= 'z') || *tmp == '_')
            op = structureOperationLineVarFunc(&tmp, op);

        /* entier naturel Ou flottant */
        else if((*tmp >= '0' && *tmp <= '9') || *tmp == '.' )
            op = structureOperationLineIntFloat(&tmp, op);

        /* caractère */
        else if(*tmp == '\'')
            op = structureOperationLineChar(&tmp, op);

        /* Parenthèses */
        else if (*tmp == '(')
            op = structureOperationLineParenth(&tmp, op);

        /* Opérateur */
        else if (validOperand(tmp, tmp))
            op = structureOperationLineOp(&tmp, op);

        tmp++;
    }

    if (op)
        return op->first;
    else
        return op;
}

/* ====================================================
              structureOperationLineVarFunc
    --------------------------------------------------
    Arg : tmp : ligne de code à traiter
          op  : élément de la liste des opérations
    --------------------------------------------------
    Def : Permet de traiter une variable/fonction
   ==================================================== */
Operation* structureOperationLineVarFunc(char** tmp, Operation *op)
{
    char* tmp_end = NULL;

    op = addOperation(op);

    tmp_end = *tmp;
    tmp_end++;

    while((*tmp_end >= 'a' && *tmp_end <= 'z') || *tmp_end == '_')
        tmp_end++;

    /* Appel de fonction */
    if(*tmp_end == '(')
    {
        op->type = OP_FUNC_CALL;

        op->link = structureOperationLine(getBracket(tmp_end, '(', ')'));

        op->info = malloc(sizeof(char)*(tmp_end-*tmp+1));
        strncpy(op->info, *tmp, tmp_end-*tmp);
        op->info[tmp_end-*tmp] = '\0';

        *tmp = tmp_end-1;
    }

    /* Variable */
    else
    {
        op->type = OP_VAR;

        op->info = malloc(sizeof(char)*(tmp_end-*tmp+1));
        strncpy(op->info, *tmp, tmp_end-*tmp);
        op->info[tmp_end-*tmp] = '\0';

        *tmp = tmp_end-1;
    }

    return op;
}

/* ====================================================
              structureOperationLineIntFloat
    --------------------------------------------------
    Arg : tmp : ligne de code à traiter
          op  : élément de la liste des opérations
    --------------------------------------------------
    Def : Permet de traiter une entier naturel
   ==================================================== */
Operation* structureOperationLineIntFloat(char** tmp, Operation *op)
{
    char* tmp_end = NULL;
    char isFloat = 0;

    if(**tmp == '.')
        isFloat = 1;

    op = addOperation(op);
    op->type = OP_NAT_VAR;

    tmp_end = *tmp;
    tmp_end++;

    while((*tmp_end >= '0' && *tmp_end <= '9' ) || (*tmp_end == '.' && !isFloat))
    {
        if(*tmp_end == '.')
            isFloat = 1;

        tmp_end++;
    }

    op->info = malloc(sizeof(char)*(tmp_end-*tmp+1));
    strncpy(op->info, *tmp, tmp_end-*tmp);
    op->info[tmp_end-*tmp] = '\0';

    *tmp = tmp_end-1;

    return op;
}

/* ====================================================
              structureOperationLineChar
    --------------------------------------------------
    Arg : tmp : ligne de code à traiter
          op  : élément de la liste des opérations
    --------------------------------------------------
    Def : Permet de traiter un caractère
   ==================================================== */
Operation* structureOperationLineChar(char** tmp, Operation *op)
{
    char* tmp_end = NULL;

    op = addOperation(op);
    op->type = OP_NAT_VAR;

    tmp_end = *tmp;
    tmp_end++;

    while(*tmp_end != '\'')
    {
        tmp_end++;
    }
    tmp_end++;

    op->info = malloc(sizeof(char)*(tmp_end-*tmp+1));
    strncpy(op->info, *tmp, tmp_end-*tmp);
    op->info[tmp_end-*tmp] = '\0';

    *tmp = tmp_end-1;

    return op;
}

/* ====================================================
              structureOperationLineParenth
    --------------------------------------------------
    Arg : tmp : ligne de code à traiter
          op  : élément de la liste des opérations
    --------------------------------------------------
    Def : Permet de traiter une paire de parenthèses
   ==================================================== */
Operation* structureOperationLineParenth(char** tmp, Operation *op)
{
    op = addOperation(op);
    op->type = OP_PARENTH;

    op->link = structureOperationLine(getBracket(*tmp, '(', ')'));

    (*tmp)--;

    return op;
}

/* ====================================================
              structureOperationLineOp
    --------------------------------------------------
    Arg : tmp : ligne de code à traiter
          op  : élément de la liste des opérations
    --------------------------------------------------
    Def : Permet de traiter un opérateur
   ==================================================== */
Operation* structureOperationLineOp(char** tmp, Operation *op)
{
    char* tmp_end = NULL;

    op = addOperation(op);
    op->type = OP_MATHS;

    tmp_end = *tmp;
    tmp_end++;

    while(tmp_end-*tmp < 2 && *tmp_end > '\0' && !(*tmp_end >= '0' && *tmp_end <= '9') && !((*tmp_end >= 'a' && *tmp_end <= 'z') || *tmp_end == '_') && *tmp_end != '(')
    {
        if(validOperand(*tmp, tmp_end))
            tmp_end++;
        else
            break;
    }

    op->info = malloc(sizeof(char)*(tmp_end-*tmp+1));
    strncpy(op->info, *tmp, tmp_end-*tmp);
    op->info[tmp_end-*tmp] = '\0';

    *tmp = tmp_end-1;

    return op;
}
