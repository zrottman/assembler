#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

enum { A_COMMAND, C_COMMAND };

int get_command_type(char *line)
{
    if (line[0] == '@')
        return A_COMMAND;
    else
        return C_COMMAND;
}

void itob(uint16_t num, char *b, int len)
{
    for (int i=0; i<len; ++i)
    {
        b[len-i-1] = ((num & 1) == 1) ? '1' : '0';
        num >>= 1;
    }
}

void trans_A_COMMAND(char *line_in, char *line_out)
{
    uint16_t i;

    // convert line[1:] to int
    i = atoi(line_in + 1);
    
    // set MSB to 0 if i>32767
    i = i & 0x7fff;

    // convert i to 15-bit string and save to output
    itob(i, line_out, 16);
}


int main(int argc, char **argv)
{
    // exit if argc is not as expected
    if (argc != 2) {
        printf("usage: assembler <path>\n");
        return EXIT_FAILURE; // macro defined in stdlib.h
    }

    FILE *f;            
    char line_in[256];
    char line_out[17] = {0};
    int linecount = 0;


    f = fopen(argv[1], "r"); // open file passed as CL arg

    // check for errors opening file
    if (f == NULL) {
        printf("Error opening file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // loop through input file and parse
    while (fgets(line_in, sizeof line_in, f) != NULL) {
        /*
         * use getline()
         */

        // remove trailing newline or carriage return
        line_in[strcspn(line_in, "\n\r")] = '\0';

        // strip comments
        char *comment_pos = strstr(line_in, "//");
        if (comment_pos != NULL)
            *comment_pos = '\0';
        
        // do stuff if line not blank
        if (line_in[0] != '\0') {
            printf("%2d: %5s --> ", ++linecount, line_in);
            switch (get_command_type(line_in)) {
                case A_COMMAND:
                    trans_A_COMMAND(line_in, line_out);
                    printf("%s\n", line_out);
                    break;
                case C_COMMAND:
                    printf("C Command!\n");
                    break;
            }
        }
    }

    fclose(f);

    return 0;
}
