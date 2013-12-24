#include"funcStructNew.h"

/* ====================================================
                       getFuncName
    --------------------------------------------------
    Arg : op   : Opération correspondante au nom
          func : Fonction dont on cherche le nom
    --------------------------------------------------
    Def : Définit le nom d'une fonction ou bien génère
          une erreur
   ==================================================== */
void getFuncName(Operation* op, Func* func)
{
    DEBUG(INFO_LOG, "Recuperation du nom de la fonction...");

    if(func->name) /* Erreur de redéclaration */
    {
        PRINTFNV(ERROR_LOG, "Le nom d'une fonction est declare 2 fois : %s ", func->name);
        LOG(ERROR_LOG, "Le nom d'une fonction est déclaré 2 fois : %s ", func->name);

        pthread_exit((void*)ERROR_COMPIL);
    }

    /* On sait que le premier élément est "name", la suite doit être de type '=' 'variable' */
    if((op = op->next) && op->type == OP_MATHS && op->info && op->info[0] == '=')
    {
        if((op = op->next) && op->type == OP_VAR && op->info && !op->next )
        {
            /* Nom */
            func->name = malloc(sizeof(char)*(strlen(op->info)+1));
            strcpy(func->name, op->info);

            /* Génération du nom final */
            func->finalName = malloc(sizeof(char)*(strlen(op->info)+1+11));
            strncpy(func->finalName, "_CSOL_FUNC_", 11);
            strcpy(func->finalName + 11, op->info);

            return;
        }
    }

    /* Erreur sur le nom */
    PRINTFNV(ERROR_LOG, "Une affection de nom de fonction est incorrecte. Elle doit être de la forme \"name = nom_de_fonction\" : \n");
    LOG(ERROR_LOG, "Une affection de nom de fonction est incorrecte. Elle doit être de la forme \"name = nom_de_fonction\" ");

    debug_elemPrintLine(op->first, 0);

    pthread_exit((void*)ERROR_COMPIL);
}

/* ====================================================
                       getFuncType
    --------------------------------------------------
    Arg : op   : Opération correspondante au type
          func : Fonction dont on cherche le type
    --------------------------------------------------
    Def : Définit le type d'une fonction ou bien génère
          une erreur
   ==================================================== */
void getFuncType(Operation* op, Func* func)
{
    DEBUG(INFO_LOG, "Recuperation du type de la fonction...");

    if(func->type != TYPE_UNDEFINED) /* Erreur de redéclaration */
    {
        PRINTFNV(ERROR_LOG, "Le type d'une fonction est declare 2 fois : ");
        LOG(ERROR_LOG, "Le type d'une fonction est déclaré 2 fois");

        debug_elemPrintLine(op, 0);

        pthread_exit((void*)ERROR_COMPIL);
    }

    /* On sait que le premier élément est "type", la suite doit être de type '=' 'variable' */
    if((op = op->next) && op->type == OP_MATHS && op->info && op->info[0] == '=')
    {
        if((op = op->next) && op->type == OP_VAR && op->info && !op->next )
        {
            func->type = typeFormChar(op->info);

            if(func->type == TYPE_UNDEFINED)
            {
                PRINTFNV(ERROR_LOG, "Une fonction est déclarée avec un type incorrect : %s", op->info);
                LOG(ERROR_LOG, "Une fonction est déclarée avec un type incorrect : %s", op->info);

                pthread_exit((void*)ERROR_COMPIL);
            }

            return;
        }
    }

    /* Erreur sur le nom */
    PRINTFNV(ERROR_LOG, "Une affection de type de fonction est incorrecte. Elle doit être de la forme \"type = int|void|autre\" : \n");
    LOG(ERROR_LOG, "Une affection de type de fonction est incorrecte. Elle doit être de la forme \"type = int|void|autre\" ");

    debug_elemPrintLine(op->first, 0);

    pthread_exit((void*)ERROR_COMPIL);

}

