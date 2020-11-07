#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "scanner.h"

symbol *table;
int list_index = 0;

void const_dec(lexeme *list, int *no_errors, int *table_size) {
    if (list[list_index].type == 28) {
        do {
            // get next token
            list_index++;

            // check if token is identifier token
            if (list[list_index].type != 2) {
                printf("Error number 4: const, var, procedure must be followed by identifier");
                *no_errors = 0;
                return;
            }

            // save indentifier name
            symbol temp;
            int i;
            for (i = 0; i < 13; i++) temp.name[i] = list[list_index].name[i];

            // check if indentier is already in symbol table
            for (i = 0; i < *table_size; i++) {
                if (strcmp(table[i].name, temp.name) == 0) {
                    printf("Error number 26: indentifier already used");
                    *no_errors = 0;
                    return;
                }
            }

            // get next token
            list_index++;

            // check if token is not =
            if (list[list_index].type != 9) {
                printf("Error number 3: indentifier must be followed by '='");
                *no_errors = 0;
                return;
            }

            // get next token
            list_index++;

            // check if token is not a number
            if (list[list_index].type != 3) {
                printf("Error number 2: = must be followed by a number");
                *no_errors = 0;
                return;
            }

            // Adding to the symbol table.
            temp.kind = 1;
            temp.val = list[list_index].value;
            temp.level = 0;
            temp.addr = 0;
            temp.mark = 0;

            table[*table_size] = temp;
            (*table_size)++;

            // Get next token
            list_index++;
        } while(list[list_index].type == 17);

        // make sure next token is ;
        if (list[list_index].type != 18); {
            printf("Error number 5: Semicolon or comma missing");
            *no_errors = 0;
            return;
        }

        // get next Token
        list_index++;
    }
}

void var_dec(lexeme *list, int *no_errors, int *table_size) {
    if (list[list_index].type == 29) {
        int num_vars = 0;
        do {
            num_vars++;

            // get next token
            list_index++;

            // check if token is identifier
            if (list[list_index].type != 2) {
                printf("Error number 4: const, var, procedure must be followed by identifier");
                *no_errors = 0;
                return;
            }

            // save indentifier name
            symbol temp;
            int i;
            for (i = 0; i < 13; i++) temp.name[i] = list[list_index].name[i];

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
            list_index++;
        } while(list[list_index].type == 17);

        // make sure next token is semicolon
        if (list[list_index].type != 18); {
            printf("Error number 5: Semicolon or comma missing");
            *no_errors = 0;
            return;
        }

        // get next token
        list_index++;
    }
}

void factor(lexeme *list, int *no_errors, int *table_size) {

}

void term(lexeme *list, int *no_errors, int *table_size) {

}

void expression(lexeme *list, int *no_errors, int *table_size) {

}

void condition(lexeme *list, int *no_errors, int *table_size) {

}

void statement(lexeme *list, int *no_errors, int *table_size) {
    // check if identifier
    if (list[list_index].type == 2) {
        // check if indent even in symbol table
        int i, notInTable = 1;
        for (i = 0; i < *table_size; i++) {
            if (strcmp(table[i].name, list[list_index].name) == 0) {
                notInTable = 0;
                break;
            }
        }
        if (notInTable) {
            printf("Error number 11: undeclared indentifier");
            *no_errors = 0;
            return;
        }

        // get next token
        list_index++;

        // see if indentifier is of variable token
        int correspondingVar = -1;
        for (i = 0; i < *table_size; i++) {
            if (strcmp(table[i].name, list[list_index].name) == 0) {
                correspondingVar = i;
                break;
            }
        }
        if (table[correspondingVar].kind != 2) {
            printf("Error number 12: assignment to constant or procedure is not allowed");
            *no_errors = 0;
            return;
        }

        // get next token
        list_index++;

        // check if token is :=
        if (list[list_index].type != 20) {
            printf("Error number 13: assignment operator expected");
            *no_errors = 0;
            return;
        }

        // get next token
        list_index++;

        expression(list, no_errors, table_size);
        return;
    }

    // check if begin
    if (list[list_index].type == 21) {
        // get next token
        list_index++;

        // recurse
        statement(list, no_errors, table_size);

        // another recurse
        while (list[list_index].type == 18) {
            // get next token
            list_index++;
            statement(list, no_errors, table_size);
        }

        // check if end
        if (list[list_index].type != 22) {
            printf("Error number 27: end expected");
            *no_errors = 0;
            return;
        }

        // get next token
        list_index++;

        // return
        return;
    }

    // check if if
    if (list[list_index].type == 23) {
        // get next token
        list_index++;

        // condition
        condition(list, no_errors, table_size);

        // check if token is then
        if (list[list_index].type != 24) {
            printf("Error number 16: then expected");
            *no_errors = 0;
            return;
        }

        // get next token
        list_index++;

        // statement
        statement(list, no_errors, table_size);

        // return
        return;
    }

    // check if while
    if (list[list_index].type == 25) {
        // get next token
        list_index++;

        // condition
        condition(list, no_errors, table_size);

        // check if token do
        if (list[list_index].type == 26) {
            printf("Error number 18: do expected");
            *no_errors = 0;
            return;
        }

        // get next token
        list_index++;

        // statement
        statement(list, no_errors, table_size);

        return;
    }

    // check if read
    if (list[list_index].type == 32) {
        // get next token
        list_index++;

        // check if identifier
        if (list[list_index].type != 2) {
            printf("Error number 28: indentifier expected");
            *no_errors = 0;
            return;
        }

        // check if identifier not in symbol table
        int i, notInTable = 1;
        for (i = 0; i < *table_size; i++) {
            if (strcmp(list[list_index].name, table[i].name) == 0) {
                break;
            }
        }
        if (notInTable) {
            printf("Error number 11: undeclared indentifier");
            *no_errors = 0;
            return;
        }

        // see if indentifier is of variable token
        int correspondingVar = -1;
        for (i = 0; i < *table_size; i++) {
            if (strcmp(table[i].name, list[list_index].name) == 0) {
                correspondingVar = i;
                break;
            }
        }
        if (table[correspondingVar].kind != 2) {
            printf("Error number 12: assignment to constant or procedure is not allowed");
            *no_errors = 0;
            return;
        }

        // get next token
        list_index++;

        // return
        return;
    }

    // check if write
    if (list[list_index].type == 31) {
        // get next token
        list_index++;

        // check if identifier
        if (list[list_index].type != 2) {
            printf("Error number 28: indentifier expected");
            *no_errors = 0;
            return;
        }

        // check if identifier not in symbol table
        int i, notInTable = 1;
        for (i = 0; i < *table_size; i++) {
            if (strcmp(list[list_index].name, table[i].name) == 0) {
                break;
            }
        }
        if (notInTable) {
            printf("Error number 11: undeclared indentifier");
            *no_errors = 0;
            return;
        }

        // get next token
        list_index++;

        // return
        return;
    }
    // return
    return;
}

void block(lexeme *list, int *no_errors, int *table_size) {

    // do const check
    const_dec(list, no_errors, table_size);
    if (*no_errors == 0) return;
    // do var check
    var_dec(list, no_errors, table_size);
    if (*no_errors == 0) return;
    // do statement check
    statement(list, no_errors, table_size);
}

symbol* parse(lexeme *list, int *no_errors, int *table_size) {
    table = (symbol*) calloc(1, sizeof(symbol) + 12);

    block(list, no_errors, table_size);


    return table;
}

