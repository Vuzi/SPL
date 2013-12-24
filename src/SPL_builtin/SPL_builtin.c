
#include "SPL_builtin.h"

char* clean (char *str)
{
    char *p = strchr(str, '\n');

    if (p)
        *p = '\0';
    else
        purger();

    return str;
}

void purger(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/*=======================================
              Fonction out
  =======================================
        Fonction bluit-in du SPL

  Permet d'afficher à la suites les
  variables indiquées
  =======================================*/
void out(int nb,...)
{
    int i = 0;
    va_list ap;
    TYPE_VAR type;

    va_start(ap, nb);

    for( i=0; i < nb; i += 2)
    {
        type = va_arg(ap, TYPE_VAR);

        switch(type)
        {
            case TYPE_INT :
                printf("%d", va_arg(ap,int));
                break;

            case TYPE_CHAR :
                printf("%c", (char)va_arg(ap,int));
                break;

            case TYPE_FLOAT :
                printf("%f", (float)va_arg(ap,double));
                break;

            case TYPE_STRING :
                printf("%s", va_arg(ap,char*));
                break;

            default :
                break;
        }
    }
    va_end(ap);
}

/*=======================================
              Fonction in
  =======================================
        Fonction bluit-in du SPL

  Permet de récupérer les valeurs des
  variables
  =======================================*/
void in(int nb,...)
{
    int i = 0;
    va_list ap;
    TYPE_VAR type;

    char buff[256];

    int buff_int = 0;
    char buff_char = 0;
    char* buff_str = malloc(sizeof(char)*256);
    float buff_float = 0;

    va_start(ap, nb);

    for( i=0; i < nb; i += 2)
    {
        type = va_arg(ap, TYPE_VAR);

        switch(type)
        {
            case TYPE_INT :
                while(!sscanf(clean(fgets(buff, 255, stdin)), "%d", &buff_int))
                    printf("Erreur de saisie. Le programme attend un entier\n");
                *(va_arg(ap,int*)) = buff_int;
                break;  

            case TYPE_CHAR :
                while(!sscanf(clean(fgets(buff, 255, stdin)), "%c", &buff_char) || buff_char == '\0')
                    printf("Erreur de saisie. Le programme attend un caractere\n");
                *(va_arg(ap,char*)) = buff_char;
                break;  

            case TYPE_FLOAT :
                while(!sscanf(clean(fgets(buff, 255, stdin)), "%f", &buff_float))
                    printf("Erreur de saisie. Le programme attend un flottant\n");
                *(va_arg(ap,float*)) = buff_float;
                break;  

            case TYPE_STRING :
                *(va_arg(ap,char**)) = clean(fgets(buff_str, 255, stdin));
                break;

            default :
                break;
        }
    }
    va_end(ap);
}
