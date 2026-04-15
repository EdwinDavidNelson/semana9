#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "ast.h"
#include "symbols.h"

/* ── Runtime value ── */
typedef struct {
    DataType type;
    union {
        int    i_val;
        double f_val;
        char   c_val;
        char  *s_val;
        int    b_val;
    };
} Value;

/* ── Function record stored at the global level ── */
typedef struct FuncRecord {
    char            *name;
    DataType         return_type;
    ParamInfo       *params;
    int              param_count;
    ASTNode         *body;
    struct FuncRecord *next;
} FuncRecord;

/* ── Interpreter context ── */
typedef struct {
    Scope      *scope;         /* current variable scope */
    FuncRecord *functions;     /* linked list of known functions */
    int         returning;     /* 1 when a return stmt was hit */
    Value       return_val;    /* value being returned */
} Interpreter;

Interpreter *interp_create  (void);
void         interp_destroy (Interpreter *interp);
void         interp_run     (Interpreter *interp, ASTNode *program);

#endif /* INTERPRETER_H */
