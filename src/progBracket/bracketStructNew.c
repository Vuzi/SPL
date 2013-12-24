
#include "bracketStructNew.h"

/* ====================================================
                        getBracketFromElem
    --------------------------------------------------
    Arg : elem      : Element à tester
          container : Bracket contenant l'élément à
          traiter
    --------------------------------------------------
    Def : Permet transformer un élément en bracket, sans
          traiter le code à l'intérieur.
          Cette structuration permet de préparer le
          traitement futur des instructions
   ==================================================== */
Bracket* getBracketFromElem(Elem *elem, Bracket *container)
{
    CodeLine *cl = elem->code;

    /* On accroche la liste de cl */
    container->code = elem->code;

    if(g->g_debug)
    {
        printf("\n\n");
        debug_elemPrint(elem, 0);
    }

    LOOP_TYPE lastLoop = L_NONE;

    while(cl)
    {
        /* Boucle à l'intérieur */
        if(cl->link)
        {
            if(cl->link->name && !strcmp(cl->link->name, "var"))
            {
                DEBUG(INFO_LOG, "Variable detectee, lancement du traitement");
                INC_DEBUG;

                container->var = getVarFromElem(cl->link, container->var);

                DEC_DEBUG;
            }
            else
            {
                DEBUG(INFO_LOG, "Boucle detectee, lancement du traitement");
                INC_DEBUG;

                /* On ajoute une boucle */
                container->content = addBracket(container->content);
                container->content->func = container->func;

                /* On récupère le type, la condition et les lignes de code */
                getBracketType(container->content, cl->link);

                DEBUG(INFO_LOG, "Recuperation des informations...");
                container->content->cond = cl->link->cond;

                checkIf(container->content->type, lastLoop);
                lastLoop = container->content->type;

                container->content->content = getBracketFromElem(cl->link, container->content);
                cl->link_b = container->content;

                container->content->container = container;

                DEC_DEBUG;
            }

        }

        cl = cl->next;
    }

    if(container->content)
        return container->content->first;

    else
        return container->content;
}


/* ====================================================
                         checkIf
    --------------------------------------------------
    Arg : newType : Nouvelle boucle ou condition
          oldType : Dernière boucle ou condition
    --------------------------------------------------
    Def : Permet de vérifier qu'un else ou un else if
          suit toujours un if, et qu'une condition ne peut
          commencer par un else ou un else if seul
   ==================================================== */
void checkIf(LOOP_TYPE newType, LOOP_TYPE oldType)
{
    if(newType == L_IF || newType == L_WHILE || newType == L_NONE)
        return;
    else if(!((newType == L_ELSE || newType == L_ELSEIF) && (oldType == L_IF || oldType == L_ELSEIF)))
    {
        PRINTFNV(ERROR_LOG, "Une condition de type \"else\" ou \"else if\" n'est pas placee apres une condition \"if\" ");
        LOG(ERROR_LOG, "Une condition de type \"else\" ou \"else if\" n'est pas placee apres une condition \"if\" ");

        pthread_exit((void*)ERROR_COMPIL);
    }
}

/* ====================================================
                         checkIf
    --------------------------------------------------
    Arg : br   : Bracket où est la condition
          elem : Elément où est la condition à traiter
    --------------------------------------------------
    Def : Permet de récupérer les informations sur la
          nature de la boucle à tester, et vérifie
          sa validité
   ==================================================== */
void getBracketType(Bracket *br, Elem *elem)
{
    DEBUG(INFO_LOG, "Recuperation du type de la boucle...");

    /* Si on a définit une condition */
    if(elem->cond)
    {
        if(elem->name)
        {
            if(!strcmp(elem->name, "if"))
                br->type = L_IF;
            else if(!strcmp(elem->name, "elseif"))
                br->type = L_ELSEIF;
            else if(!strcmp(elem->name, "else"))
                br->type = L_ELSE;
            else if(!strcmp(elem->name, "while"))
                br->type = L_WHILE;
            else
            {
                PRINTFNV(ERROR_LOG, "Une declaration conditionnelle n'a pas pu etre reconnue : \n");
                LOG(ERROR_LOG, "Une declaration conditionnelle n'a pas pu être reconnue\n");

                debug_elem(elem);

                pthread_exit((void*)ERROR_COMPIL);
            }
        }
        else
        {
            PRINTFNV(ERROR_LOG, "Une declaration de boucle sans type comporte une condition : \n");
            LOG(ERROR_LOG, "Une declaration de boucle sans type comporte une condition");

            debug_elem(elem);

            pthread_exit((void*)ERROR_COMPIL);
        }
    }
    else
    {
        if(!elem->name)
                br->type = L_NONE;
        else if(!strcmp(elem->name, "else"))
                br->type = L_ELSE;
        else
        {
            PRINTFNV(ERROR_LOG, "Une declaration non conditionnelle n'a pas pu etre reconnue : \n");
            LOG(ERROR_LOG, "Une declaration non conditionnelle n'a pas pu être reconnue\n");

            debug_elem(elem);

            pthread_exit((void*)ERROR_COMPIL);
        }
    }
}
