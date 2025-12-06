#ifndef AST_H
#define AST_H

    typedef enum {
        VAL, ID, OP, IF, WHILE, SEQ, INT, VOID, PARAM, CHAMADA, FUNCAO, IF_ELSE, RETURN 
    } Tipo;

    typedef enum {
        SOMA, MENOS, MULTI, DIVISAO, ATRIBUICAO, MAIOR, MENOR, MAIORI, MENORI, IGUAL, DIFERENTE
    } Operador;

typedef struct AST {
    Tipo tipo;
    union {
        char *id;
        int valor;
        Operador operador;
    } dado; 
    struct AST* esquerda;
    struct AST* direita;
    struct AST* meio;
} AST;

AST *novoNo(Tipo tipo);

// void printAST(AST *n, int nivel);

void printTree(AST* root);
int getHeight(AST* root);
void fillBuffer(AST* node, int level, int left, int right);


#endif 