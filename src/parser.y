%{

#include "actions.h"
#include "symbol_table.h"
#include "symbol.h"
#include "quad_list.h"

#define YYDEBUG 1

extern int yylex(void);
extern int yylineno;
extern char *yytext;

void yyerror(char const *s) {
	fprintf(stderr, "line %d: %s when reading token %s\n", yylineno, s, yytext);
}

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
%token	<name>	        IDENTIFIER
%token  <intval>        INTEGERCONST
%token  <floatval>      FLOATCONST

/* Analysis variables */
%type   <symbol>        variable_declaration
%type   <symbol>        assignation
%type   <symbol>        expression
%type   <symbol>        identifier
%type   <type>          type

%%

program					:	functions

functions				:	function
						|	function functions

function				:	type IDENTIFIER '(' ')'
							'{' statements '}'

statements				:	statement ';'
						|	statement ';'
							statements

statement				:	variable_declaration
						|   assignation

variable_declaration    :   type IDENTIFIER                 { $$ = declare($1, $2); }
                        |   type IDENTIFIER '=' expression  { $$ = declare_and_assign($1, $2, $4); }

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

identifier              :   IDENTIFIER                      { $$ = symbol_table_lookup($1); }

type                    :   T_INT                           { $$ = INT; }
						|   T_FLOAT                         { $$ = FLOAT; }
						|   T_MATRIX                        { $$ = MATRIX; }

%%

int main(int argc, char **argv)
{
	for(int argi = 1; argi < argc; argi++) {
        if (strcmp(argv[argi], "-h") == 0) {
            printf("usage: %s [-h|-d]\n", argv[0]);
            return EXIT_SUCCESS;
        }
        else if (strcmp(argv[argi], "-d") == 0) {
            fprintf(stderr, "Debug mode enabled\n");
            yydebug = 1;
        }
        else {
            fprintf(stderr, "Unrecognized option: %s\n", argv[argi]);
            return EXIT_FAILURE;
        }
    }

    ht_init(&symbol_table, 16);

	yyparse();

	printf("=variables\n");
	symbol_table_print_variables();
	printf("=code\n");
    quad_list_print(quad_list);

    ht_free(&symbol_table);

	return EXIT_SUCCESS;
}
