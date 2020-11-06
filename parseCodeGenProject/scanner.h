#ifndef __SCANNER_H
#define __SCANNER_H

typedef struct
{
    char *name;
    int value;
    int type;
} lexeme;

lexeme* scan(char *inputfile, int flag, int *list_size);

#endif
