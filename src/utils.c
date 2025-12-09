#include <stdio.h>
#include <string.h>

#include "ast.h"

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


