#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabelaSimbolo.h"
#include "utils.h"

Simbolo *listaSimbolo = NULL;

void salvaSimboloTabela(char *nome, Tipo tipo, int linha, char *escopo, Categoria categoria, int tamanho) {
    Simbolo *s = malloc(sizeof(Simbolo));

    s->nome   = strdup(nome);
    s->tipo   = tipo;
    s->linha  = linha;
    s->tamanho = tamanho;
    s->escopo = strdup(escopo);
    s->categoria = categoria;
    s->prox   = NULL;

    if (!listaSimbolo) {
        listaSimbolo = s;
    } else {
        Simbolo *tmp = listaSimbolo;
        while (tmp->prox)
            tmp = tmp->prox;
        tmp->prox = s;
    }
}

void imprimeTabela(Simbolo *listaSimbolo) {
    printf("\n----------- TABELA DE SIMBOLOS -----------\n");
    printf("| NOME | TIPO | LINHA | ESCOPO | CATEGORIA |\n");

    for (Simbolo *s = listaSimbolo; s; s = s->prox) {
        printf("| %s | %s | %d | %s | %s |\n",
               s->nome,
               tipoToString(s->tipo),
               s->linha,
               s->escopo,
               categoriaToString(s->categoria));
    }
}
