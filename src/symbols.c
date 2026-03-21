#include "include/symbols.h"
#include <dlfcn.h>
#include <stdio.h>

#define MAX_LIBS 64
static void *handles[MAX_LIBS];
static int   handle_count = 0;

void symbols_init(void) { handle_count = 0; }

int symbols_load(const char *path) {
    void *h = dlopen(path, RTLD_LAZY | RTLD_GLOBAL);
    if (!h) { fprintf(stderr, "dlopen failed for %s: %s\n", path, dlerror()); return -1; }
    fprintf(stdout, "loaded: %s\n", path);
    handles[handle_count++] = h;
    return 0;
}

void *symbols_find(const char *name) {
    for (int i = 0; i < handle_count; i++) {
        void *sym = dlsym(handles[i], name);
        if (sym) return sym;
    }
    return NULL;
}