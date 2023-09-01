#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "symboltable/symboltable.h"
#include "parser/parser.h"
#include "codewriter/codewriter.h"

#define MAXLINE 256

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


int main(int argc, char **argv)
{

    // exit if argc is not as expected
    if (argc != 2) {
        printf("usage: assembler <read path>\n");
        return EXIT_FAILURE; // macro defined in stdlib.h
    }

    FILE *fp_in, *fp_out;
    
    // open input file and check for errors
    fp_in = fopen(argv[1], "r"); 
    if (fp_in == NULL) {
        printf("Error opening input file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // start clock
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    // create output file path, open, and check for errors 
    char out_path[100]; 
    make_output_path(out_path, argv[1]);
    fp_out = fopen(out_path, "w");
    if (fp_in == NULL) {
        printf("Error opening output file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // print output file name at top of file
    // fprintf(fp_out, "//.hack translation of %s\n", argv[1]);

    char line_in[MAXLINE];
    char line_out[17] = {0};
    int linecount;

    // instantiate symbols table
    SymbolTable* symbols = create_symbol_table();

    // pass 1: parse labels
    linecount = 0;
    while (fgets(line_in, sizeof line_in, fp_in) != NULL) {
        parse_label(line_in, symbols, &linecount);
    }

    // pass 2: loop through input file and parse
    rewind(fp_in);
    linecount = 0;
    while (fgets(line_in, sizeof line_in, fp_in) != NULL) {
        parse_instruction(line_in, symbols, line_out, &linecount, fp_out);
    }

    // close input/output files
    fclose(fp_in);
    fclose(fp_out);

    // stop clock
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    // print duration and output file path
    printf("Output file: %s\n", out_path);
    printf("Took %f seconds\n", cpu_time_used);

    return 0;
}
