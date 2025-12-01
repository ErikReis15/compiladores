#ifndef AST_H
#define AST_H

typedef enum { OP, VAL } Tipo;

typedef struct AST {
    Tipo tipo;
    union {
        char operador;
        float valor;
    } dado; 
    struct AST* esquerda;
    struct AST* direita;
} AST;

AST* NoOp(char operador, AST* esquerda, AST* direita);
AST* NoValor(float valor);

#endif 