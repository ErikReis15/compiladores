Trabalho de compiladores C-

Como rodar no linux (UBUNTU)

Execute no terminal em ordem

1º Diretório dos arquivos

cd src

2º Compilar o bison

bison -d -o parser.tab.c parser.y

3º Compilar o flex

flex -o lex.yy.c lex.l

4º Compilar o C

gcc lex.yy.c parser.tab.c ast.c tabelaSimbolo.c semantico.c utils.c  main.c -o compilador

Caso suporte, também é possível utilizar o Makefile

Digite no terminal

1º cd src

2º make


Apos realizar toda compilação execute no terminal estando no /src

	./compilador teste.c
