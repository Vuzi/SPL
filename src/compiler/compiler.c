
#include "compiler.h"

/* ====================================================
                       startCompil
    --------------------------------------------------
    Arg : f          : fichier où est écrit le code
          fileOutput : nom du fichier de sortie
    --------------------------------------------------
    Def : Permet de lancer la compilation sur le contenu
          du fichier pointé par f et d'écrire le résultat
          dans fileOutput
   ==================================================== */
void startCompil(FILE* f, char* fileOutput)
{
    pthread_t compil_th;
    STRUCT_CODE ret;
    Prog* prog = malloc(sizeof(Prog));

    /* Création de la structure représentative du programme */
    prog->func = NULL;
    prog->globalVar = NULL;
    prog->elem = NULL;
    prog->f = f;
    prog->fileOutput = fileOutput;
    prog->fileOutputCode = NULL;
    prog->specialFunc = createSpecialFunc();

    prog->warning = 0;

    /* On lance le thread */
    pthread_create(&compil_th, NULL, compilCode, (void*)prog);

    /* On attends la fin de son execution */
    pthread_join(compil_th, (void*)&ret);

    /* Libération de la mémoire */
    destroyElemList(prog->elem);
    destroyFuncList(prog->func);
    destroyFuncList(prog->specialFunc);
    destroyVarList(prog->globalVar);
    if(prog->fileOutputCode) free(prog->fileOutputCode);
    free(prog);

    /* Test suivant le type de retour */
    if (ret == DONE)
    {
        LOG(INFO_LOG, "Aucune erreur ou warnings durant la compilation");
        PRINTFNV(INFO_LOG, "Aucune erreur ou warnings durant la compilation");
    }
    else if (ret == DONE_WARNING)
    {
        LOG(INFO_LOG, "Des warnings ont été détecté lors de la compilation");
        PRINTFNV(INFO_LOG, "Des warnings ont ete detecte lors de la compilation");
    }
    else if (ret == ERROR_COMPIL)
    {
        LOG(INFO_LOG, "Des erreurs ont été détecté lors de la compilation, impossible de continuer. Vérifiez le fichier log.txt ou bien utilisez le mode debug (Option -debug) pour plus d'informations");
        PRINTFNV(INFO_LOG, "Des erreurs ont ete detecte lors de la compilation, impossible de continuer. Verifiez le fichier log.txt ou bien utilisez le mode debug (Option -debug) pour plus d'informations");
    }
    else
    {
        LOG(INFO_LOG, "Une erreur inconnue est survenue lors de la compilation,  impossible de continuer");
        PRINTFNV(INFO_LOG, "Une erreur inconnue est survenue lors de la compilation,  impossible de continuer");
    }

    PRINTFNV(INFO_LOG, "Compilation terminee, appuyez sur une touche pour quitter...\n");
    LOG(INFO_LOG, "Fin de la compilation");

    fclose(g->logFile);
    free(g);

    getchar();

    if(ret == DONE || ret == DONE_WARNING)
        exit(EXIT_SUCCESS);
    else
        exit(EXIT_FAILURE);
}

/* ====================================================
                       prepareCode
    --------------------------------------------------
    Arg : code : code à traiter
    --------------------------------------------------
    Def : Permet de préparer le code pour le traitement
          en retirant les caractère inutiles
   ==================================================== */
char* prepareCode(char* code)
{
    /* On supprime les commentaires */
    LOG(INFO_LOG, "Suppression des commentaires");
    PRINTFV(INFO_LOG, "Suppression des commentaires");

    removeComments(code);

    LOG(-1, "...terminé");
    PRINTFV(-1, "...termine");

    /* Suppression des retours à la ligne, sauts de ligne, et espaces */
    LOG(INFO_LOG, "Suppression des caractères inutiles");
    PRINTFV(INFO_LOG, "Suppression des caracteres inutiles");

    code = removeChar(code, 13);
    code = removeChar(code, 10);
    code = removeChar(code, 9);
    code = removeChar(code, ' ');

    LOG(-1, "...terminé");
    PRINTFV(-1, "...termine");

    /* Passage en miniscule de tout le code */
    LOG(INFO_LOG, "Passage en minuscule du code");
    PRINTFV(INFO_LOG, "Passage en minuscule du code");

    strLower(code);

    LOG(-1, "...terminé");
    PRINTFV(-1, "...termine");

    return code;
}

