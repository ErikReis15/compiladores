#ifndef AST_H
#define AST_H

    typedef enum {
        VAL, ID, OP, IF, WHILE, SEQ, INT, VOID, PARAM, CHAMADA, FUNCAO, IF_ELSE, RETURN, DECLARA, DECLARAVETOR, INDEFINIDO
    } Tipo;

    typedef enum {
        SOMA, MENOS, MULTI, DIVISAO, ATRIBUICAO, MAIOR, MENOR, MAIORI, MENORI, IGUAL, DIFERENTE
    } Operador;

typedef struct AST {
    Tipo tipo;
    Tipo tipoValor;  // int ou void para variavel e retornos
    union {
        char *id;
        int valor;
        Operador operador;
    } dado; 
    int linha;
    struct AST* esquerda;
    struct AST* direita;
    struct AST* meio;
} AST;

AST *novoNo(Tipo tipo);

void printAST(AST *n, int nivel);

#endif 