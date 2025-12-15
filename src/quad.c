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

void printTAC(AST *n, int nivel, int *reg, int *label, int *param);

void printOP(AST *n, int *reg, int first, int *label, int *param){
    if (!n) return;
    int reg_ant, reg_ant2 = -1, lado = 0;
    
    if(n->tipo == ID){
        if(n->direita){

            //printOP(n->direita, reg, first, label, param);
            
            // printTAC(n->direita, 0, reg, label, param);
            // printf("t%d = t%d", *reg, *reg-1);
            // *reg = *reg + 1;
            // printf(" * 4\n");
            // printf("t%d = ", *reg - 1);
            // *reg = *reg + 1;
        }
    }
    if(n->tipo == CHAMADA){
        if(first == -1){
            printf(" t%d", *reg-2);
            *param = *param + 1;
        }
        else{
            printf("a");
            printf("t%d", *reg-2);
        }

        return;
    }
    if(n->esquerda){
        if(n->esquerda->tipo == CHAMADA){
            printf("e");
            *param = -2;
            printTAC(n->esquerda, 0, reg, label, param);
        }
        
        if(n->esquerda->tipo != OP){
            printf("t%d = ", *reg);
            *reg = *reg + 1;

        }
    }
    reg_ant = *reg;
    printOP(n->esquerda, reg, 0, label, param);
    if(n->direita){
        if(n->direita->tipo == ID){
            if(n->direita->direita){
                printf("A");
                printTAC(n->direita->direita, 0, reg, label, param);
                printf("B");
                printf("t%d = t%d * 4", *reg, *reg-1);
                *reg = *reg + 1;
            }
        }
        if(n->direita->tipo == CHAMADA){
            printf("\n");
            reg_ant2 = *reg - 1;
            *param = - 2;
            printTAC(n->direita, 0, reg, label, param);
            lado = -1;
        }

        if(n->direita->tipo != OP){
            if(*reg != reg_ant){
                if(lado == -1){
                    printf("t%d = t%d", *reg, reg_ant2);
                    *reg = *reg + 1;
                }
                else{
                    printf("t%d = t%d", *reg, *reg-1);
                    *reg = *reg + 1;
                }
            }
            printIDVALOP(n);
            
        }
    }
     if(n->tipo == ID){
        if(n->direita){
            printf("[t%d]\n", *reg-2);
        }
    }
    if(!n->direita && !n->esquerda){

        if(first){
            printf("t%d = ", *reg);
            *reg = *reg + 1;
        }
        printIDVALOP(n);
        
        if(first)
            printf("\n");
    }
    
  
    printOP(n->direita, reg, lado, label, param);
    if(n->direita && n->esquerda && n->direita->tipo == OP && n->esquerda->tipo == OP){
        printf("t%d = t%d %s t%d", *reg, *reg-1, operadorToString(n->dado.operador), *reg-2);
        *reg = *reg + 1;
    }
    if(n->tipo == OP)
        printf("\n");
}


