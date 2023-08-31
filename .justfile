compile:
    cc main.c SymbolTable.c Code.c Parser.c -o assembler

assemble project:
    find sample_asm -name "{{project}}.asm" -type f -exec ./assembler {} \;

test project:
    find sample_asm -name "{{project}}.*hack" -type f -exec diff -qs {} +

assemble_and_test project:
    just assemble {{project}}
    just test {{project}}

doitall project:
    just compile
    just assemble {{project}}
    just test {{project}}
    
