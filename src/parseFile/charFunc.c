#include "charFunc.h"

/* ====================================================
                         getNbChars
    --------------------------------------------------
    Arg : f : pointeur vers un fichier
    --------------------------------------------------
    Def : Retourne le nombre de caractères du fichier f
   ==================================================== */
int getNbChars(FILE *f)
{
    int nbChar = 0;

    fseek(f, 0, SEEK_END);
    nbChar = (int)ftell(f);

    fseek(f, 0, SEEK_SET);

    return nbChar;
}

/* ====================================================
                     getFileContent
    --------------------------------------------------
    Arg : f : pointeur vers un fichier
    --------------------------------------------------
    Def : Retourne le contenu du fichier f sous forme
          d'une chaine de caractères
   ==================================================== */
char* getFileContent(FILE *f)
{
    int size = getNbChars(f);
    char* content = malloc(sizeof(char)*(size+1));

    if (fread(content, sizeof(char), size, f) != (unsigned)size)
    {
        PRINTFNV(ERROR_LOG, "Impossible de lire correctement le fichier");
        LOG(ERROR_LOG, "Impossible de lire correctement le fichier");

        pthread_exit((void*)ERROR_COMPIL);
    }

    content[size] = '\0';

    return content;
}

/* ====================================================
                       removeChar
    --------------------------------------------------
    Arg : src : chaine a traiter
          c   : caractère à supprimer
    --------------------------------------------------
    Def : Supprimme le caractère c de la chaine src, et
          renvoit un pointeur vers src
   ==================================================== */
char* removeChar(char* src, char c)
{
    char* tmp = src;
    bool modif = false;
    bool exept = false;
    int i;

    while(*tmp != '\0')
    {
        if(*tmp == '\'' || *tmp == '"')
        {
            if(exept)
            {
                if(*(tmp-1) != '\\')
                    exept = false;
            }
            else
                exept = true;
        }

        if (!exept && *tmp == c)
        {
            modif = true;

            for(i = 0; tmp[i+1] != '\0'; i++)
                tmp[i] = tmp[i+1];

            tmp[i] = '\0';
        }
        else
            tmp++;
    }

    if(modif)
    {
        i = strlen(src)+1;
        tmp = malloc(sizeof(char)*(i));
        memcpy(tmp, src, i);
        free(src);
        return tmp;
    }

    else
        return src;
}

/* ====================================================
                       cutSegment
    --------------------------------------------------
    Arg : start : début du découpage
          end   : fin du découpage
          code  : code à découper
    --------------------------------------------------
    Def : Supprimme un morceau de la chaine code, en
          partant de start jusqu'à end

          /!\ Aucune réallocation mémoire n'est faite
   ==================================================== */
void cutSegment(const char* start, const char* end, char* code)
{
    size_t i = 0, dec = 0, stop = strlen(code);

    if (stop > 0 )
    {
        i = start - code;
        dec = end - start;
        while(i <= stop - 2)
        {
            code[i] = code[i + dec];
            i++;
        }
        code[stop-1] = '\0';
    }
}

/* ====================================================
                       getSegment
    --------------------------------------------------
    Arg : start : élément de début
          end   : élément de fin
          code  : code à découper
    --------------------------------------------------
    Def : Découpe le morceau de chaine entre start et
          end, et le retourne.

          /!\ Cette fonction ne gère pas les réouverture
          de balise à l'intérieur du code

          /!\ Aucune réallocation mémoire n'est faite
          sur la chaine modifiée
   ==================================================== */
