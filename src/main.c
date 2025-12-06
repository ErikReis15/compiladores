#include <stdio.h>
#include "ast.h"

extern int yyparse();
extern AST *raiz;


int main() {
    if (yyparse() == 0) {
        printTree(raiz);
    } 
    return 0;
}