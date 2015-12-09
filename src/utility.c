#include <string.h>
#include "utility.h"

void check_pointer(void *ptr) {
    if(ptr == NULL) {
        perror("bad pointer");
        exit(EXIT_FAILURE);
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

void *safe_realloc(void *ptr, size_t size) {
    void *res = realloc(ptr, size);
    check_pointer(res);
    return res;
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

FILE *safe_fopen(const char *path, const char *mode) {
    FILE *f = fopen(path, mode);
    if(!f) {
        fprintf(stderr, "error opening file %s: ", path);
        perror("");
        exit(EXIT_FAILURE);
    }
    return f;
}

int safe_fclose(FILE *fp) {
    if(fp) {
        return fclose(fp);
    }
    else {
        return 0;
    }
}
