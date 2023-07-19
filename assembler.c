#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "linkedlist.h"

#define MAXLINE 256

enum { A_COMMAND, C_COMMAND, L_COMMAND, JUMP, COMP, DEST };

char *dest_keys[] = { "M", "D", "MD", "A", "AM", "AD", "AMD" };
int   dest_vals[] = { 1, 2, 3, 4, 5, 6, 7 };

char *jump_keys[] = { "JGT", "JEQ", "JGE", "JLT", "JNE", "JLE", "JMP" };
int   jump_vals[] = { 1, 2, 3, 4, 5, 6, 7 };

char *comp_keys[] = {
    "0", "1", "-1", "D", "A", "M", "!D", "!A", "!M", "-D", "-A", "-M",
    "D+1", "A+1", "M+1", "D-1", "A-1", "M-1", "D+A", "D+M", "D-A", "D-M",
    "A-D", "M-D", "D&A", "D&M", "D|A", "D|M" 
};
char  comp_vals[] = {
    42, 63, 58, 12, 48, 112, 13, 49, 113, 15, 51, 115, 31, 55, 119, 14, 
    50, 114, 2, 66, 19, 83, 7, 71, 0, 64, 21, 85
};

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

// Function:    itob
// Description: Encodes an integer to a bitstring of length `len`
// Parameters:
//              num:    16-bit unsigned int to encode
//              b:      pointer to destination string that will be mutated
//              len:    length of output bitstring
// Returns:     void
void itob(uint16_t num, char *b, int len)
{
    for (int i=0; i<len; ++i)
    {
        b[len-i-1] = ((num & 1) == 1) ? '1' : '0';
        num >>= 1;
    }
}

