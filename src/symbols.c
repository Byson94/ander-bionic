#include "include/symbols.h"
#include <dlfcn.h>
#include <stdio.h>
#include <string.h>
#include <link.h>
#include <elf.h>

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

typedef struct {
    void (*cb)(const char *name, void *udata);
    void *udata;
} ForeachState;

static int phdr_callback(struct dl_phdr_info *info, size_t size, void *data) {
    ForeachState *state = data;

    const ElfW(Dyn) *dyn = NULL;
    for (int i = 0; i < info->dlpi_phnum; i++) {
        if (info->dlpi_phdr[i].p_type == PT_DYNAMIC) {
            dyn = (const ElfW(Dyn) *)(info->dlpi_addr + info->dlpi_phdr[i].p_vaddr);
            break;
        }
    }
    if (!dyn) return 0;

    const ElfW(Sym) *symtab = NULL;
    const char      *strtab = NULL;
    size_t           syment = sizeof(ElfW(Sym));

    for (const ElfW(Dyn) *d = dyn; d->d_tag != DT_NULL; d++) {
        if      (d->d_tag == DT_SYMTAB) symtab = (const ElfW(Sym) *)(info->dlpi_addr + d->d_un.d_ptr);
        else if (d->d_tag == DT_STRTAB) strtab = (const char *)      (info->dlpi_addr + d->d_un.d_ptr);
        else if (d->d_tag == DT_SYMENT) syment = d->d_un.d_val;
    }
    if (!symtab || !strtab) return 0;

    const uint32_t *hashtab = NULL;
    for (const ElfW(Dyn) *d = dyn; d->d_tag != DT_NULL; d++) {
        if (d->d_tag == DT_HASH) {
            hashtab = (const uint32_t *)(info->dlpi_addr + d->d_un.d_ptr);
            break;
        }
    }
    if (!hashtab) return 0;

    size_t symcount = hashtab[1];
    for (size_t j = 1; j < symcount; j++) {
        const ElfW(Sym) *sym = (const ElfW(Sym) *)((const char *)symtab + j * syment);
        if (ELF64_ST_BIND(sym->st_info) == STB_GLOBAL && sym->st_value != 0) {
            state->cb(strtab + sym->st_name, state->udata);
        }
    }
    return 0;
}

void symbols_foreach(void (*cb)(const char *name, void *udata), void *udata) {
    ForeachState state = { cb, udata };
    dl_iterate_phdr(phdr_callback, &state);
}