#include <stdio.h>
#include "ast.h"
#include "tabelaSimbolo.h"
#include "semantico.h"
#include "quad.c"

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
        
        printf("\n------- AST -------\n");
        printAST(raiz, 0, 'r');
        gerarASTDot(raiz, "ast.dot");
        printf("\n");

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
        int *param = malloc(sizeof(int));
        *param = -1;
        printTAC(raiz, 0, reg, label, param);
    } 

 
    if (yyin != stdin) fclose(yyin);
    return 0;
}