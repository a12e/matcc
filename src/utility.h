//
// Created by abrooke on 10/11/15.
//

#ifndef MATCC_UTILITY_H
#define MATCC_UTILITY_H

#include <stdlib.h>
#include <stdio.h>

void check_pointer(void *ptr);
void *safe_malloc(size_t size);
void *safe_calloc(size_t nmemb, size_t size);
void safe_free(void *ptr);
char *safe_strdup(const char *s);

#endif //MATCC_UTILITY_H
