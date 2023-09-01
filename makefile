assembler : main.o parser.o codewriter.o symboltable.o
	cc -o assembler main.o parser.o codewriter.o symboltable.o

main.o : src/main.c src/symboltable/symboltable.h src/parser/parser.h src/codewriter/codewriter.h
	cc -c src/main.c

parser.o : src/parser/parser.c src/symboltable/symboltable.h src/parser/parser.h src/codewriter/codewriter.h
	cc -c src/parser/parser.c

codewriter.o : src/codewriter/codewriter.c src/codewriter/codewriter.h src/symboltable/symboltable.h
	cc -c src/codewriter/codewriter.c

symboltable.o : src/symboltable/symboltable.c src/symboltable/symboltable.h
	cc -c src/symboltable/symboltable.c

clean :
	rm assembler main.o parser.o codewriter.o symboltable.o
