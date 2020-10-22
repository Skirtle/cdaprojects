/* HW2 Scanner
Team Dalton Kajander and Shawn Guydeene
Due-date: 10/22/2020, 11:59
Goal:
*/

#include <stdio.h>
#include <stdlib.h>

#define IDENTIFIER_MAX_LENGTH 11
#define NUMBER_MAX_LENGTH 5

int main(int argc, char* argv[]) {
    // argv[0] - name of this file itself
    // argv[1] - name of file to compile


    char* filename = argv[1];
    FILE* ipf = fopen(filename, "r");

    if (ipf == NULL) {
        printf("Filename %s invalid, please try again.", filename);
        return 2;
    }

    // Reserved words
    char* word[] = {"const", "var", "procedure", "call", "begin", "end", "if", "then", "else", "while", "do", "read", "write", "odd"}; 


    fclose(ipf);
    return 1;
}