/* ====================================================
                       getFuncArgs
    --------------------------------------------------
    Arg : op   : Opération correspondante au type
          func : Fonction dont on cherche le type
    --------------------------------------------------
    Def : Définit les arguments de la fonction
   ==================================================== */
void getFuncArgs(Elem *elem, Func* func)
{

    while(elem)
    {
        if(!elem->name)
        {
            PRINTFNV(ERROR_LOG, "Une declaration d'argument est sans nom. Une déclaration d'argument doit toujours commencer par \"var\" et ne pas comporter de condition :\n");
            LOG(ERROR_LOG, "Une declaration d'argument est sans nom. Une déclaration d'argument doit toujours commencer par \"var\" et ne pas comporter de condition");

            debug_elem(elem);

            pthread_exit((void*)ERROR_COMPIL);
        }

        else if(!strcmp(elem->name, "var") && !elem->cond)
        {
            DEBUG(INFO_LOG, "Argument detecte, lancement du traitement");
            INC_DEBUG;

            func->arg = getVarFromElem(elem, func->arg);

            DEC_DEBUG;
        }

        else if(!strcmp(elem->name, "ref") && !elem->cond)
        {
            DEBUG(INFO_LOG, "Reference detectee, lancement du traitement");
            INC_DEBUG;

            func->arg = getVarFromElem(elem, func->arg);
            func->arg->isRef = 1;

            DEC_DEBUG;
        }

        else
        {
            PRINTFNV(ERROR_LOG, "Une declaration d'argument est incorrecte. Une déclaration d'argument doit toujours commencer par \"var\" et ne pas comporter de condition :\n");
            LOG(ERROR_LOG, "Une declaration d'argument est incorrecte. Une déclaration d'argument doit toujours commencer par \"var\" et ne pas comporter de condition");

            debug_elem(elem);

            pthread_exit((void*)ERROR_COMPIL);
        }
        elem = elem->next;
    }

}

/* ====================================================
                       getFuncCode
    --------------------------------------------------
    Arg : op   : Opération correspondante au type
          func : Fonction dont on cherche le type
    --------------------------------------------------
    Def : Définit les arguments de la fonction
   ==================================================== */
void getFuncCode(Elem *elem, Func* func)
{
    if (func->content)
    {
        /* Redéclaration du code */
        PRINTFNV(ERROR_LOG, "Le code d'une fonction est declare deux fois : \n");
        LOG(ERROR_LOG, "Le code d'une fonction est déclaré deux fois");

        debug_elem(elem);

        pthread_exit((void*)ERROR_COMPIL);
    }

    func->content = addBracket(NULL);
    func->content->func = func;

    DEBUG(INFO_LOG, "Recuperation du code de la fonction...");
    INC_DEBUG;

    getBracketFromElem(elem, func->content);

    DEC_DEBUG;
}

/* ====================================================
                       getFuncFromElem
    --------------------------------------------------
    Arg : elem    : Élément où est la fonction
          varList : Liste de fonctions ou la raccrocher
    --------------------------------------------------
    Def : Récupère une valeur depuis un Elem ou bien
          genère une erreur
   ==================================================== */
