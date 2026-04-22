#include "semantic.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Error reporting ── */
static int error_count = 0;

static void sem_error(int line, const char *msg) {
    fprintf(stderr, "[Error Semántico | línea %d] %s\n", line, msg);
    error_count++;
}

/* ── Lightweight scope for semantic analysis (not runtime) ── */
typedef struct SemScope {
    char           **names;
    DataType        *types;
    int              count;
    int              cap;
    struct SemScope *parent;
} SemScope;

static SemScope *sem_scope_push(SemScope *parent) {
    SemScope *s = (SemScope *)calloc(1, sizeof(SemScope));
    s->parent   = parent;
    s->cap      = 8;
    s->names    = (char **)malloc(sizeof(char *) * s->cap);
    s->types    = (DataType *)malloc(sizeof(DataType) * s->cap);
    return s;
}

static void sem_scope_pop(SemScope *s) {
    for (int i = 0; i < s->count; i++) free(s->names[i]);
    free(s->names); free(s->types); free(s);
}

/* Returns TYPE_VOID if not found */
static DataType sem_lookup(SemScope *s, const char *name) {
    while (s) {
        for (int i = 0; i < s->count; i++)
            if (strcmp(s->names[i], name) == 0) return s->types[i];
        s = s->parent;
    }
    return TYPE_VOID;
}

static int sem_lookup_local(SemScope *s, const char *name) {
    for (int i = 0; i < s->count; i++)
        if (strcmp(s->names[i], name) == 0) return 1;
    return 0;
}

static void sem_define(SemScope *s, const char *name, DataType type) {
    if (s->count >= s->cap) {
        s->cap *= 2;
        s->names = (char **)realloc(s->names, sizeof(char *) * s->cap);
        s->types = (DataType *)realloc(s->types, sizeof(DataType) * s->cap);
    }
    s->names[s->count] = strdup(name);
    s->types[s->count] = type;
    s->count++;
}

/* ── Function table for semantic analysis ── */
typedef struct FuncSig {
    char       *name;
    DataType    return_type;
    DataType   *param_types;
    int         param_count;
    struct FuncSig *next;
} FuncSig;

static FuncSig *func_table = NULL;

static void register_sig(const char *name, DataType ret, DataType *ptypes, int pc) {
    FuncSig *sig       = (FuncSig *)malloc(sizeof(FuncSig));
    sig->name          = strdup(name);
    sig->return_type   = ret;
    sig->param_count   = pc;
    sig->param_types   = NULL;
    if (pc > 0) {
        sig->param_types = (DataType *)malloc(sizeof(DataType) * pc);
        for (int i = 0; i < pc; i++) sig->param_types[i] = ptypes[i];
    }
    sig->next  = func_table;
    func_table = sig;
}

static FuncSig *find_sig(const char *name) {
    FuncSig *s = func_table;
    while (s) { if (strcmp(s->name, name) == 0) return s; s = s->next; }
    return NULL;
}

static void free_func_table(void) {
    FuncSig *s = func_table;
    while (s) {
        FuncSig *next = s->next;
        free(s->name);
        free(s->param_types);
        free(s);
        s = next;
    }
    func_table = NULL;
}

/* ── Forward declaration ── */
static DataType check_expr (ASTNode *node, SemScope *scope);
static void     check_stmt (ASTNode *node, SemScope *scope);
static void     check_block(ASTNode *node, SemScope *scope);

/* ─────────────────────────────────────────────
   Expression checker — returns the DataType of the expression
   ───────────────────────────────────────────── */
