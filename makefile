assembler : main.o parser.o code.o symboltable.o
	cc -o assembler main.o parser.o code.o symboltable.o

main.o : main.c SymbolTable.h Parser.h Code.h
	cc -c main.c

parser.o : Parser.c SymbolTable.h Parser.h Code.h
	cc -c Parser.c

code.o : Code.c Code.h SymbolTable.h
	cc -c Code.c

symboltable.o : SymbolTable.c SymbolTable.h
	cc -c SymbolTable.c

clean :
	rm assembler main.o parser.o code.o symboltable.o
