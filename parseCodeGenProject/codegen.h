#ifndef __CODEGEN_H
#define __CODEGEN_H

#include "scanner.h"
#include "parser.h"

typedef struct
{
    int opcode;
    char *op;
    int l;
    int m;
} instruction;


#endif
