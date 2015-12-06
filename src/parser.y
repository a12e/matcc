%{

#include "actions.h"

#define YYDEBUG 1

extern int yylex(void);
extern int yylineno;
extern int tokenpos;
extern char linebuf[];
extern char *yytext;

void yyerror(char const *s) {
	fprintf(stderr, "line %d: %s\n", yylineno, s);
}


hashtable_t symbol_table;
iinstr_list instructions = NULL;

%}

/* Attributs des tokens */
%union {
	char *name;
    struct {
        union {
            int intval;
            float floatval;
        };
        uint register_id;
    };
}

/* Tokens fournits par le lexer */
%token                  INT
%token			        FLOAT
%token			        MATRIX
%token                  IF
%token                  ELSE
%token                  WHILE
%token                  FOR
%token                  RETURN
%token	<name>	        IDENTIFIER
%token  <intval>        INTEGERCONST
%token  <floatval>      FLOATCONST
%type   <register_id>   int_expression

%%

program					:	functions

functions				:	function
						|	function functions

function				:	INT IDENTIFIER '(' ')'
							'{' statements '}'

statements				:	statement ';'
						|	statement ';' statements

statement				:	variable_decl_list

variable_decl_list      :   type variable_declarations

variable_declarations   :   variable_declaration
                        |   variable_declaration ',' variable_declarations

variable_declaration    :   IDENTIFIER                          { declare_int($1); }
                        |   IDENTIFIER '=' int_expression       { declare_int_with_init($1, $3);  }

int_expression          :   INTEGERCONST                        { $$ = load_constant($1); }
                        |   IDENTIFIER                          { $$ = load_symbol($1); }
                        |   int_expression '+' int_expression   { $$ = int_add($1, $3); }

type                    :   INT     
                        |   FLOAT  
                        |   MATRIX

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

    //ht_dump(&symtab);
    ht_free(&symbol_table);

    print_iinstr_list(&instructions);

	return EXIT_SUCCESS;
}
