%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src/symbols.h"
#include "src/ast.h"
#include "src/interpreter.h"
#include "src/semantic.h"

int  yylex(void);
void yyerror(const char *s);
extern FILE *yyin;
extern int   yylineno;

/* ── Statement accumulator ── */
static ASTNode **stmts    = NULL;
static int stmt_count     = 0;
static int stmt_cap       = 16;

static void push_stmt(ASTNode *node) {
    if (!stmts) stmts = malloc(sizeof(ASTNode *) * stmt_cap);
    if (stmt_count >= stmt_cap) { stmt_cap *= 2; stmts = realloc(stmts, sizeof(ASTNode *) * stmt_cap); }
    stmts[stmt_count++] = node;
}

/* ── Block-statement stack ── */
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
    if (!block_stmts[d]) block_stmts[d] = malloc(sizeof(ASTNode *) * block_caps[d]);
    if (block_counts[d] >= block_caps[d]) {
        block_caps[d] *= 2;
        block_stmts[d] = realloc(block_stmts[d], sizeof(ASTNode *) * block_caps[d]);
    }
    block_stmts[d][block_counts[d]++] = node;
}
static ASTNode *block_pop_ctx(void) {
    block_depth--;
    int d = block_depth;
    ASTNode *b = ast_block(block_stmts[d], block_counts[d]);
    b->line = yylineno;
    return b;
}

/* ── Argument list ── */
typedef struct { ASTNode **nodes; int count; int cap; } ArgList;
static ArgList *arglist_new(void) {
    ArgList *l = malloc(sizeof(ArgList)); l->nodes=NULL; l->count=0; l->cap=4; return l;
}
static void arglist_add(ArgList *l, ASTNode *n) {
    if(!l->nodes) l->nodes=malloc(sizeof(ASTNode*)*l->cap);
    if(l->count>=l->cap){l->cap*=2;l->nodes=realloc(l->nodes,sizeof(ASTNode*)*l->cap);}
    l->nodes[l->count++]=n;
}

/* ── Parameter list ── */
typedef struct { ParamInfo *params; int count; int cap; } ParmList;
static ParmList *parmlist_new(void) {
    ParmList *l=malloc(sizeof(ParmList)); l->params=NULL; l->count=0; l->cap=4; return l;
}
static void parmlist_add(ParmList *l, const char *name, DataType type) {
    if(!l->params) l->params=malloc(sizeof(ParamInfo)*l->cap);
    if(l->count>=l->cap){l->cap*=2;l->params=realloc(l->params,sizeof(ParamInfo)*l->cap);}
    l->params[l->count].name=strdup(name); l->params[l->count].type=type; l->count++;
}
%}

%union {
    int    i_val;
    double f_val;
    char   c_val;
    char  *s_val;
    int    type_enum;
    void  *ptr_val;
    struct ASTNode *node;
}

/* ── Tokens sin valor ── */
%token SET COMO SHOW FUNC RETORNA SI SINO MIENTRAS
%token BAFLE_INT_TYPE BAFLE_FLOAT_TYPE BAFLE_CHAR_TYPE BAFLE_STR_TYPE BAFLE_BOOL_TYPE
%token SEMICOLON ASSIGN LPARENT RPARENT LBRACE RBRACE COMMA ARROW
%token PLUS SUB MUL DIV
%token EQ NEQ LT GT LTE GTE

/* ── Tokens con valor ── */
%token <s_val>  IDENTIFIER
%token <i_val>  INT_LITERAL
%token <f_val>  FLOAT_LITERAL
%token <c_val>  CHAR_LITERAL
%token <s_val>  STRING_LITERAL

/* ── Precedencia ── */
%left  EQ NEQ
%left  LT GT LTE GTE
%left  PLUS SUB
%left  MUL  DIV

/* ── Tipos de no-terminales ── */
%type <type_enum> data_type
%type <node>      expression
%type <node>      condition
%type <node>      statement
%type <node>      declaration
%type <node>      show_stmt
%type <node>      func_def
%type <node>      return_stmt
%type <node>      func_call_expr
%type <node>      if_stmt
%type <ptr_val>   if_else_body
%type <ptr_val>   then_body
%type <node>      while_stmt
%type <ptr_val>   arg_list
%type <ptr_val>   param_list

