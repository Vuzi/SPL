
#include "translation.h"


/* ====================================================
                      translateProgToC
    --------------------------------------------------
    Arg : prog : Structure du programme à traduire
          f    : Fichier de destination
    --------------------------------------------------
    Def : Ecrit la traduction en C de prog dans le fichier
          f
   ==================================================== */
void translateProgToC(Prog* prog, FILE *f)
{
    struct tm *instant;
    time_t sec;

    time(&sec);
    instant = localtime(&sec);

    DEBUG(INFO_LOG, "Lancement de la traduction en C");
    INC_DEBUG;

    fprintf(f, "/*======================================\n");
    fprintf(f, "       Code généré automatiquement      \n");
    fprintf(f, "        %d - %d - %d a %d:%d:%d\n",
            instant->tm_mday, instant->tm_mon + 1, instant->tm_year + 1900, instant->tm_hour, instant->tm_min, instant->tm_sec);
    fprintf(f, "  ======================================\n");
    fprintf(f, "        Interpréteur SPL - v0.1\n");
    fprintf(f, "  ======================================*/\n\n");

    if(g->path)
        fprintf(f, "#include \"%s/SPL_builtin/SPL_builtin.h\"\n\n", g->path);
    else
        fprintf(f, "#include \"SPL_builtin/SPL_builtin.h\"\n\n");

    DEBUG(INFO_LOG, "Traitement des variables globales");
    /* Les variables gloables */
    translateVarList(prog->globalVar, f, 0);
    fprintf(f, "\n");

    DEBUG(INFO_LOG, "Traitement des prototypes de fonctions");
    /* Les prototypes de fonctions */
    translateFuncProto(prog->func, f);
    fprintf(f, "\n");

    DEBUG(INFO_LOG, "Traitement des fonctions");
    /* Les fonctions */
    translateFunc(prog->func, f);
    fprintf(f, "\n");

    DEC_DEBUG;
    DEBUG(INFO_LOG, "Traduction termine");
}

/* ====================================================
                      translateVarList
    --------------------------------------------------
    Arg : var : Liste de variable à traiter
          f   : Fichier de destination
          lvl : Niveau de profondeur
    --------------------------------------------------
    Def : Ecrit les variables de la liste
   ==================================================== */
void translateVarList(Var* var, FILE *f, int lvl)
{
    int i = 0;

    if(var)
        var = var->first;

    while(var)
    {
        for(i=0;i<lvl;i++)
            fprintf(f, "\t");

        translateVar(var, f);

        if(var->type == TYPE_INT)
            fprintf(f, " = %d", var->val->i);
        else if(var->type == TYPE_CHAR)
            fprintf(f, " = (char)%d", var->val->c);
        else if(var->type == TYPE_FLOAT)
            fprintf(f, " = %f", var->val->f);

        fprintf(f, ";\n");
        var = var->next;
    }
}

/* ====================================================
                      translateVar
    --------------------------------------------------
    Arg : var : Variable à traiter
          f   : Fichier de destination
    --------------------------------------------------
    Def : Ecrit la variable dans le fichier f
   ==================================================== */
void translateVar(Var* var, FILE *f)
{
    if(!var)
        return;

    if(var->type == TYPE_INT)
        fprintf(f, "int ");
    else if(var->type == TYPE_CHAR)
        fprintf(f, "char ");
    else if(var->type == TYPE_FLOAT)
        fprintf(f, "float ");

    if(var->isRef)
        fprintf(f, "*");

    fprintf(f, "%s", var->finalName);

}

/* ====================================================
                      translateFuncProto
    --------------------------------------------------
    Arg : func : Liste de fonction à écrire
          f    : Fichier de destination
    --------------------------------------------------
    Def : Ecrit les prototypes des fonctions de la liste
   ==================================================== */
