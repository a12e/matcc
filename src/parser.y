%{

#include "actions.h"
#include "symbol_table.h"
#include "symbol.h"

#define YYDEBUG 1

extern int yylex(void);
extern void yyerror(char const *s);

%}

/* Token attributes */
%union {
    char *name;
    int intval;
    float floatval;
    enum symbol_type type;
    struct symbol* symbol;
}

/* Lex tokens */
%token                  T_INT T_FLOAT T_MATRIX
%token                  IF ELSE
%token                  WHILE
%token                  FOR
%token                  RETURN
%token  <name>          IDENTIFIER
%token  <intval>        INTEGERCONST
%token  <floatval>      FLOATCONST

/* Analysis variables */
%type   <symbol>        variable_declaration
%type   <symbol>        assignation
%type   <symbol>        expression
%type   <symbol>        identifier
%type   <type>          type

%%

program                 :   functions

functions               :   function
                        |   function functions

function                :   type IDENTIFIER '(' ')'
                            '{' statements '}'              { safe_free($2); }

statements              :   statement ';'
                        |   statement ';'
                            statements

statement               :   variable_declaration
                        |   assignation

variable_declaration    :   type IDENTIFIER                 { $$ = declare($1, $2); safe_free($2); }
                        |   type IDENTIFIER '=' expression  { $$ = declare_and_assign($1, $2, $4); safe_free($2); }

assignation             :   identifier    '=' expression    { $$ = assign($1, $3); }
                        |   identifier '+''=' expression    { $$ = assign($1, add($1, $4)); }
                        |   identifier '-''=' expression    { $$ = assign($1, substract($1, $4)); }
                        |   identifier '*''=' expression    { $$ = assign($1, multiply($1, $4)); }
                        |   identifier '/''=' expression    { $$ = assign($1, divide($1, $4)); }

expression              :   INTEGERCONST                    { $$ = declare_int_constant($1); }
                        |   FLOATCONST                      { $$ = declare_float_constant($1); }
                        |   identifier                      { $$ = $1; }
                        |   expression '+' expression       { $$ = add($1, $3); }
                        |   expression '-' expression       { $$ = substract($1, $3); }
                        |   expression '*' expression       { $$ = multiply($1, $3); }
                        |   expression '/' expression       { $$ = divide($1, $3); }

identifier              :   IDENTIFIER                      { $$ = symbol_table_lookup($1); safe_free($1); }

type                    :   T_INT                           { $$ = INT; }
                        |   T_FLOAT                         { $$ = FLOAT; }
                        |   T_MATRIX                        { $$ = MATRIX; }

%%