void printTAC(AST *n, int nivel, int *reg, int *label, int *param) {
    if (!n) return;
    int skip = 0, reg1 = 0, reg2 = 0, temp = 0, temp2 = 0;


    switch (n->tipo) {
        case VAL:
            if(*param > -1){
                printf("param %d\n", n->dado.valor);
            }
            
            break;
        case ID:
            printf("%d", *param);
            if(*param > -1){
                printf("param %s\n", n->dado.id);
            }
           
            break;
        case OP:
            switch(n->dado.operador){
                case 4: 
                    if(n->direita->tipo == CHAMADA){
                        *param = -2;
                        printTAC(n->direita, nivel + 1, reg, label, param);
                        printf("%s = t%d\n", n->esquerda->dado.id, *reg-1);
                    }
                    else{
                        printOP(n->direita, reg, 1, label, param);
                        
                        if(n->esquerda->direita){
                            temp = *reg - 1;
                            if(n->esquerda->direita->tipo == ID){
                                printf("t%d = %s", *reg, n->esquerda->direita->dado.id);
                                *reg = *reg + 1;
                            }
                            if(n->esquerda->direita->tipo == VAL){
                                printf("t%d = %d", *reg, n->esquerda->direita->dado.valor);
                                *reg = *reg + 1;
                            }
                            printTAC(n->esquerda->direita, nivel, reg, label, param);
                            printf("t%d = t%d * 4\n", *reg, *reg-1);
                            *reg = *reg + 1;
                            printf("%s[t%d] = t%d\n", n->esquerda->dado.id, *reg-1, temp);
                        }
                        else{

                            printf("%s = t%d\n", n->esquerda->dado.id, *reg-1);
                        }
                        //skip = 0;
                    }        
                    skip = 1;
                    break;
                case 0:
                case 1:
                case 2:
                case 3:
                    
                    printOP(n, reg, 1, label, param);
                    if(*param > -1){
                        //printf("%d", *param);
                        printf("param t%d\n", *reg-1);
                    }
                    skip = 1;
                    break;
                default:
                    break;
            }
            break;
        case IF:
            switch(n->esquerda->tipo){
                case VAL:
                    printf("t%d = (%d == 0)\n", *reg, n->esquerda->dado.valor);
                    break;
                case ID:
                    printf("t%d = (%s == 0)\n", *reg, n->esquerda->dado.id);
                    break;
                case OP:
                    printOP(n->esquerda->esquerda, reg, 1, label, param);
                    reg1 = *reg;
                    printOP(n->esquerda->direita, reg, 1, label, param);
                    reg2 = *reg; 
                    printf("t%d =  (t%d %s t%d)\n", *reg, reg1-1, o peradorToString(n->esquerda->dado.operador), reg2-1);
                    break;
            }
            printf("ifFalse t%d goto L_end%d\n", *reg, *label);
            temp = *label;
            *label = *label + 1;
            *reg = *reg + 1;
            printTAC(n->meio, nivel, reg, label, param);
            printf("L_end%d:\n", temp);
            skip = 1;
            
            break;
        case IF_ELSE:
            switch(n->esquerda->tipo){
                case VAL:
                    printf("t%d = (%d == 0)\n", *reg, n->esquerda->dado.valor);
                    break;
                case ID:
                    printf("t%d = (%s == 0)\n", *reg, n->esquerda->dado.id);
                    break;
                case OP:
                    printOP(n->esquerda->esquerda, reg, 1, label, param);
                    reg1 = *reg;
                    printOP(n->esquerda->direita, reg, 1, label, param);
                    reg2 = *reg; 
                    printf("t%d =  (t%d %s t%d)\n", *reg, reg1-1, operadorToString(n->esquerda->dado.operador), reg2-1);
                    break;
            }
            printf("ifFalse t%d goto L_else%d\n", *reg, *label);
            *reg = *reg + 1;
            temp = *label;
            *label = *label + 1;
            printTAC(n->meio, nivel, reg, label, param);
            printf("goto L_end%d\n", temp);
            printf("L_else%d:\n", temp);
            printTAC(n->direita, nivel, reg, label, param);

            printf("L_end%d:\n", temp);
            skip = 1;
            break;
        case WHILE:
            printf("L_while%d:\n", *label);
            temp = *label;
            *label = *label + 1;
            switch(n->esquerda->tipo){
                case VAL:
                    printf("t%d = (%d == 0)\n", *reg, n->esquerda->dado.valor);
                    break;
                case ID:
                    printf("t%d = (%s == 0)\n", *reg, n->esquerda->dado.id);
                    break;
                case OP:
                    printOP(n->esquerda->esquerda, reg, 1, label, param);
                    reg1 = *reg;
                    printOP(n->esquerda->direita, reg, 1, label, param);
                    reg2 = *reg; 
                    printf("t%d =  (t%d %s t%d)\n", *reg, reg1-1, operadorToString(n->esquerda->dado.operador), reg2-1);
                    break;
            }
            printf("ifFalse t%d goto L_end_while%d\n", *reg, temp);
            *reg = *reg + 1;

            printTAC(n->direita, nivel + 1, reg, label, param);

            printf("goto L_while%d\n", temp);
            printf("L_end_while%d:\n", temp);
            skip = 1;
            break;
        case SEQ:
            if(*param > -1){
                *param = *param + 1;
                printTAC(n->esquerda, nivel + 1, reg, label, param);
                printTAC(n->meio,   nivel + 1, reg, label, param);
                printTAC(n->direita, nivel + 1, reg, label, param);
                skip = 1;

            }
            break;
        case FUNCAO:
            printf("L_%s:\n", n->dado.id);
            printTAC(n->direita, nivel + 1, reg, label, param);
            skip = 1;
            break;
        case DECLARA: break;
        case INT: break;
        case VOID: break;
        case CHAMADA: 

            if(*param == -2){
                temp = 1;
            }
            if(*param == -2 || *param == -1){
                *param = 0;
                reg1 = 1;
                reg2 = *reg;
                //temp = 1;
            }
            else{
                temp = *param;
                *param = 0;
            }

            printTAC(n->direita, nivel + 1, reg, label, param);
            
            if(reg1 == 1 && temp != 1){
                
                printf("call %s, %d\n", n->dado.id, *param+1);
            }
            else{
                if(n->direita->tipo != ID && n->direita->tipo != OP && n->direita->tipo != VAL && n->direita->tipo != SEQ)
                    printf("param t%d\n", *reg-1);
                
                printf("t%d = call %s, %d\n", *reg, n->dado.id, *param+1);
                *reg = *reg + 1;
            }
            if(reg1 == 0){
                *param = temp;
            }
            if(reg1 == 1)
                *param = -1;
            
            skip = 1;
            break;
        case PARAM: break;
        case RETURN:
            if(n->esquerda){
                switch(n->esquerda->tipo){
                    case VAL:
                    printf("Return %d\n", n->esquerda->dado.valor);
                    break;
                    case ID:
                    printf("Return %s\n", n->esquerda->dado.id);
                    break;
                    case OP:
                    printTAC(n->esquerda, nivel + 1, reg, label, param);
                    printf("Return t%d\n", *reg-1);
                    break;
                }
            }
            else{
                printf("Return\n");
            }
            skip = 1;
            break;
        default:     
    }

   if(skip == 0){

        printTAC(n->esquerda, nivel + 1, reg, label, param);
        printTAC(n->meio,   nivel + 1, reg, label, param);
        printTAC(n->direita, nivel + 1, reg, label, param);
   }
   
  
}

