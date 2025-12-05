#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

AST * novoNo(Tipo tipo){
    AST* no = (AST*) malloc(sizeof(AST));
    no->tipo = tipo;
    no->dado = NULL;
    no->esquerda = NULL;
    no->direita = NULL;
    no->meio = NULL;

    return no;
}