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

Simbolo *buscaEscopo(char *nome) {
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
        declara(n->dado.id, n->esquerda->tipo, n->linha, C_FUNCAO,0);

        entraEscopo(n->dado.id);
        analisa(n->meio);
        analisa(n->direita);
        saiEscopo();

        if(n->direita->tipoValor != n->esquerda->tipo){
            printf("ERRO SEMANTICO: tipo de retorno da funcao '%s' nao corresponde (linha %d)\n",
                   n->dado.id, n->linha);
        }

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
        Simbolo *s = buscaEscopo(n->dado.id);
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

        if(n->direita->tipo != INDEFINIDO){
            n->tipoValor = n->direita->tipoValor;
        }

        break;

    case IF:
        analisa(n->esquerda);
        entraEscopo("if");
        analisa(n->meio);
        saiEscopo();

        n->tipoValor = n->meio->tipoValor;
        break;

    case WHILE:
        analisa(n->esquerda);
        entraEscopo("while");
        analisa(n->meio);
        saiEscopo();

        n->tipoValor = n->meio->tipoValor;
        break;

    case IF_ELSE:
        analisa(n->esquerda);

        entraEscopo("if");
        analisa(n->meio);
        saiEscopo();

        entraEscopo("else");
        analisa(n->direita);
        saiEscopo();

        if(n->meio->tipoValor == n->direita->tipoValor){
            n->tipoValor = n->meio->tipoValor;
        }

         else{
            printf("ERRO SEMANTICO: tipos diferentes de retorno no if-else (linha %d)\n", n->linha);
        }

        break;

    case RETURN:
        analisa(n->esquerda);
        if(n->esquerda != NULL){
            n->tipoValor = n->esquerda->tipoValor;
        }
        else{
            n->tipoValor = VOID;
        }
        break;

    case OP:
        analisa(n->esquerda);
        analisa(n->direita);

        if ((n->esquerda->tipoValor != INT || n->direita->tipoValor != INT)) {
            printf("ERRO SEMANTICO: operacao invalida (linha %d)\n", n->esquerda->linha);
        }

        if (n->direita && n->direita->tipo == CHAMADA){
            if (n->dado.operador == ATRIBUICAO && strcmp(n->direita->dado.id,"input") == 0 && n->esquerda->tipoValor == VOID) {
                printf("ERRO SEMANTICO: atribuicao de valor de void para 'input' (linha %d)\n", n->esquerda->linha);

            }

            if (n->dado.operador == ATRIBUICAO && strcmp(n->direita->dado.id,"output") == 0) {
                printf("ERRO SEMANTICO: 'output' não pode ser atribuido (linha %d)\n", n->esquerda->linha);

            }
        }
        n->tipoValor = INT;
        break;
    
    case CHAMADA: 
        Simbolo *s = buscaTabela(n->dado.id);
        if (s == NULL){
        printf("ERRO SEMANTICO: chamada de funcao '%s' nao declarada (linha %d)\n", n->dado.id, n->linha);
        }
        else if (s->categoria == C_FUNCAO){ 
            analisa(n->direita);
            n->tipoValor = s->tipo;
        }
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