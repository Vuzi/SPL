
#include "debug.h"

/* ====================================================
                       debug_funcOnly
    --------------------------------------------------
    Arg : var : Fonction à débuger (afficher)
    --------------------------------------------------
    Def : Affiche à l'écran les infos de la variable
          dans un encadré
   ==================================================== */
void debug_funcOnly(Func* func)
{
    printf("\n==================================================================");
    printf("\n                          Fonction");
    printf("\n==================================================================\n");
    debug_func(func);
    printf("\n==================================================================");
}

/* ====================================================
                       debug_func
    --------------------------------------------------
    Arg : func : Variable à débuger (afficher)
    --------------------------------------------------
    Def : Affiche à l'écran les infos de la variable
   ==================================================== */
void debug_func(Func* func)
{
    Var* arg = NULL;
    Bracket *br;

    printf("Nom : %s | Type : ", func->name);

    if(func->type == TYPE_INT)
        printf("entier");
    else if(func->type == TYPE_VOID)
        printf("void (Aucun retour)");
    else
        printf("aucun");

    if(!func->arg)
        printf("\n\nAucun argument");

    else
    {
        printf("\n\nArguments : ");
        arg = func->arg->first;

        while(arg)
        {
            printf("\n-----------------------------------------------------\n");
            debug_var(arg);
            arg = arg->next;
        }
        printf("\n-----------------------------------------------------\n");
    }


    if(!func->content)
        printf("\n\nAucun code");

    else
    {
        printf("\n\nCode : \n");
        br = func->content;

        while(br)
        {
            debug_bracketPrint(br, 0);
            printf("\n");

            br = br->next;
        }

    }

}

void debug_bracketPrint(Bracket* br, int lvl)
{
    int i = 0;
    CodeLine *cl = NULL;

    if(br)
    {
        for(i = 0; i < lvl; i++)
            printf("\t");

        if(br->type == L_IF)
            printf("IF ");
        else if(br->type == L_ELSE)
            printf("ELSE ");
        else if(br->type == L_ELSEIF)
            printf("ELSE IF ");

        if(br->cond)
        {
            printf(" ( ");
            debug_elemPrintLine(br->cond, 0);
            printf(" ) ");
        }

        printf("\n");

        cl = br->code;

        while(cl)
        {
            if(cl->link_b)
                debug_bracketPrint(cl->link_b, lvl+2);

            else
            {
                debug_elemPrintLine(cl->op, lvl+1);
                printf("\n");
            }

            cl = cl->next;
        }
    }
}

/* ====================================================
                       debug_varOnly
    --------------------------------------------------
    Arg : var : Variable à débuger (afficher)
    --------------------------------------------------
    Def : Affiche à l'écran les infos de la variable
          dans un encadré
   ==================================================== */
void debug_varOnly(Var* var)
{
    printf("\n==================================================================");
    printf("\n                          Variable");
    printf("\n==================================================================\n");
    debug_var(var);
    printf("\n==================================================================");
}

/* ====================================================
                       debug_var
    --------------------------------------------------
    Arg : var : Variable à débuger (afficher)
    --------------------------------------------------
    Def : Affiche à l'écran les infos de la variable
   ==================================================== */
void debug_var(Var* var)
{
    printf("Nom : %s | Type : ", var->name);

    if(var->type == TYPE_INT)
        printf("entier | Valeur : %d", var->val->i);
    else
        printf("aucun");
}

/* ====================================================
                       debug_elem
    --------------------------------------------------
    Arg : elem : Elément à débuger (afficher)
    --------------------------------------------------
    Def : Affiche à l'écran une réprésentation
          schématique de l'élément elem pour mieux
          le visualiser
   ==================================================== */
void debug_elem(Elem *elem)
{
    printf("\n==================================================================");
    printf("\n Lancement de l'affichage de la representation en memoire du code");
    printf("\n==================================================================\n\n");
    debug_elemPrint(elem, 0);
    printf("\n==================================================================\n\n");

}

/* ====================================================
                       debug_elemPrint
    --------------------------------------------------
    Arg : elem : Elément à débuger (afficher)
          lvl  : Niveau de profondeur
    --------------------------------------------------
    Def : Permet de dessiner les boucles du programme
   ==================================================== */
