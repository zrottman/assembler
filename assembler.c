#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
    if (argc != 2) {
        printf("usage: assembler <path>\n");
        return EXIT_FAILURE; // macro defined in stdlib.h
    }

    printf("You're opening file %s\n", argv[1]);

    return 0;
}
