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

void printAST(AST *n, int nivel, char c) {
    if (!n) return;

    for (int i = 0; i < nivel; i++) printf("   ");

    switch (n->tipo) {
        case VAL:     printf("%c-(VAL %d)\n", c,n->dado.valor); break;
        case ID:      printf("%c-(ID \"%s\")\n",c,n->dado.id); break;
        case OP:      printf("%c-(OP %d)\n", c,n->dado.operador); break;
        case IF:      printf("%c-(IF)\n",c); break;
        case IF_ELSE: printf("%c-(IF_ELSE)\n",c); break;
        case WHILE:   printf("%c-(WHILE)\n",c); break;
        case SEQ:     printf("%c-(SEQ)\n",c); break;
        case FUNCAO:  printf("%c-(FUNCAO %s)\n",c, n->dado.id ); break;
        case DECLARA: printf("%c-(DECLARA %s)\n",c, n->dado.id ); break;
        case INT:    printf("%c-(INT)\n",c); break;
        case VOID: printf("%c-(VOID)\n",c); break;
        case CHAMADA: printf("%c-(CHAMADA %s)\n",c, n->dado.id ); break;
        case PARAM:   printf("%c-(PARAM %s)\n",c, n->dado.id ); break;
        case RETURN:  printf("%c-(RETURN)\n",c); break;
        default:      printf("%c-(NODE %d)\n",c, n->tipo); break;
    }

    printAST(n->esquerda, nivel + 1, 'E');
    printAST(n->meio,   nivel + 1, 'M');
    printAST(n->direita, nivel + 1, 'D');
}

