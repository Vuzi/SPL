#include "varStructNew.h"

VAR_TYPE getNewType(VAR_TYPE a, VAR_TYPE b)
{
    if(a == TYPE_FLOAT)
    {
        return TYPE_FLOAT;
    }
    else if(a == TYPE_INT)
    {
        if(b == TYPE_FLOAT)
            return TYPE_FLOAT;
        else
            return TYPE_INT;
    }
    else if(a == TYPE_CHAR)
    {
        if(b == TYPE_FLOAT)
            return TYPE_FLOAT;
        else if (b == TYPE_INT)
            return TYPE_INT;
        else
            return TYPE_CHAR;
    }
    else
    {
        /* Erreur car type indéfini */
        return TYPE_UNDEFINED;
    }
}

/* ====================================================
                       typeFormChar
    --------------------------------------------------
    Arg : code : Code a traiter
    --------------------------------------------------
    Def : Retourne le type définit par code, ou bien
          TYPE_UNDEFINED.
   ==================================================== */
VAR_TYPE typeFormChar(char* code)
{
    if (!strcmp(code, "int"))
        return TYPE_INT;
    else if (!strcmp(code, "char"))
        return TYPE_CHAR;
    else if (!strcmp(code, "float"))
        return TYPE_FLOAT;
    else if (!strcmp(code, "void"))
        return TYPE_VOID;
    else
        return TYPE_UNDEFINED;
}

/* ====================================================
                       getVarName
    --------------------------------------------------
    Arg : op  : Opération correspondante au nom
          var : Variable dont on cherche le nom
    --------------------------------------------------
    Def : Définit le nom d'une variable ou bien génère
          une erreur
   ==================================================== */
void getVarName(Operation* op, Var* var)
{
    DEBUG(INFO_LOG, "Recuperation du nom de la variable...");

    if(var->name) /* Erreur de redéclaration */
    {
        PRINTFNV(ERROR_LOG, "Le nom d'une variable est declare 2 fois : %s ", var->name);
        LOG(ERROR_LOG, "Le nom d'une variable est déclaré 2 fois : %s ", var->name);

        pthread_exit((void*)ERROR_COMPIL);
    }

    /* On sait que le premier élément est "name", la suite doit être de type '=' 'variable' */
    if((op = op->next) && op->type == OP_MATHS && op->info && op->info[0] == '=')
    {
        if((op = op->next) && op->type == OP_VAR && op->info && !op->next )
        {
            /* nom */
            var->name = malloc(sizeof(char)*(strlen(op->info)+1));
            strcpy(var->name, op->info);

            /* Génération du nom final */
            var->finalName = malloc(sizeof(char)*(strlen(op->info)+1+10));
            strncpy(var->finalName, "_CSOL_VAR_", 10);
            strcpy(var->finalName + 10, op->info);

            return;
        }
    }

    /* Erreur sur le nom */
    PRINTFNV(ERROR_LOG, "Une affection de nom de variable est incorrecte. Elle doit etre de la forme \"name = nom_de_variable\" : \n");
    LOG(ERROR_LOG, "Une affection de nom de variable est incorrecte. Elle doit être de la forme \"name = nom_de_variable\" ");

    debug_elemPrintLine(op->first, 0);

    pthread_exit((void*)ERROR_COMPIL);
}

/* ====================================================
                       getVarType
    --------------------------------------------------
    Arg : op  : Opération correspondante au type
          var : Variable dont on cherche le type
    --------------------------------------------------
    Def : Définit le type d'une variable ou bien génère
          une erreur
   ==================================================== */
