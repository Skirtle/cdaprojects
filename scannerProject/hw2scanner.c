#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define IDENTIFIER_MAX_LENGTH 11
#define NUMBER_MAX_LENGTH 5

typedef struct {
    int token;
    char* identifier;
} lexeme;

int main(int argc, char* argv[]) {
    char* filename = argv[1];
    FILE* ipf = fopen(filename, "r");

    if (ipf == NULL) {
        printf("Filename %s invalid, please try again.", filename);
        return 1;
    }

    // Reserved words
    char* reservedword[] = {"const", "var", "procedure", "call", "begin", "end", "if", "then", "else", "while", "do", "read", "write", "odd"};
    char specialSymbols[] = {'+','-','*','/','(',')','=',',','.','<','>',';',':'};

    char c;
    int letter = 0, i, size = 0, comment = 0;
    char* word = (char*)calloc(IDENTIFIER_MAX_LENGTH, sizeof(char));
    lexeme* table = (lexeme*)calloc(1, sizeof(lexeme) + 12);


    while((c = fgetc(ipf)) != EOF) {
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '=' || c == ',' || c == '.' || c == '<' || c == '>' || c == ';' || c == ':') {
            if (c == '/') {
                if ((c = fgetc(ipf)) == '*') {
                    comment = 1;
                    while ((c = fgetc(ipf)) != EOF) {
                        if (c == '*') {
                            if ((c = fgetc(ipf)) == '/') {
                                break;
                            }
                        }
                    }
                }
                else {
                    fseek(ipf, -1, SEEK_CUR);
                }
            } 
            if(comment == 1) {
                comment = 0;
                continue;
            }
            word[letter] = c;
            letter++;
            while((c = fgetc(ipf)) != EOF) {
                if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '=' || c == ',' || c == '.' || c == '<' || c == '>' || c == ';' || c == ':') {
                    word[letter] = c;
                    letter++;
                }
                else {
                    lexeme temp;
                    temp.identifier = (char*)calloc(letter, sizeof(char));
                    for (i = 0; i < letter; i++) temp.identifier[i] = word[i];
                    
                    if (strcmp(word, "+") == 0) temp.token = 4;
                    else if (strcmp(word, "-") == 0) temp.token = 5;
                    else if (strcmp(word, "*") == 0) temp.token = 6;
                    else if (strcmp(word, "/") == 0) temp.token = 7;
                    else if (strcmp(word, "=") == 0) temp.token = 9;
                    else if (strcmp(word, "<>") == 0) temp.token = 10;
                    else if (strcmp(word, "<") == 0) temp.token = 11;
                    else if (strcmp(word, "<=") == 0) temp.token = 12;
                    else if (strcmp(word, ">") == 0) temp.token = 13;
                    else if (strcmp(word, ">=") == 0) temp.token = 14;
                    else if (strcmp(word, "(") == 0) temp.token = 15;
                    else if (strcmp(word, ")") == 0) temp.token = 16;
                    else if (strcmp(word, ",") == 0) temp.token = 17;
                    else if (strcmp(word, ";") == 0) temp.token = 18;
                    else if (strcmp(word, ".") == 0) temp.token = 19;
                    else if (strcmp(word, ":=") == 0) temp.token = 20;
                    else temp.token = -1;
                    for (i = 0; i < IDENTIFIER_MAX_LENGTH; i++) word[i] = '\0';
                    letter = 0;
                    if (c != ' ' && c != '\t' && c != '\n') fseek(ipf, -1, SEEK_CUR);

                    table[size] = temp;
                    size++;
                    lexeme* newTable = (lexeme*) realloc(table, size * (sizeof(lexeme) + 12));
                    if (newTable == NULL) {
                        printf("Size of failure: %d\n", size);
                        printf("Memory amount: %d\n", size * (sizeof(lexeme) + 12));
                        while(newTable == NULL) {
                            lexeme* newTable = (lexeme*) realloc(table, size * (sizeof(lexeme) + 12));
                        }
                        table = newTable;
                    }
                    else {
                        table = newTable;
                    }
                    
                    break;
                }
            }
        }
        else if (isdigit(c)) {
            word[letter] = c;
            letter++;
            while((c = fgetc(ipf)) != EOF) {
                if (c != ' ' && c != '\t' && c != '\n' && c != '+' && c != '-' && c != '*' && c != '/' && c != '(' && c != ')' && c != '=' && c != ',' && c != '.' && c != '<' && c != '>' && c != ';' && c != ':') {
                    word[letter] = c;
                    letter++;
                }
                else {
                    lexeme temp;
                    temp.identifier = (char*)calloc(letter, sizeof(char));
                    for (i = 0; i < letter; i++) temp.identifier[i] = word[i];
                    for (i = 0; i < letter; i++) {
                        if (!isdigit(word[i])) {
                            temp.token = -2;
                            for (i = 0; i < IDENTIFIER_MAX_LENGTH; i++) word[i] = '\0';
                            letter = 0;
                            break;
                        }
                    }
                    if (letter > NUMBER_MAX_LENGTH && temp.token != -2) {
                        temp.token = -3;
                        for (i = 0; i < IDENTIFIER_MAX_LENGTH; i++) word[i] = '\0';
                        letter = 0;
                    }
                    else if (temp.token != -2) {
                        temp.token = 3;
                    }
                    //temp.token = 3;

                    for (i = 0; i < IDENTIFIER_MAX_LENGTH; i++) word[i] = '\0';
                    letter = 0;
                    if (c != ' ' && c != '\t' && c != '\n') fseek(ipf, -1, SEEK_CUR);
                    table[size] = temp;
                    size++;
                    lexeme* newTable = (lexeme*) realloc(table, size * (sizeof(lexeme) + 12));
                    if (newTable == NULL) {
                        printf("Size of failure: %d\n", size);
                        printf("Memory amount: %d\n", size * (sizeof(lexeme) + 12));
                        while(newTable == NULL) {
                            lexeme* newTable = (lexeme*) realloc(table, size * (sizeof(lexeme) + 12));
                        }
                        table = newTable;
                    }
                    else {
                        table = newTable;
                    }
                    break;
                }
            }
        }
        else if (isalpha(c)) {
            word[letter] = c;
            letter++;
            while((c = fgetc(ipf)) != EOF) {
                if (c != ' ' && c != '\t' && c != '\n' && c != '+' && c != '-' && c != '*' && c != '/' && c != '(' && c != ')' && c != '=' && c != ',' && c != '.' && c != '<' && c != '>' && c != ';' && c != ':') {
                    word[letter] = c;
                    letter++;
                }
                else {
                    lexeme temp;
                    temp.identifier = (char*)calloc(letter, sizeof(char));
                    for (i = 0; i < letter; i++) temp.identifier[i] = word[i];

                    if (strcmp(word, "const") == 0) temp.token = 28;
                    else if (strcmp(word, "var") == 0) temp.token = 29;
                    else if (strcmp(word, "procedure") == 0) temp.token = 30;
                    else if (strcmp(word, "call") == 0) temp.token = 27;
                    else if (strcmp(word, "begin") == 0) temp.token = 21;
                    else if (strcmp(word, "end") == 0) temp.token = 22;
                    else if (strcmp(word, "if") == 0) temp.token = 23;
                    else if (strcmp(word, "then") == 0) temp.token = 24;
                    else if (strcmp(word, "else") == 0) temp.token = 33;
                    else if (strcmp(word, "while") == 0) temp.token = 25;
                    else if (strcmp(word, "do") == 0) temp.token = 26;
                    else if (strcmp(word, "read") == 0) temp.token = 32;
                    else if (strcmp(word, "write") == 0) temp.token = 31;
                    else if (strcmp(word, "odd") == 0) temp.token = 8;
                    else {
                        if (letter > IDENTIFIER_MAX_LENGTH) {
                            temp.token = -4;
                        }
                        else {
                            temp.token = 2;
                        }
                    }

                    for (i = 0; i < IDENTIFIER_MAX_LENGTH; i++) word[i] = '\0';
                    letter = 0;
                    if (c != ' ' && c != '\t' && c != '\n') fseek(ipf, -1, SEEK_CUR);
                    table[size] = temp;
                    size++;
                    lexeme* newTable = (lexeme*) realloc(table, size * (sizeof(lexeme) + 12));
                    if (newTable == NULL) {
                        printf("Size of failure: %d\n", size);
                        printf("Memory amount: %d\n", size * (sizeof(lexeme) + 12));
                        while(newTable == NULL) {
                            lexeme* newTable = (lexeme*) realloc(table, size * (sizeof(lexeme) + 12));
                        }
                        table = newTable;
                    }
                    else {
                        table = newTable;
                    }
                    break;
                }
            }
        }
        else if (c == '/') {
            if (c = fgetc(ipf) == '*') {
                while (c = fgetc(ipf) != EOF) {
                    if (c == '*') {
                        if (c = fgetc(ipf) == '/') {
                            break;
                        }
                    }
                }
            }
        }
    
    }
    
    printf("Source Program:\n");
    rewind(ipf);
    while((c = fgetc(ipf)) != EOF) printf("%c", c);
    
    if (word[0] != '\0') {
        lexeme temp;
        temp.identifier = (char*)calloc(letter, sizeof(char));
        for (i = 0; i < letter; i++) temp.identifier[i] = word[i];
        if (strcmp(word, "+") == 0) temp.token = 4;
        else if (strcmp(word, "-") == 0) temp.token = 5;
        else if (strcmp(word, "*") == 0) temp.token = 6;
        else if (strcmp(word, "/") == 0) temp.token = 7;
        else if (strcmp(word, "=") == 0) temp.token = 9;
        else if (strcmp(word, "<>") == 0) temp.token = 10;
        else if (strcmp(word, "<") == 0) temp.token = 11;
        else if (strcmp(word, "<=") == 0) temp.token = 12;
        else if (strcmp(word, ">") == 0) temp.token = 13;
        else if (strcmp(word, ">=") == 0) temp.token = 14;
        else if (strcmp(word, "(") == 0) temp.token = 15;
        else if (strcmp(word, ")") == 0) temp.token = 16;
        else if (strcmp(word, ",") == 0) temp.token = 17;
        else if (strcmp(word, ";") == 0) temp.token = 18;
        else if (strcmp(word, ".") == 0) temp.token = 19;
        else if (strcmp(word, ":=") == 0) temp.token = 20;
        else if (strcmp(word, "const") == 0) temp.token = 28;
        else if (strcmp(word, "var") == 0) temp.token = 29;
        else if (strcmp(word, "procedure") == 0) temp.token = 30;
        else if (strcmp(word, "call") == 0) temp.token = 27;
        else if (strcmp(word, "begin") == 0) temp.token = 21;
        else if (strcmp(word, "end") == 0) temp.token = 22;
        else if (strcmp(word, "if") == 0) temp.token = 23;
        else if (strcmp(word, "then") == 0) temp.token = 24;
        else if (strcmp(word, "else") == 0) temp.token = 33;
        else if (strcmp(word, "while") == 0) temp.token = 25;
        else if (strcmp(word, "do") == 0) temp.token = 26;
        else if (strcmp(word, "read") == 0) temp.token = 32;
        else if (strcmp(word, "write") == 0) temp.token = 31;
        else if (strcmp(word, "odd") == 0) temp.token = 8;
        else if (isalnum(word[0])) {
            if (isalpha(word[0])) {
                if (letter > IDENTIFIER_MAX_LENGTH) {
                    temp.token = -4;
                }
                else {
                    temp.token = 2;
                }
            }
            else {
                for (i = 0; i < letter; i++) {
                    if (!isdigit(word[i])) {
                        temp.token = -2;
                        for (i = 0; i < IDENTIFIER_MAX_LENGTH; i++) word[i] = '\0';
                        letter = 0;
                        break;
                    }
                }
                if (letter > NUMBER_MAX_LENGTH && temp.token != -2) {
                    temp.token = -3;
                    for (i = 0; i < IDENTIFIER_MAX_LENGTH; i++) word[i] = '\0';
                    letter = 0;
                }
                else if (temp.token != -2) {
                    temp.token = 3;
                }
            }
        }
        else {
            temp.token = -1;
        }
        table[size] = temp;
        size++;
        lexeme* newTable = (lexeme*) realloc(table, size * (sizeof(lexeme) + 12));
        if (newTable == NULL) {
            printf("Size of failure: %d\n", size);
            printf("Memory amount: %d\n", size * (sizeof(lexeme) + 12));
            while(newTable == NULL) {
                lexeme* newTable = (lexeme*) realloc(table, size * (sizeof(lexeme) + 12));
            }
            table = newTable;
        }
        else {
            table = newTable;
        }
    }

    printf("\n\nLexeme Table:\nlexeme\t\ttoken type\n");
    for (i = 0; i < size; i++) {
        printf("%s\t\t%d\n", table[i].identifier, table[i].token);
    }

    printf("\nLexeme List:\n");
    for (i = 0; i < size; i++) {
        printf("%d ", table[i].token);
        if (table[i].token == 2 || table[i].token == 3) {
            printf("%s ", table[i].identifier);
        }
    }
    

    fclose(ipf);
    free(word);
    free(table);
    return 0;
}

//table = (lexeme*)realloc(table, (size + 1) * sizeof(lexeme));
