#include <stdio.h>
#include <string.h>

void print_tok(char *line)
{
    char *token;
    char delim[] = "=";

    token = strtok(line, delim);

    for (int i=0; token != NULL; ++i) {
        printf( "%d: %s\n", i, token);
        token = strtok(NULL, delim);
    }
}

int main()
{
    char lines[][12] = { "D;JLT\0", "M=D+M\0", "0;JMP\0", "MD=M+1;JEQ\0" };
    int len;


    len = sizeof(lines) / sizeof(lines[0]);

    for (int i=0; i<len; ++i) {
        printf("LINE: %s\n", lines[i]);
        print_tok(lines[i]);
        printf("\n");
    }


    return 0;
}