void getVarType(Operation* op, Var* var)
{
    DEBUG(INFO_LOG, "Recuperation du type de la variable...");

    if(var->type != TYPE_UNDEFINED) /* Erreur de redéclaration */
    {
        PRINTFNV(ERROR_LOG, "Le type d'une variable est declare 2 fois : ");
        LOG(ERROR_LOG, "Le type d'une variable est déclaré 2 fois");

        debug_elemPrintLine(op, 0);

        pthread_exit((void*)ERROR_COMPIL);
    }

    /* On sait que le premier élément est "type", la suite doit être de type '=' 'variable' */
    if((op = op->next) && op->type == OP_MATHS && op->info && op->info[0] == '=')
    {
        if((op = op->next) && op->type == OP_VAR && op->info && !op->next )
        {
            var->type = typeFormChar(op->info);

            if(var->type == TYPE_VOID || var->type == TYPE_UNDEFINED)
            {
                PRINTFNV(ERROR_LOG, "Une variable est déclarée avec un type incorrect : %s", op->info);
                LOG(ERROR_LOG, "Une variable est déclarée avec un type incorrect : %s", op->info);

                pthread_exit((void*)ERROR_COMPIL);
            }

            return;
        }
    }

    /* Erreur sur le nom */
    PRINTFNV(ERROR_LOG, "Une affection de type de variable est incorrecte. Elle doit etre de la forme \"type = int|autre\" : \n");
    LOG(ERROR_LOG, "Une affection de type de variable est incorrecte. Elle doit être de la forme \"type = int|autre\" ");

    debug_elemPrintLine(op->first, 0);

    pthread_exit((void*)ERROR_COMPIL);

}

/* ====================================================
                       getVarValue
    --------------------------------------------------
    Arg : op  : Opération correspondante a la valeur
          var : Variable dont on cherche la valeur
    --------------------------------------------------
    Def : Définit la valeur d'une variable ou bien génère
          une erreur
   ==================================================== */
void getVarValue(Operation* op, Var* var)
{
    OperationDebug* val = NULL;
    DEBUG(INFO_LOG, "Recuperation de la valeur de la variable...");

    if(var->val) /* Erreur de redéclaration */
    {
        PRINTFNV(ERROR_LOG, "La valeur d'une variable est declare 2 fois ");
        LOG(ERROR_LOG, "La valeur d'une variable est déclaré 2 fois ");

        pthread_exit((void*)ERROR_COMPIL);
    }

    else if(var->type == TYPE_UNDEFINED)
    {
        PRINTFNV(ERROR_LOG, "Une valeur est assignee a une variable avant d'avoir declare son type");
        LOG(ERROR_LOG, "Une valeur est assignée à une variable avant d'avoir déclaré son type");

        pthread_exit((void*)ERROR_COMPIL);
    }

    /* On sait que le premier élément est "value", la suite doit être de type '=' 'var naturel' */
    else if((op = op->next) && op->type == OP_MATHS && op->info && op->info[0] == '=')
    {
        if((op = op->next) && op->type == OP_NAT_VAR && op->info && !op->next )
        {
            val = getOpDebugNatVar(val, op);
            if(val->varType != var->type)
            {
                PRINTFNV(ERROR_LOG, "Une valeur du mauvais type est assignee a une variable");
                LOG(ERROR_LOG, "Une valeur du mauvais type est assignée a une variable");

                pthread_exit((void*)ERROR_COMPIL);
            }

            var->val = malloc(sizeof(Value));

            if(var->type == TYPE_INT)
                var->val->i = val->elem1->i;

            else if(var->type == TYPE_FLOAT)
                var->val->f = val->elem1->f;

            else if(var->type == TYPE_CHAR)
                var->val->c = val->elem1->c;

            destroyOpDebug(val);

            return;
        }
    }

    /* Erreur sur la valeur */
    PRINTFNV(ERROR_LOG, "Une affection de valeur de variable est incorrecte. Elle doit etre de la forme \"valeur = [0-9]\" : \n");
    LOG(ERROR_LOG, "Une affection de valeur de variable est incorrecte. Elle doit être de la forme \"valeur = [0-9]\" ");

    debug_elemPrintLine(op->first, 0);

    pthread_exit((void*)ERROR_COMPIL);
}

