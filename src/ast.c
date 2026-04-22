#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static ASTNode *new_node(NodeType kind) {
    ASTNode *n = (ASTNode *)calloc(1, sizeof(ASTNode));
    n->kind = kind; n->line = 0; return n;
}

ASTNode *ast_program(ASTNode **s, int c)  { ASTNode *n=new_node(NODE_PROGRAM); n->block.stmts=s; n->block.count=c; return n; }
ASTNode *ast_block  (ASTNode **s, int c)  { ASTNode *n=new_node(NODE_BLOCK);   n->block.stmts=s; n->block.count=c; return n; }

ASTNode *ast_var_decl(const char *name, DataType type, ASTNode *init) {
    ASTNode *n=new_node(NODE_VAR_DECL);
    n->var_decl.name=strdup(name); n->var_decl.type=type; n->var_decl.init=init; return n;
}
ASTNode *ast_assign(const char *name, ASTNode *value) {
    ASTNode *n=new_node(NODE_ASSIGN);
    n->assign.name=strdup(name); n->assign.value=value; return n;
}
ASTNode *ast_identifier(const char *name) {
    ASTNode *n=new_node(NODE_IDENTIFIER); n->ident.name=strdup(name); return n;
}
ASTNode *ast_lit_int   (int v)         { ASTNode *n=new_node(NODE_LITERAL_INT);    n->literal.type=TYPE_INT;    n->literal.i_val=v; return n; }
ASTNode *ast_lit_float (double v)      { ASTNode *n=new_node(NODE_LITERAL_FLOAT);  n->literal.type=TYPE_FLOAT;  n->literal.f_val=v; return n; }
ASTNode *ast_lit_char  (char v)        { ASTNode *n=new_node(NODE_LITERAL_CHAR);   n->literal.type=TYPE_CHAR;   n->literal.c_val=v; return n; }
ASTNode *ast_lit_string(const char *v) { ASTNode *n=new_node(NODE_LITERAL_STRING); n->literal.type=TYPE_STRING; n->literal.s_val=strdup(v); return n; }
ASTNode *ast_lit_bool  (int v)         { ASTNode *n=new_node(NODE_LITERAL_BOOL);   n->literal.type=TYPE_BOOL;   n->literal.b_val=v; return n; }

ASTNode *ast_binop(BinOp op, ASTNode *l, ASTNode *r) {
    ASTNode *n=new_node(NODE_BINARY_OP); n->binop.op=op; n->binop.left=l; n->binop.right=r; return n;
}
ASTNode *ast_compare(CmpOp op, ASTNode *l, ASTNode *r) {
    ASTNode *n=new_node(NODE_COMPARE); n->cmp.op=op; n->cmp.left=l; n->cmp.right=r; return n;
}
ASTNode *ast_show_var(ASTNode *expr) {
    ASTNode *n=new_node(NODE_SHOW); n->show.expr=expr; n->show.str_literal=NULL; return n;
}
ASTNode *ast_show_str(const char *s) {
    ASTNode *n=new_node(NODE_SHOW); n->show.expr=NULL; n->show.str_literal=strdup(s); return n;
}
ASTNode *ast_func_decl(const char *name, DataType ret, ParamInfo *params, int pc, ASTNode *body) {
    ASTNode *n=new_node(NODE_FUNC_DECL);
    n->func_decl.name=strdup(name); n->func_decl.return_type=ret;
    n->func_decl.param_count=pc; n->func_decl.body=body;
    if(pc>0){
        n->func_decl.params=(ParamInfo*)malloc(sizeof(ParamInfo)*pc);
        for(int i=0;i<pc;i++){n->func_decl.params[i].name=strdup(params[i].name);n->func_decl.params[i].type=params[i].type;}
    } else n->func_decl.params=NULL;
    return n;
}
ASTNode *ast_func_call(const char *name, ASTNode **args, int ac) {
    ASTNode *n=new_node(NODE_FUNC_CALL);
    n->func_call.name=strdup(name); n->func_call.arg_count=ac; n->func_call.args=args; return n;
}
ASTNode *ast_return(ASTNode *value) { ASTNode *n=new_node(NODE_RETURN); n->ret.value=value; return n; }
ASTNode *ast_if(ASTNode *cond, ASTNode *then_block, ASTNode *else_block) {
    ASTNode *n=new_node(NODE_IF);
    n->if_stmt.cond=cond; n->if_stmt.then_block=then_block; n->if_stmt.else_block=else_block; return n;
}
ASTNode *ast_while(ASTNode *cond, ASTNode *body) {
    ASTNode *n=new_node(NODE_WHILE); n->while_stmt.cond=cond; n->while_stmt.body=body; return n;
}

