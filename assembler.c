#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { A_COMMAND, C_COMMAND };

int get_command_type(char *line)
{
    if (line[0] == '@')
        return A_COMMAND;
    else
        return C_COMMAND;
}

int main(int argc, char **argv)
{
    // exit if argc is not as expected
    if (argc != 2) {
        printf("usage: assembler <path>\n");
        return EXIT_FAILURE; // macro defined in stdlib.h
    }

    FILE *f;            
    //int c;
    char line[256];
    int linecount = 0;

    f = fopen(argv[1], "r"); // open file passed as CL arg

    // check for errors opening file
    if (f == NULL) {
        printf("Error opening file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    /*
    printf("You're opening file %s\n", argv[1]);
    while ((c = fgetc(f)) != EOF)
    {
        printf("%c", c);
    }
    */

    while (fgets(line, sizeof line, f) != NULL) {

        // remove trailing newline or carriage return
        line[strcspn(line, "\n\r")] = '\0';

        // strip comments
        char *comment_pos = strstr(line, "//");
        if (comment_pos != NULL)
            *comment_pos = '\0';
        
        // do stuff if line not blank
        if (line[0] != '\0') {
            switch (get_command_type(line)) {
                case A_COMMAND:
                    printf("A Command!\n");
                    break;
                case C_COMMAND:
                    printf("C Command!\n");
                    break;
            }
            printf("%2d: %s\n", ++linecount, line);
        }


        // 1: match command type
        //
        // 2: based on command type, 
    }

    fclose(f);

    return 0;
}