/* ====================================================
                       getVarFromElem
    --------------------------------------------------
    Arg : elem    : Élément où est la variable
          varList : Liste de variable ou la raccrocher
    --------------------------------------------------
    Def : Récupère une valeur depuis un Elem ou bien
          genère une erreur
   ==================================================== */
Var* getVarFromElem(Elem* elem, Var* varList)
{
    Var* var = addVar(varList);
    CodeLine* cl = elem->code;

    while(cl)
    {
        /* Une opération, soit type/nom/valeur */
        if(cl->op)
        {
            if(cl->op->info)
            {
                if(!strcmp(cl->op->info, "name"))
                    getVarName(cl->op, var);
                else if(!strcmp(cl->op->info, "type"))
                    getVarType(cl->op, var);
                else if(!strcmp(cl->op->info, "value"))
                    getVarValue(cl->op, var);
                else
                {
                    /* Erreur : un mot clef n'a pas pu être reconnu */
                    PRINTFNV(ERROR_LOG,"Un mot clef dans la declaration de la variable n'a pas pu etre reconnu :\n");
                    LOG(ERROR_LOG,"Un mot clef dans la déclaration de la variable n'a pas pu être reconnu");

                    debug_elemPrintLine(cl->op, 0);

                    pthread_exit((void*)ERROR_COMPIL);
                }

            }
            else
            {
                /* Erreur : un mot clef n'a pas pu être reconnu */
                PRINTFNV(ERROR_LOG, "Un ligne de la déclaration de variable est invalide :\n");
                LOG(ERROR_LOG, "Un ligne de la déclaration de variable est invalide");

                debug_elemPrintLine(cl->op, 0);

                pthread_exit((void*)ERROR_COMPIL);
            }
        }

        else if (cl->link)
        {
            /* Erreur : les déclaration ne sont pas autorisée dans les déclarations de variables */
            PRINTFNV(ERROR_LOG, "Une déclaration est effectuée dans une déclaration de variable");
            LOG(ERROR_LOG, "Une declaration est effectuee dans une declaration de variable :");

            debug_elem(cl->link);

            pthread_exit((void*)ERROR_COMPIL);
        }

        cl = cl->next;
    }

    checkVar(var, elem);

    return var;
}

/* ====================================================
                       getVarFromElem
    --------------------------------------------------
    Arg : elem : Élément où est la variable
          var  : Variable à vérfier
    --------------------------------------------------
    Def : Vérifie les éléments de la variable
   ==================================================== */
void checkVar(Var* var, Elem *elem)
{
    Var* varTmp = var;

    if(!var->name)
    {
        PRINTFNV(ERROR_LOG, "Une variable est declaree sans nom :");
        LOG(ERROR_LOG, "Une variable est déclarée sans nom");

        debug_elem(elem);

        pthread_exit((void*)ERROR_COMPIL);
    }

    else if(var->type == TYPE_UNDEFINED)
    {
        PRINTFNV(ERROR_LOG, "Une variable est declaree sans type :");
        LOG(ERROR_LOG, "Une variable est déclarée sans type");

        debug_elem(elem);

        pthread_exit((void*)ERROR_COMPIL);
    }

    if (!var->val)
    {
        DEBUG(INFO_LOG, "La variable n'a pas de valeur par defaut, attribution d'une valeur nulle");

        var->val = malloc(sizeof(Value));
        if(var->type == TYPE_INT)
            var->val->i = 0;
        else if(var->type == TYPE_CHAR)
            var->val->c = 0;
        else if(var->type == TYPE_FLOAT)
            var->val->f = 0;
    }

    var = var->first;

    while(var)
    {
        if(!strcmp(var->name, varTmp->name) && var->ID != varTmp->ID)
        {
            PRINTFNV(ERROR_LOG, "Deux variables possedent le meme nom au meme niveau : %s", var->name);
            LOG(ERROR_LOG, "Deux variables possèdent le même nom au même niveau : %s", var->name);

            debug_elem(elem);

            pthread_exit((void*)ERROR_COMPIL);
        }
        var = var->next;
    }

    return;
}
