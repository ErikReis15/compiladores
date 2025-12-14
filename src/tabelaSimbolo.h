#ifndef TABELA_SIMBOLO_H
#define TABELA_SIMBOLO_H

#include "ast.h"

typedef enum {
    C_VARIAVEL,
    C_FUNCAO,
    C_PARAMETRO
} Categoria;

typedef struct Simbolo {
    char *nome;
    Tipo tipo;
    int linha;
    int tamanho;
    Categoria categoria;
    char *escopo;
    struct Simbolo *prox;
} Simbolo;

extern Simbolo *listaSimbolo;

void salvaSimboloTabela(char *nome, Tipo tipo, int linha, char *escopo, Categoria categoria, int tamanho);
void imprimeTabela(Simbolo *listaSimbolo);
Simbolo *buscaTabela(char *nome);

#endif
