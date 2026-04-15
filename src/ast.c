#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ── Generic allocator ── */
static ASTNode *new_node(NodeType kind) {
    ASTNode *n = (ASTNode *)calloc(1, sizeof(ASTNode));
    n->kind = kind;
    return n;
}

ASTNode *ast_program(ASTNode **stmts, int count) {
    ASTNode *n     = new_node(NODE_PROGRAM);
    n->block.stmts = stmts;
    n->block.count = count;
    return n;
}

ASTNode *ast_block(ASTNode **stmts, int count) {
    ASTNode *n     = new_node(NODE_BLOCK);
    n->block.stmts = stmts;
    n->block.count = count;
    return n;
}

ASTNode *ast_var_decl(const char *name, DataType type, ASTNode *init) {
    ASTNode *n        = new_node(NODE_VAR_DECL);
    n->var_decl.name  = strdup(name);
    n->var_decl.type  = type;
    n->var_decl.init  = init;
    return n;
}

ASTNode *ast_assign(const char *name, ASTNode *value) {
    ASTNode *n       = new_node(NODE_ASSIGN);
    n->assign.name   = strdup(name);
    n->assign.value  = value;
    return n;
}

ASTNode *ast_identifier(const char *name) {
    ASTNode *n      = new_node(NODE_IDENTIFIER);
    n->ident.name   = strdup(name);
    return n;
}

ASTNode *ast_lit_int(int v) {
    ASTNode *n         = new_node(NODE_LITERAL_INT);
    n->literal.type    = TYPE_INT;
    n->literal.i_val   = v;
    return n;
}

ASTNode *ast_lit_float(double v) {
    ASTNode *n         = new_node(NODE_LITERAL_FLOAT);
    n->literal.type    = TYPE_FLOAT;
    n->literal.f_val   = v;
    return n;
}

ASTNode *ast_lit_char(char v) {
    ASTNode *n         = new_node(NODE_LITERAL_CHAR);
    n->literal.type    = TYPE_CHAR;
    n->literal.c_val   = v;
    return n;
}

ASTNode *ast_lit_string(const char *v) {
    ASTNode *n         = new_node(NODE_LITERAL_STRING);
    n->literal.type    = TYPE_STRING;
    n->literal.s_val   = strdup(v);
    return n;
}

ASTNode *ast_lit_bool(int v) {
    ASTNode *n         = new_node(NODE_LITERAL_BOOL);
    n->literal.type    = TYPE_BOOL;
    n->literal.b_val   = v;
    return n;
}

ASTNode *ast_binop(BinOp op, ASTNode *l, ASTNode *r) {
    ASTNode *n      = new_node(NODE_BINARY_OP);
    n->binop.op     = op;
    n->binop.left   = l;
    n->binop.right  = r;
    return n;
}

ASTNode *ast_show_var(ASTNode *expr) {
    ASTNode *n          = new_node(NODE_SHOW);
    n->show.expr        = expr;
    n->show.str_literal = NULL;
    return n;
}

ASTNode *ast_show_str(const char *s) {
    ASTNode *n          = new_node(NODE_SHOW);
    n->show.expr        = NULL;
    n->show.str_literal = strdup(s);
    return n;
}

ASTNode *ast_func_decl(const char *name, DataType ret_type,
                        ParamInfo *params, int param_count, ASTNode *body) {
    ASTNode *n                  = new_node(NODE_FUNC_DECL);
    n->func_decl.name           = strdup(name);
    n->func_decl.return_type    = ret_type;
    n->func_decl.param_count    = param_count;
    n->func_decl.body           = body;
    if (param_count > 0) {
        n->func_decl.params = (ParamInfo *)malloc(sizeof(ParamInfo) * param_count);
        for (int i = 0; i < param_count; i++) {
            n->func_decl.params[i].name = strdup(params[i].name);
            n->func_decl.params[i].type = params[i].type;
        }
    } else {
        n->func_decl.params = NULL;
    }
    return n;
}

ASTNode *ast_func_call(const char *name, ASTNode **args, int arg_count) {
    ASTNode *n              = new_node(NODE_FUNC_CALL);
    n->func_call.name       = strdup(name);
    n->func_call.arg_count  = arg_count;
    n->func_call.args       = args; /* ownership transferred */
    return n;
}

ASTNode *ast_return(ASTNode *value) {
    ASTNode *n   = new_node(NODE_RETURN);
    n->ret.value = value;
    return n;
}

