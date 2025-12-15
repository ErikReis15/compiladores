all:
	bison -d -o parser.tab.c parser.y
	flex -o lex.yy.c lex.l
	gcc lex.yy.c parser.tab.c ast.c tabelaSimbolo.c semantico.c utils.c main.c -o compilador
	./compilador teste.c