#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "utils.h"


void printTAC(AST *n, int nivel) {
    if (!n) return;

    for (int i = 0; i < nivel; i++) printf("   ");

    switch (n->tipo) {
        case VAL:     printf("(VAL %d)\n", n->dado.valor); break;
        case ID:      printf("(ID \"%s\")\n",n->dado.id); break;
        case OP:      printf("(OP %d)\n", n->dado.operador);
            switch(n->dado.operador){
                case 4:
                    printOP(n, nivel);     
                    break;
            }
            break;
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

    printTAC(n->esquerda, nivel + 1);
    printTAC(n->meio,   nivel + 1);
}

void printOP(AST *n, int nivel){
    if (!n) return;
    
    
    printOP(n->esquerda, nivel + 1);
    if(n->)
    printf()
    printOP(n->direita, nivel + 1);

    printf(" %s", n->dado.operador);
    switch (n->direita->tipo){
        case ID:
            printf(" %s", n->direita->dado.id)
        break;

        case VAL:
            printf(" %s", n->direita->dado.valor)
            break;
    }
}