void debug_elemPrint(Elem *elem, int lvl)
{
    int i = 0;
    CodeLine *cl = NULL;

    if(elem)
    {
        for(i = 0; i < lvl; i++)
            printf("\t");

        printf("Nom : %s | ID : %d", elem->name, elem->ID);

        if(elem->cond)
        {
            printf(" | cond : ");
            debug_elemPrintLine(elem->cond, 0);
        }

        printf("\n");

        cl = elem->code;

        while(cl)
        {
            if(cl->link)
                debug_elemPrint(cl->link, lvl+2);

            else
            {
                debug_elemPrintLine(cl->op, lvl+1);
                printf("\n");
            }

            cl = cl->next;
        }
    }
}

/* ====================================================
                  debug_elemPrintLine
    --------------------------------------------------
    Arg : op   : Opération à débuger (afficher)
          lvl  : Niveau de profondeur
    --------------------------------------------------
    Def : Permet de dessiner les lignes de code du
          programme
   ==================================================== */
void debug_elemPrintLine(Operation *op, int lvl)
{
    int i = 0;

    for(i = 0; i < lvl; i++)
        printf("\t");

    while(op)
    {
        if(op->type == OP_PARENTH)
        {
            printf("(");
            debug_elemPrintLine(op->link,0);
            printf(")");
        }

        else if (op->type == OP_FUNC_CALL)
        {
            printf("%s(", op->info);
            debug_elemPrintLine(op->link,0);
            printf(")");
        }

        else if(op->info)
            printf("%s ", op->info);

        op = op->next;
    }
}

/* ====================================================
                  debug_opPrintList
    --------------------------------------------------
    Arg : op   : Opération à débuger (afficher)
          lvl  : Niveau de profondeur
    --------------------------------------------------
    Def : Permet de dessiner les lignes de code du
          programme
   ==================================================== */
void debug_opPrintList(OperationDebug *op, int lvl)
{
    int i = 1;

    while(op)
    {
        if(op->startOrder == i)
        {
             debug_opPrint(op, lvl);
             op = op->first;
             i++;
        }
        else
            op = op->next;
    }
}

/* ====================================================
                  debug_opPrintList
    --------------------------------------------------
    Arg : op   : Opération à débuger (afficher)
          lvl  : Niveau de profondeur
    --------------------------------------------------
    Def : Permet de dessiner une ligne de code du
          programme en détaillant les opérations
   ==================================================== */
void debug_opPrint(OperationDebug *op, int lvl)
{
    int i = 0;

    if(!op)
        return;

    printf("\nOperation #%d : ", op->ID);

    for(i = 0; i < lvl; i++)
        printf("\t");

    if(op->type == OP_FUNC_CALL)
    {
        printf("fonction %s :", op->func->name);
        debug_opPrintList(op->elem1->ref, lvl + 1);
    }

    if(op->type == OP_MATHS || op->type == OP_ASSIGN)
    {
        if(op->elem1Type == ELEM_REF)
            printf("REF%d", op->elem1->ref->ID);

        if(op->operand)
            printf("%s", op->operand);

        if(op->elem2Type == ELEM_REF)
            printf("REF%d", op->elem2->ref->ID);
    }

    if(op->type == OP_VAR)
        printf("Variable %s", op->elem1->var->name);

    if(op->type == OP_NAT_VAR)
    {
        if(op->varType == TYPE_INT)
            printf("Valeur : %d", op->elem1->i);
        else if(op->varType == TYPE_CHAR)
            printf("Valeur : '%c'", op->elem1->c);
        else if(op->varType == TYPE_FLOAT)
            printf("Valeur : %f", op->elem1->f);
    }


    if(op->type == OP_PARENTH && op->elem1Type == ELEM_REF)
        printf("(REF%d)", op->elem1->ref->ID);

    if(op->elem1Type == ELEM_REF && op->type != OP_FUNC_CALL)
        debug_opPrint(op->elem1->ref, lvl);

    if(op->elem2Type == ELEM_REF && op->type != OP_FUNC_CALL)
        debug_opPrint(op->elem2->ref, lvl);
}
