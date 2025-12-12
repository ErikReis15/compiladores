#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "utils.h"

AST * novoNo(Tipo tipo){
    AST* no = (AST*) malloc(sizeof(AST));
    no->tipo = tipo;
    memset(&no->dado, 0, sizeof(no->dado));
    no->esquerda = NULL;
    no->direita = NULL;
    no->meio    = NULL;
    no->linha = 0;
    no->tipoValor = INDEFINIDO;
    return no;
}

void printAST(AST *n, int nivel) {
    if (!n) return;

    for (int i = 0; i < nivel; i++) printf("   ");

    switch (n->tipo) {
        case VAL:     printf("(VAL %d)\n", n->dado.valor); break;
        case ID:      printf("(ID \"%s\")\n",n->dado.id); break;
        case OP:      printf("(OP %d)\n", n->dado.operador); break;
        case IF:      printf("(IF)\n"); break;
        case IF_ELSE: printf("(IF_ELSE)\n"); break;
        case WHILE:   printf("(WHILE)\n"); break;
        case SEQ:     printf("(SEQ)\n"); break;
        case FUNCAO:  printf("(FUNCAO %s)\n", n->dado.id ); break;
        case DECLARA: printf("(DECLARA %s)\n", n->dado.id ); break;
        case INT:    printf("(INT)\n"); break;
        case VOID: printf("(VOID)\n"); break;
        case CHAMADA: printf("(CHAMADA %s)\n", n->dado.id ); break;
        case PARAM:   printf("(PARAM %s)\n", n->dado.id ); break;
        case RETURN:  printf("(RETURN)\n"); break;
        default:      printf("(NODE %d)\n", n->tipo); break;
    }

    printAST(n->esquerda, nivel + 1);
    printAST(n->meio,   nivel + 1);
    printAST(n->direita, nivel + 1);
}

