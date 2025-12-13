#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "semantico.h"
#include "utils.h"

Escopo *topo = NULL;

void entraEscopo(char *local) {
    Escopo *novo = malloc(sizeof(Escopo));
    novo->simbolos = NULL;
    novo->local = strdup(local);
    novo->prox = topo;
    topo = novo;
}

void saiEscopo() {
    if (!topo) return;

    Simbolo *s = topo->simbolos;
    while (s) {
        Simbolo *tmp = s;
        s = s->prox;
        free(tmp->nome);
        free(tmp);
    }

    Escopo *fechado = topo;
    topo = topo->prox;
    free(fechado->local);
    free(fechado);
}

void declara(char *nome, Tipo tipo, int linha, Categoria categoria, int tamanho) {
    if (!topo) {
        printf("ERRO INTERNO: sem escopo ativo\n");
        return;
    }

    for (Simbolo *s = topo->simbolos; s; s = s->prox) {
        if (!strcmp(s->nome, nome)) {
            printf("ERRO SEMANTICO: redeclaracao de '%s' 'LINHA %d'\n", nome, linha);
            return;
        }
    }

    salvaSimboloTabela(nome, tipo, linha, topo->local, categoria, tamanho);

    Simbolo *s = malloc(sizeof(Simbolo));
    s->nome = strdup(nome);
    s->tipo = tipo;
    s->linha = linha;
    s->escopo = NULL;   
    s->prox = topo->simbolos;
    topo->simbolos = s;
}

Simbolo *busca(char *nome) {
    for (Escopo *e = topo; e; e = e->prox) {
        for (Simbolo *s = e->simbolos; s; s = s->prox) {
            if (!strcmp(s->nome, nome))
                return s;
        }
    }
    return NULL;
}

void analisa(AST *n) {
    if (!n) return;

    switch (n->tipo) {

    case VAL:
        n->tipoValor = INT;
        break;

    case FUNCAO:
        declara(n->dado.id, INT, n->linha, C_FUNCAO,0);

        entraEscopo(n->dado.id);
        analisa(n->meio);
        analisa(n->direita);
        saiEscopo();
        break;

    case PARAM:
        declara(n->dado.id, n->esquerda->tipo, n->linha, C_PARAMETRO,0);
        n->tipoValor = n->esquerda->tipo;
        break;

    case DECLARA:
        if (n->esquerda->tipo == VOID) {
            printf("ERRO SEMANTICO: variavel '%s' declarada como void (linha %d)\n",
                   n->dado.id, n->linha);
        }
        declara(n->dado.id, n->esquerda->tipo, n->linha, C_VARIAVEL,0);
        n->tipoValor = n->esquerda->tipo;
        break;
    
    case DECLARAVETOR:
        if (n->esquerda->tipo == VOID) {
            printf("ERRO SEMANTICO: variavel '%s' declarada como void (linha %d)\n",
                   n->dado.id, n->linha);
        }
        declara(n->dado.id, n->esquerda->tipo, n->linha, C_VARIAVEL, n->direita->dado.valor);
        n->tipoValor = n->esquerda->tipo;
        break;

    case ID: {
        Simbolo *s = busca(n->dado.id);
        if (!s){
            printf("ERRO SEMANTICO: variavel '%s' nao declarada 'LINHA - %d'\n", n->dado.id, n->linha);
        }
        else if (n->direita != NULL && n->direita->dado.valor > s->tamanho) {
            printf("ERRO SEMANTICO: indice fora do limite '%s' (linha %d)\n", n->dado.id, n->linha);
        }
            n->tipoValor = s->tipo;
        break;
    }

    case SEQ:
        analisa(n->esquerda);
        analisa(n->direita);
        break;

    case IF:
        analisa(n->esquerda);
        entraEscopo("if");
        analisa(n->meio);
        saiEscopo();
        break;

    case WHILE:
        analisa(n->esquerda);
        entraEscopo("while");
        analisa(n->meio);
        saiEscopo();
        break;

    case IF_ELSE:
        analisa(n->esquerda);

        entraEscopo("if");
        analisa(n->meio);
        saiEscopo();

        entraEscopo("else");
        analisa(n->direita);
        saiEscopo();
        break;

    case RETURN:
        analisa(n->esquerda);
        break;

    case OP:
        analisa(n->esquerda);
        analisa(n->direita);

        if (n->esquerda->tipoValor != INT ||
            n->direita->tipoValor != INT) {
            printf("ERRO SEMANTICO: operacao invalida (linha %d)\n", n->linha);
        }

        n->tipoValor = INT;
        break;
    
    default:
        analisa(n->esquerda);
        analisa(n->meio);
        analisa(n->direita);
        break;
    }
}

void analisaRegra(Simbolo *tabela){
    Simbolo *s = tabela;
    int verificaRegras = 0;
    while(s){
        if(s->categoria == C_FUNCAO && (strcmp(s->nome, "main") == 0)){
            verificaRegras = 1;
        }
        s = s->prox;
    }
    if(!verificaRegras){
        printf("ERRO SEMANTICO: funcao main nao declarada\n");
    }
}

void inicializacaoRegrasGlobais(){
    salvaSimboloTabela("input", INT, 0, "global", C_FUNCAO,0);
    salvaSimboloTabela("output", VOID, 0, "global", C_FUNCAO,0);
}