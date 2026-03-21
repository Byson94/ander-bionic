#pragma once
#include <stddef.h>

void  symbols_init(void);
int   symbols_load(const char *path);
void *symbols_find(const char *name);
void symbols_foreach(void (*cb)(const char *name, void *udata), void *udata);