#include "symbols.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Create a new scope frame (push) ── */
Scope *scope_push(Scope *parent) {
    Scope *s = (Scope *)malloc(sizeof(Scope));
    s->entries = NULL;
    s->parent  = parent;
    return s;
}

/* ── Destroy a scope frame and all its entries (pop) ── */
void scope_pop(Scope *scope) {
    SymbolEntry *e = scope->entries;
    while (e) {
        SymbolEntry *next = e->next;
        if (e->data_type == TYPE_STRING && e->data.s_value)
            free(e->data.s_value);
        free(e->identifier);
        free(e);
        e = next;
    }
    free(scope);
}

/* ── Create a variable in the current scope (shadowing is implicit:
       a new entry hides any same-named entry in outer scopes) ── */
SymbolEntry *symbol_create(Scope *scope, const char *name, DataType type) {
    SymbolEntry *entry = (SymbolEntry *)calloc(1, sizeof(SymbolEntry)); /* calloc zeros all fields including s_value */
    entry->identifier  = strdup(name);
    entry->data_type   = type;
    entry->next        = scope->entries;
    scope->entries     = entry;
    return entry;
}

/* ── Walk up the scope chain to find a variable (shadowing respected) ── */
SymbolEntry *symbol_lookup(Scope *scope, const char *name) {
    while (scope) {
        SymbolEntry *e = scope->entries;
        while (e) {
            if (strcmp(e->identifier, name) == 0) return e;
            e = e->next;
        }
        scope = scope->parent;
    }
    return NULL;
}

/* ── Look only in the current frame ── */
SymbolEntry *symbol_lookup_local(Scope *scope, const char *name) {
    SymbolEntry *e = scope->entries;
    while (e) {
        if (strcmp(e->identifier, name) == 0) return e;
        e = e->next;
    }
    return NULL;
}
