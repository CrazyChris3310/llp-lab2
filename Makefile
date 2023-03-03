.PHONY: build

CPPFLAGS = -pedantic-errors -Wall -Werror -g3 -O0 --sanitize=address,undefined,leak

build:
	bison -t -d parser.y -o parser.c
	flex -o lexer.c --header-file=lexer.h lexer.l
	g++ $(CPPFLAGS) lexer.c parser.c ast.cpp main.cpp -o main