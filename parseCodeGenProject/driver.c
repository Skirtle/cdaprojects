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


int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Error: No file to compile");
        return 1;
    }
    // Flag declarations for all the command line arguments
    int aFlag = 0, vFlag = 0, lFlag = 0;
    if (argc == 5) {
        aFlag = 1;
        vFlag = 1;
        lFlag = 1;
    }
    else if (argc != 2) {
        if (argv[2][1] == 'v')
            vFlag = 1;
        else if (argv[2][1] == 'a')
            aFlag = 1;
        else
            lFlag = 1;

        if (argc == 4) {
            if (argv[3][1] == 'v')
                vFlag = 1;
            else if (argv[3][1] == 'a')
                aFlag = 1;
            else
                lFlag = 1;
        }
    }
    FILE *ipf = fopen(argv[1], "r");

    // Grabbing the input file and storing it inside a char array.
    char *inputfile = (char*) malloc(500 * sizeof(char));
    char c = fgetc(ipf);
    int i = 0;
    while(1) {
        inputfile[i++] = c;
        c = fgetc(ipf);
        if (c == EOF)
            break;
    }
    inputfile[i] == '\0';

    int no_errors = 1, l_size = 0, t_size = 0, i_size = 0;
    // Printing the source program.
    printf("\nSource Program:\n%s\n", inputfile);
    // Set up the lexeme list
    lexeme *list = scan(inputfile, lFlag, &l_size);
    // Set up the symbol table
    symbol *table = parse(list, &no_errors, &t_size);
    if(no_errors)
        printf("\nNo errors, program is syntactically correct.\n");
    else {
        free(table);
        free(list);
        free(inputfile);
        return 2;
    }
    // Generating the instruction set
    instruction *set = generate_code(table, list, &i_size, aFlag, t_size);


    free(table);
    free(list);
    free(inputfile);
    return 0;
}