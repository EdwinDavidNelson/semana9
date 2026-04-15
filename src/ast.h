#ifndef AST_H
#define AST_H

#include "symbols.h"

/* ── Node types ── */
typedef enum {
    NODE_PROGRAM,
    NODE_BLOCK,
    NODE_VAR_DECL,
    NODE_ASSIGN,
    NODE_IDENTIFIER,
    NODE_LITERAL_INT,
    NODE_LITERAL_FLOAT,
    NODE_LITERAL_CHAR,
    NODE_LITERAL_STRING,
    NODE_LITERAL_BOOL,
    NODE_BINARY_OP,
    NODE_SHOW,
    NODE_FUNC_DECL,
    NODE_FUNC_CALL,
    NODE_RETURN,
} NodeType;

/* ── Binary operators ── */
typedef enum {
    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
} BinOp;

/* ── Forward declaration ── */
typedef struct ASTNode ASTNode;

/* ── Parameter descriptor ── */
typedef struct {
    char    *name;
    DataType type;
} ParamInfo;

/* ── The AST node ── */
struct ASTNode {
    NodeType kind;

    union {
        /* NODE_PROGRAM / NODE_BLOCK */
        struct {
            ASTNode **stmts;
            int       count;
        } block;

        /* NODE_VAR_DECL */
        struct {
            char    *name;
            DataType type;
            ASTNode *init;   /* initialiser expression */
        } var_decl;

        /* NODE_ASSIGN */
        struct {
            char    *name;
            ASTNode *value;
        } assign;

        /* NODE_IDENTIFIER */
        struct {
            char *name;
        } ident;

        /* Literals */
        struct {
            DataType type;
            union {
                int    i_val;
                double f_val;
                char   c_val;
                char  *s_val;
                int    b_val;
            };
        } literal;

        /* NODE_BINARY_OP */
        struct {
            BinOp    op;
            ASTNode *left;
            ASTNode *right;
        } binop;

        /* NODE_SHOW */
        struct {
            ASTNode *expr;        /* NULL when showing a string literal */
            char    *str_literal; /* used when expr==NULL               */
        } show;

        /* NODE_FUNC_DECL */
        struct {
            char      *name;
            DataType   return_type;
            ParamInfo *params;
            int        param_count;
            ASTNode   *body;   /* NODE_BLOCK */
        } func_decl;

        /* NODE_FUNC_CALL */
        struct {
            char     *name;
            ASTNode **args;
            int       arg_count;
        } func_call;

        /* NODE_RETURN */
        struct {
            ASTNode *value; /* may be NULL */
        } ret;
    };
};

/* ── Constructor helpers ── */
ASTNode *ast_program   (ASTNode **stmts, int count);
ASTNode *ast_block     (ASTNode **stmts, int count);
ASTNode *ast_var_decl  (const char *name, DataType type, ASTNode *init);
ASTNode *ast_assign    (const char *name, ASTNode *value);
ASTNode *ast_identifier(const char *name);
ASTNode *ast_lit_int   (int v);
ASTNode *ast_lit_float (double v);
ASTNode *ast_lit_char  (char v);
ASTNode *ast_lit_string(const char *v);
ASTNode *ast_lit_bool  (int v);
ASTNode *ast_binop     (BinOp op, ASTNode *l, ASTNode *r);
ASTNode *ast_show_var  (ASTNode *expr);
ASTNode *ast_show_str  (const char *s);
ASTNode *ast_func_decl (const char *name, DataType ret_type,
                         ParamInfo *params, int param_count, ASTNode *body);
ASTNode *ast_func_call (const char *name, ASTNode **args, int arg_count);
ASTNode *ast_return    (ASTNode *value);

void     ast_free(ASTNode *node);
void     ast_print(ASTNode *node, int indent); /* optional debug printer */

#endif /* AST_H */
