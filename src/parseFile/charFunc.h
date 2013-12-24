#ifndef _CHARFUNC_H_
#define _CHARFUNC_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "../args/args.h"
#include "../compiler/compiler.h"

/* Prototypes */
int getNbChars(FILE *f);
char* getFileContent(FILE *f);
char* removeChar(char* src, char c);
void cutSegment(const char* start, const char* end, char* code);
char* getSegment(const char* start, const char* end, char* code);
char* getBracket(char* code, char open, char close);
char* getClosingBracket(char* start, char open, char close);
char* removeComments(char* code);
char* strLower(char* str);
char* strInsert(char* str, char* whereInsert, int n, char* toInsert);
char* makeLink(char* code);
char isValidName(char* name);

#endif