/* ====================================================
                       compilCode
    --------------------------------------------------
    Arg : prog_void : pointeur sur la structure
                      du programme
    --------------------------------------------------
    Def : Permet de traiter le code et de le transformer
          en C
   ==================================================== */
void* compilCode(void* prog_void)
{
    char *code = NULL;
    char buffer[1024];

    #ifdef __linux__
        pid_t pid = 0;
        int ret = 0;
    #endif
    Prog* prog = (Prog*)prog_void;
    FILE* tmp = NULL;

    prog->elem = addElem(NULL);

    /* lire et traiter le code */
    LOG(INFO_LOG, "Ouverture du fichier à traiter et debut du traitement");
    PRINTFNV(INFO_LOG, "Ouverture du fichier a traiter et debut du traitement");
    INC_DEBUG;

    /* On commence par récupérer le code */
    code = getFileContent(prog->f);
    fclose(prog->f);

    code = prepareCode(code);

    DEC_DEBUG;
    LOG(INFO_LOG, "Fin de récupération du code");
    PRINTFV(INFO_LOG, "Fin de recuperation du code");

    /* Premier parcours du code pour le structurer en mémoire */
    LOG(INFO_LOG, "Début du premier parcours du code pour structuration");
    PRINTFV(INFO_LOG, "Debut du premier parcours du code pour structuration");
    INC_DEBUG;

    prog->elem->content = structureCode(code, prog->elem);

    DEC_DEBUG;
    PRINTFV(INFO_LOG, "Fin du premier parcours du code pour structuration");
    LOG(INFO_LOG, "Fin du premier parcours du code pour structuration");

    if(g->g_debug)
        debug_elem(prog->elem);

    /* Début du second parcours pour le structurer en tant que programme */
    PRINTFV(INFO_LOG, "Debut du second parcours du code pour recuperation des informations");
    LOG(INFO_LOG, "Début du second parcours du code pour récupération des informations");
    INC_DEBUG;

    constructProg(prog);
    /*  Libération de la mémoire des éléments */
    prog->elem = destroyElemNoClList(prog->elem);

    DEC_DEBUG;
    LOG(INFO_LOG, "Fin du second parcours du code pour structuration");
    PRINTFV(INFO_LOG, "Fin du second parcours du code pour structuration");

    /* Début du troisieme parcours pour le structurer en tant que programme */
    LOG(INFO_LOG, "Début du troisieme parcours du code pour vérification des informations et construction du code");
    PRINTFV(INFO_LOG, "Debut du troisieme parcours du code pour verification des informations et construction du code");
    INC_DEBUG;

    constructProgCode(prog);

    DEC_DEBUG;
    LOG(INFO_LOG, "Fin du troisieme parcours du code pour verification");
    PRINTFV(INFO_LOG, "Fin du troisieme parcours du code pour verification");

    /* Création du code en C */
    LOG(INFO_LOG, "Début de la création du code en C");
    PRINTFV(INFO_LOG, "Debut de la creation du code en C");
    INC_DEBUG;

    constructCCode(prog);

    DEC_DEBUG;
    LOG(INFO_LOG, "Fin de la création du code en C");
    PRINTFV(INFO_LOG, "Fin de la creation du code en C");

    if(!g->g_compil)
    {
        PRINTFNV(INFO_LOG, "L'option 'no-compil' est activee, seul le code sera genere");
        LOG(INFO_LOG, "L'option 'no-compil' est activée, seul le code sera généré");
        PRINTFNV(INFO_LOG, "Le code C est situe dans le fichier : %s ", prog->fileOutput);
        LOG(INFO_LOG, "Le code C est situé dans le fichier : %s ", prog->fileOutput);

        /* Supprimer le fichier temporaire */
        remove(prog->fileOutputCode);
    }

    else
    {
        PRINTFNV(INFO_LOG, "Lancement de la compilation avec ");
        LOG(INFO_LOG, "Lancement de la compilation avec ");

        #ifdef __linux__
            if(g->path)
                sprintf(buffer, "%s/SPL_builtin/SPL_builtin.c", g->path);

            else
                sprintf(buffer, "SPL_builtin/SPL_builtin.c");
        #endif

        if(g->g_tcc)
        {
            PRINTFNV(-1, "TinyCC");
            LOG(-1, "TinyCC");

            #ifdef _WIN32
                if(g->path)
                {
                    while((code = strchr(g->path, '\\')))
                        *code = '\\';

                    sprintf(buffer, "%s\\tcc\\tcc.exe -o %s %s \"%s/SPL_builtin/SPL_builtin.c\" 1> tmp 2>&1", g->path, prog->fileOutput, prog->fileOutputCode, g->path);
                }

                else
                    sprintf(buffer, "\"tcc\\tcc.exe\" -o %s %s SPL_builtin/SPL_builtin.c 1> tmp 2>&1", prog->fileOutput, prog->fileOutputCode);
            #endif

            #ifdef __linux__
                if((pid = vfork())) /* Père */
                {
                    wait(&ret);
                }
                else if(pid == -1)
                {
                    /* Impossible de fork */
                    PRINTFNV(ERROR_LOG, "Un appel à la fonction fork a echoue");
                    LOG(ERROR_LOG, "Un appel à la fonction fork a echoue");

                    return (void*)ERROR_COMPIL;
                }
                else /* Fils */
                {
                    /* Redirection des entrée/sorties */
                    if(!freopen("tmp","w",stdout) || !freopen("tmp","w",stderr))
                    {
                        PRINTFNV(ERROR_LOG, "Impossible de rediriger la sortie standard et/ou d'erreur lors de l'appel a TinyCC. Le fichier \"tmp\" est il en cours d'utilisation ? ");
                        LOG(ERROR_LOG,  "Impossible de rediriger la sortie standard et/ou d'erreur lors de l'appel a TinyCC. Le fichier \"tmp\" est il en cours d'utilisation ? ");
                    }

                    /* Lancement de la compilation par tcc */
                    execl("/usr/bin/tcc", "tcc", "-o", prog->fileOutput, prog->fileOutputCode, buffer, NULL);

                    /* Revenir ici signifie erreur de execl */
                    printf("Impossible de lancer TinyCC (Echec de la fonction execl)");
                    exit(EXIT_FAILURE);
                }
            #endif
        }

        else if(g->g_gcc)
        {
            PRINTFNV(-1, "GCC");
            LOG(-1, "GCC");

            #ifdef _WIN32
                if(g->path)
                    sprintf(buffer, "gcc -o %s %s \"%s/SPL_builtin/SPL_builtin.c\" 1> tmp 2>&1", prog->fileOutput, prog->fileOutputCode, g->path);
                else
                    sprintf(buffer, "gcc -o %s %s SPL_builtin/SPL_builtin.c 1> tmp 2>&1", prog->fileOutput, prog->fileOutputCode);
            #endif

            #ifdef __linux__
                if((pid = vfork())) /* Père */
                {
                    wait(&ret);
                }
                else if(pid == -1)
                {
                    /* Impossible de fork */
                    PRINTFNV(ERROR_LOG, "Un appel à la fonction fork a echoue");
                    LOG(ERROR_LOG, "Un appel à la fonction fork a echoue");

                    return (void*)ERROR_COMPIL;
                }
                else /* Fils */
                {
                    /* Redirection des entrée/sorties */
                    if(!freopen("tmp","w",stdout) || !freopen("tmp","w",stderr))
                    {
                        PRINTFNV(ERROR_LOG, "Impossible de rediriger la sortie standard et/ou d'erreur lors de l'appel a GCC. Le fichier \"tmp\" est il en cours d'utilisation ? ");
                        LOG(ERROR_LOG,  "Impossible de rediriger la sortie standard et/ou d'erreur lors de l'appel a GCC. Le fichier \"tmp\" est il en cours d'utilisation ? ");
                    }

                    /* Lancement de la compilation par GCC */
                    execl("/usr/bin/gcc", "gcc", "-o", prog->fileOutput, prog->fileOutputCode, buffer, NULL);

                    /* Revenir ici signifie erreur de execl */
                    printf("Impossible de lancer GCC (Echec de la fonction execl)");
                    exit(EXIT_FAILURE);
                }
            #endif
        }

        #ifdef _WIN32
        if(system(buffer))
        #endif

        #ifdef __linux__
        if(ret)
        #endif
        {
            if(g->g_tcc)
            {
                PRINTFNV(ERROR_LOG, "TinyCC a rencontre une erreur lors de la compilation du code C");
                LOG(ERROR_LOG, "TinyCC a rencontré une erreur lors de la compilation du code C");
            }

            else if(g->g_gcc)
            {
                PRINTFNV(ERROR_LOG, "GCC a rencontre une erreur lors de la compilation du code C");
                LOG(ERROR_LOG, "GCC a rencontré une erreur lors de la compilation du code C");
            }


            if(!(tmp = fopen("tmp", "rb")))
            {
                PRINTFNV(ERROR_LOG, "Impossible de recuperer le message d'erreur");
                LOG(ERROR_LOG, "Impossible de récuperer le message d'erreur");
            }
            else
            {
                code = getFileContent(tmp);
                fclose(tmp);

                PRINTFNV(ERROR_LOG, "Message d'erreur :\n%s", code);
                LOG(ERROR_LOG, "Message d'erreur :\n%s", code);
                free(code);
            }

            /* Supprimer le fichier temporaire */
            remove("tmp");
            remove(prog->fileOutputCode);

            return (void*)ERROR_COMPIL;
        }

        PRINTFNV(INFO_LOG, "Fichier compile avec succes. Programme : %s", prog->fileOutput);
        LOG(INFO_LOG, "Fichier compilé avec succès. Programme : %s", prog->fileOutput);

        /* Supprimer le fichier temporaire */
        remove("tmp");
        remove(prog->fileOutputCode);
    }

    if(prog->warning == 0)
        return (void*)DONE;
    else
        return (void*)DONE_WARNING;
}