%%

program
    : program statement          { if ($2) push_stmt($2); }
    | /* empty */
    ;

statement
    : declaration   SEMICOLON    { $$ = $1; }
    | show_stmt     SEMICOLON    { $$ = $1; }
    | func_def                   { $$ = $1; }
    | return_stmt   SEMICOLON    { $$ = $1; }
    | func_call_expr SEMICOLON   { $$ = $1; }
    | if_stmt                    { $$ = $1; }
    | while_stmt                 { $$ = $1; }
    ;

data_type
    : BAFLE_INT_TYPE             { $$ = TYPE_INT;    }
    | BAFLE_FLOAT_TYPE           { $$ = TYPE_FLOAT;  }
    | BAFLE_CHAR_TYPE            { $$ = TYPE_CHAR;   }
    | BAFLE_STR_TYPE             { $$ = TYPE_STRING; }
    | BAFLE_BOOL_TYPE            { $$ = TYPE_BOOL;   }
    ;

/* ── Condiciones (comparaciones) ── */
condition
    : expression EQ  expression  { $$ = ast_compare(CMP_EQ,  $1, $3); $$->line = yylineno; }
    | expression NEQ expression  { $$ = ast_compare(CMP_NEQ, $1, $3); $$->line = yylineno; }
    | expression LT  expression  { $$ = ast_compare(CMP_LT,  $1, $3); $$->line = yylineno; }
    | expression GT  expression  { $$ = ast_compare(CMP_GT,  $1, $3); $$->line = yylineno; }
    | expression LTE expression  { $$ = ast_compare(CMP_LTE, $1, $3); $$->line = yylineno; }
    | expression GTE expression  { $$ = ast_compare(CMP_GTE, $1, $3); $$->line = yylineno; }
    ;

/* ── Expresiones ── */
expression
    : INT_LITERAL                { $$ = ast_lit_int($1);    $$->line = yylineno; }
    | FLOAT_LITERAL              { $$ = ast_lit_float($1);  $$->line = yylineno; }
    | CHAR_LITERAL               { $$ = ast_lit_char($1);   $$->line = yylineno; }
    | STRING_LITERAL             { $$ = ast_lit_string($1); $$->line = yylineno; free($1); }
    | IDENTIFIER                 { $$ = ast_identifier($1); $$->line = yylineno; free($1); }
    | func_call_expr             { $$ = $1; }
    | expression PLUS expression { $$ = ast_binop(OP_ADD,$1,$3); $$->line = yylineno; }
    | expression SUB  expression { $$ = ast_binop(OP_SUB,$1,$3); $$->line = yylineno; }
    | expression MUL  expression { $$ = ast_binop(OP_MUL,$1,$3); $$->line = yylineno; }
    | expression DIV  expression { $$ = ast_binop(OP_DIV,$1,$3); $$->line = yylineno; }
    | LPARENT expression RPARENT { $$ = $2; }
    ;

func_call_expr
    : IDENTIFIER LPARENT RPARENT {
        $$ = ast_func_call($1, NULL, 0); $$->line = yylineno; free($1);
    }
    | IDENTIFIER LPARENT arg_list RPARENT {
        ArgList *al = (ArgList *)$3;
        $$ = ast_func_call($1, al->nodes, al->count); $$->line = yylineno;
        free($1); free(al);
    }
    ;

arg_list
    : expression {
        ArgList *al = arglist_new(); arglist_add(al, $1); $$ = (void *)al;
    }
    | arg_list COMMA expression {
        ArgList *al = (ArgList *)$1; arglist_add(al, $3); $$ = (void *)al;
    }
    ;

declaration
    : SET IDENTIFIER COMO data_type ASSIGN expression {
        $$ = ast_var_decl($2, (DataType)$4, $6); $$->line = yylineno; free($2);
    }
    ;