Func* getFuncFromElem(Elem* elem, Func* funcList)
{
    Func* func = addFunc(funcList);
    CodeLine* cl = elem->code;

    while(cl)
    {
        /* Une opération, soit type/nom/valeur */
        if(cl->op)
        {
            if(cl->op->info)
            {
                if(!strcmp(cl->op->info, "name"))
                    getFuncName(cl->op, func);
                else if(!strcmp(cl->op->info, "type"))
                    getFuncType(cl->op, func);
                else
                {
                    /* Erreur : un mot clef n'a pas pu être reconnu */
                    PRINTFNV(ERROR_LOG,"Un mot clef dans la declaration de la fonction n'a pas pu etre reconnu :\n");
                    LOG(ERROR_LOG,"Un mot clef dans la déclaration de la fonction n'a pas pu être reconnu");

                    debug_elemPrintLine(cl->op, 0);

                    pthread_exit((void*)ERROR_COMPIL);
                }

            }
            else
            {
                /* Erreur : un mot clef n'a pas pu être reconnu */
                PRINTFNV(ERROR_LOG, "Un ligne de la déclaration de fonction est invalide :\n");
                LOG(ERROR_LOG, "Un ligne de la déclaration de fonction est invalide");

                debug_elemPrintLine(cl->op, 0);

                pthread_exit((void*)ERROR_COMPIL);
            }
        }

        /* Un lien, argument ou code */
        else if (cl->link)
        {
            if(cl->link->name)
            {
                if(!strcmp(cl->link->name, "args"))
                    getFuncArgs(cl->link->content, func);

                else if(!strcmp(cl->link->name, "code"))
                    getFuncCode(cl->link, func);

                else
                {
                    /* Erreur : les déclaration ne sont pas autorisée dans les déclarations de variables */
                    PRINTFNV(ERROR_LOG, "Une declaration est effectuee avec un mot clef inconnu (%s), seules les declarations d'arguments (args) ou de code (code) sont possibles :\n", cl->link->name);
                    LOG(ERROR_LOG, "Une déclaration est effectuée avec un mot clef inconnu (%s), seules les déclarations d'arguments (args) ou de code (code) sont possibles", cl->link->name);

                    debug_elem(cl->link);

                    pthread_exit((void*)ERROR_COMPIL);
                }
            }
            else
            {
                /* Erreur déclaration sans nom */
                PRINTFNV(ERROR_LOG, "Une declaration est effectuee sans mot clef : \n");
                LOG(ERROR_LOG, "Une déclaration est effectuée sans mot clef");

                debug_elem(cl->link);

                pthread_exit((void*)ERROR_COMPIL);
            }
        }

        cl = cl->next;
    }

    checkFunc(func, elem);

    return func;
}

/* ====================================================
                       getMainFromElem
    --------------------------------------------------
    Arg : elem    : Élément où est la fonction
          varList : Liste de fonctions ou la raccrocher
    --------------------------------------------------
    Def : Récupère une valeur depuis un Elem ou bien
          genère une erreur
   ==================================================== */
Func* getMainFromElem(Elem* elem, Func* funcList)
{
    Func* func = addFunc(funcList);

    /* nom */
    func->name = malloc(sizeof(char)*6);
    strncpy(func->name, "start", 5);
    func->name[5] = '\0';
    func->ID = 0;

    /* type */
    func->type = TYPE_VOID;

    getFuncCode(elem, func);

    return func;
}

/* ====================================================
                       checkFunc
    --------------------------------------------------
    Arg : func : Fonction a tester
          elem : Element associé
    --------------------------------------------------
    Def : Teste si une fonction est valide ou genère une
          erreur
   ==================================================== */
void checkFunc(Func* func, Elem* elem)
{
    Func* funcTmp = func;

    if(!func->name)
    {
        PRINTFNV(ERROR_LOG, "Une fonction est declaree sans nom : \n");
        LOG(ERROR_LOG, "Une fonction est déclarée sans nom");

        debug_elem(elem);

        pthread_exit((void*)ERROR_COMPIL);
    }

    else if(func->type == TYPE_UNDEFINED)
    {
        DEBUG(ERROR_LOG, "Une fonction est declaree sans type de retour, attribution du type void");
        func->type = TYPE_VOID;
    }

    func = func->first;

    while(func)
    {
        if(!strcmp(func->name, funcTmp->name) && func->ID != funcTmp->ID)
        {
            PRINTFNV(ERROR_LOG, "Deux fonctions possedent le meme nom au meme niveau : %s\n", func->name);
            LOG(ERROR_LOG, "Deux fonctions possèdent le même nom au même niveau : %s", func->name);

            debug_elem(elem);

            pthread_exit((void*)ERROR_COMPIL);
        }
        func = func->next;
    }

    return;
}

