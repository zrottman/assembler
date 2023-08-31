#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include "Code.h"
#include "SymbolTable.h"

const char *dest_keys[] = { "M", "D", "MD", "A", "AM", "AD", "AMD" };
const int   dest_vals[] = { 1, 2, 3, 4, 5, 6, 7 };

const char *jump_keys[] = { "JGT", "JEQ", "JGE", "JLT", "JNE", "JLE", "JMP" };
const int   jump_vals[] = { 1, 2, 3, 4, 5, 6, 7 };

const char *comp_keys[] = {
    "0", "1", "-1", "D", "A", "M", "!D", "!A", "!M", "-D", "-A", "-M",
    "D+1", "A+1", "M+1", "D-1", "A-1", "M-1", "D+A", "D+M", "D-A", "D-M",
    "A-D", "M-D", "D&A", "D&M", "D|A", "D|M" 
};
const char  comp_vals[] = {
    42, 63, 58, 12, 48, 112, 13, 49, 113, 15, 51, 115, 31, 55, 119, 14, 
    50, 114, 2, 66, 19, 83, 7, 71, 0, 64, 21, 85
};

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

// Function:    build_C_COMMAND
// Description: Builds 16-bit C-instruction and assigns to `line_out`. This function
//              sets the most significant 3 bits to `1`, then tokenizes the line, then
//              performs lookups for each token to obtain integer equivalents, then
//              appends these integer equivalents to `line_out` using bitwise ops.
// Parameters: 
//              line_in:    pointer to cleaned .asm input line
//              line_out:   pointer to 16-character output line
// Returns:     void
void build_C_COMMAND(char *comp_command, char *dest_command, char *jump_command, char *line_out)
{
    uint16_t out = 7;
    int dest, comp, jump;


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

// Function:    build_A_COMMAND
// Description: builds 16-bit A-instruction
// Parameters:
//              line_in:        pointer to cleaned .asm line to translate to 16-bit encoding
//              line_out:       pointer to destination string
//              symbols:        pointer to linked list that handles symbol lookups
//              default_val:    default value to insert into `symbols` list on insertion
// Returns:     void
void build_A_COMMAND(char *line_in, char *line_out, LinkedList *symbols, int *default_val)
{
    uint16_t i;

    if ('0' <= line_in[1] && line_in[1] <= '9') { 
        // treat `line_in` as numerical
        i = atoi(line_in + 1);  // convert line[1:] to int
        i = i & 0x7fff;         // set MSB to 0 if i>32767
        itob(i, line_out, 16);  // convert i to 15+1-bit string and save to output
    } else {                                      
        // treat `line_in` as symbol
        i = search(symbols, line_in + 1, *default_val);
        if (i == *default_val)
            (*default_val)++;
        itob(i, line_out, 16);
    }
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

