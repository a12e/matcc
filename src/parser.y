%{
#include "actions.h"
#include "symbol_table.h"
#include "symbol.h"
#include "quad_list.h"
#define YYDEBUG 1
%}

%locations

%printer {
    fprintf(yyoutput, "%s", $$);
} IDENTIFIER

%printer {
    fprintf(yyoutput, "%s", $$->name);
} <symbol>;

%printer {
    fprintf(yyoutput, "quad_list >>> ");
    quad_list_print(yyoutput, $$);
    fprintf(yyoutput, "<<<");
} <quad_list>;

%printer {
    fprintf(yyoutput, "cond_attr\n");
    fprintf(yyoutput, ".code = quadlist >>> ");
    quad_list_print(yyoutput, $$.code);
    fprintf(yyoutput, "<<<");
    fprintf(yyoutput, ".true_list = quadlist >>> ");
    quad_list_print(yyoutput, $$.true_list);
    fprintf(yyoutput, "<<<");
    fprintf(yyoutput, ".false_list = quadlist >>> ");
    quad_list_print(yyoutput, $$.false_list);
    fprintf(yyoutput, "<<<");
} <cond_attr>;

/* Token attributes */
%union {
    char *name;
    int intval;
    float floatval;
    enum symbol_type type;
    struct symbol* symbol;
    struct quad_list *quad_list;
    struct expr_attr expr_attr;
    struct cond_attr cond_attr;
    enum quad_op quad_op;
}

/* Lex tokens */
%token                  T_INT T_FLOAT T_MATRIX
%token                  IF ELSE TRUE FALSE OR AND NOT
%token                  EQ NEQ LT GT LTE GTE
%token                  WHILE FOR
%token                  RETURN
%token  <name>          IDENTIFIER
%token  <intval>        INTEGERCONST
%token  <floatval>      FLOATCONST

/* Analysis variables */
%type   <type>          type
%type   <symbol>        identifier tag nexttag
%type   <quad_list>     statement statements instruction control_structure
%type   <expr_attr>     expression assignation variable_declaration
%type   <cond_attr>     condition
%type   <quad_op>       compar_op

%left '-' '+'
%left '*' '/'

%{
#define YYLEX_PARAM &yylval, &yylloc

extern int yylex(YYSTYPE *yylval_param, YYLTYPE *yylloc_param);
extern void yyerror(char const *s);
%}

%%

program                 :   functions

functions               :   function
                        |   function functions

function                :   type IDENTIFIER '(' ')'
                            statements                      { quad_list = $5; safe_free($2); }

statements              :   statement                       { $$ = $1; }
                        |   statement
                            statements                      { $$ = NULL; quad_list_concat(&$$, 2, $1, $2); }
                        |   '{' '}'                         { $$ = NULL; }
                        |   '{' statements '}'              { $$ = $2; }

statement               :   instruction ';'
                        |   control_structure

instruction             :   variable_declaration            { $$ = $1.code; }
                        |   assignation                     { $$ = $1.code; }

variable_declaration    :   type IDENTIFIER                 { $$.code = NULL; declare($1, $2); safe_free($2); }
                        |   type IDENTIFIER '=' expression  { $$ = declare_and_assign($1, $2, $4); safe_free($2); }

assignation             :   identifier    '=' expression    { $$ = assign($1, $3); }
/*
                        |   identifier '+''=' expression    { $$ = assign($1, binary_arithmetic_op($1, ADD, $4)); }
                        |   identifier '-''=' expression    { $$ = assign($1, binary_arithmetic_op($1, SUB, $4)); }
                        |   identifier '*''=' expression    { $$ = assign($1, binary_arithmetic_op($1, MUL, $4)); }
                        |   identifier '/''=' expression    { $$ = assign($1, binary_arithmetic_op($1, DIV, $4)); }
*/

expression              :   INTEGERCONST                    { $$ = declare_int_constant($1); }
                        |   FLOATCONST                      { $$ = declare_float_constant($1); }
                        |   identifier                      { $$.code = NULL; $$.symbol = $1; }
                        |   expression '+' expression       { $$ = binary_arithmetic_op($1, ADD, $3); }
                        |   expression '-' expression       { $$ = binary_arithmetic_op($1, SUB, $3); }
                        |   expression '*' expression       { $$ = binary_arithmetic_op($1, MUL, $3); }
                        |   expression '/' expression       { $$ = binary_arithmetic_op($1, DIV, $3); }

identifier              :   IDENTIFIER                      { $$ = symbol_table_lookup($1); safe_free($1); }

type                    :   T_INT                           { $$ = INT; }
                        |   T_FLOAT                         { $$ = FLOAT; }
                        |   T_MATRIX                        { $$ = MATRIX; }

condition               :   condition OR  tag condition     { $$ = condition_or($1, $3, $4); }
                        |   condition AND tag condition     { $$ = condition_and($1, $3, $4); }
                        |   NOT condition                   { $$ = condition_not($2); }
                        |   '(' condition ')'               { memcpy(&$$, &$2, sizeof($$)); }
                        |   TRUE                            { $$ = condition_true(); }
                        |   FALSE                           { $$ = condition_false(); }
                        |   expression compar_op expression { $$ = condition_compare_expressions($1, $2, $3); }

compar_op               :   EQ                              { $$ = BE; }
                        |   NEQ                             { $$ = BNE; }
                        |   LT                              { $$ = BLT; }
                        |   GT                              { $$ = BGT; }
                        |   LTE                             { $$ = BLTE; }
                        |   GTE                             { $$ = BGTE; }

tag                     :   /* empty */                     { $$ = (declare_int_constant(next_quad)).symbol; }
nexttag                 :   /* empty */                     { $$ = (declare_int_constant(next_quad+1)).symbol; }

control_structure       :   IF '(' condition ')' tag
                            statements
                            tag                             { $$ = control_if($3, $5, $6, $7); }
                        |   IF '(' condition ')' tag
                            statements
                            ELSE nexttag
                            statements
                            nexttag                         { $$ = control_if_else($3, $5, $6, $8, $9, $10); }

%%
