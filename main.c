#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "SymbolTable.h"
#include "Parser.h"
#include "Code.h"

#define MAXLINE 256

int token_lookup(char* command, int command_type)
{
    // TODO: consolidate parse_dest, parse_comp, and parse_command functions into
    //       single parsing function, which will need to to take a command_type
    //       argument so we know which lookup arrays to refer to (can be an enum)
    return 0;
}


// Function:    ltrim
// Description: String cleaning function that removes leading spaces and tabs
//              by shifting non-space characters left and then terminating the 
//              string.
// Parameters:
//              line_in:    pointer to .asm line that will be mutated
// Returns:     void
// TODO:       consider using reallocate
void ltrim(char* line_in)
{
    int i, j;
    i = j = 0;
    while (line_in[i] == ' ' || line_in[i] == '\t') // check for a string.h function that looks for whitespace
        i++;
    if (i > 0) {
        while (line_in[i] != '\0')
            line_in[j++] = line_in[i++];
        line_in[j] = '\0';
    }
}

// Function:   cleanline
// Decription: Function to clean entire input line: strips comments, trims
//             leading spaces, removes trailing spaces/newlines/CRs
// Parameters: 
//             line_in:     pointer to .asm line that will be mutated
// Returns:    void
void cleanline(char *line_in)
{
        // strip comments
        char *comment_pos = strstr(line_in, "//");
        if (comment_pos != NULL)
            *comment_pos = '\0';

        // trim leading spaces
        ltrim(line_in);
        
        // remove trailing newline or carriage return
        line_in[strcspn(line_in, "\n\r ")] = '\0';
}

// Function:    make_output_path
// Description: Generates path to output file.
// Parameters:
//              in_path:    input file path for .asm file
//              out_path:   output file path replacing .asm with .hack
// Returns:     void
void make_output_path(char *out_path, char *in_path)
{
    char *p;

    strcpy(out_path, in_path);      // copy argv[1] to out_path
    p = strrchr(out_path, '.');     // get location of file extension
    strcpy(p, ".hack\0");           // replace .asm with .hack
}

void parse_label(char* line_in, LinkedList *symbols, int *linecount) {
        // strip comments and trim
        cleanline(line_in);

        // add to symbols linkedlist
        if (get_command_type(line_in) == L_COMMAND) {
            line_in[strcspn(line_in, ")")] = '\0';
            search(symbols, line_in + 1, *linecount);
        } else if (line_in[0] != '\0') {
            (*linecount)++;
        }
}

int main(int argc, char **argv)
{

    // exit if argc is not as expected
    if (argc != 2) {
        printf("usage: assembler <read path>\n");
        return EXIT_FAILURE; // macro defined in stdlib.h
    }

    FILE *fp_in, *fp_out;
    
    // open input file passed as CL arg and check for errors
    fp_in = fopen(argv[1], "r"); 
    if (fp_in == NULL) {
        printf("Error opening input file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // clock
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    // create output file path, open output file, and check for errors _path by replacing .asm from argv[1] with .hack
    char out_path[100]; //, *p;
    make_output_path(out_path, argv[1]);

    // open output file
    fp_out = fopen(out_path, "w");
    if (fp_in == NULL) {
        printf("Error opening output file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // print output file name at top of file
    //fprintf(fp_out, "//.hack translation of %s\n", argv[1]);

    char line_in[MAXLINE];
    char line_out[17] = {0};
    int linecount = 0;
    int i, j;

    // Instantiate linked list
    LinkedList* symbols = create_linked_list();
    initialize_symbols(symbols);
    int default_val = 16; // default starting value for new symbols
    

    char dest_command[4] = {0};
    char comp_command[4] = {0};
    char jump_command[4] = {0};

    // PASS 1: parse labels
    while (fgets(line_in, sizeof line_in, fp_in) != NULL) {

        // TODO: parse_label(line_in, &linecount) <-- start refactoring like this
        parse_label(line_in, symbols, &linecount);
        
        /*
        // strip comments and trim
        cleanline(line_in);

        // add to symbols linkedlist
        if (get_command_type(line_in) == L_COMMAND) {
            line_in[strcspn(line_in, ")")] = '\0';
            search(symbols, line_in + 1, linecount);
        } else if (line_in[0] != '\0') {
            linecount++;
        }
        */
    }

    // PASS 2: loop through input file and parse
    rewind(fp_in);
    linecount = 0;
    while (fgets(line_in, sizeof line_in, fp_in) != NULL) {
        /*
         * consider using getline()
         */
        
        // TODO: parse_instruction(line_in, ...)
        //      line_out could be inside here ^, maybe default val too
        //

        // strip comments and trim
        cleanline(line_in);

        // process non-blank lines
        if (line_in[0] != '\0') {

            printf("%2d: %15s --> ", linecount++, line_in); // print line_in

            switch (get_command_type(line_in)) {
                case A_COMMAND:
                    build_A_COMMAND(line_in, line_out, symbols, &default_val); 
                    printf("%s\n", line_out);
                    fprintf(fp_out, "%s\n", line_out);
                    break;
                case C_COMMAND:
                    tokenize(line_in, comp_command, dest_command, jump_command);
                    build_C_COMMAND(comp_command, dest_command, jump_command, line_out);
                    printf("%s\n", line_out);
                    fprintf(fp_out, "%s\n", line_out);
                    break;
                case L_COMMAND:
                    linecount--;
                    printf("\n");
                    break;
            }
        }
    }

    // close input/output files
    fclose(fp_in);
    fclose(fp_out);

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Output file: %s\n", out_path);
    printf("Took %f seconds\n", cpu_time_used);

    return 0;
}
