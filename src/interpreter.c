#include "interpreter.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Forward declarations ── */
static Value eval_expr (Interpreter *interp, ASTNode *node);
static void  exec_stmt (Interpreter *interp, ASTNode *node);
static void  exec_block(Interpreter *interp, ASTNode *node);

/* ─────────────────────────────────────────────
   Helpers
   ───────────────────────────────────────────── */

static Value make_int   (int v)    { Value r; r.type = TYPE_INT;    r.i_val = v; return r; }
static Value make_float (double v) { Value r; r.type = TYPE_FLOAT;  r.f_val = v; return r; }
static Value make_char  (char v)   { Value r; r.type = TYPE_CHAR;   r.c_val = v; return r; }
static Value make_bool  (int v)    { Value r; r.type = TYPE_BOOL;   r.b_val = v; return r; }
static Value make_string(const char *s) {
    Value r; r.type = TYPE_STRING; r.s_val = strdup(s); return r;
}
static Value make_void(void) {
    Value r; r.type = TYPE_VOID; r.i_val = 0; return r;
}

/* Convert any numeric Value to a double for arithmetic */
static double as_double(Value v) {
    switch (v.type) {
        case TYPE_INT:   return (double)v.i_val;
        case TYPE_FLOAT: return v.f_val;
        case TYPE_CHAR:  return (double)v.c_val;
        case TYPE_BOOL:  return (double)v.b_val;
        default: return 0.0;
    }
}

/* Write a Value into a SymbolEntry */
static void value_to_entry(SymbolEntry *e, Value v) {
    e->data_type = v.type;
    switch (v.type) {
        case TYPE_INT:    e->data.i_value = v.i_val; break;
        case TYPE_FLOAT:  e->data.f_value = v.f_val; break;
        case TYPE_CHAR:   e->data.c_value = v.c_val; break;
        case TYPE_BOOL:   e->data.b_value = v.b_val; break;
        case TYPE_STRING:
            if (e->data.s_value) free(e->data.s_value);
            e->data.s_value = strdup(v.s_val);
            break;
        default: break;
    }
}

/* Read a Value from a SymbolEntry */
static Value entry_to_value(SymbolEntry *e) {
    switch (e->data_type) {
        case TYPE_INT:    return make_int(e->data.i_value);
        case TYPE_FLOAT:  return make_float(e->data.f_value);
        case TYPE_CHAR:   return make_char(e->data.c_value);
        case TYPE_BOOL:   return make_bool(e->data.b_value);
        case TYPE_STRING: return make_string(e->data.s_value);
        default:          return make_void();
    }
}

/* Store a function record */
static void register_function(Interpreter *interp, ASTNode *decl) {
    FuncRecord *rec = (FuncRecord *)malloc(sizeof(FuncRecord));
    rec->name        = strdup(decl->func_decl.name);
    rec->return_type = decl->func_decl.return_type;
    rec->param_count = decl->func_decl.param_count;
    rec->params      = decl->func_decl.params; /* borrow – AST owns them */
    rec->body        = decl->func_decl.body;
    rec->next        = interp->functions;
    interp->functions = rec;
}

static FuncRecord *find_function(Interpreter *interp, const char *name) {
    FuncRecord *r = interp->functions;
    while (r) {
        if (strcmp(r->name, name) == 0) return r;
        r = r->next;
    }
    return NULL;
}

/* ─────────────────────────────────────────────
   Expression evaluator
   ───────────────────────────────────────────── */

static Value eval_expr(Interpreter *interp, ASTNode *node) {
    if (!node) return make_void();

    switch (node->kind) {

        /* ── Literals ── */
        case NODE_LITERAL_INT:    return make_int(node->literal.i_val);
        case NODE_LITERAL_FLOAT:  return make_float(node->literal.f_val);
        case NODE_LITERAL_CHAR:   return make_char(node->literal.c_val);
        case NODE_LITERAL_BOOL:   return make_bool(node->literal.b_val);
        case NODE_LITERAL_STRING: return make_string(node->literal.s_val);

        /* ── Variable read (shadowing: nearest scope wins) ── */
        case NODE_IDENTIFIER: {
            SymbolEntry *e = symbol_lookup(interp->scope, node->ident.name);
            if (!e) {
                fprintf(stderr, "Runtime error: undefined variable '%s'\n", node->ident.name);
                return make_void();
            }
            return entry_to_value(e);
        }

        /* ── Arithmetic ── */
        case NODE_BINARY_OP: {
            Value lv = eval_expr(interp, node->binop.left);
            Value rv = eval_expr(interp, node->binop.right);
            double l = as_double(lv);
            double r = as_double(rv);
            double result;
            switch (node->binop.op) {
                case OP_ADD: result = l + r; break;
                case OP_SUB: result = l - r; break;
                case OP_MUL: result = l * r; break;
                case OP_DIV:
                    if (r == 0.0) {
                        fprintf(stderr, "Runtime error: division by zero\n");
                        return make_void();
                    }
                    result = l / r;
                    break;
                default: result = 0; break;
            }
            /* Preserve int type when both operands are ints */
            if (lv.type == TYPE_INT && rv.type == TYPE_INT)
                return make_int((int)result);
            return make_float(result);
        }

        /* ── Function call ── */
        case NODE_FUNC_CALL: {
            FuncRecord *rec = find_function(interp, node->func_call.name);
            if (!rec) {
                fprintf(stderr, "Runtime error: undefined function '%s'\n", node->func_call.name);
                return make_void();
            }
            if (node->func_call.arg_count != rec->param_count) {
                fprintf(stderr,
                    "Runtime error: '%s' expects %d args, got %d\n",
                    rec->name, rec->param_count, node->func_call.arg_count);
                return make_void();
            }

            /* Evaluate arguments in the *caller* scope first */
            Value *arg_vals = NULL;
            if (rec->param_count > 0) {
                arg_vals = (Value *)malloc(sizeof(Value) * rec->param_count);
                for (int i = 0; i < rec->param_count; i++)
                    arg_vals[i] = eval_expr(interp, node->func_call.args[i]);
            }

            /* Push a new scope for the function body (shadowing) */
            Scope *saved_scope = interp->scope;
            interp->scope = scope_push(NULL); /* functions get an isolated scope */

            /* Bind parameters */
            for (int i = 0; i < rec->param_count; i++) {
                SymbolEntry *e = symbol_create(interp->scope,
                                               rec->params[i].name,
                                               rec->params[i].type);
                value_to_entry(e, arg_vals[i]);
                if (arg_vals[i].type == TYPE_STRING && arg_vals[i].s_val)
                    free(arg_vals[i].s_val);
            }
            free(arg_vals);

            /* Execute body */
            exec_block(interp, rec->body);

            Value ret = interp->returning ? interp->return_val : make_void();
            interp->returning = 0;

            /* Pop function scope, restore caller scope */
            scope_pop(interp->scope);
            interp->scope = saved_scope;

            return ret;
        }

        default:
            fprintf(stderr, "eval_expr: unhandled node kind %d\n", node->kind);
            return make_void();
    }
}

