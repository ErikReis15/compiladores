%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    extern int yylex(void);
    extern int yyparse(void);
    extern FILE *yyin;

    void yyerror(const char *s);

    typedef enum {
        SOMA, MENOS, MULTI, DIVISAO, ATRIBUICAO, MAIOR, MENOR, MAIORI, MENORI, IGUAL, DIFERENTE
    } Operador;

%}

%union{
    int ival;
    char *id;
    Operador op;
}

%token<ival> T_NUM
%token<id> T_ID

%token <op> T_SOMA T_MENOS T_MULTI T_DIVISAO T_MAIOR T_MENOR T_MAIORI T_MENORI T_IGUAL T_DIFERENTE
%token T_ACOMENTARIO T_FCOMENTARIO T_ACOLCHETE T_FCOLCHETE T_APARENTESES T_FPARENTESES T_ATRIBUICAO T_ACHAVE T_FCHAVE T_PONTOEVIRGULA T_VIRGULA

%token T_IF T_ELSE T_WHILE T_INT T_VOID T_RETURN T_MAIN

%token T_NLINHA

%left T_SOMA T_MENOS 
%left T_MULTI T_DIVISAO

%type <op> rel soma mult
%type <ival> soma_exp termo fator simples_exp exp


%start programa

%%

programa:
    decl_lista
    ;

decl_lista:
    decl_lista decl
    | decl
    ;

decl:
    var-decl
    | fun-decl
    ;

var_decl:
    tipo-espec T_ID T_PONTOEVIRGULA
    | tipo-espec T_ID T_ACOLCHETE T_NUM T_FCOLCHETE T_PONTOEVIRGULA
    ;

tipo_espec:
    T_INT {$$ = $1}
    | T_VOID {{$$ = $1}}
    ;

fun-decl:
    tipo_espec T_ID T_APARENTESES params T_FPARENTESES comp_decl
    ;

params:
    param_list
    | T_VOID
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
    exp-decl 
    | comp_decl
    | sel_decl
    | ite_decl
    | ret_decl
    ;

exp_decl:
    exp T_PONTOEVIRGULA
    | T_PONTOEVIRGULA
    ;

sel_decl:
    T_IF T_APARENTESES exp T_FPARENTESES state
    | T_IF T_APARENTESES exp T_FPARENTESES state T_ELSE state
    ;

ite_decl:
    T_WHILE T_APARENTESES exp T_FPARENTESES state
    ;

ret_decl:
    T_RETURN T_PONTOEVIRGULA
    | T_RETURN exp T_PONTOEVIRGULA
    ;

exp:
    var T_ATRIBUICAO 
    | simples_exp
    ;

var:
    T_ID
    | T_ID T_ACOLCHETE exp T_FCOLCHETE
    ;

simples_exp:
    soma_exp rel soma_exp {
        switch ($2){
            case MENORI:
                $$ = $1 <= $3;                
                break;
            case MENOR:
                $$ = $1 < $3;
                break;
            case MAIOR:
                $$ = $1 > $3;
                break;
            case MAIORI:
                $$ = $1 >= $3;
                break;
            case IGUAL:
                $$ = $1 == $3;
                break;
            case DIFERENTE:
                $$ = $1 != $3;
                break;
            default:
                fprintf(stderr("Erro semantico, esperado sinais >,>=,<,<=,==,!=, recebido:) $2);
        }
        
    }
    | soma_exp { $$ = $1; }
 
rel:
    T_MENORI {$$ = MENORI;}
    | T_MENOR {$$ = MENOR;}
    | T_MAIOR {$$ = MAIOR;}
    | T_MAIORI {$$ = MAIORI;}
    | T_IGUAL {$$ = IGUAL;}
    | T_DIFERENTE {$$ = DIFERENTE;}
    ;

soma_exp:
    soma_exp soma termo{
        if ($2 = SOMA){
            $$ = $1 + $3;
        }
        elseif ($2 = MENOS){
            $$ = $1 - $3;
        }
        else{
            fprintf(stderr, "Esperado + ou -, recebido", $2);
        }
    }
    | termo { $$ = $1; }
    ;

soma:
    T_SOMA {$$ = SOMA;}
    | T_MENOS {$$ = MENOS;}
    ;

termo:
    termo mult fator {
        if($2 = MULTI){
            $$ = $1 * $3;
        }
        else{
            if($3 !=0){
                $$ = $1 / $3;
            }
            else{
                fprintf(stderr, "Erro semantico: divisao por 0 ");
            }
        }
    }
    | fator {$$ = $1;}
    ;

mult:
    T_MULTI {$$ = MULTI;}
    | T_DIVISAO {$$ = DIVISAO;}
    ;

fator:
    T_APARENTESES exp T_FPARENTESES {$$ = $2}
    | var   {$$ = $1}
    | ativacao {$$ = $1}
    | T_NUM {$$ = $1}
    ;

ativacao: 
    T_ID T_APARENTESES args T_FPARENTESES
    ;

args:
    args_list
    |
    ;

args_list:
    args_list T_VIRGULA exp
    | exp
    ;