void translateFuncProto(Func* func, FILE *f)
{
    Var* var = NULL;

    if(func)
        func = func->first;

    while(func)
    {
        if(func->ID > 0)
        {
            /* Le type */
            if(func->type == TYPE_INT)
                fprintf(f, "int ");
            else if(func->type == TYPE_CHAR)
                fprintf(f, "char ");
            else if(func->type == TYPE_FLOAT)
                fprintf(f, "float ");
            else if (func->type == TYPE_VOID)
                fprintf(f, "void ");

            fprintf(f, "%s(", func->finalName);

            if((var = func->arg))
            {

                var = var->first;

                while(var)
                {
                    translateVar(var, f);

                    if((var = var->next))
                        fprintf(f, ", ");
                }
            }
            else
                fprintf(f, "void");

            fprintf(f,");\n");
        }

        func = func->next;
    }
}

/* ====================================================
                      translateFunc
    --------------------------------------------------
    Arg : func : Liste de fonction à écrire
          f    : Fichier de destination
    --------------------------------------------------
    Def : Ecrit les fonctions de la liste
   ==================================================== */
void translateFunc(Func* func, FILE *f)
{
    Var* var = NULL;

    if(func)
        func = func->first;

    while(func)
    {
        if(func->ID > 0)
        {
            /* Le type */
            if(func->type == TYPE_INT)
                fprintf(f, "int ");
            else if(func->type == TYPE_CHAR)
                fprintf(f, "char ");
            else if(func->type == TYPE_FLOAT)
                fprintf(f, "float ");
            else if (func->type == TYPE_VOID)
                fprintf(f, "void ");

            fprintf(f, "%s(", func->finalName);

            /* Les arguments */
            if((var = func->arg))
            {
                var = var->first;

                while(var)
                {
                    translateVar(var, f);

                    if((var = var->next))
                        fprintf(f, ", ");
                }
            }
            else
                fprintf(f, "void");

            fprintf(f,")\n{\n");

            if(func->content)
            {
                translateVarList(func->content->var, f, 1);
                translateCl(func->content->code, f, 1);
            }


            fprintf(f,"\n}\n\n");
        }
        else if (func->ID == 0)
        {
            fprintf(f, "int main(int argc, char* argv[])\n{\n");

            if(func->content)
            {
                translateVarList(func->content->var, f, 1);
                translateCl(func->content->code, f, 1);
            }


            fprintf(f,"\n\treturn 0;\n}\n\n");
        }

        func = func->next;
    }
}

/* ====================================================
                      translateCl
    --------------------------------------------------
    Arg : cl : Liste de lignes de code
          f  : Fichier de destination
    --------------------------------------------------
    Def : Ecrit les lignes de code en C
   ==================================================== */
void translateCl(CodeLine *cl, FILE *f, int lvl)
{
    int i = 0;

    while(cl)
    {
        if(cl->link_b)
        {
            for(i=0;i<lvl;i++)
                fprintf(f, "\t");

            if(cl->link_b->type == L_IF)
                fprintf(f, "if ");
            else if(cl->link_b->type == L_ELSEIF)
                fprintf(f, "else if ");
            else if(cl->link_b->type == L_ELSE)
                fprintf(f, "else ");
            else if(cl->link_b->type == L_WHILE)
                fprintf(f, "while ");

            if(cl->link_b->cond_d)
            {
                fprintf(f, "(");
                translateOperationStart(cl->link_b->cond_d, f, 0, NULL);
                fprintf(f, ") ");
            }

            fprintf(f, "{\n");

            translateVarList(cl->link_b->var, f, lvl+1);

            translateCl(cl->link_b->code, f, lvl + 1);

            for(i=0;i<lvl;i++)
                fprintf(f, "\t");

            fprintf(f, "}\n");
        }
        else if (cl->opDebug)
        {
            for(i=0;i<lvl;i++)
                fprintf(f, "\t");

            translateOperationStart(cl->opDebug, f, 0, NULL);

            fprintf(f, ";\n");
        }

        cl = cl->next;
    }
}

/* ====================================================
                translateOperationStart
    --------------------------------------------------
    Arg : op : Liste des opérations
          f  : Fichier de destination
    --------------------------------------------------
    Def : Ecrit les opérations de la liste en C
   ==================================================== */