/* ─────────────────────────────────────────────
   Statement executor
   ───────────────────────────────────────────── */

static void exec_stmt(Interpreter *interp, ASTNode *node) {
    if (!node || interp->returning) return;

    switch (node->kind) {

        /* ── Variable declaration with shadowing ── */
        case NODE_VAR_DECL: {
            Value v = eval_expr(interp, node->var_decl.init);
            /* shadow_create: always create a new entry in the current frame */
            SymbolEntry *e = symbol_create(interp->scope,
                                           node->var_decl.name,
                                           node->var_decl.type);
            value_to_entry(e, v);
            if (v.type == TYPE_STRING && v.s_val) free(v.s_val);
            break;
        }

        /* ── Assignment (mutate nearest visible binding) ── */
        case NODE_ASSIGN: {
            SymbolEntry *e = symbol_lookup(interp->scope, node->assign.name);
            if (!e) {
                fprintf(stderr, "Runtime error: undefined variable '%s'\n", node->assign.name);
                break;
            }
            Value v = eval_expr(interp, node->assign.value);
            value_to_entry(e, v);
            if (v.type == TYPE_STRING && v.s_val) free(v.s_val);
            break;
        }

        /* ── Show ── */
        case NODE_SHOW: {
            if (node->show.str_literal) {
                printf("%s\n", node->show.str_literal);
            } else {
                Value v = eval_expr(interp, node->show.expr);
                switch (v.type) {
                    case TYPE_INT:    printf("%d\n",  v.i_val); break;
                    case TYPE_FLOAT:  printf("%g\n",  v.f_val); break;
                    case TYPE_CHAR:   printf("%c\n",  v.c_val); break;
                    case TYPE_BOOL:   printf("%d\n",  v.b_val); break;
                    case TYPE_STRING:
                        printf("%s\n", v.s_val);
                        free(v.s_val);
                        break;
                    default: break;
                }
            }
            break;
        }

        /* ── Function declaration – register, don't execute ── */
        case NODE_FUNC_DECL:
            register_function(interp, node);
            break;

        /* ── Return ── */
        case NODE_RETURN:
            interp->return_val = eval_expr(interp, node->ret.value);
            interp->returning  = 1;
            break;

        /* ── A bare function call used as a statement ── */
        case NODE_FUNC_CALL:
            eval_expr(interp, node);
            break;

        default:
            fprintf(stderr, "exec_stmt: unhandled node kind %d\n", node->kind);
            break;
    }
}

/* Execute a BLOCK or PROGRAM node (sequences of statements) */
static void exec_block(Interpreter *interp, ASTNode *node) {
    if (!node) return;
    /* Push a new variable scope for each block (enables shadowing) */
    Scope *prev = interp->scope;
    interp->scope = scope_push(prev);

    for (int i = 0; i < node->block.count && !interp->returning; i++)
        exec_stmt(interp, node->block.stmts[i]);

    scope_pop(interp->scope);
    interp->scope = prev;
}

/* ─────────────────────────────────────────────
   Public API
   ───────────────────────────────────────────── */

Interpreter *interp_create(void) {
    Interpreter *interp  = (Interpreter *)calloc(1, sizeof(Interpreter));
    interp->scope        = scope_push(NULL); /* global scope */
    interp->functions    = NULL;
    interp->returning    = 0;
    return interp;
}

void interp_destroy(Interpreter *interp) {
    scope_pop(interp->scope);
    FuncRecord *r = interp->functions;
    while (r) {
        FuncRecord *next = r->next;
        free(r->name);
        free(r);
        r = next;
    }
    free(interp);
}

void interp_run(Interpreter *interp, ASTNode *program) {
    /* First pass: register all functions so forward calls work */
    for (int i = 0; i < program->block.count; i++) {
        ASTNode *s = program->block.stmts[i];
        if (s && s->kind == NODE_FUNC_DECL)
            register_function(interp, s);
    }
    /* Second pass: execute everything else in order */
    for (int i = 0; i < program->block.count && !interp->returning; i++) {
        ASTNode *s = program->block.stmts[i];
        if (s && s->kind != NODE_FUNC_DECL)
            exec_stmt(interp, s);
    }
}
