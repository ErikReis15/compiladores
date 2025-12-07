#include <stdio.h>
#include "ast.h"
#include "tabelaSimbolo.h"

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
        printAST(raiz, 0);
        entraEscopo("global");
        analisa(raiz);
        saiEscopo();
        imprimeTabela(listaSimbolo);
    } 

 
    if (yyin != stdin) fclose(yyin);
    return 0;
}