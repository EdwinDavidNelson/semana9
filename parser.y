%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/symbols.h"
#include "src/ast.h"
#include "src/interpreter.h"

int  yylex(void);
void yyerror(const char *s);
extern FILE *yyin;

/* ── Statement accumulator ── */
static ASTNode **stmts      = NULL;
static int       stmt_count = 0;
static int       stmt_cap   = 16;

static void push_stmt(ASTNode *node) {
    if (!stmts) stmts = malloc(sizeof(ASTNode *) * stmt_cap);
    if (stmt_count >= stmt_cap) {
        stmt_cap *= 2;
        stmts = realloc(stmts, sizeof(ASTNode *) * stmt_cap);
    }
    stmts[stmt_count++] = node;
}

/* ── Block-statement accumulator stack ──
   Each nested block uses its own array; we keep a simple stack of these. */
#define MAX_BLOCK_DEPTH 64
static ASTNode **block_stmts [MAX_BLOCK_DEPTH];
static int       block_counts[MAX_BLOCK_DEPTH];
static int       block_caps  [MAX_BLOCK_DEPTH];
static int       block_depth = 0;

static void block_push_ctx(void) {
    block_stmts [block_depth] = NULL;
    block_counts[block_depth] = 0;
    block_caps  [block_depth] = 8;
    block_depth++;
}

static void block_add_stmt(ASTNode *node) {
    int d = block_depth - 1;
    if (!block_stmts[d])
        block_stmts[d] = malloc(sizeof(ASTNode *) * block_caps[d]);
    if (block_counts[d] >= block_caps[d]) {
        block_caps[d] *= 2;
        block_stmts[d] = realloc(block_stmts[d], sizeof(ASTNode *) * block_caps[d]);
    }
    block_stmts[d][block_counts[d]++] = node;
}

static ASTNode *block_pop_ctx(void) {
    block_depth--;
    int d = block_depth;
    return ast_block(block_stmts[d], block_counts[d]);
}

/* ── Argument list helper ── */
typedef struct { ASTNode **nodes; int count; int cap; } ArgList;
static ArgList *arglist_new(void) {
    ArgList *l = malloc(sizeof(ArgList));
    l->nodes = NULL; l->count = 0; l->cap = 4;
    return l;
}
static void arglist_add(ArgList *l, ASTNode *n) {
    if (!l->nodes) l->nodes = malloc(sizeof(ASTNode *) * l->cap);
    if (l->count >= l->cap) { l->cap *= 2; l->nodes = realloc(l->nodes, sizeof(ASTNode *) * l->cap); }
    l->nodes[l->count++] = n;
}

/* ── Parameter list helper ── */
typedef struct { ParamInfo *params; int count; int cap; } ParmList;
static ParmList *parmlist_new(void) {
    ParmList *l = malloc(sizeof(ParmList));
    l->params = NULL; l->count = 0; l->cap = 4;
    return l;
}
static void parmlist_add(ParmList *l, const char *name, DataType type) {
    if (!l->params) l->params = malloc(sizeof(ParamInfo) * l->cap);
    if (l->count >= l->cap) { l->cap *= 2; l->params = realloc(l->params, sizeof(ParamInfo) * l->cap); }
    l->params[l->count].name = strdup(name);
    l->params[l->count].type = type;
    l->count++;
}
%}

/* ── Union ── */
%union {
    int    i_val;
    double f_val;
    char   c_val;
    char  *s_val;
    int    type_enum;
    void  *ptr_val;
    struct ASTNode *node;
}

/* ── Tokens without values ── */
%token SET COMO SHOW FUNC RETORNA
%token BAFLE_INT_TYPE BAFLE_FLOAT_TYPE BAFLE_CHAR_TYPE BAFLE_STR_TYPE BAFLE_BOOL_TYPE
%token SEMICOLON ASSIGN LPARENT RPARENT LBRACE RBRACE COMMA ARROW
%token PLUS SUB MUL DIV

/* ── Tokens with values ── */
%token <s_val>  IDENTIFIER
%token <i_val>  INT_LITERAL
%token <f_val>  FLOAT_LITERAL
%token <c_val>  CHAR_LITERAL
%token <s_val>  STRING_LITERAL

/* ── Precedence ── */
%left  PLUS SUB
%left  MUL  DIV

/* ── Non-terminal types ── */
%type <type_enum> data_type
%type <node>      expression
%type <node>      statement
%type <node>      declaration
%type <node>      show_stmt
%type <node>      func_def
%type <node>      return_stmt
%type <node>      func_call_expr
%type <ptr_val>   arg_list
%type <ptr_val>   param_list

%%

/* ── Top-level program ── */
program
    : program statement          { if ($2) push_stmt($2); }
    | /* empty */
    ;

statement
    : declaration  SEMICOLON     { $$ = $1; }
    | show_stmt    SEMICOLON     { $$ = $1; }
    | func_def                   { $$ = $1; }
    | return_stmt  SEMICOLON     { $$ = $1; }
    | func_call_expr SEMICOLON   { $$ = $1; }
    ;

