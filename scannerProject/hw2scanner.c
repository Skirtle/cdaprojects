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
    char identifier[];
} lexeme;

int main(int argc, char* argv[]) {
    // argv[0] - name of this file itself
    // argv[1] - name of file to compile


    char* filename = argv[1];
    FILE* ipf = fopen(filename, "r");

    if (ipf == NULL) {
        printf("Filename %s invalid, please try again.", filename);
        return 1;
    }

    // Reserved words
    char* reservedWords[] = {"const", "var", "procedure", "call", "begin", "end", "if", "then", "else", "while", "do", "read", "write", "odd"}; 
    char specialSymbols[] = {'+','-','*','/','(',')','=',',','.','<','>',';',':'};
    
    char c;
    char* word = (char*)calloc(IDENTIFIER_MAX_LENGTH + 1, sizeof(char));
    int letter = 0, i;
    while((c = fgetc(ipf)) != EOF) {
        //printf("%c", c);
        if(c != ' ' && c != '\t' && c != '\n') {
            word[letter] = c;
            letter++;
        }
        else {
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
    return 0;
}
