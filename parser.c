#include <string.h>
#include "parser.h"


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
