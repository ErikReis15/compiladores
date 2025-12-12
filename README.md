Trabalho de compiladores


Como rodar, executar em ordem
bison -d -o parser.tab.c parser.y
flex -o lex.yy.c lex.l
gcc lex.yy.c parser.tab.c ast.c tabelaSimbolo.c main.c -o compilador