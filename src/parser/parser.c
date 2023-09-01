#include <string.h>
#include <stdio.h>
#include "parser.h"
#include "../symboltable/symboltable.h"
#include "../codewriter/codewriter.h"


// Function:    get_command_type
// Description: This function takes in a cleaned line from an .asm file and returns
//              the instruction type: A-instruction, C-instruction, or L-instruction
// Parameters:
//              - line: cleaned .asm line
// Returns:     Command-type enum
int get_command_type(char *line)
{
    if (line[0] == '@')
        return A_COMMAND;
    else if (line[0] == '(')
        return L_COMMAND;
    else
        return C_COMMAND;
}

// Function:    tokenize
// Description: Tokenizes cleaned C-instruction line into its three component parts
// Parameters:
//              line:   pointer to line to tokenize
//              comp:   pointer to computation token (mutated by function)
//              dest:   pointer to destination token (mutated by function)
//              jump:   pointer to jump token (mutated by function)
//  Returns:    void
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

void parse_label(char* line_in, SymbolTable *symbols, int *linecount) {
        // strip comments and trim
        cleanline(line_in);

        // add to symbols linkedlist
        if (get_command_type(line_in) == L_COMMAND) {
            line_in[strcspn(line_in, ")")] = '\0';
            append(symbols, create_node(line_in + 1, *linecount));
        } else if (line_in[0] != '\0') {
            (*linecount)++;
        }
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

void parse_instruction(char* line_in, SymbolTable *symbols, char *line_out, int *linecount, FILE *fp_out)
{
    char dest_command[4] = {0};
    char comp_command[4] = {0};
    char jump_command[4] = {0};

    // strip comments and trim
    cleanline(line_in);

    // process non-blank lines
    if (line_in[0] != '\0') {

        printf("%2d: %15s --> ", (*linecount)++, line_in); // print line_in

        switch (get_command_type(line_in)) {
            case A_COMMAND:
                build_A_COMMAND(line_in, line_out, symbols); 
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
                (*linecount)--;
                printf("\n");
                break;
        }
    }
}
