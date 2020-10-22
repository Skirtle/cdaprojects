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
        
    }

    fclose(ipf);
    free(word);
    free(table);
    return 0;
}

//table = (lexeme*)realloc(table, (size + 1) * sizeof(lexeme));
