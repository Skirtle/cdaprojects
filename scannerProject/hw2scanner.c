/* HW2 Scanner
Team Dalton Kajander and Shawn Guydeene
Due-date: 10/22/2020, 11:59
Goal:
*/

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
    // argv[0] - name of this file itself
    // argv[1] - name of file to compile

    char* filename = argv[1];
    filename = "testInput.txt";
    FILE* ipf = fopen(filename, "r");

    if (ipf == NULL) {
        printf("Filename %s invalid, please try again.", filename);
        return 1;
    }

    // Reserved words
    char* reservedWords[] = {"const", "var", "procedure", "call", "begin", "end", "if", "then", "else", "while", "do", "read", "write", "odd"};
    char specialSymbols[] = {'+','-','*','/','(',')','=',',','.','<','>',';',':'};

    char c;
    char* word = (char*)calloc(IDENTIFIER_MAX_LENGTH, sizeof(char));
    lexeme* table = (lexeme*)malloc(sizeof(lexeme) * 1);
    int letter = 0, i, size = 1;
    while((c = fgetc(ipf)) != EOF) {
        //printf("%c", c);
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '=' || c == ',' || c == '.' || c == '<' || c == '>' || c == ';' || c == ':') {
            lexeme temp;
            strcpy(temp.identifier, word);
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
            table = (lexeme*)realloc(table, size+1);
            size++;
            
            for (i = 0; i < IDENTIFIER_MAX_LENGTH; i++)
                temp.identifier[i] = '\0';
            
            temp.identifier[0] = c;

            if (c == '+')
                temp.token = 28;
            else if (c == '-')
                temp.token = 29;
            else if (c == '*')
                temp.token = 30;
            else if (c == '/')
                temp.token = 27;
            else if (c == '(')
                temp.token = 21;
            else if (c == ')')
                temp.token = 22;
            else if (c == '=')
                temp.token = 23;
            else if (c == ',')
                temp.token = 24;
            else if (c == '.')
                temp.token = 33;
            else if (c == '<')
                temp.token = 25;
            else if (c == '>')
                temp.token = 26;
            else if (c == ';')
                temp.token = 32;
            else if (c == ':')
                temp.token = 31;

            table[size-1] = temp;
            table = (lexeme*)realloc(table, size+1);
            size++;

            printf("%s\n", word);
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
            strcpy(temp.identifier, word);
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
            table = (lexeme*)realloc(table, size+1);
            size++;
            
            if (word[0] != '\0') {
                printf("%s\n", word);
                for (i = 0; i < IDENTIFIER_MAX_LENGTH; i++)
                    word[i] = '\0';
                letter = 0;
            }
        }
    }

    fclose(ipf);
    free(word);
    free(table);
    return 0;
}
