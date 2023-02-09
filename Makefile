.PHONY: build

build:
	bison -d parser.y -o parser.c
	flex -o lexer.c lexer.l
	g++ lexer.c parser.c