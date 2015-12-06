//
// Created by abrooke on 17/11/15.
//

#include "instructions.h"
#include "utility.h"
#include <stdarg.h>
#include <string.h>

void iinstr_put(iinstr_list *iilist, char const *fmt, ...) {
    char buffer[32];
    va_list argptr;
    va_start(argptr, fmt);
    vsnprintf(buffer, 32, fmt, argptr);
    va_end(argptr);

    iinstr *new_instr = safe_calloc(1, sizeof(iinstr));
    new_instr->instruction_str = safe_strdup(buffer);

    if(*iilist == NULL) {
        *iilist = new_instr;
    }
    else {
        iinstr *i = *iilist;
        for (; i->successor != NULL; i = i->successor);
        i->successor = new_instr;
    }
}

void print_iinstr_list(iinstr_list *iilist) {
    uint instruction_number = 0;
    for(iinstr *instruction = *iilist; instruction != NULL; instruction = instruction->successor) {
        printf("%04u: %s\n", instruction_number, instruction->instruction_str);

        if(strchr(instruction->instruction_str, '.') == NULL) // pas un label
            instruction_number += 4;
    }
}
