#ifndef SEMANTICO_H
#define SEMANTICO_H

#include "ast.h"
#include "tabelaSimbolo.h"

typedef struct Escopo {
    Simbolo *simbolos;    
    char *local;
    struct Escopo *prox;
} Escopo;

void entraEscopo(char *local);
void saiEscopo();
void declara(char *nome, Tipo tipo, int linha, Categoria categoria);
Simbolo *busca(char *nome);
void analisa(AST *n);
void analisaRegra(Simbolo *tabela);

#endif