char* getSegment(const char* start, const char* end, char* code)
{
    char *segStart = NULL, *segEnd = NULL, *segTmp = NULL, *segment = NULL;
    size_t segmentSize = 0;

    if ((segStart = strstr(code, start)) != NULL)
    {
        segTmp = segStart;
        while ((segEnd = strstr(segTmp, end)) != NULL && segEnd[-1] == '\\')
            segTmp = segEnd + 1;

        if (segEnd != NULL)
        {
            segStart += strlen(start);
            segmentSize = segEnd - segStart;
            segment = malloc(sizeof(wchar_t)*(segmentSize+1));

            strncpy(segment, segStart, segmentSize);
            segment[segmentSize] = '\0';

            cutSegment(segStart - strlen(start), segEnd + strlen(end), code);

            return segment;
        }

        else
        {
            PRINTFNV(ERROR_LOG, "Une déclaration (%s - %s) n'est pas correctement refermée", start, end);
            LOG(ERROR_LOG, "Une déclaration (%s - %s) n'est pas correctement refermée", start, end);
            pthread_exit((void*)ERROR_COMPIL);
        }

    }

    return NULL;
}

/* ====================================================
                       getBracket
    --------------------------------------------------
    Arg : code   : code à découper
          open_  : char de début
          close_ : char de fin
    --------------------------------------------------
    Def : Permet de récupérer la chaine entre open et
          close. Cette fonction gère les réouverture et
          fermeture.
   ==================================================== */
char* getBracket(char* code, char open_, char close_)
{
    char *segEnd = code, *segTmp = code;
    char *result = NULL;

    /* Le code doit commencer par une [ sinon c'est que quelque chose ne va pas */
    if(!code || strlen(code) <= 1 || code[0] != open_)
        return NULL;

    /* Ici le but est de trouver le prochain ']' tout en regardant si le prochain '[' est soit == NULL soit après */
    /* Si ce n'est pas le cas, on recommence la recherche en partant du dernier ']' */

    /* Tant qu'on en trouve un (Sinon nombre impaire et erreur) */
    while((segEnd = strchr(segEnd+1, close_)))
    {
        /* Si on ne trouve plus d'ouverture ou si elle est après la fermeture */
        if( !(segTmp = strchr(segTmp+1, open_)) || (segEnd < segTmp) )
        {
            result = malloc((segEnd-code)*sizeof(char));
            strncpy(result, code+1, (segEnd-code)-1);
            result[segEnd-code-1] = '\0';

            cutSegment(code, segEnd+1, code);
            return result;
        }
    }

    /* Erreur car '[' n'est pas refermé !! */
    LOG(ERROR_LOG, "Une expression ( %c ) n'est pas fermée : %s", open_, code);
    PRINTFNV(ERROR_LOG, "Une expression ( %c ) n'est pas fermee : %s", open_, code);
    pthread_exit((void*)ERROR_COMPIL);

    return NULL;
}

/* ====================================================
                       getClosingBracket
    --------------------------------------------------
    Arg : code   : code à découper
          open_  : char de début
          close_ : char de fin
    --------------------------------------------------
    Def : Retourne l'emplacement de l'élément fermant
          ou NULL s'il est impossible à trouver
          ( Gère les ouvertures/fermetures )
   ==================================================== */
char* getClosingBracket(char* start, char open_, char close_)
{
    char* segEnd = start, *segTmp = start;

    /* Le code doit commencer par une [ sinon c'est que quelque chose ne va pas */
    if(!start || strlen(start) <= 2 || start[0] != open_)
        return NULL;

    while((segEnd = strchr(segEnd+1, close_)))
    {
        /* Si on ne trouve plus d'ouverture ou si elle est après la fermeture */
        if( !(segTmp = strchr(segTmp+1, open_)) || (segEnd < segTmp) )
        {
            return segEnd;
        }
    }

    LOG(ERROR_LOG, "Une expression ( %c ) n'est pas fermée : %s", open_, start);
    PRINTFNV(ERROR_LOG, "Une expression ( %c ) n'est pas fermee : %s", open_, start);
    pthread_exit((void*)ERROR_COMPIL);

    return NULL;
}

/* ====================================================
                       removeComments
    --------------------------------------------------
    Arg : str  : code à traiter
    --------------------------------------------------
    Def : Permet de retirer les commentaires multi et
          simple ligne d'une chaine
   ==================================================== */
char* removeComments(char* str)
{
    char* tmp = NULL;

    while((tmp = getSegment("#--", "--#", str)))
        free(tmp);

    while((tmp = getSegment("#", "\n", str)))
        free(tmp);

    return str;
}

