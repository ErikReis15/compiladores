#include <stdio.h>
#include "ast.h"

extern AST *raiz;

extern FILE *yyin;
extern int yyparse(void);

int main(int argc, char **argv){
    if(argc > 1) yyin = fopen(argv[1], "r");
    
    if (!yyin) {
        perror("Erro ao abrir arquivo");
        return 1;
    }

    if (yyparse() == 0) {
        printTree(raiz);
    } 

    if (yyin != stdin) fclose(yyin);
    return 0;
}