/* ── Free ── */
void ast_free(ASTNode *node) {
    if(!node) return;
    switch(node->kind){
        case NODE_PROGRAM: case NODE_BLOCK:
            for(int i=0;i<node->block.count;i++) ast_free(node->block.stmts[i]);
            free(node->block.stmts); break;
        case NODE_VAR_DECL:   free(node->var_decl.name); ast_free(node->var_decl.init); break;
        case NODE_ASSIGN:     free(node->assign.name); ast_free(node->assign.value); break;
        case NODE_IDENTIFIER: free(node->ident.name); break;
        case NODE_LITERAL_STRING: free(node->literal.s_val); break;
        case NODE_BINARY_OP:  ast_free(node->binop.left); ast_free(node->binop.right); break;
        case NODE_COMPARE:    ast_free(node->cmp.left); ast_free(node->cmp.right); break;
        case NODE_SHOW: ast_free(node->show.expr); if(node->show.str_literal)free(node->show.str_literal); break;
        case NODE_FUNC_DECL:
            free(node->func_decl.name);
            for(int i=0;i<node->func_decl.param_count;i++) free(node->func_decl.params[i].name);
            free(node->func_decl.params); ast_free(node->func_decl.body); break;
        case NODE_FUNC_CALL:
            free(node->func_call.name);
            for(int i=0;i<node->func_call.arg_count;i++) ast_free(node->func_call.args[i]);
            free(node->func_call.args); break;
        case NODE_RETURN: ast_free(node->ret.value); break;
        case NODE_IF: ast_free(node->if_stmt.cond); ast_free(node->if_stmt.then_block); ast_free(node->if_stmt.else_block); break;
        case NODE_WHILE: ast_free(node->while_stmt.cond); ast_free(node->while_stmt.body); break;
        default: break;
    }
    free(node);
}

/* ───────────────────────────────────────────────────────────
   AST PRINTER — human-readable intermediate representation
   ─────────────────────────────────────────────────────────── */

static const char *op_str (BinOp o){
    switch(o){case OP_ADD:return"+";case OP_SUB:return"-";case OP_MUL:return"*";case OP_DIV:return"/";}return"?";
}
static const char *cmp_str(CmpOp o){
    switch(o){case CMP_EQ:return"==";case CMP_NEQ:return"!=";case CMP_LT:return"<";
    case CMP_GT:return">";case CMP_LTE:return"<=";case CMP_GTE:return">=";}return"?";
}
static const char *type_str(DataType t){
    switch(t){case TYPE_INT:return"bafle_int";case TYPE_FLOAT:return"bafle_float";
    case TYPE_CHAR:return"bafle_char";case TYPE_STRING:return"bafle_str";
    case TYPE_BOOL:return"bafle_bool";case TYPE_VOID:return"void";}return"?";
}

/* Draw tree lines: "│  " for ongoing branches, "   " for last child */
static void print_prefix(const char *prefix, int is_last) {
    printf("%s%s", prefix, is_last ? "└─ " : "├─ ");
}
static void make_child_prefix(const char *prefix, int is_last, char *out, int out_size) {
    snprintf(out, out_size, "%s%s", prefix, is_last ? "   " : "│  ");
}

/* expr printer — returns a short inline string for simple nodes */
static void print_expr(ASTNode *node, const char *prefix, int is_last);
static void print_node(ASTNode *node, const char *prefix, int is_last);

static void print_children(ASTNode **stmts, int count, const char *prefix) {
    for (int i = 0; i < count; i++)
        print_node(stmts[i], prefix, i == count - 1);
}

