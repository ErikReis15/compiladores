#include "ast.h"

typedef struct Simbolo {
    char *nome;
    Tipo tipo;
    int linha;
    char *escopo;
    struct Simbolo *prox;
} Simbolo;

typedef struct Escopo {
    Simbolo *simbolos;
    char *local;
    struct Escopo *prox;
} Escopo;

void entraEscopo(char *local);
void saiEscopo();
void salvaSimboloTabela(char *nome, Tipo tipo, int linha, char* escopo);
void declara(char *nome, Tipo tipo, int linha);
void analisa(AST *n);
Simbolo *busca(char *nome);
void imprimeTabela(Simbolo *listaSimbolo);
