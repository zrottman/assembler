#ifndef PARSER_H
#define PARSER_H

#include "SymbolTable.h"
#include "Code.h"

enum { A_COMMAND, C_COMMAND, L_COMMAND, JUMP, COMP, DEST };

int     get_command_type(char *line);
void    tokenize(char *line, char *comp, char *dest, char *jump);
void    parse_label(char* line_in, LinkedList *symbols, int *linecount);
void    ltrim(char* line_in);
void    cleanline(char *line_in);
void parse_instruction(char* line_in, LinkedList *symbols, char *line_out, int *default_val, int *linecount, FILE *fp_out);

#endif // PARSER_H