/* ── Type keywords ── */
data_type
    : BAFLE_INT_TYPE             { $$ = TYPE_INT;    }
    | BAFLE_FLOAT_TYPE           { $$ = TYPE_FLOAT;  }
    | BAFLE_CHAR_TYPE            { $$ = TYPE_CHAR;   }
    | BAFLE_STR_TYPE             { $$ = TYPE_STRING; }
    | BAFLE_BOOL_TYPE            { $$ = TYPE_BOOL;   }
    ;

/* ── Expressions ── */
expression
    : INT_LITERAL                { $$ = ast_lit_int($1);    }
    | FLOAT_LITERAL              { $$ = ast_lit_float($1);  }
    | CHAR_LITERAL               { $$ = ast_lit_char($1);   }
    | STRING_LITERAL             { $$ = ast_lit_string($1); free($1); }
    | IDENTIFIER                 { $$ = ast_identifier($1); free($1); }
    | func_call_expr             { $$ = $1; }
    | expression PLUS expression { $$ = ast_binop(OP_ADD, $1, $3); }
    | expression SUB  expression { $$ = ast_binop(OP_SUB, $1, $3); }
    | expression MUL  expression { $$ = ast_binop(OP_MUL, $1, $3); }
    | expression DIV  expression { $$ = ast_binop(OP_DIV, $1, $3); }
    | LPARENT expression RPARENT { $$ = $2; }
    ;

/* ── Function call expression ── */
func_call_expr
    : IDENTIFIER LPARENT RPARENT {
        $$ = ast_func_call($1, NULL, 0);
        free($1);
    }
    | IDENTIFIER LPARENT arg_list RPARENT {
        ArgList *al = (ArgList *)$3;
        $$ = ast_func_call($1, al->nodes, al->count);
        free($1);
        free(al);
    }
    ;

arg_list
    : expression {
        ArgList *al = arglist_new();
        arglist_add(al, $1);
        $$ = (void *)al;
    }
    | arg_list COMMA expression {
        ArgList *al = (ArgList *)$1;
        arglist_add(al, $3);
        $$ = (void *)al;
    }
    ;

/* ── Variable declaration: set nombre como bafle_type = expr ; ── */
declaration
    : SET IDENTIFIER COMO data_type ASSIGN expression {
        $$ = ast_var_decl($2, (DataType)$4, $6);
        free($2);
    }
    ;

/* ── show statement ── */
show_stmt
    : SHOW expression            { $$ = ast_show_var($2); }
    ;

/* ── Function definition ──
   func nombre(params) -> bafle_type { body } */
func_def
    : FUNC IDENTIFIER LPARENT RPARENT ARROW data_type
      LBRACE { block_push_ctx(); } func_body RBRACE {
        ASTNode *body = block_pop_ctx();
        $$ = ast_func_decl($2, (DataType)$6, NULL, 0, body);
        free($2);
    }
    | FUNC IDENTIFIER LPARENT param_list RPARENT ARROW data_type
      LBRACE { block_push_ctx(); } func_body RBRACE {
        ParmList *pl = (ParmList *)$4;
        ASTNode  *body = block_pop_ctx();
        $$ = ast_func_decl($2, (DataType)$7, pl->params, pl->count, body);
        for (int i = 0; i < pl->count; i++) free(pl->params[i].name);
        free(pl->params);
        free(pl);
        free($2);
    }
    ;

param_list
    : data_type IDENTIFIER {
        ParmList *pl = parmlist_new();
        parmlist_add(pl, $2, (DataType)$1);
        free($2);
        $$ = (void *)pl;
    }
    | param_list COMMA data_type IDENTIFIER {
        ParmList *pl = (ParmList *)$1;
        parmlist_add(pl, $4, (DataType)$3);
        free($4);
        $$ = (void *)pl;
    }
    ;

/* statements inside a function body accumulate into block context */
func_body
    : func_body func_body_stmt
    | /* empty */
    ;

func_body_stmt
    : declaration  SEMICOLON     { block_add_stmt($1); }
    | show_stmt    SEMICOLON     { block_add_stmt($1); }
    | return_stmt  SEMICOLON     { block_add_stmt($1); }
    | func_call_expr SEMICOLON   { block_add_stmt($1); }
    ;

return_stmt
    : RETORNA expression         { $$ = ast_return($2); }
    | RETORNA                    { $$ = ast_return(NULL); }
    ;

%%

/* ── Error handler ── */
void yyerror(const char *s) {
    fprintf(stderr, "Parse error: %s\n", s);
}

/* ── Entry point ── */
int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (!f) {
            fprintf(stderr, "Cannot open file: %s\n", argv[1]);
            return 1;
        }
        yyin = f;
    }

    int parse_result = yyparse();
    if (parse_result != 0) return parse_result;

    /* Build the root program node */
    ASTNode *program = ast_program(stmts, stmt_count);

    /* Optional: print AST for debugging (comment out in production) */
    /* ast_print(program, 0); */

    /* Interpret the AST */
    Interpreter *interp = interp_create();
    interp_run(interp, program);
    interp_destroy(interp);

    ast_free(program);
    printf("--- Backfile OK ---\n");
    return 0;
}
