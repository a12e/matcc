//
// Created by abrooke on 10/11/15.
//

#include <string.h>
#include "utility.h"

void check_pointer(void *ptr) {
    if(ptr == NULL) {
        perror("bad pointer");
        exit(-1);
    }
}

void *safe_malloc(size_t size) {
    void *ptr = malloc(size);
    check_pointer(ptr);
    return ptr;
}

void *safe_calloc(size_t nmemb, size_t size) {
    void *ptr = calloc(nmemb, size);
    check_pointer(ptr);
    return ptr;
}

void safe_free(void *ptr) {
    if(ptr) {
        free(ptr);
    }
}

char *safe_strdup(const char *s) {
    char *ptr = strdup(s);
    check_pointer(ptr);
    return ptr;
}