/* ====================================================
                       constructCCode
    --------------------------------------------------
    Arg : prog : pointeur sur la structure
                 du programme
    --------------------------------------------------
    Def : Permet de traduire en C le programme
   ==================================================== */
void constructCCode(Prog* prog)
{
    FILE *f = NULL;

    PRINTFNV(INFO_LOG, "Ouverture du fichier temporaire ou generer le code");
    LOG(INFO_LOG, "Ouverture du fichier temporaire où générer le code");

    if (!prog->fileOutput) /* s'il n'est pas spécifié */
    {
        prog->warning++;

        if(g->g_compil)
        #ifdef _WIN32
            {
                prog->fileOutput = malloc(sizeof(char)*(8));
                strcpy(prog->fileOutput, "out.exe");
            }
        #else
            {
                prog->fileOutput = malloc(sizeof(char)*(4));
                strcpy(prog->fileOutput, "out");
            }
        #endif
        else
        {
            prog->fileOutput = malloc(sizeof(char)*(6));
            strcpy(prog->fileOutput, "out.c");
        }


        PRINTFNV(WARNING_LOG, "Aucun fichier de sortie specifie, le nom utilise sera \"%s\"", prog->fileOutput);
        LOG(WARNING_LOG, "Aucun fichier de sortie spécifié, le nom utilisé sera \"%s\"", prog->fileOutput);
    }

    prog->fileOutputCode = malloc(sizeof(char)*7);
    strcpy(prog->fileOutputCode, "_tmp.c");



    if ((f = fopen(prog->fileOutputCode, "w+")) == NULL ) /* Impossible d'ouvrir le fichier de sortie */
    {
        PRINTFNV(ERROR_LOG, "Impossible d'ouvrir le fichier temporaire (%s) ", prog->fileOutputCode);
        LOG(ERROR_LOG, "Impossible d'ouvrir le fichier temporaire (%s) ", prog->fileOutputCode);

        pthread_exit((void*)ERROR_COMPIL);
    }

    translateProgToC(prog, f);

    fclose(f);

    if(!g->g_compil)
    {
        if(rename(prog->fileOutputCode, prog->fileOutput) < 0)
        {
            PRINTFNV(ERROR_LOG, "Impossible de modifier le nom du fichier temporaire (%s). Il est possible que le fichier %s existe deja", prog->fileOutputCode ,prog->fileOutput);
            LOG(ERROR_LOG, "Impossible de modifier le nom du fichier temporaire (%s). Il est possible que le fichier %s existe déjà", prog->fileOutputCode, prog->fileOutput);
        }
    }

    PRINTFNV(INFO_LOG, "Fermeture du fichier temporaire");
    LOG(INFO_LOG, "Fermeture du fichier temporaire");
}