// Function:    build_A_COMMAND
// Description: builds 16-bit A-instruction
// Parameters:
//              line_in:        pointer to cleaned .asm line to translate to 16-bit encoding
//              line_out:       pointer to destination string
//              symbols:        pointer to linked list that handles symbol lookups
//              default_val:    default value to insert into `symbols` list on insertion
// Returns:     void
void build_A_COMMAND(char *line_in, char *line_out, LinkedList *symbols, int default_val)
{
    uint16_t i;

    if ('0' <= line_in[1] && line_in[1] <= '9') { 
        // treat `line_in` as numerical
        i = atoi(line_in + 1);  // convert line[1:] to int
        i = i & 0x7fff;         // set MSB to 0 if i>32767
        itob(i, line_out, 16);  // convert i to 15+1-bit string and save to output
    } else {                                      
        // treat `line_in` as symbol
        i = search(symbols, line_in + 1, default_val);
        itob(i, line_out, 16);
    }
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

// Function:    parse_dest, parse_comp, parse_jump
// Description: Functions that parse `comp`, `dest`, and `jump` tokens by performing
//              lookups in relevant parallel arrays.
// Parameters:
//              <token>_command:    pointer to token
// Returns:     The integer value associated with the token in the relevant
//              lookup table
// TODO:        Consolidate these three functions into a single `token_lookup` function
int parse_dest(char *dest_command)
{
    int len = sizeof(dest_vals)/sizeof(dest_vals[0]);

    for (int i=0; i<len; ++i)
        if (!strcmp(dest_command, dest_keys[i]))
            return dest_vals[i];
    return 0;
}

int parse_comp(char *comp_command)
{
    int len = sizeof(comp_vals)/sizeof(comp_vals[0]);

    for (int i=0; i<len; ++i)
        if (!strcmp(comp_command, comp_keys[i]))
            return comp_vals[i];
    return 0;
}

int parse_jump(char *jump_command)
{
    int len = sizeof(jump_vals)/sizeof(jump_vals[0]);

    for (int i=0; i<len; ++i)
        if (!strcmp(jump_command, jump_keys[i]))
            return jump_vals[i];
    return 0;
}

int token_lookup(char* command, int command_type)
{
    // TODO: consolidate parse_dest, parse_comp, and parse_command functions into
    //       single parsing function, which will need to to take a command_type
    //       argument so we know which lookup arrays to refer to (can be an enum)
    return 0;
}

// Function:    build_C_COMMAND
// Description: Builds 16-bit C-instruction and assigns to `line_out`. This function
//              sets the most significant 3 bits to `1`, then tokenizes the line, then
//              performs lookups for each token to obtain integer equivalents, then
//              appends these integer equivalents to `line_out` using bitwise ops.
// Parameters: 
//              line_in:    pointer to cleaned .asm input line
//              line_out:   pointer to 16-character output line
// Returns:     void
void build_C_COMMAND(char *line_in, char *line_out)
{
    uint16_t out = 7;
    int dest, comp, jump;
    char dest_command[4] = {0};
    char comp_command[4] = {0};
    char jump_command[4] = {0};

    tokenize(line_in, comp_command, dest_command, jump_command);

    dest = parse_dest(dest_command); // TODO: consolidate three parse functions into one general parse function
    comp = parse_comp(comp_command);
    jump = parse_jump(jump_command);

    // set output bits
    out = 7; out <<= 7;         // set most signifiant 3 bits to 111
    out |= comp; out <<= 3;     // set 7 comp bits
    out |= dest; out <<= 3;     // set 3 dest bits
    out |= jump;                // set 3 jump bits
    itob(out, line_out, 16);    // convert to binstring
}

// Function:    initialize_symbols
// Description: Initializes `symbols` linked list with default symbols key/value
//              pairs.
// Parameters:  
//              symbols:    pointer to linked list
// Returns:     void
void initialize_symbols(LinkedList* symbols)
{
    // default key/value pairs for initializing symbols linkedlist
    char* keys[]    = { 
        "SP", "LCL", "ARG", "THIS", "THAT", "R0", "R1", "R2", "R3", "R4", 
        "R5", "R6", "R7", "R8", "R9", "R10", "R11", "R12", "R13", "R14", 
        "R15", "SCREEN", "KBD"
    };
    int nums[]      = {
        0, 1, 2, 3, 4, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 
        11, 12, 13, 14, 15, 16384, 24576
    };
    int len         = sizeof(nums) / sizeof(nums[0]);

    // populate linked list with default symbols
    for (int i=0; i<len; ++i)
        append(symbols, create_node(keys[i], nums[i]));
}

// Function:    ltrim
// Description: String cleaning function that removes leading spaces and tabs
//              by shifting non-space characters left and then terminating the 
//              string.
// Parameters:
//              line_in:    pointer to .asm line that will be mutated
// Returns:     void
void ltrim(char* line_in)
{
    int i, j;
    i = j = 0;
    while (line_in[i] == ' ' || line_in[i] == '\t')
        i++;
    if (i > 0) {
        while (line_in[i] != '\0')
            line_in[j++] = line_in[i++];
        line_in[j] = '\0';
    }
}

int main(int argc, char **argv)
{
    // exit if argc is not as expected
    if (argc != 2) {
        printf("usage: assembler <path>\n");
        return EXIT_FAILURE; // macro defined in stdlib.h
    }

    FILE *f;            
    
    // open file passed as CL arg
    f = fopen(argv[1], "r"); 

    // check for errors opening file
    if (f == NULL) {
        printf("Error opening file %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    char line_in[MAXLINE];
    char line_out[17] = {0};
    int linecount = 0;
    int i, j;

    // Instantiate linked list
    LinkedList* symbols = create_linked_list();
    initialize_symbols(symbols);
    int default_val = 16; // default starting value for new symbols

    // PASS 1: parse labels
    while (fgets(line_in, sizeof line_in, f) != NULL) {
        // TODO: refactor line-cleaning functions to avoid redundancies cleaning
        //       lines on both passes

        // strip comments
        char *comment_pos = strstr(line_in, "//");
        if (comment_pos != NULL)
            *comment_pos = '\0';

        // trim leading spaces
        ltrim(line_in);
        
        // remove trailing newline or carriage return
        line_in[strcspn(line_in, "\n\r ")] = '\0';

        // add to symbols linkedlist
        if (get_command_type(line_in) == L_COMMAND) {
            line_in[strcspn(line_in, ")")] = '\0';
            search(symbols, line_in + 1, linecount);
        } else if (line_in[0] != '\0') {
            linecount++;
        }
    }

    // PASS 2: loop through input file and parse
    rewind(f);
    linecount = 0;
    while (fgets(line_in, sizeof line_in, f) != NULL) {
        /*
         * consider using getline()
         */

        // strip comments
        char *comment_pos = strstr(line_in, "//");
        if (comment_pos != NULL)
            *comment_pos = '\0';

        // trim leading spaces
        ltrim(line_in);
        
        // remove trailing newline or carriage return
        line_in[strcspn(line_in, "\n\r ")] = '\0';

        // process non-blank lines
        if (line_in[0] != '\0') {

            printf("%2d: %15s --> ", linecount++, line_in); // print line_in

            switch (get_command_type(line_in)) {
                case A_COMMAND:
                    build_A_COMMAND(line_in, line_out, symbols, default_val++); 
                    printf("%s\n", line_out);
                    break;
                case C_COMMAND:
                    build_C_COMMAND(line_in, line_out);
                    printf("%s\n", line_out);
                    break;
                case L_COMMAND:
                    linecount--;
                    printf("\n");
                    break;
            }
        }
    }

    fclose(f);

    return 0;
}
