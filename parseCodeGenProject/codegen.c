#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "parser.h"
#include "codegen.h"

int lindex = 0; //list index
int tindex = 0; // table index

instruction *set;
void block_gen(symbol *table, lexeme *list, int *set_size);
void statement_gen(symbol *table, lexeme *list, int *set_size);
void condition_gen(symbol *table, lexeme *list, int *set_size);
void expression_gen(symbol *table, lexeme *list, int *set_size, int reg);
void term_gen(symbol *table, lexeme *list, int *set_size, int reg);
void factor_gen(symbol *table, lexeme *list, int *set_size, int reg);

// debug functions
void print_set(int *set_size) {
    int i;
    for (i = 0; i < (*set_size); i++) {
        printf("%d %d %d %d\n", set[i].opcode, set[i].r, set[i].l, set[i].m);
    }
}

instruction* generate_code(symbol *table, lexeme *list, int *set_size, int flag) {
    set = (instruction*) calloc(1, sizeof(instruction) + 13);

    // Program begins with a JMP for main
    instruction main;
    main.opcode = 7;
    main.r = 0;
    main.l = 0;
    main.m = 0;

    // Adding JMP to the instruction set
    set[*set_size] = main;
    (*set_size)++;

    // EXPAND the instruction set
    instruction *new_set = (instruction*) realloc(set, ((*set_size) + 1) * sizeof(instruction));
    if (new_set == NULL) {
        // realloc failed, try again
        while (new_set == NULL) {
            new_set = (instruction*) realloc(set, ((*set_size) + 1) * sizeof(instruction));
        }
    }
    set = new_set;

    // Begin codegen for the rest of the program.
    block_gen(table, list, set_size);

    // Program ends with halt
    instruction halt;
    halt.opcode = 9;
    halt.r = 0;
    halt.l = 0;
    halt.m = 3;

    // Adding halt to the instruction set
    set[*set_size] = halt;
    (*set_size)++;
            
    // EXPAND the instruction set
    new_set = (instruction*) realloc(set, ((*set_size) + 1) * sizeof(instruction));
    if (new_set == NULL) {
        // realloc failed, try again
        while (new_set == NULL) {
            new_set = (instruction*) realloc(set, ((*set_size) + 1) * sizeof(instruction));
        }
    }
    set = new_set;

    if (flag)
        print_set(set_size);

    return set;
}

void block_gen(symbol *table, lexeme *list, int *set_size) {
    int numVars = 0;
    // do stuff for consts
    if (list[lindex].type == 28) {
        do {
            lindex += 4;
        } while(list[lindex].type == 17);
        lindex++;
    }
    // do stuff for var
    if (list[lindex].type == 29) {
        do {
            numVars++;
            lindex += 2;
        } while(list[lindex].type == 17);
        lindex++;
    }

    // emit
    instruction temp;
    temp.opcode = 6;
    temp.r = 0;
    temp.l = 0;
    temp.m = 3 + numVars;

    // Adding temp to the instruction set
    set[*set_size] = temp;
    (*set_size)++;

    // EXPAND the instruction set
    instruction *new_set = (instruction*) realloc(set, ((*set_size) + 1) * sizeof(instruction));
    if (new_set == NULL) {
        // realloc failed, try again
        while (new_set == NULL) {
            new_set = (instruction*) realloc(set, ((*set_size) + 1) * sizeof(instruction));
        }
    }
    set = new_set;
    statement_gen(table, list, set_size);

}

void statement_gen(symbol *table, lexeme *list, int *set_size) {
    
}

void condition_gen(symbol *table, lexeme *list, int *set_size) {

}

void expression_gen(symbol *table, lexeme *list, int *set_size, int reg) {

}

void term_gen(symbol *table, lexeme *list, int *set_size, int reg) {

}

void factor_gen(symbol *table, lexeme *list, int *set_size, int reg) {
    
}