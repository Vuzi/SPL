#ifndef ARGS_H_INCLUDED
#define ARGS_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

typedef enum LOG_INFO_TYPE
{
    INFO_LOG, ERROR_LOG, WARNING_LOG
}LOG_INFO_TYPE;

struct VarGlobal
{
    /* == LOG du traitement == */
    char g_log; /* log activé par défaut */
    FILE* logFile; /* Fichier du log */

    /* == Mode verbeux == */
    char g_verbose; /* log désactivé par défaut */

    /* == Mode compilation == */
    char g_compil; /* activé par défaut */

    /* == Compilateur */
    char g_tcc; /* Désactivé par défaut */
    char g_gcc; /* Activé par défaut */

    /* == Mode debug == */
    char g_debug; /* désactivé par défaut */
    char g_debugOp;
    int printLvl;

    /* == Emplacement executable == */
    char* path;
};

#ifndef _ARGS_GLOBAL_

    #define _ARGS_GLOBAL_

    extern struct VarGlobal *g;

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

#ifndef _COLOR_DEBUG_

    #define _COLOR_DEBUG_

    #ifdef __linux__
        /* Liste des couleurs */
        #define COLOR_BLACK 30
        #define COLOR_RED 31
        #define COLOR_GREEN 32
        #define COLOR_BROWN 33
        #define COLOR_BLUE 34
        #define COLOR_MAGENTA 35
        #define COLOR_CYAN 36
        #define COLOR_GRAY 37

        /* Fonctions de syntaxe */
        #define BOLD_TXT printf("\033[1m")
        #define BOLD_COLOR_TXT(color) printf("\033[1;%dm", color)
        #define COLOR_TXT(color) printf("\033[0;%dm", color)
        #define NORMAL_TXT printf("\033[0m")
    #else
        /* Liste des couleurs */
        #define COLOR_BLACK 30
        #define COLOR_RED 31
        #define COLOR_GREEN 32
        #define COLOR_BROWN 33
        #define COLOR_BLUE 34
        #define COLOR_MAGENTA 35
        #define COLOR_CYAN 36
        #define COLOR_GRAY 37

        /* Fonctions de syntaxe */
        #define BOLD_TXT
        #define BOLD_COLOR_TXT(color)
        #define COLOR_TXT(color)
        #define NORMAL_TXT

    #endif

#endif

/* Prototypes */
void defArgs(int argc, char *argv[]);
char* defFileInput(int argc, char *argv[]);
char* defFileOutput(int argc, char *argv[]);

void printLog(LOG_INFO_TYPE type);
void printfLog(LOG_INFO_TYPE type);
void printfSpace(void);
void startLog(void);

#endif
