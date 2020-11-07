#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "scanner.h"

symbol *table;
int list_index = 0;
// prototypes
void const_dec(lexeme *list, int *no_errors, int *table_size);
void var_dec(lexeme *list, int *no_errors, int *table_size);
void parse_expression(lexeme *list, int *no_errors, int *table_size);
void parse_factor(lexeme *list, int *no_errors, int *table_size);
void parse_term(lexeme *list, int *no_errors, int *table_size);
void parse_condition(lexeme *list, int *no_errors, int *table_size);
void parse_statement(lexeme *list, int *no_errors, int *table_size);
void parse_block(lexeme *list, int *no_errors, int *table_size);
symbol* parse(lexeme *list, int *no_errors, int *table_size);


symbol* parse(lexeme *list, int *no_errors, int *table_size) {
    table = (symbol*) calloc(1, sizeof(symbol) + 13);

    parse_block(list, no_errors, table_size);

    return table;
}

void parse_block(lexeme *list, int *no_errors, int *table_size) {
    
    // do const check
    const_dec(list, no_errors, table_size);
    if (*no_errors == 0) return;
    // do var check
    var_dec(list, no_errors, table_size);
    if (*no_errors == 0) return;
    // do statement check
    parse_statement(list, no_errors, table_size);
}

void const_dec(lexeme *list, int *no_errors, int *table_size) {
    if (list[list_index].type == 28) {
        do {
            // get next token
            list_index++;

            // make sure token is identifier
            if (list[list_index].type != 2) {
                if (list[list_index].type == -4)
                    printf("\nError number 26: identifier name is too long");
                else
                    printf("\nError number 4: const, var, procedure must be followed by identifier");
                *no_errors = 0;
                return;
            }

            // check if indentifier is already in symbol table
            int i;
            for (i = 0; i < (*table_size); i++) {
                if (strcmp(table[i].name, list[list_index].name) == 0) {
                    printf("\nError number 26: indentifier already used");
                    *no_errors = 0;
                    return;
                }
            }

            // save indentifier name
            symbol temp;
            temp.name = (char*) calloc(13, sizeof(char));
            for (i = 0; i < 13; i++) temp.name[i] = list[list_index].name[i];

            // get the next one
            list_index++;

            // make sure token is equal
            if (list[list_index].type != 9) {
                printf("\nError number 3: indentifier must be followed by '='");
                *no_errors = 0;
                return;
            }

            // get the next one
            list_index++;

            // check if token is not a number
            if (list[list_index].type != 3) {
                if (list[list_index].type == -3)
                    printf("\nError number 25: number is too large");
                else
                    printf("\nError number 2: = must be followed by a number");
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
            
            // EXPAND the symbol table
            symbol *new_table = (symbol*) realloc(table, ((*table_size) + 1) * (sizeof(symbol) + 13));
            if (new_table == NULL) {
                while (new_table == NULL) {
                    new_table = (symbol*) realloc(table, ((*table_size) + 1) * (sizeof(symbol) + 13));
                }
            }
            table = new_table;

            // Get next token
            list_index++;

        } while(list[list_index].type == 17);

        // make sure next token is ;
        if (list[list_index].type != 18) {
            printf("%d", list[list_index].type);
            printf("\nError number 5: Semicolon or comma missing");
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

            // get the next token
            list_index++;

            // make sure token is identifier
            if (list[list_index].type != 2) {
                if (list[list_index].type == -4)
                    printf("\nError number 26: identifier name is too long");
                else
                    printf("\nError number 4: const, var, procedure must be followed by identifier");
                *no_errors = 0;
                return;
            }

            // check if indentier is already in symbol table
            int i;
            for (i = 0; i < (*table_size); i++) {
                if (strcmp(table[i].name, list[list_index].name) == 0) {
                    printf("\nError number 26: indentifier already used");
                    *no_errors = 0;
                    return;
                }
            }

            // Adding to the symbol table.
            symbol temp;
            temp.kind = 2;
            temp.name = (char*) calloc(13, sizeof(char));
            for (i = 0; i < 13; i++) temp.name[i] = list[list_index].name[i];
            temp.val = 0;
            temp.level = 0;
            temp.addr = num_vars + 2;
            temp.mark = 0;

            table[*table_size] = temp;
            (*table_size)++;

            // EXPAND the symbol table
            symbol *new_table = (symbol*) realloc(table, ((*table_size) + 1) * (sizeof(symbol) + 13));
            if (new_table == NULL) {
                while (new_table == NULL) {
                    new_table = (symbol*) realloc(table, ((*table_size) + 1) * (sizeof(symbol) + 13));
                }
            }
            table = new_table;

            // get next token
            list_index++;
        } while(list[list_index].type == 17);

        // make sure next token is semicolon
        if (list[list_index].type != 18) {
            printf("Error number 5: Semicolon or comma missing");
            *no_errors = 0;
            return;
        }

        // get next token
        list_index++;
    }
}

void parse_statement(lexeme *list, int *no_errors, int *table_size) {
    if (list[list_index].type == 2) { // identifiers
        // Make sure indentifier has been declared
        int i, not_in_table = 1, table_index;
        for (i = 0; i < (*table_size); i++) {
            if (strcmp(table[i].name, list[list_index].name) == 0) {
                not_in_table = 0;
                table_index = i;
            }
        }
        if (not_in_table) {
            printf("\nError number 11: undeclared identifier");
            *no_errors = 0;
            return;
        }

        // check to see if this is a var
        if (table[table_index].kind != 2) {
            printf("\nError number 12: assignment to constant or procedure is not allowed");
            *no_errors = 0;
            return;
        }

        // get the next token
        list_index++;

        // check if token is :=
        if (list[list_index].type != 20) {
            printf("\nError number 13: assignment operator expected");
            *no_errors = 0;
            return;
        }

        // get next token
        list_index++;

        parse_expression(list, no_errors, table_size);
        return;
    }

    if (list[list_index].type == 21) { // begin
        // get the next token
        list_index++;

        parse_statement(list, no_errors, table_size);
        if (!(*no_errors)) return;

        while (list[list_index].type == 18) {
            // get next token
            list_index++;

            parse_statement(list, no_errors, table_size);
            if (!(*no_errors)) return;
        }

        if (list[list_index].type != 22) {
            printf("\nError number 27: end expected");
            *no_errors = 0;
            return;
        }

        // get next token
        list_index++;
        return;
    }

    if (list[list_index].type == 23) { // if
        // get the next
        list_index++;

        parse_condition(list, no_errors, table_size);

        // check if token is then
        if (list[list_index].type != 24) {
            printf("\nError number 16: then expected");
            *no_errors = 0;
            return;
        }

        // get the next
        list_index++;

        parse_statement(list, no_errors, table_size);
        return;
    }

    if (list[list_index].type == 25) { // while
        // get next token
        list_index++;

        // condition
        parse_condition(list, no_errors, table_size);

        // check if token do
        if (list[list_index].type != 26) {
            printf("\nError number 18: do expected");
            *no_errors = 0;
            return;
        }

        // get next token
        list_index++;

        // statement
        parse_statement(list, no_errors, table_size);
        return;
    }

    if (list[list_index].type == 32) { // read
        // get next token
        list_index++;

        // check if identifier
        if (list[list_index].type != 2) {
            printf("\nError number 28: indentifier expected");
            *no_errors = 0;
            return;
        }

        // check if identifier not in symbol table
        int i, notInTable = 1;
        for (i = 0; i < *table_size; i++) {
            if (strcmp(list[list_index].name, table[i].name) == 0) {
                notInTable = 0;
                break;
            }
        }
        if (notInTable) {
            printf("\nError number 11: undeclared indentifier");
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
            printf("\nError number 12: assignment to constant or procedure is not allowed");
            *no_errors = 0;
            return;
        }

        // get next token
        list_index++;

        // return
        return;
    }

    if (list[list_index].type == 31) { // write
        // get next token
        list_index++;

        // check if identifier
        if (list[list_index].type != 2) {
            printf("\nError number 28: indentifier expected");
            *no_errors = 0;
            return;
        }

        // check if identifier not in symbol table
        int i, notInTable = 1;
        for (i = 0; i < *table_size; i++) {
            if (strcmp(list[list_index].name, table[i].name) == 0) {\
                notInTable = 0;
                break;
            }
        }
        if (notInTable) {
            printf("\nError number 11: undeclared indentifier");
            *no_errors = 0;
            return;
        }

        // get next token
        list_index++;

        // return
        return;
    }
    return;
}

void parse_condition(lexeme *list, int *no_errors, int *table_size) {
    // check if odd
    if (list[list_index].type == 8) {
        // get next token
        list_index++;

        // expression
        parse_expression(list, no_errors, table_size);
    }
    // otherwise
    else {
        // expression
        parse_expression(list, no_errors, table_size);

        // check if token is relational operator
        if (list[list_index].type != 9 && list[list_index].type == 10 && list[list_index].type != 11 && list[list_index].type != 12
                && list[list_index].type != 13  && list[list_index].type != 14) {
            printf("\nError number 20: relational operator edpected");
            *no_errors = 0;
            return;
        }

        // get next token
        list_index++;

        // expression
        parse_expression(list, no_errors, table_size);
        return;
    }
}

void parse_expression(lexeme *list, int *no_errors, int *table_size) {
    // check if token add or sub
    if (list[list_index].type == 4 || list[list_index].type == 5) {
        // get next token
        list_index++;
    }
    // term
    parse_term(list, no_errors, table_size);

    // check while token is + or -
    while (list[list_index].type == 4 || list[list_index].type == 5) {
        // get next token
        list_index++;

        // term
        parse_term(list, no_errors, table_size);
    }
}

void parse_term(lexeme *list, int *no_errors, int *table_size) {
    // factor
    parse_factor(list, no_errors, table_size);

    // while token mult or slash
    while (list[list_index].type == 6 || list[list_index].type == 7) {
        // get next token
        list_index++;

        // factor
        parse_factor(list, no_errors, table_size);
    }
}

void parse_factor(lexeme *list, int *no_errors, int *table_size) {
    // check if token is ident
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
            printf("\nError number 11: undeclared indentifier");
            *no_errors = 0;
            return;
        }

        list_index++;
    }

    // check if token is number
    else if (list[list_index].type == 3) {
        // get next token
        list_index++;
    }

    // check if left parenthesis
    else if (list[list_index].type == 15) {
        // get next token
        list_index++;

        // expression
        parse_expression(list, no_errors, table_size);

        // check if right parenthesis
        if (list[list_index].type != 16) {
            printf("\nError number 22: right parenthesis missing");
            *no_errors = 0;
            return;
        }

        // get next token
        list_index++;
    }
    else {
        printf("\nError number 24: an expression cannot begin with this symbol");
        *no_errors = 0;
        return;
    }
}