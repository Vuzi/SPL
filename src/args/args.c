
#include "args.h"

/* ====================================================
                         defArgs
    --------------------------------------------------
    Arg : argc : nb d'argument
          argv : tableau de chaine d'argument
    --------------------------------------------------
    Def : Permet de traiter les arguments reçu par le
          programmme et de modifier les variables
          globales en conséquence
   ==================================================== */
void defArgs(int argc, char *argv[])
{
    int i = 0;

    g = malloc(sizeof(struct VarGlobal));
    g->logFile = NULL;
    g->printLvl = 0;

    g->g_verbose = 0;
    g->g_log = 1;
    g->g_compil = 1;
    g->g_debug = 0;
    g->g_debugOp = 0;
    g->g_gcc = 0;
    g->g_tcc = 1;

    if(argv[0]
       #ifdef _WIN32
            && strchr(argv[0], '/')
       #endif
       )
    {
        g->path = argv[0];
        *(strrchr(g->path, '/')) = '\0';
    }
    else
    {
        g->path = NULL;
    }

    for(i = 1; i < argc; i++)
    {
        if(argv[i][0] == '-') /* Si c'est un argument autre que le nom de fichier ou de celui de destination */
        {
            if(!strcmp("-nv", argv[i]))
                g->g_verbose = 0;
            else if(!strcmp("-v", argv[i]))
                g->g_verbose = 1;

            else if(!strcmp("-no-log", argv[i]))
                g->g_log = 0;
            else if(!strcmp("-log", argv[i]))
                g->g_log = 1;

            else if(!strcmp("-no-compil", argv[i]))
                g->g_compil = 0;
            else if(!strcmp("-compil", argv[i]))
                g->g_compil = 1;

            else if(!strcmp("-gcc", argv[i]))
            {
                g->g_gcc = 1;
                g->g_tcc = 0;
            }
            else if(!strcmp("-tcc", argv[i]))
            {
                g->g_gcc = 0;
                g->g_tcc = 1;
            }

            else if(!strcmp("-debug", argv[i]))
            {
                g->g_debug = 1;
                g->g_verbose = 1;
                #ifdef _WIN32
                    system("mode CON: COLS=120");
                #endif
            }

            else if(!strcmp("-debugOp", argv[i]))
            {
                g->g_debug = 1;
                g->g_verbose = 1;
                g->g_debugOp = 1;
                #ifdef _WIN32
                    system("mode CON: COLS=120");
                #endif
            }

        }
    }
}

/* ====================================================
                        defFileInput
    --------------------------------------------------
    Arg : argc : nb d'argument
          argv : tableau de chaine d'argument
    --------------------------------------------------
    Def : Permet de récupérer le nom du fichier source
   ==================================================== */
char* defFileInput(int argc, char *argv[])
{
    int i = 0;

    for(i = 1; i < argc; i++)
    {
        if(argv[i][0] != '-') /* Si c'est un argument autre que le nom de fichier ou de celui de destination */
        {
            return argv[i];
        }
    }

    return NULL;
}

/* ====================================================
                     defFileOutput
    --------------------------------------------------
    Arg : argc : nb d'argument
          argv : tableau de chaine d'argument
    --------------------------------------------------
    Def : Permet de récupérer le nom du fichier de
          sortie du programme
   ==================================================== */
char* defFileOutput(int argc, char *argv[])
{
    int i = 0;
    char cpt = 0;

    for(i = 1; i < argc; i++)
    {
        if(argv[i][0] != '-' && cpt++) /* Si c'est un argment autre que le nom de fichier ou de celui de destination */
        {
            return argv[i];
        }
    }

    return NULL;
}

/* ====================================================
                       printLog
    --------------------------------------------------
    Arg : type : type du log à écrire dans le log
    --------------------------------------------------
    Def : Permet de marquer simplement un début de
          ligne dans le fichier log
   ==================================================== */
void printLog(LOG_INFO_TYPE type)
{
    switch (type)
    {
        case INFO_LOG :
            fprintf(g->logFile, "\n[Info] ");
            break;

        case ERROR_LOG :
            fprintf(g->logFile, "\n[Erreur] ");
            break;

        case WARNING_LOG :
            fprintf(g->logFile, "\n[Warning] ");
            break;

        default :
            break;
    }
}

/* ====================================================
                       printfLog
    --------------------------------------------------
    Arg : type : type du log à écrire
    --------------------------------------------------
    Def : Permet de marquer simplement un début de
          ligne dans la console
   ==================================================== */
void printfLog(LOG_INFO_TYPE type)
{
    switch (type)
    {
        case INFO_LOG :
            BOLD_COLOR_TXT(COLOR_BLACK);
            printf("\n[Info] ");
            break;

        case ERROR_LOG :
            BOLD_COLOR_TXT(COLOR_RED);
            printf("\n[Erreur] ");
            break;

        case WARNING_LOG :
            BOLD_COLOR_TXT(COLOR_RED);
            printf("\n[Warning] ");
            break;

        default :
            break;
    }

    NORMAL_TXT;
}


/* ====================================================
                       printfSpace
    --------------------------------------------------
    Arg : -
    --------------------------------------------------
    Def : Permet de marquer simplement des espaces
   ==================================================== */
void printfSpace(void)
{
    int i = 0;

    COLOR_TXT(COLOR_GRAY);

    for(i = 0; i < g->printLvl ; i++)
        printf(">");

    NORMAL_TXT;
}


/* ====================================================
                       startLog
    --------------------------------------------------
    Arg : -
    --------------------------------------------------
    Def : Permet de 'démarrer' le fichier de log
   ==================================================== */
void startLog(void)
{
    struct tm *instant;
    time_t sec;
    char *path = NULL;

    if(!g->g_log)
        return;

    if(g->path)
    {
        path = malloc(sizeof(char)*(strlen(g->path)+9));
        strcpy(path, g->path);
        strcat(path, "/log.txt");

        g->logFile = fopen(path, "a");
        free(path);
    }

    else
        g->logFile = fopen("log.txt", "a");


    if (g->logFile == NULL)
    {
        PRINTFNV(ERROR_LOG, "Impossible d'ouvrir le fichier de log, appuyez sur une touche pour quitter...");
        getchar();
        exit(EXIT_SUCCESS);
    }

    time(&sec);
    instant = localtime(&sec);
    fprintf(g->logFile, "\n\n\n===== Debut du log : %d - %d - %d a %d:%d:%d =====\n",
            instant->tm_mday, instant->tm_mon + 1, instant->tm_year + 1900, instant->tm_hour, instant->tm_min, instant->tm_sec);

}

