#ifndef SYMBOLS_H
#define SYMBOLS_H

/* ── Data types understood by Backfile ── */
typedef enum {
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_CHAR,
    TYPE_STRING,
    TYPE_BOOL,
    TYPE_VOID,
} DataType;

/* ── One variable slot in a scope ── */
typedef struct SymbolEntry {
    char    *identifier;
    DataType data_type;
    union {
        int    i_value;
        double f_value;
        char   c_value;
        char  *s_value;
        int    b_value;
    } data;
    struct SymbolEntry *next; /* next entry in the same scope level */
} SymbolEntry;

/* ── A single scope frame ── */
typedef struct Scope {
    SymbolEntry  *entries; /* linked list of vars in this frame */
    struct Scope *parent;  /* enclosing scope (NULL = global)   */
} Scope;

/* ── Scope stack operations ── */
Scope       *scope_push  (Scope *parent);
void         scope_pop   (Scope *scope);

/* ── Symbol CRUD (always operate on the given scope) ── */
SymbolEntry *symbol_create (Scope *scope, const char *name, DataType type);
SymbolEntry *symbol_lookup (Scope *scope, const char *name); /* walks up parent chain */
SymbolEntry *symbol_lookup_local(Scope *scope, const char *name); /* current frame only */

#endif /* SYMBOLS_H */
