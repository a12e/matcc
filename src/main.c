#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "quad_list.h"
#include "utility.h"
#include "parsing/actions.h"
#include "mips/translation.h"
#include <parser.h>

extern char *yytext;
extern FILE *yyin;
extern int yydebug;
extern int yyparse();
bool debug = false;
extern bool parse_successful;

FILE *source_file = NULL;
FILE *output_file = NULL;
bool stop_after_quads = false;

void yyerror(char const *s) {
    fprintf(stderr, "line %d:%d-%d %s when reading token %s\n",
            yylloc.first_line, yylloc.first_column, yylloc.last_column, s, yytext);
}

void print_help_and_exit(char *progname, int exit_code) {
    printf("Usage: %s [options] [source_file]\n", progname);
    printf("\n");
    printf("If source_file is not specified, then read form stdin\n");
    printf("\n");
    printf("Options are:\n");
    printf("-h\tdisplay this help\n");
    printf("-c\tstop the compilation after quad generation\n");
    printf("-o file\tspecifiy output file (stdout by default)\n");
    printf("-d\tenable matcc debug mode\n");
    printf("-dd\tenable both matcc and YACC debug mode\n");
    exit(exit_code);
}

int main(int argc, char **argv) {
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-h") == 0) {
            print_help_and_exit(argv[0], EXIT_SUCCESS);
        }
        else if(strcmp(argv[i], "-d") == 0) {
            fprintf(stderr, "matcc debug mode enabled\n");
            debug = true;
        }
        else if(strcmp(argv[i], "-dd") == 0) {
            fprintf(stderr, "matcc debug mode enabled\n");
            debug = true;
            fprintf(stderr, "YACC debug enabled\n");
            yydebug = 1;
        }
        else if(strcmp(argv[i], "-c") == 0) {
            stop_after_quads = true;
        }
        else if(strcmp(argv[i], "-o") == 0) {
            if(++i >= argc) {
                fprintf(stderr, "Please specify file name for -o option\n");
                print_help_and_exit(argv[0], EXIT_FAILURE);
            }

            output_file = safe_fopen(argv[i], "w+");
        }
        else {
            if(i == argc-1) { // last argument => source file
                source_file = safe_fopen(argv[i], "r");
            }
            else {
                fprintf(stderr, "Unrecognized option: %s\n", argv[i]);
                print_help_and_exit(argv[0], EXIT_FAILURE);
            }
        }
    }

    if(source_file == NULL) source_file = stdin;
    if(output_file == NULL) output_file = stdout;

    // === Generate QUADS from MATC ===

    yyin = source_file;
    ht_init(&symbol_table, 16);

    yyparse();
    // Add a return statement at the end of the function
    quad_list_push(&quad_list, quad_new_empty(RET));

    if(!parse_successful) {
        fprintf(stderr, "compilation aborted\n");
        return EXIT_FAILURE;
    }

    if(debug) {
        fprintf(stderr, "=variables (%zu symbols)\n", ht_size(&symbol_table));
        symbol_table_print_variables(stderr);
        fprintf(stderr, "=code ");
        quad_list_print(stderr, quad_list);
    }

    if(stop_after_quads) {
        fprintf(output_file, "=variables (%zu symbols)\n", ht_size(&symbol_table));
        symbol_table_print_variables(output_file);
        fprintf(output_file, "=code ");
        quad_list_print(output_file, quad_list);
    }
    else {
        // === Generatine MIPS from QUADS ===

        compute_symbols_lifetime();
        if(debug) print_symbols_lifetime(stderr);

        symbol_table_print_data_section(output_file);

        struct instruction_list *instruction_list = generate_code();
        fprintf(output_file, "\n.text\n");
        instruction_list_print(output_file, instruction_list);

        instruction_list_delete(instruction_list);
    }

    cleanup:
    quad_list_delete(quad_list);
    ht_free(&symbol_table);

    safe_fclose(source_file);
    safe_fclose(output_file);

    return EXIT_SUCCESS;
}
