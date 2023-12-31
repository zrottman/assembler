#ifndef CODE_H
#define CODE_H

#include <stdint.h>
#include "../symboltable/symboltable.h"

extern const char *dest_keys[];
extern const int   dest_vals[];
extern const char *jump_keys[];
extern const int   jump_vals[];
extern const char *comp_keys[];
extern const char  comp_vals[];

void build_C_COMMAND(char *comp_command, char *dest_command, char *jump_command, char *line_out);
void build_A_COMMAND(char *line_in, char *line_out, SymbolTable *symbols);
int  parse_dest(char *dest_command);
int  parse_comp(char *comp_command);
int  parse_jump(char *jump_command);
void itob(uint16_t num, char *b, int len);

#endif // CODE_H
