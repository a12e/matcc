%{
#include <stdio.h>
#include "parser.h"
#include "utility.h"
%}

identifier		[_a-zA-Z][_a-zA-Z0-9]*
delimiter		[ \n]+
integer         [0-9]+
float           [-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?

%%

[{}(),;=+]		return yytext[0];

int             return INT;
float           return FLOAT;
matrix          return MATRIX;
if              return IF;
else            return ELSE;
while           return WHILE;
for             return FOR;
return          return RETURN;

{identifier}	{
					yylval.name = safe_strdup(yytext);
					return IDENTIFIER;
				}
				
{integer}       {
                    yylval.intval = atoi(yytext);
                    return INTEGERCONST;
                }

{float}         {
                    yylval.floatval = strtof(yytext, NULL);
                    return FLOATCONST;
                }                

{delimiter}		{
					/* do nothing */
				}

.

%%

