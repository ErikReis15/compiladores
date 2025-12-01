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

"if" {return T_IF;}
"else" {return T_ELSE;}
"while" {return T_WHILE;}
"int" {return T_INT;}
"void" {return T_VOID;}
"return" {return T_RETURN;}
"main" {return T_MAIN;}


">=" {yylval.op = MAIORI; return T_MAIORI;}
"<=" {yylval.op = MENORI; return T_MENORI; }
"==" {yylval.op = IGUAL; return T_IGUAL;}
"!=" {yylval.op = DIFERENTE; return T_DIFERENTE;}
">" {yylval.op = MAIOR; return T_MAIOR; }
"<" {yylval.op = MENOR; return T_MENOR;} 
";" {return T_PONTOEVIRGULA;}
"," {return T_VIRGULA;}

"+" {yylval.op = SOMA; return T_SOMA; }
"-" {yylval.op = MENOS; return T_MENOS;} 
"*" {yylval.op = MULTI; return T_MULTI;}
"/" {yylval.op = DIVISAO; return T_DIVISAO;}
"=" {yylval.op = ATRIBUICAO; return T_ATRIBUICAO;}

"(" {return T_APARENTESES;}
")" {return T_FPARENTESES;}
"{" {return T_ACHAVE;}
"}" {return T_FCHAVE;}

"/*" {return T_ACOMENTARIO;}
"*/" {return T_FCOMENTARIO;}
"[" {return T_ACOLCHETE;}
"]" {return T_FCOLCHETE;}

[0-9]+ {yylval.ival = atoi(yytext); return T_NUM;}
[a-zA-Z_][a-zA-Z0-9_]* {yylval.id = strdup(yytext); return T_ID;}

"\n" {return T_NLINHA;}

. {printf("Erro lexico, valor nao reconhecido %s\n",yytext);}

%%