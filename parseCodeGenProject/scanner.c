#include "scanner.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define IDENTIFIER_MAX_LENGTH 11
#define NUMBER_MAX_LENGTH 5

lexeme* scan(char *inputfile, int flag, int *list_size) {
    // Variable declaration
    char c, *word = (char*) calloc(IDENTIFIER_MAX_LENGTH + 2, sizeof(char));
    int letter = 0, i, size = 0, comment = 0, index = 0;
    lexeme *out = (lexeme*) calloc(1, sizeof(lexeme) + 12);

    c = inputfile[index++];

    while (c != '\0') {
        if (isalpha(c)) { // Found a letter, keep looking for more
            word[letter++] = c;
            c = inputfile[index++];

            while (c != '\0') {
                if (isalnum(c)) // another letter/digit was found, add to word
                    word[letter++] = c;
                else { // Found whitespace or an operator
                    lexeme temp;
                    temp.name = (char*) calloc(IDENTIFIER_MAX_LENGTH + 1, sizeof(char));
                    
                    // Determine if the word is reserved, if not, check to see if valid identifier
                    for (i = 0; i < letter; i++) temp.name[i] = word[i];
                    if (letter> IDENTIFIER_MAX_LENGTH) temp.type = -4;
                    else if (strcmp(word, "const") == 0) temp.type = 28;
                    else if (strcmp(word, "var") == 0) temp.type = 29;
                    else if (strcmp(word, "procedure") == 0) temp.type = 30;
                    else if (strcmp(word, "call") == 0) temp.type = 27;
                    else if (strcmp(word, "begin") == 0) temp.type = 21;
                    else if (strcmp(word, "end") == 0) temp.type = 22;
                    else if (strcmp(word, "if") == 0) temp.type = 23;
                    else if (strcmp(word, "then") == 0) temp.type = 24;
                    else if (strcmp(word, "else") == 0) temp.type = 33;
                    else if (strcmp(word, "while") == 0) temp.type = 25;
                    else if (strcmp(word, "do") == 0) temp.type = 26;
                    else if (strcmp(word, "read") == 0) temp.type = 32;
                    else if (strcmp(word, "write") == 0) temp.type = 31;
                    else if (strcmp(word, "odd") == 0) temp.type = 8;
                    else temp.type = 2;

                    // reset the word
                    for (i = 0; i < IDENTIFIER_MAX_LENGTH; i++) word[i] = '\0';
                    letter = 0;

                    // making sure the scanner is out of place.
                    if (c != ' ' && c != '\t' && c != '\n' && c != '\r') index--;

                    // Input the lexeme into the list
                    out[size] = temp;
                    size++;
                    lexeme *new_list = (lexeme*) realloc(out, size * (sizeof(lexeme) + 13));
                    if (new_list == NULL) {
                        while (new_list == NULL) {
                            new_list = (lexeme*) realloc(out, size * (sizeof(lexeme) + 13));
                        }
                    }
                    out = new_list;
                    break;
                }
                c = inputfile[index++];
            }
        }

        else if (isdigit(c)) { // Found a number, keep looking for more
            word[letter++] = c;
            c = inputfile[index++];

            while (c != '\0') {
                if (isalnum(c)) // Checking until whitespace or operator is found
                    word[letter++] = c;
                else { // Found whitespace or an operator
                    lexeme temp;
                    temp.type = 0;

                    // check to make sure it is number, and not an illegal identifier
                    for (i = 0; i < letter; i++) {
                        if (!isdigit(word[i])) {
                            temp.type = -2;
                            break;
                        }
                    }

                    // check if number is correct size:
                    if (letter > NUMBER_MAX_LENGTH && temp.type == 0) temp.type = -3;
                    // It is a numerical constant if no token at this point
                    if (temp.type == 0) temp.type = 3;

                    if (temp.type == 3)
                        temp.value = strtol(word, NULL, 10);

                    // Reset the word array for future use
                    for (i = 0; i < IDENTIFIER_MAX_LENGTH; i++) word[i] = '\0';
                    letter = 0;

                    // making sure the scanner is out of place.
                    if (c != ' ' && c != '\t' && c != '\n' && c != '\r') index--;

                    // Input the lexeme into the list
                    out[size] = temp;
                    size++;
                    lexeme *new_list = (lexeme*) realloc(out, size * (sizeof(lexeme) + 13));
                    if (new_list == NULL) {
                        while (new_list == NULL) {
                            new_list = (lexeme*) realloc(out, size * (sizeof(lexeme) + 13));
                        }
                    }
                    out = new_list;
                    break;
                }
                c = inputfile[index++];
            }
        }

        else if (c != ' ' && c != '\t' && c != '\n' && c != '\r' && !isalnum(c)) {
            if (c == '/') { // possibly a comment
                c = inputfile[index++];
                if (c == '*') {
                    comment  = 1;
                    // Look for end of comment
                    c = inputfile[index++];
                    while (c != '\0') {
                        if (c == '*') {
                            c = inputfile[index++];
                            if (c == '/') break;
                        }
                    }
                }
                else // Wasn't a comment, got to back it up
                    index --;
            }
            if (comment == 1) { // Was a comment so continue
                comment = 0;
                continue;
            }

            word[letter++] = c;
            c = inputfile[index++];
            // Continually check for more operators.
            while (c != '\0') {
                if (c != ' ' && c != '\t' && c != '\n' && c != '\r' && !isalnum(c))
                    word[letter++] = c;
                else { // Found a non-op, you know the drill
                    lexeme temp;
                    
                    // Figure out what operator we have.
                    if (strcmp(word, "+") == 0) temp.type = 4;
                    else if (strcmp(word, "-") == 0) temp.type = 5;
                    else if (strcmp(word, "*") == 0) temp.type = 6;
                    else if (strcmp(word, "/") == 0) temp.type = 7;
                    else if (strcmp(word, "=") == 0) temp.type = 9;
                    else if (strcmp(word, "<>") == 0) temp.type = 10;
                    else if (strcmp(word, "<") == 0) temp.type = 11;
                    else if (strcmp(word, "<=") == 0) temp.type = 12;
                    else if (strcmp(word, ">") == 0) temp.type = 13;
                    else if (strcmp(word, ">=") == 0) temp.type = 14;
                    else if (strcmp(word, "(") == 0) temp.type = 15;
                    else if (strcmp(word, ")") == 0) temp.type = 16;
                    else if (strcmp(word, ",") == 0) temp.type = 17;
                    else if (strcmp(word, ";") == 0) temp.type = 18;
                    else if (strcmp(word, ".") == 0) temp.type = 19;
                    else if (strcmp(word, ":=") == 0) temp.type = 20;
                    else temp.type = -1;

                    // Reset the word array for future use
                    for (i = 0; i < IDENTIFIER_MAX_LENGTH; i++) word[i] = '\0';
                    letter = 0;

                    // making sure the scanner is out of place.
                    if (c != ' ' && c != '\t' && c != '\n' && c != '\r') index--;

                    // Input the lexeme into the list
                    out[size] = temp;
                    size++;
                    lexeme *new_list = (lexeme*) realloc(out, size * (sizeof(lexeme) + 13));
                    if (new_list == NULL) {
                        while (new_list == NULL) {
                            new_list = (lexeme*) realloc(out, size * (sizeof(lexeme) + 13));
                        }
                    }
                    out = new_list;
                    break;
                }
                c = inputfile[index++];
            }
        }

        c = inputfile[index++];
    }

    if (word[0] != '\0') {
        lexeme temp;

        if (isdigit(word[0])) {
            for (i = 0; i < letter; i++) {
                if (!isdigit(word[i])) {
                    temp.type = -2;
                    break;
                }
            }
            if (temp.type != -2 && letter > NUMBER_MAX_LENGTH) {
                temp.type = -3;
            }
            else if (temp.type != -2) {
                temp.type = 3;
                temp.value = strtol(word, NULL, 10);
            }
        }
        
        // Figure out what we have.
        if (strcmp(word, "+") == 0) temp.type = 4;
        else if (strcmp(word, "-") == 0) temp.type = 5;
        else if (strcmp(word, "*") == 0) temp.type = 6;
        else if (strcmp(word, "/") == 0) temp.type = 7;
        else if (strcmp(word, "odd") == 0) temp.type = 8;
        else if (strcmp(word, "=") == 0) temp.type = 9;
        else if (strcmp(word, "<>") == 0) temp.type = 10;
        else if (strcmp(word, "<") == 0) temp.type = 11;
        else if (strcmp(word, "<=") == 0) temp.type = 12;
        else if (strcmp(word, ">") == 0) temp.type = 13;
        else if (strcmp(word, ">=") == 0) temp.type = 14;
        else if (strcmp(word, "(") == 0) temp.type = 15;
        else if (strcmp(word, ")") == 0) temp.type = 16;
        else if (strcmp(word, ",") == 0) temp.type = 17;
        else if (strcmp(word, ";") == 0) temp.type = 18;
        else if (strcmp(word, ".") == 0) temp.type = 19;
        else if (strcmp(word, ":=") == 0) temp.type = 20;
        else if (strcmp(word, "begin") == 0) temp.type = 21;
        else if (strcmp(word, "end") == 0) temp.type = 22;
        else if (strcmp(word, "if") == 0) temp.type = 23;
        else if (strcmp(word, "then") == 0) temp.type = 24;
        else if (strcmp(word, "while") == 0) temp.type = 25;
        else if (strcmp(word, "do") == 0) temp.type = 26;
        else if (strcmp(word, "call") == 0) temp.type = 27;
        else if (strcmp(word, "const") == 0) temp.type = 28;
        else if (strcmp(word, "var") == 0) temp.type = 29;
        else if (strcmp(word, "procedure") == 0) temp.type = 30;
        else if (strcmp(word, "write") == 0) temp.type = 31;
        else if (strcmp(word, "read") == 0) temp.type = 32;
        else if (strcmp(word, "else") == 0) temp.type = 33;
        else temp.type = 2;

        if (temp.type == 2) {
            for (i = 0; i < letter; i++) temp.name[i] = word[i];
        }

        // Input the lexeme into the list
        out[size] = temp;
        size++;
        lexeme *new_list = (lexeme*) realloc(out, size * (sizeof(lexeme) + 13));
        if (new_list == NULL) {
            while (new_list == NULL) {
                new_list = (lexeme*) realloc(out, size * (sizeof(lexeme) + 13));
            }
        }
        out = new_list;
    }

    *list_size = size;

    if (flag) {
        // Print list
        printf("\nLexeme List:\n");
        for (i = 0; i < size; i++) {
            if (out[i].type < 0) {
                continue;
            }
            printf("%d ", out[i].type);
            if (out[i].type == 2)
                printf("%s ", out[i].name);
            else if( out[i].type == 3)
                printf("%d ", out[i].value);
        }
        printf("\n");
    }

    free(word);
    return out;
}