/* ====================================================
                    constructProgCode
    --------------------------------------------------
    Arg : prog : pointeur sur la structure
                 du programme
    --------------------------------------------------
    Def : Permet d'analyser les instructions du
          programme
   ==================================================== */
void constructProgCode(Prog* prog)
{
    Func* func = prog->func;

    if(func)
        func = func->first;

    /* On parcours chacunes des fonctions */
    while(func)
    {
        /* Chaque fonction contient un/des bracket(s), qui contiennent d'autre(s) bracket(s) */
        DEBUG(INFO_LOG, "Debut traitement de la fonction %s (ID %d)", func->name, func->ID);
        INC_DEBUG;
        getOpDebugBr(func->content, prog);
        DEC_DEBUG;
        func = func -> next;
    }
}

/* ====================================================
                    constructProg
    --------------------------------------------------
    Arg : prog : pointeur sur la structure
                 du programme
    --------------------------------------------------
    Def : Permet d'analyser la structuration du code et
          de récupérer les variables et fonctions
   ==================================================== */
void constructProg(Prog *prog)
{
    char test_main = 0;
    CodeLine *cl = prog->elem->code;

    while(cl)
    {
        if(!cl->link)
        {
            PRINTFNV(ERROR_LOG, "Du code est ecrit en dehors de toute declaration : \n");
            LOG(ERROR_LOG, "Du code est écrit en dehors de toute déclaration");

            debug_elemPrintLine(cl->op,0);

            pthread_exit((void*)ERROR_COMPIL);
        }

        if(!cl->link->name)
        {
            PRINTFNV(ERROR_LOG, "Une declaration sans nom est présente : ");
            LOG(ERROR_LOG, "Une déclaration sans nom est présente");

            debug_elem(cl->link);

            pthread_exit((void*)ERROR_COMPIL);
        }

        else if(!strcmp(cl->link->name, "var"))
        {
            if(cl->link->cond)
            {
                PRINTFNV(ERROR_LOG, "Une declaration de variable est faite avec une condition : ");
                LOG(ERROR_LOG, "Une déclaration de variable est faite avec une condition");

                debug_elem(cl->link);

                pthread_exit((void*)ERROR_COMPIL);
            }
            else
            {
                DEBUG(INFO_LOG, "Variable detectee, lancement du traitement");
                INC_DEBUG;

                prog->globalVar = getVarFromElem(cl->link, prog->globalVar);

                if(g->g_debug)
                    debug_varOnly(prog->globalVar);

                DEC_DEBUG;
            }
        }

        else if (!strcmp(cl->link->name, "func"))
        {
            if(cl->link->cond)
            {
                PRINTFNV(ERROR_LOG, "Une declaration de fonction est faite avec une condition : ");
                LOG(ERROR_LOG, "Une déclaration de fonction est faite avec une condition");

                debug_elem(cl->link);

                pthread_exit((void*)ERROR_COMPIL);
            }
            else
            {
                DEBUG(INFO_LOG, "Fonction detectee, lancement du traitement");
                INC_DEBUG;

                prog->func = getFuncFromElem(cl->link, prog->func);

                if(g->g_debug)
                    debug_funcOnly(prog->func);

                DEC_DEBUG;
            }
        }

        else if (!strcmp(cl->link->name, "start"))
        {
            if(cl->link->cond)
            {
                PRINTFNV(ERROR_LOG, "La declaration du point d'entree est faite avec une condition");
                LOG(ERROR_LOG, "La déclaration du point d'entrée est faite avec une condition");

                debug_elem(cl->link);

                pthread_exit((void*)ERROR_COMPIL);
            }
            else if (test_main)
            {
                PRINTFNV(ERROR_LOG, "Le programme comporte plusieurs points d'entree");
                LOG(ERROR_LOG, "Le programme comporte plusieurs points d'entrée");

                pthread_exit((void*)ERROR_COMPIL);
            }
            else
            {
                test_main++;
                DEBUG(INFO_LOG, "Point d'entree detecte, lancement du traitement");
                INC_DEBUG;

                prog->func = getMainFromElem(cl->link, prog->func);

                if(g->g_debug)
                    debug_funcOnly(prog->func);

                DEC_DEBUG;
            }
        }

        else
        {
            PRINTFNV(ERROR_LOG, "Une declaration comporte un mot clef inconnu (%s) : ", cl->link->name);
            LOG(ERROR_LOG, "Une déclaration comporte un mot clef inconnu (%s) ", cl->link->name);

            debug_elem(cl->link);

            pthread_exit((void*)ERROR_COMPIL);
        }

        cl = cl->next;
    }

    /* On test si un point d'entrée est déclaré */
    if(!getFuncByID(0, prog->func))
    {
        PRINTFNV(ERROR_LOG, "Le programme ne comporte pas de point d'entree");
        LOG(ERROR_LOG, "Le programme ne comporte pas de point d'entrée");

        pthread_exit((void*)ERROR_COMPIL);
    }

    return;
}

