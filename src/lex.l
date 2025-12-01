%option noyywrap

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define YY_DECL int yylex()

#include "parser.tab.h"

%}

%%

[ \t\r]+ ;

[0-9]+ {yylval.ival = atoi(yytext); return T_NUM;}
[a-zA-Z_][a-zA-Z0-9_]* {yylval.id = strdup(yytext; return T_ID;)}

"+" {return T_SOMA; yylval.op = SOMA;}
"-" {return T_MENOS; yylval.op = MENOS;}
"*" {return T_MULTI; yyval.op = MULTI;}
"/" {return T_DIVISAO; yyval.op = DIVISAO;}
"(" {return T_APARENTESES;}
")" {return T_FPARENTESES;}
"=" {return T_ATRIBUICAO; yyval.op = ATRIBUICAO;}
"{" {return T_ACHAVE;}
"}" {return T_FCHAVE;}
">" {return T_MAIOR; yyval.op = MAIOR;}
"<" {return T_MENOR; yyval.op = MENOR;}
";" {return T_PONTOEVIRGULA}
"," {return T_VIRGULA}
">=" {return T_MAIORI; yyval.op = MAIORI;}
"<=" {return T_MENORI; yyval.op = MENORI;}
"==" {return T_IGUAL; yyval.op = IGUAL;}
"!=" {return T_DIFERENTE; yyval.op = DIFERENTE;}
"/*" {return T_ACOMENTARIO;}
"*/" {return T_FCOMENTARIO;}
"[" {return T_ACOLCHETE;}
"]" {return T_FCOLCHETE;}

"if" {return T_IF;}
"else" {return T_ELSE;}
"while" {return T_WHILE;}
"int" {return T_INT;}
"void" {return T_VOID;}
"return" {return T_RETURN;}
"main" {return T_MAIN;}

"/n" {return T_NLINHA}

. {printf("Erro lexico, valor nao reconhecido);}

%%