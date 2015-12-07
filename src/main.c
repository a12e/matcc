#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "quad_list.h"
#include "utility.h"

extern int yylineno;
extern char *yytext;
extern FILE *yyin;
extern int yydebug;
extern int yyparse();

FILE *source_file = NULL;
FILE *quad_file = NULL;
FILE *mips_file = NULL;
FILE *binary_file = NULL;
bool do_mips = true;
bool do_binary = true;

void generate_quads();
void generate_mips();
void generate_binary();

void yyerror(char const *s) {
    fprintf(stderr, "line %d: %s when reading token %s\n", yylineno, s, yytext);
}

void print_help_and_exit(char *progname, int exit_code) {
    printf("Usage: %s [options] [source_file]\n", progname);
    printf("If source_file is not specified, then read form stdin\n");
    printf("Options are:\n");
    printf("-h\tdisplay this help\n");
    printf("-c\tstop the compilation after quad generation\n");
    printf("-S\tstop the compilation after assembler code generation\n");
    printf("-o file\tspecifiy output file for quad code\n");
    printf("-d\tenable debug mode\n");
    exit(exit_code);
}

int main(int argc, char **argv) {
    for(int i = 1; i < argc; i++) {
        if(strcmp(argv[i], "-h") == 0) {
            print_help_and_exit(argv[0], EXIT_SUCCESS);
        }
        else if(strcmp(argv[i], "-d") == 0) {
            fprintf(stderr, "Debug mode enabled\n");
            yydebug = 1;
        }
        else if(strcmp(argv[i], "-c") == 0) {
            do_mips = false;
        }
        else if(strcmp(argv[i], "-S") == 0) {
            do_binary = false;
        }
        else if(strcmp(argv[i], "-o") == 0) {
            if(++i >= argc) {
                fprintf(stderr, "Please specify file name for -o option\n");
                print_help_and_exit(argv[0], EXIT_FAILURE);
            }

            quad_file = safe_fopen(argv[i], "w+");
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
    if(quad_file == NULL) quad_file = stdout;

    generate_quads();
    if(do_mips) {
        generate_mips();
        if(do_binary) {
            generate_binary();
        }
    }

    safe_fclose(source_file);
    safe_fclose(quad_file);
    safe_fclose(mips_file);
    safe_fclose(binary_file);

    return EXIT_SUCCESS;
}

void generate_quads() {
    printf("Generating QUADS from MATC\n");

    yyin = source_file;
    ht_init(&symbol_table, 16);

    yyparse();

    fprintf(quad_file, "=variables\n");
    symbol_table_print_variables(quad_file);
    fprintf(quad_file, "=code\n");
    quad_list_print(quad_file, quad_list);

    quad_list_delete(quad_list);
    ht_free(&symbol_table);
}

void generate_mips() {
    printf("Generating MIPS from QUADS\n");
}

void generate_binary() {
    printf("Generating BINARY from MIPS\n");
}