/* ── Free the entire subtree ── */
void ast_free(ASTNode *node) {
    if (!node) return;
    switch (node->kind) {
        case NODE_PROGRAM:
        case NODE_BLOCK:
            for (int i = 0; i < node->block.count; i++)
                ast_free(node->block.stmts[i]);
            free(node->block.stmts);
            break;
        case NODE_VAR_DECL:
            free(node->var_decl.name);
            ast_free(node->var_decl.init);
            break;
        case NODE_ASSIGN:
            free(node->assign.name);
            ast_free(node->assign.value);
            break;
        case NODE_IDENTIFIER:
            free(node->ident.name);
            break;
        case NODE_LITERAL_STRING:
            free(node->literal.s_val);
            break;
        case NODE_BINARY_OP:
            ast_free(node->binop.left);
            ast_free(node->binop.right);
            break;
        case NODE_SHOW:
            ast_free(node->show.expr);
            if (node->show.str_literal) free(node->show.str_literal);
            break;
        case NODE_FUNC_DECL:
            free(node->func_decl.name);
            for (int i = 0; i < node->func_decl.param_count; i++)
                free(node->func_decl.params[i].name);
            free(node->func_decl.params);
            ast_free(node->func_decl.body);
            break;
        case NODE_FUNC_CALL:
            free(node->func_call.name);
            for (int i = 0; i < node->func_call.arg_count; i++)
                ast_free(node->func_call.args[i]);
            free(node->func_call.args);
            break;
        case NODE_RETURN:
            ast_free(node->ret.value);
            break;
        default:
            break;
    }
    free(node);
}

/* ── Optional debug printer ── */
static const char *op_str(BinOp op) {
    switch (op) {
        case OP_ADD: return "+";
        case OP_SUB: return "-";
        case OP_MUL: return "*";
        case OP_DIV: return "/";
    }
    return "?";
}

static const char *type_str(DataType t) {
    switch (t) {
        case TYPE_INT:    return "int";
        case TYPE_FLOAT:  return "float";
        case TYPE_CHAR:   return "char";
        case TYPE_STRING: return "string";
        case TYPE_BOOL:   return "bool";
        case TYPE_VOID:   return "void";
    }
    return "?";
}

void ast_print(ASTNode *node, int indent) {
    if (!node) return;
    for (int i = 0; i < indent; i++) printf("  ");
    switch (node->kind) {
        case NODE_PROGRAM:
            printf("Program (%d stmts)\n", node->block.count);
            for (int i = 0; i < node->block.count; i++)
                ast_print(node->block.stmts[i], indent + 1);
            break;
        case NODE_BLOCK:
            printf("Block (%d stmts)\n", node->block.count);
            for (int i = 0; i < node->block.count; i++)
                ast_print(node->block.stmts[i], indent + 1);
            break;
        case NODE_VAR_DECL:
            printf("VarDecl %s : %s\n", node->var_decl.name, type_str(node->var_decl.type));
            ast_print(node->var_decl.init, indent + 1);
            break;
        case NODE_ASSIGN:
            printf("Assign %s\n", node->assign.name);
            ast_print(node->assign.value, indent + 1);
            break;
        case NODE_IDENTIFIER:
            printf("Identifier '%s'\n", node->ident.name);
            break;
        case NODE_LITERAL_INT:
            printf("LitInt %d\n", node->literal.i_val);
            break;
        case NODE_LITERAL_FLOAT:
            printf("LitFloat %g\n", node->literal.f_val);
            break;
        case NODE_LITERAL_CHAR:
            printf("LitChar '%c'\n", node->literal.c_val);
            break;
        case NODE_LITERAL_STRING:
            printf("LitString \"%s\"\n", node->literal.s_val);
            break;
        case NODE_LITERAL_BOOL:
            printf("LitBool %s\n", node->literal.b_val ? "true" : "false");
            break;
        case NODE_BINARY_OP:
            printf("BinOp '%s'\n", op_str(node->binop.op));
            ast_print(node->binop.left,  indent + 1);
            ast_print(node->binop.right, indent + 1);
            break;
        case NODE_SHOW:
            if (node->show.str_literal)
                printf("Show \"%s\"\n", node->show.str_literal);
            else {
                printf("Show\n");
                ast_print(node->show.expr, indent + 1);
            }
            break;
        case NODE_FUNC_DECL:
            printf("FuncDecl '%s' -> %s (%d params)\n",
                   node->func_decl.name,
                   type_str(node->func_decl.return_type),
                   node->func_decl.param_count);
            for (int i = 0; i < node->func_decl.param_count; i++) {
                for (int j = 0; j < indent + 1; j++) printf("  ");
                printf("Param '%s' : %s\n",
                       node->func_decl.params[i].name,
                       type_str(node->func_decl.params[i].type));
            }
            ast_print(node->func_decl.body, indent + 1);
            break;
        case NODE_FUNC_CALL:
            printf("FuncCall '%s' (%d args)\n", node->func_call.name, node->func_call.arg_count);
            for (int i = 0; i < node->func_call.arg_count; i++)
                ast_print(node->func_call.args[i], indent + 1);
            break;
        case NODE_RETURN:
            printf("Return\n");
            ast_print(node->ret.value, indent + 1);
            break;
    }
}
