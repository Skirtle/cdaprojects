#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    int letter = 0, i, size = 100;
    char* word = (char*)calloc(IDENTIFIER_MAX_LENGTH, sizeof(char));
    lexeme* table = (lexeme*)malloc(sizeof(lexeme) * size);
    while((c = fgetc(ipf)) != EOF) {
        //printf("%c", c);
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '=' || c == ',' || c == '.' || c == '<' || c == '>' || c == ';' || c == ':') {
            lexeme temp;
            temp.identifier = (char*)calloc(letter, sizeof(char));

            for (i = 0; i < letter; i++)
                temp.identifier[i] = word[i];
            
            if (strcmp(word, "const") == 0)
                temp.token = 28;
            else if (strcmp(word, "var") == 0)
                temp.token = 29;
            else if (strcmp(word, "procedure") == 0)
                temp.token = 30;
            else if (strcmp(word, "call") == 0)
                temp.token = 27;
            else if (strcmp(word, "begin") == 0)
                temp.token = 21;
            else if (strcmp(word, "end") == 0)
                temp.token = 22;
            else if (strcmp(word, "if") == 0)
                temp.token = 23;
            else if (strcmp(word, "then") == 0)
                temp.token = 24;
            else if (strcmp(word, "else") == 0)
                temp.token = 33;
            else if (strcmp(word, "while") == 0)
                temp.token = 25;
            else if (strcmp(word, "do") == 0)
                temp.token = 26;
            else if (strcmp(word, "read") == 0)
                temp.token = 32;
            else if (strcmp(word, "write") == 0)
                temp.token = 31;
            else if (strcmp(word, "odd") == 0)
                temp.token = 8;
            else {
                temp.token = 2;
            }

            table[size-1] = temp;
            table = (lexeme*)realloc(table, (size + 1) * sizeof(lexeme));
            size++;

            for (i = 0; i < IDENTIFIER_MAX_LENGTH; i++)
                word[i] = '\0';
            letter = 0;
        }
        else if (c != ' ' && c != '\t' && c != '\n') {
            word[letter] = c;
            letter++;
        }
        else {
            lexeme temp;
            temp.identifier = (char*)calloc(letter, sizeof(char));

            for (i = 0; i < letter; i++)
                temp.identifier[i] = word[i];

            if (strcmp(word, "const") == 0)
                temp.token = 28;
            else if (strcmp(word, "var") == 0)
                temp.token = 29;
            else if (strcmp(word, "procedure") == 0)
                temp.token = 30;
            else if (strcmp(word, "call") == 0)
                temp.token = 27;
            else if (strcmp(word, "begin") == 0)
                temp.token = 21;
            else if (strcmp(word, "end") == 0)
                temp.token = 22;
            else if (strcmp(word, "if") == 0)
                temp.token = 23;
            else if (strcmp(word, "then") == 0)
                temp.token = 24;
            else if (strcmp(word, "else") == 0)
                temp.token = 33;
            else if (strcmp(word, "while") == 0)
                temp.token = 25;
            else if (strcmp(word, "do") == 0)
                temp.token = 26;
            else if (strcmp(word, "read") == 0)
                temp.token = 32;
            else if (strcmp(word, "write") == 0)
                temp.token = 31;
            else if (strcmp(word, "odd") == 0)
                temp.token = 8;
            
            table[size-1] = temp;
            table = (lexeme*)realloc(table, (size + 1) * sizeof(lexeme));
            size++;

            if (word[0] != '\0') {
                //printf("%s\n", word);
                for (i = 0; i < IDENTIFIER_MAX_LENGTH; i++)
                    word[i] = '\0';
                letter = 0;
            }
        }
    }

    printf("%d", size);
    printf("%s", table[0].identifier);
    for(i = 0; i < size; i++)
        printf("%s", table[i].identifier);
    
    fclose(ipf);
    free(word);
    free(table);
    return 0;
}
