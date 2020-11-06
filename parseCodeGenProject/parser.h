#ifndef __PARSER_H
#define __PARSER_H

#include "scanner.h"

typedef struct {
    int kind;
    char* name;
    int val;
    int level;
    int addr;
    int mark;
} symbol;

symbol* parse(lexeme *list, int *no_errors);

#endif
