%{
#include "parsing/actions.h"
#include "symbol_table.h"
#include "symbol.h"
#include "quad_list.h"
#include <matrix.h>
#define YYDEBUG 1
%}

// Enable location tracking
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

/* Token and variables attributes */
%union {
    char *              name;
    int                 intval;
    float               floatval;
    char *              stringval;
    enum symbol_type    type;
    struct symbol*      symbol;
    struct quad_list *  quad_list;
    struct matrix_size  matrix_size;
    struct expr_attr    expr_attr;
    struct matrix *     matrix;
    struct cond_attr    cond_attr;
    enum quad_op        quad_op;
}

/* Tokens */
%token                  T_INT T_FLOAT T_MATRIX
%token                  IF ELSE TRUE FALSE OR AND NOT
%token                  EQ NEQ LT GT LTE GTE
%token                  WHILE DO FOR
%token                  RETURN
%token  <name>          IDENTIFIER
%token  <intval>        INTEGERCONST
%token  <floatval>      FLOATCONST
%token  <stringval>     STRINGCONST

/* Analysis variables */
%type   <type>          type
%type   <symbol>        identifier tag tag1 tag2
%type   <quad_list>     statement statements instruction control_structure
%type   <matrix_size>   size_expr
%type   <expr_attr>     expression assignation variable_declaration
%type   <matrix>        float_matrix float_array_list float_array float_list
%type   <cond_attr>     condition
%type   <quad_op>       compar_op

%left '-' '+'
%left '*' '/'

%{
#define YYLEX_PARAM &yylval, &yylloc

extern int yylex(YYSTYPE *yylval_param, YYLTYPE *yylloc_param);
extern void yyerror(char const *s);
bool parse_successful = false;
%}

%%

program                 :   functions                       { parse_successful = true; }

functions               :   function
                        |   function functions

function                :   type IDENTIFIER '(' ')'
                            statements                      { quad_list = $5; safe_free($2); }

statements              :   statement                       { $$ = $1; }
                        |   statement
                            statements                      { $$ = quad_list_concat(2, $1, $2); }
                        |   '{' '}'                         { $$ = NULL; }
                        |   '{' statements '}'              { $$ = $2; }

statement               :   instruction ';'
                        |   control_structure

instruction             :   variable_declaration            { $$ = $1.code; }
                        |   assignation                     { $$ = $1.code; }
                        |   IDENTIFIER '(' identifier ')'   { $$ = call_function_with_identifier($1, $3); }
                        |   IDENTIFIER '(' STRINGCONST ')'  { $$ = call_function_with_string($1, $3); }
                        |   RETURN                          { $$ = control_return(); }

variable_declaration    :   type IDENTIFIER                 { $$.code = NULL; declare($1, $2); safe_free($2); }
                        |   type IDENTIFIER '=' expression  { $$ = declare_and_assign($1, $2, $4); safe_free($2); }
                        |   T_MATRIX IDENTIFIER size_expr   { $$.code = NULL; declare_matrix($2, $3); safe_free($2); }

identifier              :   IDENTIFIER                      { $$ = symbol_table_lookup($1); safe_free($1); }

type                    :   T_INT                           { $$ = INT; }
                        |   T_FLOAT                         { $$ = FLOAT; }

size_expr               :   '[' INTEGERCONST ']'                        { $$ = declare_matrix_size(1, $2); }
                        |   '[' INTEGERCONST ']' '[' INTEGERCONST ']'   { $$ = declare_matrix_size($2, $5); }

assignation             :   identifier    '=' expression    { $$ = assign($1, $3); }
/*
                        |   identifier '+''=' expression    { $$ = assign($1, binary_arithmetic_op($1, ADD, $4)); }
                        |   identifier '-''=' expression    { $$ = assign($1, binary_arithmetic_op($1, SUB, $4)); }
                        |   identifier '*''=' expression    { $$ = assign($1, binary_arithmetic_op($1, MUL, $4)); }
                        |   identifier '/''=' expression    { $$ = assign($1, binary_arithmetic_op($1, DIV, $4)); }
*/

expression              :   INTEGERCONST                    { $$ = declare_int_constant($1); }
                        |   FLOATCONST                      { $$ = declare_float_constant($1); }
                        |   '{' float_matrix '}'            { $$ = declare_matrix_constant($2); }
                        |   identifier                      { $$.code = NULL; $$.symbol = $1; }
                        |   expression '+' expression       { $$ = binary_arithmetic_op($1, ADD, $3); }
                        |   expression '-' expression       { $$ = binary_arithmetic_op($1, SUB, $3); }
                        |   expression '*' expression       { $$ = binary_arithmetic_op($1, MUL, $3); }
                        |   expression '/' expression       { $$ = binary_arithmetic_op($1, DIV, $3); }

float_matrix            :   float_list                      { $$ = $1; }
                        |   float_array_list                { $$ = $1; }

float_array_list        :   float_array                     { $$ = $1; }
                        |   float_array_list ','
                            float_array                     { $$ = $1; matrix_append_row($$, $3); matrix_delete($3); }

float_array             :   '{' float_list '}'              { $$ = $2; }

float_list              :   FLOATCONST                      { $$ = matrix_new_with_value($1); }
                        |   float_list ','
                            FLOATCONST                      { $$ = $1; matrix_append_value($$, $3); }

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
tag1                    :   /* empty */                     { $$ = (declare_int_constant(next_quad+1)).symbol; }
tag2                    :   /* empty */                     { $$ = (declare_int_constant(next_quad+2)).symbol; }

control_structure       :   IF '(' condition ')' tag
                            statements
                            tag                             { $$ = control_if($3, $5, $6, $7); }
                        |   IF '(' condition ')' tag
                            statements
                            ELSE tag1
                            statements
                            tag1                            { $$ = control_if_else($3, $5, $6, $8, $9, $10); }
                        |   WHILE '(' tag condition ')' tag
                            statements tag1                 { $$ = control_while($3, $4, $6, $7, $8); }
                        |   DO tag statements
                            WHILE '('
                            condition tag ')'';'            { $$ = control_do_while($2, $3, $6, $7); }
                        |   FOR '('
                            variable_declaration ';'
                            tag condition ';'
                            tag assignation ')'
                            tag1 statements tag2            { $$ = control_for($3, $5, $6, $8, $9, $11, $12, $13); }

%%