static void print_node(ASTNode *node, const char *prefix, int is_last) {
    if (!node) return;
    char child_pfx[512];
    make_child_prefix(prefix, is_last, child_pfx, sizeof(child_pfx));

    switch (node->kind) {

        case NODE_PROGRAM:
            printf("Programa  (%d sentencias)\n", node->block.count);
            print_children(node->block.stmts, node->block.count, "");
            break;

        case NODE_BLOCK:
            printf("Bloque\n");
            print_children(node->block.stmts, node->block.count, child_pfx);
            break;

        case NODE_VAR_DECL:
            print_prefix(prefix, is_last);
            printf("set '%s'  tipo: %s  (línea %d)\n",
                   node->var_decl.name, type_str(node->var_decl.type), node->line);
            if (node->var_decl.init) {
                printf("%s└─ valor: ", child_pfx);
                print_expr(node->var_decl.init, child_pfx, 1);
            }
            break;

        case NODE_SHOW:
            print_prefix(prefix, is_last);
            if (node->show.str_literal)
                printf("show  \"%s\"  (línea %d)\n", node->show.str_literal, node->line);
            else {
                printf("show  (línea %d)\n", node->line);
                printf("%s└─ ", child_pfx);
                print_expr(node->show.expr, child_pfx, 1);
            }
            break;

        case NODE_FUNC_DECL:
            print_prefix(prefix, is_last);
            printf("func '%s'  retorna: %s  (línea %d)\n",
                   node->func_decl.name, type_str(node->func_decl.return_type), node->line);
            /* params */
            if (node->func_decl.param_count > 0) {
                printf("%s├─ parametros:\n", child_pfx);
                for (int i = 0; i < node->func_decl.param_count; i++) {
                    int last_p = (i == node->func_decl.param_count - 1)
                                 && (node->func_decl.body == NULL);
                    printf("%s│  %s '%s' : %s\n", child_pfx,
                           last_p ? "└─" : "├─",
                           node->func_decl.params[i].name,
                           type_str(node->func_decl.params[i].type));
                }
            }
            /* body */
            if (node->func_decl.body) {
                printf("%s└─ cuerpo:\n", child_pfx);
                char body_pfx[512];
                snprintf(body_pfx, sizeof(body_pfx), "%s   ", child_pfx);
                print_children(node->func_decl.body->block.stmts,
                               node->func_decl.body->block.count, body_pfx);
            }
            break;

        case NODE_FUNC_CALL:
            print_prefix(prefix, is_last);
            printf("llamar '%s'  (línea %d)\n", node->func_call.name, node->line);
            for (int i = 0; i < node->func_call.arg_count; i++) {
                int last_a = (i == node->func_call.arg_count - 1);
                printf("%s%s arg %d: ", child_pfx, last_a ? "└─" : "├─", i + 1);
                print_expr(node->func_call.args[i], child_pfx, last_a);
            }
            break;

        case NODE_RETURN:
            print_prefix(prefix, is_last);
            printf("retorna  (línea %d)\n", node->line);
            if (node->ret.value) {
                printf("%s└─ ", child_pfx);
                print_expr(node->ret.value, child_pfx, 1);
            }
            break;

        case NODE_IF:
            print_prefix(prefix, is_last);
            printf("si  (línea %d)\n", node->line);
            /* condition */
            printf("%s├─ condicion: ", child_pfx);
            print_expr(node->if_stmt.cond, child_pfx, 0);
            /* then */
            {
                int has_else = (node->if_stmt.else_block != NULL);
                printf("%s%s entonces:\n", child_pfx, has_else ? "├─" : "└─");
                char then_pfx[512];
                snprintf(then_pfx, sizeof(then_pfx), "%s%s   ", child_pfx, has_else ? "│" : " ");
                print_children(node->if_stmt.then_block->block.stmts,
                               node->if_stmt.then_block->block.count, then_pfx);
                if (has_else) {
                    printf("%s└─ sino:\n", child_pfx);
                    char else_pfx[512];
                    snprintf(else_pfx, sizeof(else_pfx), "%s      ", child_pfx);
                    print_children(node->if_stmt.else_block->block.stmts,
                                   node->if_stmt.else_block->block.count, else_pfx);
                }
            }
            break;

        case NODE_WHILE:
            print_prefix(prefix, is_last);
            printf("mientras  (línea %d)\n", node->line);
            printf("%s├─ condicion: ", child_pfx);
            print_expr(node->while_stmt.cond, child_pfx, 0);
            printf("%s└─ cuerpo:\n", child_pfx);
            {
                char body_pfx[512];
                snprintf(body_pfx, sizeof(body_pfx), "%s      ", child_pfx);
                print_children(node->while_stmt.body->block.stmts,
                               node->while_stmt.body->block.count, body_pfx);
            }
            break;

        default:
            /* fallback: treat as expression */
            print_prefix(prefix, is_last);
            print_expr(node, child_pfx, 1);
            break;
    }
}

/* Inline expression printer — fits on one line when possible */
static void print_expr(ASTNode *node, const char *prefix, int is_last) {
    (void)prefix; (void)is_last;
    if (!node) { printf("(nulo)\n"); return; }
    switch (node->kind) {
        case NODE_LITERAL_INT:    printf("%d\n",   node->literal.i_val); break;
        case NODE_LITERAL_FLOAT:  printf("%g\n",   node->literal.f_val); break;
        case NODE_LITERAL_CHAR:   printf("'%c'\n", node->literal.c_val); break;
        case NODE_LITERAL_STRING: printf("\"%s\"\n", node->literal.s_val); break;
        case NODE_LITERAL_BOOL:   printf("%s\n",   node->literal.b_val ? "verdadero" : "falso"); break;
        case NODE_IDENTIFIER:     printf("%s\n",   node->ident.name); break;
        case NODE_BINARY_OP:
            printf("(%s", op_str(node->binop.op));
            /* inline sub-expressions */
            printf("  "); print_expr(node->binop.left,  prefix, 0);
            printf("%s     ", prefix); print_expr(node->binop.right, prefix, 1);
            break;
        case NODE_COMPARE:
            printf("(%s  ", cmp_str(node->cmp.op));
            print_expr(node->cmp.left,  prefix, 0);
            printf("%s      ", prefix); print_expr(node->cmp.right, prefix, 1);
            break;
        case NODE_FUNC_CALL:
            printf("llamar %s(", node->func_call.name);
            for (int i = 0; i < node->func_call.arg_count; i++) {
                if (i) printf(", ");
                switch (node->func_call.args[i]->kind) {
                    case NODE_LITERAL_INT:   printf("%d",   node->func_call.args[i]->literal.i_val); break;
                    case NODE_LITERAL_FLOAT: printf("%g",   node->func_call.args[i]->literal.f_val); break;
                    case NODE_IDENTIFIER:    printf("%s",   node->func_call.args[i]->ident.name); break;
                    default:                 printf("..."); break;
                }
            }
            printf(")\n");
            break;
        default: printf("expr\n"); break;
    }
}

void ast_print(ASTNode *node, int indent) {
    (void)indent;
    if (!node) return;
    print_node(node, "", 1);
}
