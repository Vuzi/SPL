#include <stdio.h>
#include <stdlib.h>

#ifndef _ARGS_GLOBAL_

    #define _ARGS_GLOBAL_

    struct VarGlobal *g = NULL;

    /* == LOG du traitement == */
    #define LOG(type,...) (g->g_log ? printLog(type), fprintf(g->logFile,__VA_ARGS__) : 0)

    /* == Mode verbeux == */
    #define PRINTFV(type,...) (g->g_verbose ? printfLog(type), printf(__VA_ARGS__) : 0)
    #define PRINTFNV(type,...) (printfLog(type), printf(__VA_ARGS__))

    /* == Mode debug == */
    #define DEBUG(type,...) (g->g_debug ? printfLog(type), printfSpace(), printf(__VA_ARGS__) : 0)
    #define INC_DEBUG g->printLvl++
    #define DEC_DEBUG g->printLvl--

#endif

#include "args/args.h"
#include "debug/debug.h"
#include "compiler/compiler.h"

#include "parseFile/charElemStructNew.h"
#include "parseFile/charFunc.h"


int main(int argc, char* argv[])
{
    char *fileName = NULL, *fileOutput = NULL;
    FILE *f = NULL;

    BOLD_COLOR_TXT(COLOR_BLACK);
    printf("[Lancement du programme]\n");
    NORMAL_TXT;

    defArgs(argc, argv); /* on définit les arguments */
    startLog();

    PRINTFV(INFO_LOG, "Debut de la sequence d'initialisation du programme...");

    fileName = defFileInput(argc, argv); /* nom de l'input */
    fileOutput = defFileOutput(argc, argv); /* nom de l'output */

    if (!fileName) /* s'il n'est pas spécifié */
    {
        PRINTFNV(ERROR_LOG, "Aucun fichier lance avec le programme, appuyez sur une touche pour quitter...\n");
        LOG(ERROR_LOG, "Aucun fichier lancé avec le programme");
        getchar();
        exit(EXIT_SUCCESS);
    }

    else if ((f = fopen(fileName, "rb")) == NULL ) /* s'il est spécifié mais incorrect*/
    {
        PRINTFNV(ERROR_LOG, "Impossible d'ouvrir le premier fichier, appuyez sur une touche pour quitter...\n");
        LOG(ERROR_LOG, "Impossible d'ouvrir le fichier \"%s\" ", fileName);
        getchar();
        exit(EXIT_SUCCESS);
    }

    else
    {
        PRINTFV(-1, "termine");
        startCompil(f, fileOutput);
    }

    PRINTFNV(INFO_LOG, "Compilation terminee, appuyez sur une touche pour quitter...\n");
    LOG(INFO_LOG, "Fin de la compilation");

    getchar();

    fclose(g->logFile);
    free(g);

    exit(EXIT_SUCCESS);
}
