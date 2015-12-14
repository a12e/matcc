#ifndef MATCC_TRANSLATION_H
#define MATCC_TRANSLATION_H

#include <stdio.h>
#include "hash_table.h"
#include "instruction.h"

void compute_symbols_lifetime();
void print_symbols_lifetime(FILE *f);

struct instruction_list *generate_code();

#endif //MATCC_TRANSLATION_H