show_stmt
    : SHOW expression            { $$ = ast_show_var($2); $$->line = yylineno; }
    ;

/* ── if / if-else ── */
then_body
    : LBRACE { block_push_ctx(); } block_stmts RBRACE { $$ = (void *)block_pop_ctx(); }
    ;

if_stmt
    : SI LPARENT condition RPARENT then_body {
        $$ = ast_if($3, (ASTNode *)$5, NULL); $$->line = yylineno;
    }
    | SI LPARENT condition RPARENT then_body SINO if_else_body {
        $$ = ast_if($3, (ASTNode *)$5, (ASTNode *)$7); $$->line = yylineno;
    }
    ;

/* ── while ── */
while_stmt
    : MIENTRAS LPARENT condition RPARENT then_body {
        $$ = ast_while($3, (ASTNode *)$5); $$->line = yylineno;
    }
    ;

/* ── Statements inside blocks (if/while/func bodies) ── */

if_else_body
    : LBRACE { block_push_ctx(); } block_stmts RBRACE {
        $$ = (void *)block_pop_ctx();
    }
    ;

block_stmts
    : block_stmts block_stmt
    | /* empty */
    ;

block_stmt
    : declaration   SEMICOLON    { block_add_stmt($1); }
    | show_stmt     SEMICOLON    { block_add_stmt($1); }
    | return_stmt   SEMICOLON    { block_add_stmt($1); }
    | func_call_expr SEMICOLON   { block_add_stmt($1); }
    | if_stmt                    { block_add_stmt($1); }
    | while_stmt                 { block_add_stmt($1); }
    ;

/* ── Function definition ── */
func_def
    : FUNC IDENTIFIER LPARENT RPARENT ARROW data_type
      LBRACE { block_push_ctx(); } block_stmts RBRACE {
        ASTNode *body = block_pop_ctx();
        $$ = ast_func_decl($2, (DataType)$6, NULL, 0, body); $$->line = yylineno;
        free($2);
    }
    | FUNC IDENTIFIER LPARENT param_list RPARENT ARROW data_type
      LBRACE { block_push_ctx(); } block_stmts RBRACE {
        ParmList *pl = (ParmList *)$4;
        ASTNode  *body = block_pop_ctx();
        $$ = ast_func_decl($2, (DataType)$7, pl->params, pl->count, body); $$->line = yylineno;
        for(int i=0;i<pl->count;i++) free(pl->params[i].name);
        free(pl->params); free(pl); free($2);
    }
    ;

param_list
    : data_type IDENTIFIER {
        ParmList *pl = parmlist_new();
        parmlist_add(pl, $2, (DataType)$1); free($2); $$ = (void *)pl;
    }
    | param_list COMMA data_type IDENTIFIER {
        ParmList *pl = (ParmList *)$1;
        parmlist_add(pl, $4, (DataType)$3); free($4); $$ = (void *)pl;
    }
    ;

return_stmt
    : RETORNA expression         { $$ = ast_return($2); $$->line = yylineno; }
    | RETORNA                    { $$ = ast_return(NULL); $$->line = yylineno; }
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "[línea %d] Error sintáctico: %s\n", yylineno, s);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *f = fopen(argv[1], "r");
        if (!f) { fprintf(stderr, "No se puede abrir: %s\n", argv[1]); return 1; }
        yyin = f;
    }

    if (yyparse() != 0) return 1;

    ASTNode *program = ast_program(stmts, stmt_count);

    /* ── Análisis Semántico (antes de ejecutar) ── */
    if (semantic_check(program) > 0) {
        ast_free(program);
        return 1;
    }

    /* ── Código Intermedio: imprimir el AST ── */
    printf("\n=== REPRESENTACIÓN INTERMEDIA (AST) ===\n");
    ast_print(program, 0);
    printf("=======================================\n\n");

    /* ── Ejecución ── */
    Interpreter *interp = interp_create();
    interp_run(interp, program);
    interp_destroy(interp);
    ast_free(program);

    printf("--- Backfile OK ---\n");
    return 0;
}
