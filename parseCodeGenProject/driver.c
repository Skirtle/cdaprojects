/*
Team: Dalton Kajander, Shawn Guydeene
Assignment: Parse-Code Gen
Due date: 11/8/2020
COP3402-20Fall 0001
*/

#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "parser.h"
#include "codegen.h"
#include "vm.h"


int main(int argc, char **argv) {
    if (argc < 2)
        printf("error : please include the file name");
    
    FILE *ipf = fopen(argv[1], "r");

    char *inputfile = malloc(500 * sizeof(char));
    char c = fgetc(ipf);
    int i = 0;
    
    while(1) {
        inputfile[i++] = c;
        c = fgetc(ipf);
        if (c == EOF)
            break;
    }

    inputfile[i] == '\0';

    printf("%s\n", inputfile);

    return 0;
}