/* ====================================================
                        strLower
    --------------------------------------------------
    Arg : str  : code à traiter
    --------------------------------------------------
    Def : Permet de passer en minuscule tout le code
          ( code non sensible à la casse )
   ==================================================== */
char* strLower(char* str)
{
    bool exept = false;
    char* tmp = str;

    while(*str != '\0')
    {
        if(*str == '\'' || *str == '"')
        {
            if(exept)
            {
                if(*(str-1) != '\\')
                    exept = false;
            }
            else
                exept = true;
        }

        if(!exept && (*str >= 'A' && *str <= 'Z'))
        {
            *str += 32;
        }

        str++;
    }
    return tmp;
}

/* ====================================================
                        strInsert
    --------------------------------------------------
    Arg : str : code où insérer
          whereInsert : pointeur dans str où insérer
          n : décalage de l'insertion
          toInsert : code à insérer
    --------------------------------------------------
    Def : Permet de remplacer facilement une portion
          d'une chaine de caractère
   ==================================================== */
char* strInsert(char* str, char* whereInsert, int n, char* toInsert)
{
    char* final = NULL;

    if(!str)
        return NULL;
    else if(!toInsert || !whereInsert)
        return str;

    final = malloc(sizeof(char)*(strlen(str)+strlen(toInsert)-n+2));

    if (str == whereInsert)
    {
        /* On recopi la chaine à insérer */
        strncpy(final, toInsert, strlen(toInsert));

        /* On recopi le reste en prenant en compte le décalage */
        strcpy(final+strlen(toInsert), str+n);

        final[strlen(toInsert)+strlen(str)-n] = '\0';
    }
    else
    {
        /* On recopi le premier morceau */
        strncpy(final, str, whereInsert-str);

        /* On recopi la chaine à insérer */
        strncpy(final+(whereInsert-str)-1, toInsert, strlen(toInsert));

        /* On recopi le reste en prenant en compte le décalage */
        strcpy(final+(whereInsert-str-1)+strlen(toInsert), str+(whereInsert-str-1)+n);
    }

    free(str);

    return final;
}

/* ====================================================
                        makeLink
    --------------------------------------------------
    /!\ : A refaire completement
   ==================================================== */
char* makeLink(char* code)
{
    char* tmp = NULL, *f_content = NULL;
    FILE* f = NULL;


    if(!(tmp = getSegment(">include(", ")", code)))
        return code;

    do
    {
        /* On ouvre le fichier */
        if ((f = fopen(tmp,  "rb")) == NULL ) /* s'il est spécifié mais incorrect*/
        {
            PRINTFNV(ERROR_LOG, "Impossible d'ouvrir le fichier \"%s\", appuyez sur une touche pour quitter...\n", tmp);
            LOG(ERROR_LOG, "Impossible d'ouvrir le fichier \"%s\" ", tmp);
            pthread_exit((void*)ERROR_COMPIL);
        }

        PRINTFNV(-1, "...fichier \"%s\"", tmp);
        LOG(-1, "...fichier \"%s\"", tmp);

        /* On libère le nom du fichier */
        free(tmp);
        tmp = NULL;

        /* On récupère le contenu */
        f_content = getFileContent(f);

        /* On ferme le fichier */
        fclose(f);

        /* On l'insère dans le code */
        tmp = strInsert(code, code, 0, f_content);

        free(f_content);
        f_content = NULL;

        /* On récupère le code */
        code = tmp;

        /* Et on recommence */
        tmp = getSegment(">include(", ")", code);

    }while(tmp);

    return code;
}

/* ====================================================
                        isValidName
    --------------------------------------------------
    Arg : name : Nom à tester
    --------------------------------------------------
    Def : Permet de tester la validité d'un mot clef ou
          d'un nom (1 si valide, 0 sinon)
   ==================================================== */
char isValidName(char* name)
{
    if(!name)
        return 1;

    while(*name != '\0')
    {
        if(!((*name >= 'a' && *name <= 'z') || (*name == '_')))
        {
            return 0;
        }
        name++;
    }
    return 1;
}
