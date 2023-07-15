#include <stdio.h>
#include <string.h>

#define MAXLINE 16

void tokenize(char *line, char *comp, char *dest, char *jump) 
{
    char *equal_sign, *semicolon;

    if ((equal_sign = strchr(line, '='))) {
        *equal_sign = '\0';
        strcpy(dest, line);
        strcpy(comp, equal_sign + 1);
        if ((semicolon = strchr(comp, ';'))) {
            *semicolon = '\0';
            strcpy(jump, semicolon + 1);
        } else {
            strcpy(jump, "");
        }
    } else {
        semicolon = strchr(line, ';');
        *semicolon = '\0';
        strcpy(dest, "");
        strcpy(comp, line);
        strcpy(jump, semicolon + 1);
    }
}

int main()
{
    char c, line[MAXLINE];
    char comp[4] = {0};
    char jump[4] = {0};
    char dest[4] = {0};
    int i = 0;
    while ((c = getchar()) != EOF)
    {
        if (c == '\n') {
            // do stuff
            line[i] = '\0';
            printf("Tokenizing %s...\n", line);
            tokenize(line, comp, dest, jump);
            
            printf("comp: %s\n", comp);
            printf("dest: %s\n", dest);
            printf("jump: %s\n", jump);
            printf("\n");
            
            // reset line
            for (; i > 0; i--)
                line[i] = '\0';
        } else {
            if (i < MAXLINE)
                line[i++] = c;
        }
    }

    return 0;
}
