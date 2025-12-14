#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "utils.h"


void printIDVALOP(AST *n){
    switch (n->tipo){
        case ID:
        printf(" %s", n->dado.id);
        break;
        
        case VAL:
        printf(" %d", n->dado.valor);
        break;

        case OP:
        printf(" %s", operadorToString(n->dado.operador));
        break;
    }
}

void printOP(AST *n, int *reg, int first){
    if (!n) return;
    int reg_ant;
    if(n->esquerda){
        if(n->esquerda->tipo != OP){
            printf("t%d = ", *reg);
            *reg = *reg + 1;

        }
    }
    reg_ant = *reg;
    printOP(n->esquerda, reg, 0);
    if(n->direita){
        if(n->direita->tipo != OP){
            if(*reg != reg_ant){
                printf("t%d = t%d", *reg, *reg-1);
                *reg = *reg + 1;
            }
            printIDVALOP(n);
            
        }
    }
    if(!n->direita && !n->esquerda){

        if(first){
            printf("t%d = ", *reg);
            *reg = *reg + 1;
            //printf("r%d", reg);
        }
        printIDVALOP(n);
        if(first)
            printf("\n");
    }
    
    
    printOP(n->direita, reg, 0);
    //reg--;
    if(n->direita && n->esquerda && n->direita->tipo == OP && n->esquerda->tipo == OP){
        printf("t%d = t%d %s t%d", *reg, *reg-1, operadorToString(n->dado.operador), *reg-2);
        *reg = *reg + 1;
    }
    if(n->tipo == OP)
        printf("\n");
}


void printTAC(AST *n, int nivel, int *reg, int *label) {
    if (!n) return;
    int skip = 0, reg1, reg2, temp;


    for (int i = 0; i < nivel; i++) printf("   ");

    switch (n->tipo) {
        case VAL:     printf("(VAL %d)\n", n->dado.valor); break;
        case ID:      printf("(ID \"%s\")\n",n->dado.id); break;
        case OP:      printf("(OP %d)\n", n->dado.operador);
            switch(n->dado.operador){
                case 4: 
                    printf("\n#========= Atribuicao de %s =========\n", n->esquerda->dado.id);                   
                    printOP(n->direita, reg, 1);
                    skip = 1;
                    //reg = printTAC(n->direita, nivel + 1, reg, 1);
                    
                    printf("%s = t%d\n", n->esquerda->dado.id, *reg-1);
                    //skip = 0;
                    break;
                case 0:
                case 1:
                case 2:
                case 3:
                    printOP(n, reg, 1);

                    skip = 1;
                    break;
                default:
                    break;
            }
            break;
        case IF:      printf("(IF)\n");
            printf("#========= IF =========\n");
            switch(n->esquerda->tipo){
                case VAL:
                    printf("t%d = (%d == 0)\n", *reg, n->esquerda->dado.valor);
                    break;
                case ID:
                    printf("t%d = (%s == 0)\n", *reg, n->esquerda->dado.id);
                    break;
                case OP:
                    printOP(n->esquerda->esquerda, reg, 1);
                    reg1 = *reg;
                    printOP(n->esquerda->direita, reg, 1);
                    reg2 = *reg; 
                    printf("t%d =  (t%d %s t%d)\n", *reg, reg1-1, operadorToString(n->esquerda->dado.operador), reg2-1);
                    break;
            }
            printf("ifFalse t%d goto _Lend%d\n", *reg, *label);
            temp = *label;
            *label = *label + 1;
            *reg = *reg + 1;
            printTAC(n->meio, nivel, reg, label);
            printf("_Lend%d:\n", temp);
            skip = 1;
            
            break;
        case IF_ELSE: printf("(IF_ELSE)\n");
            printf("#========= IF_ELSE =========\n");
            switch(n->esquerda->tipo){
                case VAL:
                    printf("t%d = (%d == 0)\n", *reg, n->esquerda->dado.valor);
                    break;
                case ID:
                    printf("t%d = (%s == 0)\n", *reg, n->esquerda->dado.id);
                    break;
                case OP:
                    printOP(n->esquerda->esquerda, reg, 1);
                    reg1 = *reg;
                    printOP(n->esquerda->direita, reg, 1);
                    reg2 = *reg; 
                    printf("t%d =  (t%d %s t%d)\n", *reg, reg1-1, operadorToString(n->esquerda->dado.operador), reg2-1);
                    break;
            }
            printf("ifFalse t%d goto L_else%d\n", *reg, *label);
            *reg = *reg + 1;
            temp = *label;
            *label = *label + 1;
            printTAC(n->meio, nivel, reg, label);
            printf("goto L_end%d\n", temp);
            printf("L_else%d:\n", temp);
            printTAC(n->direita, nivel, reg, label);

            printf("L_end%d:\n", temp);
            skip = 1;
            break;
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

    //if(n->tipo != OP){
   if(skip == 0){

        printTAC(n->esquerda, nivel + 1, reg, label);
        printTAC(n->meio,   nivel + 1, reg, label);
        printTAC(n->direita, nivel + 1, reg, label);
   }
   
    //printf("ret%d", reg);
    

    //}
}

