#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "utils.h"

AST * novoNo(Tipo tipo){
    AST* no = (AST*) malloc(sizeof(AST));
    no->tipo = tipo;
    memset(&no->dado, 0, sizeof(no->dado));
    no->esquerda = NULL;
    no->direita = NULL;
    no->meio    = NULL;
    no->linha = 0;
    no->tipoValor = INDEFINIDO;
    return no;
}

void printAST(AST *n, int nivel, char c) {
    if (!n) return;

    for (int i = 0; i < nivel; i++) printf("   ");

    switch (n->tipo) {
        case VAL:     printf("%c-(VAL %d)\n", c,n->dado.valor); break;
        case ID:      printf("%c-(ID \"%s\")\n",c,n->dado.id); break;
        case OP:      printf("%c-(OP %s)\n", c,operadorToString(n->dado.operador)); break;
        case IF:      printf("%c-(IF)\n",c); break;
        case IF_ELSE: printf("%c-(IF_ELSE)\n",c); break;
        case WHILE:   printf("%c-(WHILE)\n",c); break;
        case SEQ:     printf("%c-(SEQ)\n",c); break;
        case FUNCAO:  printf("%c-(FUNCAO %s)\n",c, n->dado.id ); break;
        case DECLARA: printf("%c-(DECLARA %s)\n",c, n->dado.id ); break;
        case DECLARAVETOR: printf("%c-(DECLARAVETOR %s)\n",c, n->dado.id ); break;
        case INT:    printf("%c-(INT)\n",c); break;
        case VOID: printf("%c-(VOID)\n",c); break;
        case CHAMADA: printf("%c-(CHAMADA %s)\n",c, n->dado.id ); break;
        case PARAM:   printf("%c-(PARAM %s)\n",c, n->dado.id ); break;
        case RETURN:  printf("%c-(RETURN)\n",c); break;
        default:      printf("%c-(NODE %d)\n",c, n->tipo); break;
    }

    printAST(n->esquerda, nivel + 1, 'E');
    printAST(n->meio,   nivel + 1, 'M');
    printAST(n->direita, nivel + 1, 'D');
}

void gerarASTDotRec(AST *n, FILE *f) {
    if (!n) return;

    fprintf(f, "  n%p [label=\"", (void*)n);

    switch (n->tipo) {
        case VAL: fprintf(f, "VAL\\n%d", n->dado.valor); break;
        case ID: fprintf(f, "ID\\n%s", n->dado.id); break;
        case OP: fprintf(f, "OP\\n%s", operadorToString(n->dado.operador)); break;
        case IF: fprintf(f, "IF"); break;
        case IF_ELSE: fprintf(f, "IF_ELSE"); break;
        case WHILE: fprintf(f, "WHILE"); break;
        case SEQ: fprintf(f, "SEQ"); break;
        case FUNCAO: fprintf(f, "FUNCAO\\n%s", n->dado.id); break;
        case DECLARA: fprintf(f, "DECLARA\\n%s", n->dado.id); break;
        case DECLARAVETOR: fprintf(f, "DECLARAVETOR\\n%s", n->dado.id); break;
        case INT: fprintf(f, "INT"); break;
        case VOID: fprintf(f, "VOID"); break;
        case CHAMADA: fprintf(f, "CHAMADA\\n%s", n->dado.id); break;
        case PARAM: fprintf(f, "PARAM\\n%s", n->dado.id); break;
        case RETURN: fprintf(f, "RETURN"); break;
        default: fprintf(f, "NODE\\n%d", n->tipo); break;
    }

    fprintf(f, "\"];\n");

    if (n->esquerda) {
        fprintf(f, "  n%p -> n%p [label=\"E\"];\n",(void*)n, (void*)n->esquerda);
        gerarASTDotRec(n->esquerda, f);
    }
    if (n->meio) {
        fprintf(f, "  n%p -> n%p [label=\"M\"];\n",(void*)n, (void*)n->meio);
        gerarASTDotRec(n->meio, f);
    }
    if (n->direita) {
        fprintf(f, "  n%p -> n%p [label=\"D\"];\n",(void*)n, (void*)n->direita);
        gerarASTDotRec(n->direita, f);
    }
}

void gerarASTDot(AST *raiz, const char *arquivo) {
    FILE *f = fopen(arquivo, "w");
    if (!f) return;

    fprintf(f, "digraph AST {\n");
    gerarASTDotRec(raiz, f);
    fprintf(f, "}\n");
    fclose(f);
}