static DataType check_expr(ASTNode *node, SemScope *scope) {
    if (!node) return TYPE_VOID;
    char buf[256];

    switch (node->kind) {
        case NODE_LITERAL_INT:    return TYPE_INT;
        case NODE_LITERAL_FLOAT:  return TYPE_FLOAT;
        case NODE_LITERAL_CHAR:   return TYPE_CHAR;
        case NODE_LITERAL_STRING: return TYPE_STRING;
        case NODE_LITERAL_BOOL:   return TYPE_BOOL;

        case NODE_IDENTIFIER: {
            DataType t = sem_lookup(scope, node->ident.name);
            if (t == TYPE_VOID) {
                snprintf(buf, sizeof(buf),
                         "variable '%s' no ha sido declarada.", node->ident.name);
                sem_error(node->line, buf);
            }
            return t;
        }

        case NODE_BINARY_OP: {
            DataType l = check_expr(node->binop.left,  scope);
            DataType r = check_expr(node->binop.right, scope);
            if (l == TYPE_STRING || r == TYPE_STRING) {
                sem_error(node->line,
                    "operación aritmética no permitida con tipo bafle_str.");
            }
            return (l == TYPE_FLOAT || r == TYPE_FLOAT) ? TYPE_FLOAT : TYPE_INT;
        }

        case NODE_COMPARE: {
            check_expr(node->cmp.left,  scope);
            check_expr(node->cmp.right, scope);
            return TYPE_BOOL;
        }

        case NODE_FUNC_CALL: {
            FuncSig *sig = find_sig(node->func_call.name);
            if (!sig) {
                snprintf(buf, sizeof(buf),
                         "función '%s' no está declarada.", node->func_call.name);
                sem_error(node->line, buf);
                return TYPE_VOID;
            }
            if (node->func_call.arg_count != sig->param_count) {
                snprintf(buf, sizeof(buf),
                         "función '%s' espera %d argumento(s), se pasaron %d.",
                         node->func_call.name, sig->param_count, node->func_call.arg_count);
                sem_error(node->line, buf);
            }
            /* Check each argument type */
            for (int i = 0; i < node->func_call.arg_count && i < sig->param_count; i++) {
                DataType at = check_expr(node->func_call.args[i], scope);
                DataType pt = sig->param_types[i];
                /* Allow int<->float coercion, flag everything else */
                int numeric_a = (at == TYPE_INT || at == TYPE_FLOAT);
                int numeric_p = (pt == TYPE_INT || pt == TYPE_FLOAT);
                if (at != pt && !(numeric_a && numeric_p)) {
                    snprintf(buf, sizeof(buf),
                             "argumento %d de '%s': tipo incompatible.",
                             i + 1, node->func_call.name);
                    sem_error(node->line, buf);
                }
            }
            return sig->return_type;
        }

        default:
            return TYPE_VOID;
    }
}

/* ─────────────────────────────────────────────
   Statement checker
   ───────────────────────────────────────────── */
