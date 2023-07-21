#ifndef PARSER_H
#define PARSER_H

enum { A_COMMAND, C_COMMAND, L_COMMAND, JUMP, COMP, DEST };

int     get_command_type(char *line);
void    tokenize(char *line, char *comp, char *dest, char *jump);

#endif // PARSER_H
