%option noyywrap

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define YY_DECL int yylex()

#include "ast.h"
#include "parser.tab.h"

int lineno = 1;
%}

%x COMMENT

%%

[ \t\r]+ ;
"/*"                     { BEGIN(COMMENT); }

<COMMENT>{
    "*/"                 { BEGIN(INITIAL); }
    "\n"                 { lineno++; }
    .                    ;
}

"if" {return T_IF;}
"else" {return T_ELSE;}
"while" {return T_WHILE;}
"int" {return T_INT;}
"void" {return T_VOID;}
"return" {return T_RETURN;}

">=" {return T_MAIORI;}
"<=" {return T_MENORI; }
"==" {return T_IGUAL;}
"!=" {return T_DIFERENTE;}
">" {return T_MAIOR; }
"<" {return T_MENOR;} 
";" {return T_PONTOEVIRGULA;}
"," {return T_VIRGULA;}

"+" {return T_SOMA; }
"-" {return T_MENOS;} 
"*" {return T_MULTI;}
"/" {return T_DIVISAO;}
"=" {return T_ATRIBUICAO;}

"(" {return T_APARENTESES;}
")" {return T_FPARENTESES;}
"{" {return T_ACHAVE;}
"}" {return T_FCHAVE;}

"[" {return T_ACOLCHETE;}
"]" {return T_FCOLCHETE;}

[0-9]+ {yylval.ival = atoi(yytext); return T_NUM;}
[a-zA-Z_][a-zA-Z0-9_]* {yylval.id = strdup(yytext); return T_ID;}

"\n" {lineno++;}

. {printf("ERRO LÉXICO: SIMBOLO INVALIDO '%s LINHA'%i'\n", yytext, lineno);}

%%