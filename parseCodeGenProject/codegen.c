#include <stdio.h>
#include <stdlib.h>
#include "scanner.h"
#include "parser.h"
#include "codegen.h"

int lindex = 0; //list index
int tindex = 0; // table index

instruction *set;
void block_gen(symbol *table, lexeme *list, int *set_size, int table_size);
void statement_gen(symbol *table, lexeme *list, int *set_size, int table_size);
void condition_gen(symbol *table, lexeme *list, int *set_size, int table_size);
void expression_gen(symbol *table, lexeme *list, int *set_size, int reg, int table_size);
void term_gen(symbol *table, lexeme *list, int *set_size, int reg, int table_size);
void factor_gen(symbol *table, lexeme *list, int *set_size, int reg, int table_size);

// debug functions
void print_set(int *set_size) {
    int i;
    for (i = 0; i < (*set_size); i++) {
        printf("%d %d %d %d\n", set[i].opcode, set[i].r, set[i].l, set[i].m);
    }
}

instruction* generate_code(symbol *table, lexeme *list, int *set_size, int flag, int table_size) {
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
    block_gen(table, list, set_size, table_size);

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

void block_gen(symbol *table, lexeme *list, int *set_size, int table_size) {
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
    statement_gen(table, list, set_size, table_size);

}

void statement_gen(symbol *table, lexeme *list, int *set_size, int table_size) {
    // ident
    if (list[lindex].type == 2) {
        // save to symbol table index
        int i;
        for (i = 0; i < table_size; i++) {
            if (strcmp(list[lindex].name, table[i].name) == 0) {
                tindex = i;
                break;
            }
        }

        // token + 2
        lindex += 2;
        expression_gen(table, list, set_size, 0, table_size);

        // emit
        instruction temp;
        temp.opcode = 4;
        temp.r = 0;
        temp.l = 0;
        temp.m = table[tindex].addr;

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

    }
    
    // begin
    if (list[lindex].type == 21) {
        lindex += 1;
        statement_gen(table, list, set_size, table_size);
        while (list[lindex].type == 17) {
            lindex += 1;
            statement_gen(table, list, set_size, table_size);
        }
        lindex += 1;
    } 

    // if
    if (list[lindex].type == 23) {
        lindex++;
        condition_gen(table, list, set_size, table_size);
        int saved_index = *set_size;

        // emit JPC
        instruction temp;
        temp.opcode = 8;
        temp.r = 0;
        temp.l = 0;
        temp.m = 0;

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

        lindex++;

        statement_gen(table, list, set_size, table_size);

        set[saved_index].m = *set_size;
    }

    // while
    if (list[lindex].type == 25) {
        // token + 1
        lindex++;

        // save code index for cond.
        int condIndex = *set_size;
        condition_gen(table, list, set_size, table_size);

        // token + 1
        lindex++;

        // save code index for jump
        int jumpIndex = *set_size;

        // emit
        instruction temp;
        temp.opcode = 8;
        temp.r = 0;
        temp.l = 0;
        temp.m = 0;

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

        statement_gen(table, list, set_size, table_size);

        // emit
        temp.opcode = 7;
        temp.r = 0;
        temp.l = 0;
        temp.m = condIndex;

        // Adding temp to the instruction set
        set[*set_size] = temp;
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

        set[jumpIndex].m = *set_size;

    }

    // read
    if (list[lindex].type == 32) {
        // increase token
        lindex++;

        // save to symbol table index
        int i;
        for (i = 0; i < table_size; i++) {
            if (strcmp(list[lindex].name, table[i].name) == 0) {
                tindex = i;
                break;
            }
        }

        // increase token
        lindex++;

        // emit READ
        instruction temp;
        temp.opcode = 9;
        temp.r = 0;
        temp.l = 0;
        temp.m = 2;

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

        // emit STO
        temp.opcode = 7;
        temp.r = 0;
        temp.l = 0;
        temp.m = table[tindex].addr;

        // Adding temp to the instruction set
        set[*set_size] = temp;
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
    }

    if (list[lindex].type == 31) {
        // token + 1
        lindex++;

        // save the symbol table index
        int i;
        for (i = 0; i < table_size; i++) {
            if (strcmp(list[lindex].name, table[i].name) == 0) {
                tindex = i;
                break;
            }
        }

        // if var
        if (table[tindex].kind == 2) {
            // emit lod
            instruction temp;
            temp.opcode = 3;
            temp.r = 0;
            temp.l = 0;
            temp.m = table[tindex].addr;

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

            // emit write
            temp.opcode = 9;
            temp.r = 0;
            temp.l = 0;
            temp.m = 1;

            // Adding temp to the instruction set
            set[*set_size] = temp;
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
        }

        if (table[tindex].kind == 1) {
            // emit lit
            instruction temp;
            temp.opcode = 1;
            temp.r = 0;
            temp.l = 0;
            temp.m = table[tindex].addr;

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

            // emit write
            temp.opcode = 9;
            temp.r = 0;
            temp.l = 0;
            temp.m = 1;

            // Adding temp to the instruction set
            set[*set_size] = temp;
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
        }

        // get next token
        lindex++;

    }
}

void condition_gen(symbol *table, lexeme *list, int *set_size, int table_size) {
    if (list[lindex].type == 8) {
        lindex++;

        expression_gen(table, list, set_size, 0, table_size);

        // emit ODD
        instruction temp;
        temp.opcode = 15;
        temp.r = 0;
        temp.l = 0;
        temp.m = 0;

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
    }

    else {
        expression_gen(table, list, set_size, 0, table_size);
        
        if (list[lindex].type == 9) { // equal
            lindex++;
            expression_gen(table, list, set_size, 1, table_size);

            // emit EQL
            instruction temp;
            temp.opcode = 17;
            temp.r = 0;
            temp.l = 0;
            temp.m = 1;

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
        }

        if (list[lindex].type == 10) { // not equal
            lindex++;
            expression_gen(table, list, set_size, 1, table_size);

            // emit NEQ
            instruction temp;
            temp.opcode = 18;
            temp.r = 0;
            temp.l = 0;
            temp.m = 1;

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
        }

        if (list[lindex].type == 11) { // less than
            lindex++;
            expression_gen(table, list, set_size, 1, table_size);

            // emit LSS
            instruction temp;
            temp.opcode = 19;
            temp.r = 0;
            temp.l = 0;
            temp.m = 1;

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
        }

        if (list[lindex].type == 12) { // less than or equal to
            lindex++;
            expression_gen(table, list, set_size, 1, table_size);

            // emit LEQ
            instruction temp;
            temp.opcode = 20;
            temp.r = 0;
            temp.l = 0;
            temp.m = 1;

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
        }

        if (list[lindex].type == 13) { // greater than
            lindex++;
            expression_gen(table, list, set_size, 1, table_size);

            // emit GTR
            instruction temp;
            temp.opcode = 21;
            temp.r = 0;
            temp.l = 0;
            temp.m = 1;

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
        }

        if (list[lindex].type == 14) { // greater than or equal to
            lindex++;
            expression_gen(table, list, set_size, 1, table_size);

            // emit GEQ
            instruction temp;
            temp.opcode = 22;
            temp.r = 0;
            temp.l = 0;
            temp.m = 1;

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
        }
    }
}

void expression_gen(symbol *table, lexeme *list, int *set_size, int reg, int table_size) {
    // token == +
    if (list[lindex].type == 4) lindex++;
    if (list[lindex].type == 5) {
        lindex++;
        term_gen(table, list, set_size, reg, table_size);

        // emit neg
        instruction temp;
        temp.opcode = 10;
        temp.r = reg;
        temp.l = 0;
        temp.m = 0;

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
        // loop while + or -
        while (list[lindex].type == 4 || list[lindex].type == 5) {
            // if +
            if (list[lindex].type == 4) {
                lindex++;
                term_gen(table, list, set_size, reg + 1, table_size);
                // emit ADD
                instruction temp;
                temp.opcode = 11;
                temp.r = reg;
                temp.l = reg;
                temp.m = reg + 1;

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
            }
            // if -
            if (list[lindex].type == 5) {
                lindex++;
                term_gen(table, list, set_size, reg + 1, table_size);

                // emit SUB
                instruction temp;
                temp.opcode = 12;
                temp.r = reg;
                temp.l = reg;
                temp.m = reg + 1;

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
            }
        }
        return;
    }
    term_gen(table, list, set_size, reg, table_size);

    while (list[lindex].type == 4 || list[lindex].type == 5) {
        // if +
        if (list[lindex].type == 4) {
            lindex++;
            term_gen(table, list, set_size, reg + 1, table_size);
            // emit ADD
            instruction temp;
            temp.opcode = 11;
            temp.r = reg;
            temp.l = reg;
            temp.m = reg + 1;

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
        }
        // if -
        if (list[lindex].type == 5) {
            lindex++;
            term_gen(table, list, set_size, reg + 1, table_size);

            // emit SUB
            instruction temp;
            temp.opcode = 12;
            temp.r = reg;
            temp.l = reg;
            temp.m = reg + 1;

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
        }
    }
}

void term_gen(symbol *table, lexeme *list, int *set_size, int reg, int table_size) {
    factor_gen(table, list, set_size, reg, table_size);

    while (list[lindex].type == 6 || list[lindex].type == 7) {
        if (list[lindex].type = 6) { // multiplication
            lindex++;

            factor_gen(table, list, set_size, reg + 1, table_size);

            // emit MUL
            instruction temp;
            temp.opcode = 13;
            temp.r = reg;
            temp.l = reg;
            temp.m = reg + 1;

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
        }

        if (list[lindex].type = 7) { // Division 
            lindex++;

            factor_gen(table, list, set_size, reg + 1, table_size);

            // emit DIV
            instruction temp;
            temp.opcode = 14;
            temp.r = reg;
            temp.l = reg;
            temp.m = reg + 1;

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
        }
    }
}

void factor_gen(symbol *table, lexeme *list, int *set_size, int reg, int table_size) {
    // check if token is ident
    if (list[lindex].type == 2) {
        // save table index
        // save to symbol table index
        int i;
        for (i = 0; i < table_size; i++) {
            if (strcmp(list[lindex].name, table[i].name) == 0) {
                tindex = i;
                break;
            }
        }

        if (list[lindex].type == 28) {
            // emit LIT
            instruction temp;
            temp.opcode = 1;
            temp.r = reg;
            temp.l = 0;
            temp.m = table[tindex].val;

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
        }

        // if var
        if (list[lindex].type == 29) {
            // emit LIT
            instruction temp;
            temp.opcode = 3;
            temp.r = reg;
            temp.l = 0;
            temp.m = table[tindex].addr;

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
        }
        // next token
        lindex++;
    }

    // else if token is number
    else if (list[lindex].type == 3) {
        // emit LIT
        instruction temp;
        temp.opcode = 1;
        temp.r = reg;
        temp.l = 0;
        temp.m = list[lindex].value;

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
    }

    // else 
    else {
        // next token
        lindex++;
        expression_gen(table, list, set_size, reg, table_size);
        // next token
        lindex++;
    }
}