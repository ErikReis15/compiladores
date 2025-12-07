#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tabelaSimbolo.h"
#include "ast.h"

Escopo * topo = NULL;
Simbolo *listaSimbolo = NULL;

void entraEscopo(char *local) { // PILHA
    Escopo *novoEscopo = (Escopo*) malloc(sizeof(Escopo));
    novoEscopo->simbolos = NULL;
    novoEscopo->prox = topo;
    novoEscopo->local = strdup(local);
    topo = novoEscopo; 
}

void saiEscopo() {
    if (topo == NULL) return;

    Simbolo *simbolo = topo->simbolos;
    while (simbolo) {
        salvaSimboloTabela(simbolo->nome, simbolo->tipo, simbolo->linha, topo->local);
        Simbolo *removida = simbolo;
        simbolo = simbolo->prox;
        free(removida->nome);
        free(removida);
    }

    Escopo *EscopoFechado = topo;
    topo = topo->prox;
    free(EscopoFechado->local);
    free(EscopoFechado);
}

void declara(char *nome, Tipo tipo, int linha) {
    if (!topo) {
        fprintf(stderr, "ERRO: declara chamada sem escopo (chame entraEscopo antes)\n");
    }
    
    for (Simbolo *s = topo->simbolos; s; s = s->prox) {
        if (!strcmp(s->nome, nome)) {
            printf("ERRO SEMANTICO: redeclaracao de '%s'\n", nome);
            return;
        }
    }

    Simbolo *s = malloc(sizeof(Simbolo));
    s->nome = strdup(nome);
    s->tipo = tipo;
    s->linha = linha;
    s->prox = topo->simbolos;
    topo->simbolos = s;

}

void salvaSimboloTabela(char *nome, Tipo tipo, int linha, char* escopo){
    
    Simbolo *s = (Simbolo*) malloc(sizeof(Simbolo));
    s->nome = strdup(nome);
    s->tipo = tipo;
    s->escopo = strdup(escopo);
    s->linha = linha;
    s->prox = NULL;

    if(listaSimbolo == NULL){
        listaSimbolo = s;
    }
    else{
        Simbolo *temp = listaSimbolo;
        while(temp->prox != NULL){
            temp = temp->prox;
        }
        temp->prox = s;
    }
};

Simbolo *busca(char *nome) {
    if (!topo) {
        fprintf(stderr, "ERRO: declara chamada sem escopo (chame entraEscopo antes)\n");
    }


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
            declara(n->dado.id, FUNCAO, n->linha);

            entraEscopo(n->dado.id);
            analisa(n->meio);
            analisa(n->direita);
            saiEscopo();
            break;

        case PARAM:
            declara(n->dado.id, PARAM, n->linha);
            break;

        case DECLARA:
            if(n->esquerda->tipo == VOID){
                printf("ERRO SEMANTICO: variavel '%s' declarada como void\n",
                       n->dado.id);
            }
            declara(n->dado.id, n->esquerda->tipo, n->linha);
            n->tipoValor = n->esquerda->tipo;
            break;

        case ID:
            Simbolo *s = busca(n->dado.id);
            if (!s)
                printf("ERRO SEMANTICO: variavel '%s' nao declarada\n",n->dado.id);
            else{
            n->tipoValor = s->tipo; 
            }
            break;
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

           if(n->esquerda->tipoValor == INDEFINIDO){
                printf("ERRO SEMANTICO: Tipo não declarado da variavel '%s'\n",n->esquerda->dado.id);
            }
            else if(n->esquerda->tipoValor != INT || n->direita->tipoValor != INT ){
                const char *left =
                    (n->esquerda->tipo == ID) ? n->esquerda->dado.id : "<expr>";

                const char *right =
                    (n->direita->tipo == ID) ? n->direita->dado.id : "<expr>";

                printf("ERRO SEMANTICO: operação inválida entre '%s' e '%s'\n",
                    left, right);
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

void imprimeTabela(Simbolo *listaSimbolo){
    Simbolo *tmpImprime = listaSimbolo;
    while(tmpImprime != NULL){
        printf("nome %s tipo %d linha %d escopo %s\n",tmpImprime->nome, tmpImprime->tipo, tmpImprime->linha, tmpImprime->escopo );
        tmpImprime = tmpImprime->prox;
    }
}