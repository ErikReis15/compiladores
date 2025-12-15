#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "utils.h"

void printIDVALOP(AST *n){
    switch (n->tipo){
        case ID:  printf(" %s", n->dado.id); break;
        case VAL: printf(" %d", n->dado.valor); break;
        case OP:  printf(" %s", operadorToString(n->dado.operador)); break;
        default: break;
    }
}

void printTAC(AST *n, int nivel, int *reg, int *label, int *param);


void printOP(AST *n, int *reg, int first, int *label, int *param){
    if (!n) return;

    int reg_base = *reg;

    if(!n->esquerda && !n->direita){
        if(first){
            printf("t%d = ", *reg);
            (*reg)++;
        }
        printIDVALOP(n);
        if(first) printf("\n");
        return;
    }

    if(n->tipo == CHAMADA){
        printf("t%d = call %s, 0\n", *reg, n->dado.id);
        (*reg)++;
        return;
    }

    if(n->esquerda)
        printOP(n->esquerda, reg, 1, label, param);

    int reg_left = *reg - 1;

    if(n->direita)
        printOP(n->direita, reg, 1, label, param);

    int reg_right = *reg - 1;

    if(n->tipo == OP){
        printf("t%d = t%d %s t%d\n", *reg, reg_left, operadorToString(n->dado.operador), reg_right);
        (*reg)++;
    }

    if(n->tipo == ID && n->direita){
        printf("t%d = %s[t%d]\n", *reg, n->dado.id, reg_right);
        (*reg)++;
    }
}

void printTAC(AST *n, int nivel, int *reg, int *label, int *param){
    if (!n) return;

    int skip = 0;
    int temp;

    switch(n->tipo){

    case VAL:
        if(*param > -1)
            printf("param %d\n", n->dado.valor);
        break;

    case ID:
        if(*param > -1)
            printf("param %s\n", n->dado.id);
        break;

    case OP:

        if(n->dado.operador == 4){

            printOP(n->direita, reg, 1, label, param);
            temp = *reg - 1;

            if(n->esquerda->direita){
                printOP(n->esquerda->direita, reg, 1, label, param);
                printf("t%d = t%d * 4\n", *reg, *reg - 1);
                (*reg)++;
                printf("%s[t%d] = t%d\n", n->esquerda->dado.id, *reg - 1, temp);
            }
            else{
                printf("%s = t%d\n", n->esquerda->dado.id, temp);
            }
            skip = 1;
        }
        else{
            printOP(n, reg, 1, label, param);
            if(*param > -1)
                printf("param t%d\n", *reg - 1);
            skip = 1;
        }
        break;

    case IF:
        printOP(n->esquerda, reg, 1, label, param);
        printf("ifFalse t%d goto L_end%d\n", *reg - 1, *label);
        temp = (*label)++;
        printTAC(n->meio, nivel+1, reg, label, param);
        printf("L_end%d:\n", temp);
        skip = 1;
        break;

    case IF_ELSE:
        printOP(n->esquerda, reg, 1, label, param);
        printf("ifFalse t%d goto L_else%d\n", *reg - 1, *label);
        temp = (*label)++;
        printTAC(n->meio, nivel+1, reg, label, param);
        printf("goto L_end%d\n", temp);
        printf("L_else%d:\n", temp);
        printTAC(n->direita, nivel+1, reg, label, param);
        printf("L_end%d:\n", temp);
        skip = 1;
        break;

    case WHILE:
        printf("L_while%d:\n", *label);
        temp = (*label)++;
        printOP(n->esquerda, reg, 1, label, param);
        printf("ifFalse t%d goto L_end_while%d\n", *reg - 1, temp);
        printTAC(n->direita, nivel+1, reg, label, param);
        printf("goto L_while%d\n", temp);
        printf("L_end_while%d:\n", temp);
        skip = 1;
        break;

    case FUNCAO:
        printf("L_%s:\n", n->dado.id);
        printTAC(n->direita, nivel+1, reg, label, param);
        skip = 1;
        break;

    case CHAMADA:
        if(strcmp(n->dado.id,"input")==0){
            printf("t%d = call input, 0\n", *reg);
            (*reg)++;
            skip = 1;
            break;
        }

        if(strcmp(n->dado.id,"output")==0){
            printTAC(n->direita, nivel+1, reg, label, param);
            if(n->direita->tipo == ID)
                printf("param %s\n", n->direita->dado.id);
            else
                printf("param t%d\n", *reg - 1);
            printf("call output, 1\n");
            skip = 1;
            break;
        }

        printTAC(n->direita, nivel+1, reg, label, param);
        printf("t%d = call %s, 1\n", *reg, n->dado.id);
        (*reg)++;
        skip = 1;
        break;

    case RETURN:
        if(n->esquerda){
            printOP(n->esquerda, reg, 1, label, param);
            printf("Return t%d\n", *reg - 1);
        } else {
            printf("Return\n");
        }
        skip = 1;
        break;

    default:
        break;
    }

    if(!skip){
        printTAC(n->esquerda, nivel+1, reg, label, param);
        printTAC(n->meio,   nivel+1, reg, label, param);
        printTAC(n->direita,nivel+1, reg, label, param);
    }
}