void translateOperationStart(OperationDebug *op, FILE *f, char isBuiltIn, Func *isFunc)
{
    int i = 1;
    OperationDebug *opRef = op;

    if(isBuiltIn)
    {
        while(op)
        {
            if(op->startOrder == i)
            {
                op = op->first;
                i++;
            }
            else
                op = op->next;
        }
        i--;
        fprintf(f, "%d, ", i*2);

        i = 1;
        op = opRef;
    }

    while(op)
    {
        if(op->startOrder == i)
        {
            if(i > 1)
                fprintf(f, ", ");

            /* Cas spécial de 2 fonctions builtin */
            if(isBuiltIn)
            {
                if(op->varType == TYPE_INT)
                    fprintf(f, "TYPE_INT, ");
                else if(op->varType == TYPE_CHAR)
                    fprintf(f, "TYPE_CHAR, ");
                else if(op->varType == TYPE_FLOAT)
                    fprintf(f, "TYPE_FLOAT, ");
            }


            if((isFunc && isFunc->ID > 0 && getVarByID(i, isFunc->arg)->isRef) || (isFunc && isFunc->ID == -3))
                fprintf(f, "&");

            translateOperation(op, f);

            op = op->first;
            i++;
        }
        else
            op = op->next;
    }
}

/* ====================================================
                  translateOperation
    --------------------------------------------------
    Arg : op : Liste des opérations
          f  : Fichier de destination
    --------------------------------------------------
    Def : Ecrit les opérations de la liste en C
   ==================================================== */
void translateOperation(OperationDebug *op, FILE *f)
{
    if(op)
    {
        /* cast */
        if(op->cast == TYPE_INT)
            fprintf(f, "(int)(");
        else if (op->cast == TYPE_CHAR)
            fprintf(f, "(char)(");
        else if (op->cast == TYPE_FLOAT)
            fprintf(f, "(float)(");

        /* Parenthèses */
        if(op->type == OP_PARENTH && op->elem1)
        {
            fprintf(f, "(");
            translateOperationStart(op->elem1->ref, f, 0, NULL);
            fprintf(f, ")");
        }
        /* Appels de fonctions */
        else if(op->type == OP_FUNC_CALL)
        {
            if(op->func->ID == -2 && !op->elem1->ref)
            {
                fprintf(f, "return");
            }

            else
            {
                fprintf(f, "%s(", op->func->finalName);

                if(op->func->ID == -1)
                {
                    if(op->elem1)
                        translateOperationStart(op->elem1->ref, f, 1, NULL);
                }
                else if(op->func->ID == -3)
                {
                    if(op->elem1)
                        translateOperationStart(op->elem1->ref, f, 1, op->func);
                }
                else
                {
                    if(op->elem1)
                        translateOperationStart(op->elem1->ref, f, 0, op->func);
                }

                fprintf(f, ")");
            }

        }
        /* Variables (nom) */
        else if(op->type == OP_VAR)
        {
            if(op->elem1->var->isRef)
                fprintf(f, "(*%s)", op->elem1->var->finalName);
            else
                fprintf(f, "%s", op->elem1->var->finalName);

        }
        /* Variables naturelles */
        else if(op->type == OP_NAT_VAR)
        {
            if(op->varType == TYPE_INT)
                fprintf(f, "%d", op->elem1->i);
            else if(op->varType == TYPE_CHAR)
                fprintf(f, "(char)%d", op->elem1->c);
            else if(op->varType == TYPE_FLOAT)
                fprintf(f, "%f", op->elem1->f);
        }
        else
        {
            if(op->elem1 && op->elem1Type == ELEM_REF)
                translateOperation(op->elem1->ref, f);

            if(op->operand)
                fprintf(f, " %s ", op->operand);

            if(op->elem2 && op->elem2Type == ELEM_REF)
                translateOperation(op->elem2->ref, f);
        }

        if(op->cast == TYPE_INT  || op->cast == TYPE_CHAR || op->cast == TYPE_FLOAT)
            fprintf(f, ")");
    }
}
