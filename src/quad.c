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
    if(n->tipo == SEQ){
        printf("seq");
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
        if(n->direita->tipo == CHAMADA){
            printf("d");
            printf("\n");
            reg_ant2 = *reg - 1;
            *param = - 2;
            printTAC(n->direita, 0, reg, label, param);
            printf("dasd ");
            lado = -1;
        }

        if(n->direita->tipo != OP){
            if(*reg != reg_ant){
                if(lado == -1){
                    printf("t%d = t%d", *reg, reg_ant2);
                    *reg = *reg + 1;
                }
                else{
                    printf("t%d = pt%d", *reg, *reg-1);
                    *reg = *reg + 1;
                }
            }
            //printf("e");
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
    
  
    printOP(n->direita, reg, lado, label, param);
    //reg--;
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


    for (int i = 0; i < nivel; i++) printf("   ");

    switch (n->tipo) {
        case VAL:     printf("(VAL %d)\n", n->dado.valor);
            if(*param > -1){
                printf("param %d\n", n->dado.valor);
                //*param = *param + 1; 
            }
            break;
        case ID:      printf("(ID \"%s\")\n",n->dado.id);
            printf("%d", *param);
            if(*param > -1){
                printf("param %s\n", n->dado.id);
                //*param = *param + 1; 
            }
            break;
        case OP:      printf("(OP %d)\n", n->dado.operador);
            switch(n->dado.operador){
                case 4: 
                    printf("\n#========= Atribuicao de %s =========\n", n->esquerda->dado.id);    
                    if(n->direita->tipo == CHAMADA){
                        *param = -2;
                        printTAC(n->direita, nivel + 1, reg, label, param);
                        printf("%s = t%d\n", n->esquerda->dado.id, *reg-1);
                    }
                    else{

                        printOP(n->direita, reg, 1, label, param);
                        //reg = printTAC(n->direita, nivel + 1, reg, 1);
                        
                        printf("%s = t%d\n", n->esquerda->dado.id, *reg-1);
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
                        printf("%d", *param);
                        printf("param t%d\n", *reg-1);
                        //*param = *param + 1; 
                    }
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
                    printOP(n->esquerda->esquerda, reg, 1, label, param);
                    reg1 = *reg;
                    printOP(n->esquerda->direita, reg, 1, label, param);
                    reg2 = *reg; 
                    printf("t%d =  (t%d %s t%d)\n", *reg, reg1-1, operadorToString(n->esquerda->dado.operador), reg2-1);
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
        case WHILE:   printf("(WHILE)\n");
            printf("#========= WHILE =========\n");
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
        case SEQ:     printf("(SEQ)\n");
            if(*param > -1){
                *param = *param + 1;
                printTAC(n->esquerda, nivel + 1, reg, label, param);
                printTAC(n->meio,   nivel + 1, reg, label, param);
                printTAC(n->direita, nivel + 1, reg, label, param);
                skip = 1;

            }
            break;
        case FUNCAO:  printf("(FUNCAO %s)\n", n->dado.id );
            printf("#========= FUNCAO =========\n");
            printf("L_%s:\n", n->dado.id);
            printTAC(n->direita, nivel + 1, reg, label, param);
            skip = 1;
            break;
        case DECLARA: printf("(DECLARA %s)\n", n->dado.id ); break;
        case INT: printf("(INT)\n"); break;
        case VOID: printf("(VOID)\n"); break;
        case CHAMADA: printf("(CHAMADA %s)\n", n->dado.id );
            // if(*param > 0){
            //     printf("pa%d", *param);
            //     //temp = *param;
            //     temp2 = *param;
            //     *param = 0;

            // }
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
            //printOP(n->direita, reg, 1);
            
            if(reg1 == 1 && temp != 1){
                // if(reg1 == 0){
                //     printf("t%d = ", *reg);
                //     *reg = *reg + 1;
                // }
                printf("%d %d %d %d", reg1, temp, reg2, *param);
                printf("call %s, %d\n", n->dado.id, *param+1);
            }
            else{
                if(n->direita->tipo != ID && n->direita->tipo != OP && n->direita->tipo != VAL && n->direita->tipo != SEQ)
                    printf("eparam t%d %d %d %d %d \n", *reg-1, *param, temp, reg1, reg2);
                //*param += *reg - reg2 - 1;
                //printf("%d, %d", *reg, reg2);
                printf("t%d = call %s, %d\n", *reg, n->dado.id, *param+1);
                *reg = *reg + 1;
            }
            if(reg1 == 0){
                *param = temp;
            }
            if(reg1 == 1)
                *param = -1;
            // else
            //     *param = 0;
            // if(temp2 > 0 && reg1 != 1){
            //     printf("temp2%d", temp2);
            //     *param = temp2;
            // }
            
            skip = 1;
            break;
        case PARAM:   printf("(PARAM %s)\n", n->dado.id ); break;
        case RETURN:  printf("(RETURN)\n");
            printf("#========= RETURN =========\n");
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
            skip = 1;
            break;
        default:      printf("(NODE %d)\n", n->tipo); break;
    }

    //if(n->tipo != OP){
   if(skip == 0){

        printTAC(n->esquerda, nivel + 1, reg, label, param);
        printTAC(n->meio,   nivel + 1, reg, label, param);
        printTAC(n->direita, nivel + 1, reg, label, param);
   }
   
    //printf("ret%d", reg);
    

    //}
}

