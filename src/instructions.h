//
// Created by abrooke on 17/11/15.
//

#ifndef MATCC_INTERM_CODE_H
#define MATCC_INTERM_CODE_H

typedef struct iinstr_s {
    const char *instruction_str;
    struct iinstr_s *successor;
} iinstr;

typedef iinstr * iinstr_list;

void iinstr_put(iinstr_list *iilist, char const *fmt, ...);
void print_iinstr_list(iinstr_list *iilist);

#endif //MATCC_INTERM_CODE_H
