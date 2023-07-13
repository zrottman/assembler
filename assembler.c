#include <stdio.h>
#include <stdlib.h>


int main(int argc, char **argv)
{
    // exit if argc is not as expected
    if (argc != 2) {
        printf("usage: assembler <path>\n");
        return EXIT_FAILURE; // macro defined in stdlib.h
    }

    FILE *f;
    int c;

    printf("You're opening file %s\n", argv[1]);
    f = fopen(argv[1], "r");

    if (f == NULL)
        printf("something went wrong\n");

    while ((c = fgetc(f)) != EOF)
    {
        printf("%c", c);
    }

    fclose(f);


    return 0;
}