static void check_stmt(ASTNode *node, SemScope *scope) {
    if (!node) return;
    char buf[256];

    switch (node->kind) {

        case NODE_VAR_DECL: {
            /* Duplicate in same scope? */
            if (sem_lookup_local(scope, node->var_decl.name)) {
                snprintf(buf, sizeof(buf),
                         "variable '%s' ya fue declarada en este scope.",
                         node->var_decl.name);
                sem_error(node->line, buf);
            }
            DataType init_type = check_expr(node->var_decl.init, scope);
            DataType decl_type = node->var_decl.type;

            /* Type mismatch check (allow numeric coercion, and int->bool) */
            int numeric_i = (init_type == TYPE_INT || init_type == TYPE_FLOAT || init_type == TYPE_BOOL);
            int numeric_d = (decl_type == TYPE_INT || decl_type == TYPE_FLOAT || decl_type == TYPE_BOOL);
            if (init_type != TYPE_VOID && init_type != decl_type &&
                !(numeric_i && numeric_d)) {
                snprintf(buf, sizeof(buf),
                         "tipo incompatible al declarar '%s': se declaró %s pero se asignó %s.",
                         node->var_decl.name,
                         node->var_decl.type == TYPE_INT    ? "bafle_int"   :
                         node->var_decl.type == TYPE_FLOAT  ? "bafle_float" :
                         node->var_decl.type == TYPE_CHAR   ? "bafle_char"  :
                         node->var_decl.type == TYPE_STRING ? "bafle_str"   :
                         node->var_decl.type == TYPE_BOOL   ? "bafle_bool"  : "?",
                         init_type == TYPE_INT    ? "bafle_int"   :
                         init_type == TYPE_FLOAT  ? "bafle_float" :
                         init_type == TYPE_CHAR   ? "bafle_char"  :
                         init_type == TYPE_STRING ? "bafle_str"   :
                         init_type == TYPE_BOOL   ? "bafle_bool"  : "?");
                sem_error(node->line, buf);
            }
            sem_define(scope, node->var_decl.name, decl_type);
            break;
        }

        case NODE_ASSIGN: {
            DataType t = sem_lookup(scope, node->assign.name);
            if (t == TYPE_VOID) {
                snprintf(buf, sizeof(buf),
                         "variable '%s' no ha sido declarada.", node->assign.name);
                sem_error(node->line, buf);
            }
            check_expr(node->assign.value, scope);
            break;
        }

        case NODE_SHOW:
            if (node->show.expr) check_expr(node->show.expr, scope);
            break;

        case NODE_FUNC_CALL:
            check_expr(node, scope); /* reuse expr checker */
            break;

        case NODE_RETURN:
            if (node->ret.value) check_expr(node->ret.value, scope);
            break;

        case NODE_IF:
            check_expr(node->if_stmt.cond, scope);
            check_block(node->if_stmt.then_block, scope);
            if (node->if_stmt.else_block)
                check_block(node->if_stmt.else_block, scope);
            break;

        case NODE_WHILE:
            check_expr(node->while_stmt.cond, scope);
            check_block(node->while_stmt.body, scope);
            break;

        case NODE_FUNC_DECL:
            /* Bodies are checked in the second pass below */
            break;

        default:
            break;
    }
}

static void check_block(ASTNode *node, SemScope *parent) {
    if (!node) return;
    SemScope *scope = sem_scope_push(parent);
    for (int i = 0; i < node->block.count; i++)
        check_stmt(node->block.stmts[i], scope);
    sem_scope_pop(scope);
}

/* ─────────────────────────────────────────────
   Public entry point
   ───────────────────────────────────────────── */
int semantic_check(ASTNode *program) {
    error_count = 0;
    free_func_table();

    /* Pass 1: register all function signatures so forward calls work */
    for (int i = 0; i < program->block.count; i++) {
        ASTNode *s = program->block.stmts[i];
        if (!s || s->kind != NODE_FUNC_DECL) continue;

        /* Collect parameter types */
        DataType *ptypes = NULL;
        int pc = s->func_decl.param_count;
        if (pc > 0) {
            ptypes = (DataType *)malloc(sizeof(DataType) * pc);
            for (int j = 0; j < pc; j++)
                ptypes[j] = s->func_decl.params[j].type;
        }
        register_sig(s->func_decl.name, s->func_decl.return_type, ptypes, pc);
        free(ptypes);
    }

    /* Pass 2: check the whole program in a global scope */
    SemScope *global = sem_scope_push(NULL);

    for (int i = 0; i < program->block.count; i++) {
        ASTNode *s = program->block.stmts[i];
        if (!s) continue;

        if (s->kind == NODE_FUNC_DECL) {
            /* Open a fresh scope for the function body and bind params */
            SemScope *fscope = sem_scope_push(NULL); /* isolated */
            for (int j = 0; j < s->func_decl.param_count; j++)
                sem_define(fscope, s->func_decl.params[j].name,
                                   s->func_decl.params[j].type);
            /* Check body statements */
            for (int j = 0; j < s->func_decl.body->block.count; j++)
                check_stmt(s->func_decl.body->block.stmts[j], fscope);
            sem_scope_pop(fscope);
        } else {
            check_stmt(s, global);
        }
    }

    sem_scope_pop(global);
    free_func_table();

    if (error_count > 0)
        fprintf(stderr, "\n%d error(es) semántico(s) encontrado(s). Compilación abortada.\n\n",
                error_count);

    return error_count;
}
