#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "scanner.h"

symbol *table;
int index = 0;

void const_dec(lexeme *list, int *no_errors, int *table_size) {
    if (list[index].type == 28) {
        do {
            // get next token
            index++;

            // check if token is identifier token
            if (list[index].type != 2) {
                printf("Error number 4: const, var, procedure must be followed by identifier");
                *no_errors = 0;
                return;
            }

            // save indentifier name
            symbol temp;
            int i;
            for (i = 0; i < 13; i++) temp.name[i] = list[index].name[i];

            // check if indentier is already in symbol table
            for (i = 0; i < *table_size; i++) {
                if (strcmp(table[i].name, temp.name) == 0) {
                    printf("Error number 26: indentifier already used");
                    *no_errors = 0;
                    return;
                }
            }

            // get next token
            index++;

            // check if token is not =
            if (list[index].type != 9) {
                printf("Error number 3: indentifier must be followed by '='");
                *no_errors = 0;
                return;
            }

            // get next token
            index++;

            // check if token is not a number
            if (list[index].type != 3) {
                printf("Error number 2: = must be followed by a number");
                *no_errors = 0;
                return;
            }

            // Adding to the symbol table.
            temp.kind = 1;
            temp.val = list[index].value;
            temp.level = 0;
            temp.addr = 0;
            temp.mark = 0;

            table[*table_size] = temp;
            (*table_size)++;

            // Get next token
            index++;
        } while(list[index].type == 17);

        // make sure next token is ;
        if (list[index].type != 18); {
            printf("Error number 5: Semicolon or comma missing");
            *no_errors = 0;
            return;
        }

        // get next Token
        index++;
    }
}

void var_dec(lexeme *list, int *no_errors, int *table_size) {
    if (list[index].type == 29) {
        int num_vars = 0;
        do {
            num_vars++;

            // get next token
            index++;

            // check if token is identifier
            if (list[index].type != 2) {
                printf("Error number 4: const, var, procedure must be followed by identifier");
                *no_errors = 0;
                return;
            }

            // save indentifier name
            symbol temp;
            int i;
            for (i = 0; i < 13; i++) temp.name[i] = list[index].name[i];

            // check if indentier is already in symbol table
            for (i = 0; i < *table_size; i++) {
                if (strcmp(table[i].name, temp.name) == 0) {
                    printf("Error number 26: indentifier already used");
                    *no_errors = 0;
                    return;
                }
            }

            // adding to symbol table
            temp.kind = 2;
            temp.val = 0;
            temp.level = 0;
            temp.addr = num_vars + 2;
            temp.mark = 0;

            table[*table_size] = temp;
            (*table_size)++;

            // get next token
            index++;
        } while(list[index].type == 17);

        // make sure next token is semicolon
        if (list[index].type != 18); {
            printf("Error number 5: Semicolon or comma missing");
            *no_errors = 0;
            return;
        }

        // get next token
        index++;
    }
}

void statement(lexeme *list, int *no_errors, int *table_size) {

}

void block(lexeme *list, int *no_errors, int *table_size) {
    const_dec(list, no_errors, table_size);
    if (*no_errors == 0) return;
    var_dec(list, no_errors, table_size);
    if (*no_errors == 0) return;
    statement(list, no_errors, table_size);
}

symbol* parse(lexeme *list, int *no_errors, int *table_size) {
    table = (symbol*) calloc(1, sizeof(symbol) + 12);

    block(list, no_errors, table_size);


    return table;
}



void condition(lexeme *list, int *no_errors, int *table_size) {

}

void expression(lexeme *list, int *no_errors, int *table_size) {

}

void term(lexeme *list, int *no_errors, int *table_size) {

}

void factor(lexeme *list, int *no_errors, int *table_size) {

}



