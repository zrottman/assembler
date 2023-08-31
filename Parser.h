#ifndef PARSER_H
#define PARSER_H

#include "SymbolTable.h"
#include "Code.h"

enum { A_COMMAND, C_COMMAND, L_COMMAND, JUMP, COMP, DEST };

int     get_command_type(char *line);
void    tokenize(char *line, char *comp, char *dest, char *jump);
void    parse_label(char* line_in, SymbolTable *symbols, int *linecount);
void    parse_instruction(char* line_in, SymbolTable *symbols, char *line_out, int *linecount, FILE *fp_out);
void    cleanline(char *line_in);
void    ltrim(char* line_in);

#endif // PARSER_H
