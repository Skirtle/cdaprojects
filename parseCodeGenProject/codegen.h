#ifndef __CODEGEN_H
#define __CODEGEN_H

#include "scanner.h"
#include "parser.h"

typedef struct
{
    int opcode;
    int r;
    int l;
    int m;
} instruction;

instruction* generate_code(symbol *table, lexeme *list, int *set_size, int flag, int tableSize);

#endif