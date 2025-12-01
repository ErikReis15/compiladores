#include <stdio.h>
#include <stdlib.h>
#include "ast.h"

AST* NoOp(char operador, AST* esquerda, AST* direita) {
    AST* node = (AST*) malloc(sizeof(AST));
    node->tipo = OP;
    node->dado.operador = operador;
    node->esquerda = esquerda;
    node->direita = direita;
    return node;
}

AST* NoValor(float valor) {
    AST* node = (AST*) malloc(sizeof(AST));
    node->tipo = VAL;
    node->dado.valor = valor;
    node->esquerda = NULL;
    node->direita = NULL;
    return node;
}