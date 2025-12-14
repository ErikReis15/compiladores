#include <stdio.h>
#include <string.h>

#include "ast.h"
#include "tabelaSimbolo.h"

char * operadorToString(Operador op) {
    switch (op) {
        case SOMA: return "+";
        case MENOS: return "-";
        case MULTI: return "*";
        case DIVISAO: return "/";
        case ATRIBUICAO: return "=";
        case MAIOR: return ">";
        case MENOR: return "<";
        case MAIORI: return ">=";
        case MENORI: return "<=";
        case IGUAL: return "==";
        case DIFERENTE: return "!=";

        default: return "?";
    }
}

char * tipoToString(Tipo tipo) {
    switch (tipo) {
        case INT: return "int";
        case VOID: return "void";
        case FUNCAO: return "funcao";
        case PARAM: return "parametro";
        default: return "erro";
    }
}

char * categoriaToString(Categoria categoria) {
    switch (categoria) {
        case C_VARIAVEL: return "variavel";
        case C_FUNCAO: return "funcao";
        case C_PARAMETRO: return "parametro";
        case C_VETOR: return "vetor";
        default: return "?";
    }
}