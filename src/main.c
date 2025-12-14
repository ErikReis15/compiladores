#include <stdio.h>
#include "ast.h"
#include "tabelaSimbolo.h"
#include "quad.c"
#include "semantico.h"

extern AST *raiz;
extern Simbolo *listaSimbolo;

extern FILE *yyin;
extern int yyparse(void);

int main(int argc, char **argv){
    if(argc > 1) yyin = fopen(argv[1], "r");
    
    if (!yyin) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    if (yyparse() == 0) {
        printf("=== SEMANTICO ===\n");
        printAST(raiz, 0, 'r');
        inicializacaoRegrasGlobais();
        entraEscopo("global");
        analisa(raiz);
        saiEscopo();
        analisaRegra(listaSimbolo);
        imprimeTabela(listaSimbolo);
        int *label = malloc(sizeof(int));
        *label = 1;
        int *reg = malloc(sizeof(int));
        *reg = 0;
        printTAC(raiz, 0, reg, label);
    } 

 
    if (yyin != stdin) fclose(yyin);
    return 0;
}