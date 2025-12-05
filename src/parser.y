%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    extern int yylex(void);
    extern int yyparse(void);
    extern FILE *yyin;

    void yyerror(const char *s);

    typedef enum {
        SOMA, MENOS, MULTI, DIVISAO, ATRIBUICAO, MAIOR, MENOR, MAIORI, MENORI, IGUAL, DIFERENTE,

        VAL, ID, OP, IF, WHILE, SEQ, INT, VOID, PARAM, CHAMADA, FUNCAO
    } Tipo;

%}

%union{
    int ival;
    char *id;
    AST* ast;
    Tipo op;
}

%token<ival> T_NUM
%token<id> T_ID

%token <op> T_SOMA T_MENOS T_MULTI T_DIVISAO T_MAIOR T_MENOR T_MAIORI T_MENORI T_IGUAL T_DIFERENTE
%token T_ACOMENTARIO T_FCOMENTARIO T_ACOLCHETE T_FCOLCHETE T_APARENTESES T_FPARENTESES T_ATRIBUICAO T_ACHAVE T_FCHAVE T_PONTOEVIRGULA T_VIRGULA

%token T_IF T_ELSE T_WHILE T_INT T_VOID T_RETURN T_MAIN

%token T_NLINHA

%left T_SOMA T_MENOS 
%left T_MULTI T_DIVISAO

%type <ast> soma_exp termo fator simples_exp exp
%type <ast> rel soma mult
%type <ast> var state exp_decl sel_decl ite_decl


%start programa

%%

programa:
    decl_lista
    ;

decl_lista:
    decl_lista decl { $$ = novoNo(SEQ)}
    | decl {$$ = $1}
    ;

decl:
    var_decl {$$ = $1}
    | fun_decl {$$ = $1}
    ;

var_decl:
    tipo_espec T_ID T_PONTOEVIRGULA {
        $$ = novoNo(ID);
        $$->dado.id = $2;
        $$->esquerda = $1;
    }
    | tipo_espec T_ID T_ACOLCHETE T_NUM T_FCOLCHETE T_PONTOEVIRGULA {
        $$ = novoNo(ID);
        $$->dado.id = $2;
        $$->esquerda = $1;
        $$->direita = $4;
        $$->direita->dado.valor = $4;
    }
    ;

tipo_espec:
    T_INT {$$ = novoNo(INT);}
    | T_VOID {$$ = novoNo(VOID);}
    ;

fun_decl:
    tipo_espec T_ID T_APARENTESES params T_FPARENTESES comp_decl
    {
        $$ = novoNo(FUNCAO);
        $$->dado.id = $2;
        $$->esquerda = $1;
        $$->meio = $4;
        $$->direita = $6;
    }
    ;

params:
    param_list {$$ = $1;}
    | T_VOID {$$ = novoNo(VOID);}
    ; 

param_list:
    param_list T_VIRGULA param
    | param
    ;

param:
    tipo_espec T_ID
    | tipo_espec T_ID T_ACOLCHETE T_FCOLCHETE
    ;

comp_decl:
    T_ACHAVE local_decl state_lista T_FCHAVE
    ;

local_decl:
    local_decl var_decl
    |
    ;

state_lista:
    state_lista state
    |
    ;

state:
    exp-decl {$$ = $1}
    | comp_decl {$$ = $1}
    | sel_decl {$$ = $1}
    | ite_decl {$$ = $1}
    | ret_decl {$$ = $1}
    ;

exp_decl:
    exp T_PONTOEVIRGULA {
        $$ = $1;
        }
    | T_PONTOEVIRGULA {$$ = NULL;}
    ;

sel_decl:
    T_IF T_APARENTESES exp T_FPARENTESES state {
        $$ = novoNo(IF);
        $$->dado.operador = IF;
        $$->esquerda = $3;
        $$->direita = $5;
        }

    | T_IF T_APARENTESES exp T_FPARENTESES state T_ELSE state {
        $$ = novoNo(IF);
        $$->dado.operador = IF;
        $$->esquerda = $3;
        $$->meio = $5;
        $$->direita = $7;
        }
    ;

ite_decl:
    T_WHILE T_APARENTESES exp T_FPARENTESES state {
        $$ = novoNo(WHILE);
        $$ = dado.operador = WHILE;
        $$->esquerda = $3;
        $$->direita = $5;
        }
    ;

ret_decl:
    T_RETURN T_PONTOEVIRGULA
    | T_RETURN exp T_PONTOEVIRGULA
    ;

exp:
    var T_ATRIBUICAO exp{
        $$ = novoNo(OP);
        $$->dado.operador = ATRIBUICAO;
        $$->esquerda = $1;
        $$->direita = $3
        }
    | simples_exp {$$ = $1;}
    ;

var:
    T_ID {
        $$ = novoNo(ID);
        $$->dado.id = $1;
        }

    | T_ID T_ACOLCHETE exp T_FCOLCHETE {
        $$ = novoNo(ID);
        $$ = $1;
        $$->direita = $3;
        }
    ;

simples_exp:
    soma_exp rel soma_exp {
        $$ = $2;
        $$->esquerda = $1;
        $$->direita = $3;
        }
    | soma_exp {$$ = $1;}
 
rel:
    T_MENORI {$$ = novoNo(OP);
              $$->dado.operador = MENORI;}
    | T_MENOR {$$ = novoNo(OP);
               $$->dado.operador = MENOR;}
    | T_MAIOR {$$ = novoNo(OP);
               $$->dado.operador = MAIOR;}
    | T_MAIORI {$$ = novoNo(OP);
                $$->dado.operador = MAIORI;}
    | T_IGUAL {$$ = novoNo(OP);
               $$->dado.operador = IGUAL;}
    | T_DIFERENTE {$$ = novoNo(OP);
               $$->dado.operador = DIFERENTE;}
    ;

soma_exp:
    soma_exp soma termo{
        $$ = $2;
        $$->esquerda = $1;
        $$->direita = $3;
    }
    | termo {$$ = $1;}
    ;

soma:
    T_SOMA {
        $$ = novoNo(OP);
        $$->dado.operador = SOMA;
        }
    | T_MENOS {
        $$ = novoNo(OP);
        $$->dado.operador = MENOS;
        }
    ;

termo:
    termo mult fator {
        $$ = $2;
        $$->esquerda = $1;
        $$->direita = $3;
        }
    | fator {$$ = $1;}
    ;

mult:
    T_MULTI {
        $$ = novoNo(OP);
        $$->dado.operador = MULTI;
        }
    | T_DIVISAO {
        $$ = novoNo(OP);
        $$->dado.operador = DIVISAO;
        }
    ;

fator:
    T_APARENTESES exp T_FPARENTESES {$$ = $2}
    | var {$$ = $1}
    | ativacao {$$ = $1}
    | T_NUM {
        $$ = novoNo(VAL);
        $$->dado.int = $1;
        }
    ;

ativacao: 
    T_ID T_APARENTESES args T_FPARENTESES{
        $$ = novoNo(CHAMADA);
        $$->dado.id = $1;
        $$->direita = $3;
        }
    ;

args:
    args_list {$$ = $1}
    | {$$ = NULL;}
    ;

args_list:
    args_list T_VIRGULA exp {
        $$ = novoNo(SEQ);
        $$->esquerda = $1;
        $$->direita = $3;
    }
    | exp {$$ = $1;}
    ;