%{
#include <stdio.h>
#include "parsing/actions.h"
#include "symbol.h"
#include "utility.h"
#include "parser.h"

int yycolumn = 1;

#define YY_USER_ACTION yylloc->first_line = yylloc->last_line = yylineno; \
    yylloc->first_column = yycolumn; \
    yylloc->last_column = yycolumn + yyleng - 1; \
    yycolumn += yyleng;
%}

%option bison-locations
%option yylineno

identifier      [_a-zA-Z][_a-zA-Z0-9]*
integer         [0-9]+
float           [0-9]*\.[0-9]+([eE][-+]?[0-9]+)?

%%

[{}()\[\],;=]   return yytext[0];
[\+\-\*\/]      return yytext[0];

int             return T_INT;
float           return T_FLOAT;
matrix          return T_MATRIX;

if              return IF;
else            return ELSE;
true            return TRUE;
false           return FALSE;
\|\|            return OR;
&&              return AND;
!               return NOT;
==              return EQ;
!=              return NEQ;
\<              return LT;
>               return GT;
\<=             return LTE;
>=              return GTE;

while           return WHILE;
do              return DO;
for             return FOR;

return          return RETURN;

{identifier}    {
                    yylval->name = safe_strdup(yytext);
                    return IDENTIFIER;
                }
                
{integer}       {
                    yylval->intval = atoi(yytext);
                    return INTEGERCONST;
                }

{float}         {
                    yylval->floatval = strtof(yytext, NULL);
                    return FLOATCONST;
                }

\n              {
                    yycolumn = 1;
                }

.

%%


