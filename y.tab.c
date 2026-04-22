/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

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

#line 149 "y.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

/* Use api.header.include to #include this header
   instead of duplicating it here.  */
#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    SET = 258,                     /* SET  */
    COMO = 259,                    /* COMO  */
    SHOW = 260,                    /* SHOW  */
    FUNC = 261,                    /* FUNC  */
    RETORNA = 262,                 /* RETORNA  */
    SI = 263,                      /* SI  */
    SINO = 264,                    /* SINO  */
    MIENTRAS = 265,                /* MIENTRAS  */
    BAFLE_INT_TYPE = 266,          /* BAFLE_INT_TYPE  */
    BAFLE_FLOAT_TYPE = 267,        /* BAFLE_FLOAT_TYPE  */
    BAFLE_CHAR_TYPE = 268,         /* BAFLE_CHAR_TYPE  */
    BAFLE_STR_TYPE = 269,          /* BAFLE_STR_TYPE  */
    BAFLE_BOOL_TYPE = 270,         /* BAFLE_BOOL_TYPE  */
    SEMICOLON = 271,               /* SEMICOLON  */
    ASSIGN = 272,                  /* ASSIGN  */
    LPARENT = 273,                 /* LPARENT  */
    RPARENT = 274,                 /* RPARENT  */
    LBRACE = 275,                  /* LBRACE  */
    RBRACE = 276,                  /* RBRACE  */
    COMMA = 277,                   /* COMMA  */
    ARROW = 278,                   /* ARROW  */
    PLUS = 279,                    /* PLUS  */
    SUB = 280,                     /* SUB  */
    MUL = 281,                     /* MUL  */
    DIV = 282,                     /* DIV  */
    EQ = 283,                      /* EQ  */
    NEQ = 284,                     /* NEQ  */
    LT = 285,                      /* LT  */
    GT = 286,                      /* GT  */
    LTE = 287,                     /* LTE  */
    GTE = 288,                     /* GTE  */
    IDENTIFIER = 289,              /* IDENTIFIER  */
    INT_LITERAL = 290,             /* INT_LITERAL  */
    FLOAT_LITERAL = 291,           /* FLOAT_LITERAL  */
    CHAR_LITERAL = 292,            /* CHAR_LITERAL  */
    STRING_LITERAL = 293           /* STRING_LITERAL  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define SET 258
#define COMO 259
#define SHOW 260
#define FUNC 261
#define RETORNA 262
#define SI 263
#define SINO 264
#define MIENTRAS 265
#define BAFLE_INT_TYPE 266
#define BAFLE_FLOAT_TYPE 267
#define BAFLE_CHAR_TYPE 268
#define BAFLE_STR_TYPE 269
#define BAFLE_BOOL_TYPE 270
#define SEMICOLON 271
#define ASSIGN 272
#define LPARENT 273
#define RPARENT 274
#define LBRACE 275
#define RBRACE 276
#define COMMA 277
#define ARROW 278
#define PLUS 279
#define SUB 280
#define MUL 281
#define DIV 282
#define EQ 283
#define NEQ 284
#define LT 285
#define GT 286
#define LTE 287
#define GTE 288
#define IDENTIFIER 289
#define INT_LITERAL 290
#define FLOAT_LITERAL 291
#define CHAR_LITERAL 292
#define STRING_LITERAL 293

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 79 "parser.y"

    int    i_val;
    double f_val;
    char   c_val;
    char  *s_val;
    int    type_enum;
    void  *ptr_val;
    struct ASTNode *node;

#line 288 "y.tab.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_SET = 3,                        /* SET  */
  YYSYMBOL_COMO = 4,                       /* COMO  */
  YYSYMBOL_SHOW = 5,                       /* SHOW  */
  YYSYMBOL_FUNC = 6,                       /* FUNC  */
  YYSYMBOL_RETORNA = 7,                    /* RETORNA  */
  YYSYMBOL_SI = 8,                         /* SI  */
  YYSYMBOL_SINO = 9,                       /* SINO  */
  YYSYMBOL_MIENTRAS = 10,                  /* MIENTRAS  */
  YYSYMBOL_BAFLE_INT_TYPE = 11,            /* BAFLE_INT_TYPE  */
  YYSYMBOL_BAFLE_FLOAT_TYPE = 12,          /* BAFLE_FLOAT_TYPE  */
  YYSYMBOL_BAFLE_CHAR_TYPE = 13,           /* BAFLE_CHAR_TYPE  */
  YYSYMBOL_BAFLE_STR_TYPE = 14,            /* BAFLE_STR_TYPE  */
  YYSYMBOL_BAFLE_BOOL_TYPE = 15,           /* BAFLE_BOOL_TYPE  */
  YYSYMBOL_SEMICOLON = 16,                 /* SEMICOLON  */
  YYSYMBOL_ASSIGN = 17,                    /* ASSIGN  */
  YYSYMBOL_LPARENT = 18,                   /* LPARENT  */
  YYSYMBOL_RPARENT = 19,                   /* RPARENT  */
  YYSYMBOL_LBRACE = 20,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 21,                    /* RBRACE  */
  YYSYMBOL_COMMA = 22,                     /* COMMA  */
  YYSYMBOL_ARROW = 23,                     /* ARROW  */
  YYSYMBOL_PLUS = 24,                      /* PLUS  */
  YYSYMBOL_SUB = 25,                       /* SUB  */
  YYSYMBOL_MUL = 26,                       /* MUL  */
  YYSYMBOL_DIV = 27,                       /* DIV  */
  YYSYMBOL_EQ = 28,                        /* EQ  */
  YYSYMBOL_NEQ = 29,                       /* NEQ  */
  YYSYMBOL_LT = 30,                        /* LT  */
  YYSYMBOL_GT = 31,                        /* GT  */
  YYSYMBOL_LTE = 32,                       /* LTE  */
  YYSYMBOL_GTE = 33,                       /* GTE  */
  YYSYMBOL_IDENTIFIER = 34,                /* IDENTIFIER  */
  YYSYMBOL_INT_LITERAL = 35,               /* INT_LITERAL  */
  YYSYMBOL_FLOAT_LITERAL = 36,             /* FLOAT_LITERAL  */
  YYSYMBOL_CHAR_LITERAL = 37,              /* CHAR_LITERAL  */
  YYSYMBOL_STRING_LITERAL = 38,            /* STRING_LITERAL  */
  YYSYMBOL_YYACCEPT = 39,                  /* $accept  */
  YYSYMBOL_program = 40,                   /* program  */
  YYSYMBOL_statement = 41,                 /* statement  */
  YYSYMBOL_data_type = 42,                 /* data_type  */
  YYSYMBOL_condition = 43,                 /* condition  */
  YYSYMBOL_expression = 44,                /* expression  */
  YYSYMBOL_func_call_expr = 45,            /* func_call_expr  */
  YYSYMBOL_arg_list = 46,                  /* arg_list  */
  YYSYMBOL_declaration = 47,               /* declaration  */
  YYSYMBOL_show_stmt = 48,                 /* show_stmt  */
  YYSYMBOL_then_body = 49,                 /* then_body  */
  YYSYMBOL_50_1 = 50,                      /* $@1  */
  YYSYMBOL_if_stmt = 51,                   /* if_stmt  */
  YYSYMBOL_while_stmt = 52,                /* while_stmt  */
  YYSYMBOL_if_else_body = 53,              /* if_else_body  */
  YYSYMBOL_54_2 = 54,                      /* $@2  */
  YYSYMBOL_block_stmts = 55,               /* block_stmts  */
  YYSYMBOL_block_stmt = 56,                /* block_stmt  */
  YYSYMBOL_func_def = 57,                  /* func_def  */
  YYSYMBOL_58_3 = 58,                      /* $@3  */
  YYSYMBOL_59_4 = 59,                      /* $@4  */
  YYSYMBOL_param_list = 60,                /* param_list  */
  YYSYMBOL_return_stmt = 61                /* return_stmt  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_int8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   147

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  39
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  23
/* YYNRULES -- Number of rules.  */
#define YYNRULES  61
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  122

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   293


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   129,   129,   130,   134,   135,   136,   137,   138,   139,
     140,   144,   145,   146,   147,   148,   153,   154,   155,   156,
     157,   158,   163,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   177,   180,   188,   191,   197,   203,   208,
     208,   212,   215,   222,   230,   230,   236,   237,   241,   242,
     243,   244,   245,   246,   252,   251,   258,   257,   268,   272,
     279,   280
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "SET", "COMO", "SHOW",
  "FUNC", "RETORNA", "SI", "SINO", "MIENTRAS", "BAFLE_INT_TYPE",
  "BAFLE_FLOAT_TYPE", "BAFLE_CHAR_TYPE", "BAFLE_STR_TYPE",
  "BAFLE_BOOL_TYPE", "SEMICOLON", "ASSIGN", "LPARENT", "RPARENT", "LBRACE",
  "RBRACE", "COMMA", "ARROW", "PLUS", "SUB", "MUL", "DIV", "EQ", "NEQ",
  "LT", "GT", "LTE", "GTE", "IDENTIFIER", "INT_LITERAL", "FLOAT_LITERAL",
  "CHAR_LITERAL", "STRING_LITERAL", "$accept", "program", "statement",
  "data_type", "condition", "expression", "func_call_expr", "arg_list",
  "declaration", "show_stmt", "then_body", "$@1", "if_stmt", "while_stmt",
  "if_else_body", "$@2", "block_stmts", "block_stmt", "func_def", "$@3",
  "$@4", "param_list", "return_stmt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-90)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -90,    12,   -90,   -30,    85,   -26,    85,    -8,    -5,    -4,
     -90,     8,    10,    28,   -90,   -90,   -90,    29,    27,    85,
      -4,   -90,   -90,   -90,   -90,    33,   -90,    30,    33,    85,
      85,    15,   -90,   -90,   -90,   -90,   132,    67,    85,    85,
      85,    85,   123,    35,   100,    48,   -90,    33,   -17,   -90,
     -90,   -90,   -90,   -90,    51,   -90,    14,    14,   -90,   -90,
      24,    38,   -13,    54,    85,    85,    85,    85,    85,    85,
      54,   -90,    85,    85,   132,   -90,    57,   132,   -90,    88,
      33,    33,    33,    33,    33,    33,   -90,    33,    33,    78,
     132,    65,   -90,    84,   -90,    86,   -90,    22,   -90,   -90,
     -90,   -90,   -90,    89,    91,    93,   -90,   -90,   -90,    95,
     -90,    68,   -90,   -90,   -90,   -90,   -90,    74,   -90,    80,
     -90,   -90
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     1,     0,     0,     0,    61,     0,     0,     0,
       2,     0,     0,     0,     9,    10,     6,     0,     0,     0,
      26,    22,    23,    24,    25,    38,    27,     0,    60,     0,
       0,     0,     8,     4,     5,     7,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    33,    35,     0,    11,
      12,    13,    14,    15,     0,    32,    28,    29,    30,    31,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    34,     0,     0,     0,    58,     0,     0,    39,    41,
      16,    17,    18,    19,    20,    21,    43,    36,    37,     0,
       0,     0,    47,     0,    54,     0,    59,     0,    44,    42,
      47,    56,    40,     0,     0,     0,    52,    53,    46,     0,
      47,     0,    47,    51,    48,    49,    50,     0,    55,     0,
      45,    57
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -90,   -90,   -90,   -35,    70,    -3,    -1,   -90,   111,   112,
      45,   -90,   116,   138,   -90,   -90,   -89,   -90,   -90,   -90,
     -90,   -90,   139
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,    10,    54,    43,    44,    26,    48,   104,   105,
      79,    92,   106,   107,    99,   110,    97,   108,    16,   100,
     112,    62,   109
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      11,    25,    71,    28,    18,    72,    76,    61,    27,    77,
      29,   111,     2,    30,    31,     3,    37,     4,     5,     6,
       7,   117,     8,   119,    32,     3,    33,     4,    47,     6,
       7,    36,     8,    19,    46,    56,    57,    58,    59,    89,
      40,    41,    91,   102,    34,    35,     9,    74,    42,    20,
      21,    22,    23,    24,    63,    95,     9,    38,    39,    40,
      41,    80,    81,    82,    83,    84,    85,    70,    73,    87,
      88,     3,    75,     4,    78,     6,     7,     3,     8,     4,
      90,     6,     7,     3,     8,     4,    55,     6,     7,   118,
       8,    38,    39,    40,    41,   120,   103,    93,    94,    96,
      45,   121,     9,    19,    98,   113,   101,   114,     9,   115,
     103,   116,    12,    13,     9,    86,   103,    14,   103,    20,
      21,    22,    23,    24,    38,    39,    40,    41,    64,    65,
      66,    67,    68,    69,    49,    50,    51,    52,    53,    15,
      17,     0,    60,    49,    50,    51,    52,    53
};

static const yytype_int8 yycheck[] =
{
       1,     4,    19,     6,    34,    22,    19,    42,    34,    22,
      18,   100,     0,    18,    18,     3,    19,     5,     6,     7,
       8,   110,    10,   112,    16,     3,    16,     5,    31,     7,
       8,     4,    10,    18,    19,    38,    39,    40,    41,    74,
      26,    27,    77,    21,    16,    16,    34,    23,    18,    34,
      35,    36,    37,    38,    19,    90,    34,    24,    25,    26,
      27,    64,    65,    66,    67,    68,    69,    19,    17,    72,
      73,     3,    34,     5,    20,     7,     8,     3,    10,     5,
      23,     7,     8,     3,    10,     5,    19,     7,     8,    21,
      10,    24,    25,    26,    27,    21,    97,     9,    20,    34,
      30,    21,    34,    18,    20,    16,    20,    16,    34,    16,
     111,    16,     1,     1,    34,    70,   117,     1,   119,    34,
      35,    36,    37,    38,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    11,    12,    13,    14,    15,     1,
       1,    -1,    19,    11,    12,    13,    14,    15
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    40,     0,     3,     5,     6,     7,     8,    10,    34,
      41,    45,    47,    48,    51,    52,    57,    61,    34,    18,
      34,    35,    36,    37,    38,    44,    45,    34,    44,    18,
      18,    18,    16,    16,    16,    16,     4,    44,    24,    25,
      26,    27,    18,    43,    44,    43,    19,    44,    46,    11,
      12,    13,    14,    15,    42,    19,    44,    44,    44,    44,
      19,    42,    60,    19,    28,    29,    30,    31,    32,    33,
      19,    19,    22,    17,    23,    34,    19,    22,    20,    49,
      44,    44,    44,    44,    44,    44,    49,    44,    44,    42,
      23,    42,    50,     9,    20,    42,    34,    55,    20,    53,
      58,    20,    21,    45,    47,    48,    51,    52,    56,    61,
      54,    55,    59,    16,    16,    16,    16,    55,    21,    55,
      21,    21
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    39,    40,    40,    41,    41,    41,    41,    41,    41,
      41,    42,    42,    42,    42,    42,    43,    43,    43,    43,
      43,    43,    44,    44,    44,    44,    44,    44,    44,    44,
      44,    44,    44,    45,    45,    46,    46,    47,    48,    50,
      49,    51,    51,    52,    54,    53,    55,    55,    56,    56,
      56,    56,    56,    56,    58,    57,    59,    57,    60,    60,
      61,    61
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     0,     2,     2,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     1,     1,     1,     1,     1,     1,     3,     3,
       3,     3,     3,     3,     4,     1,     3,     6,     2,     0,
       4,     5,     7,     5,     0,     4,     2,     0,     2,     2,
       2,     2,     1,     1,     0,    10,     0,    11,     2,     4,
       2,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: program statement  */
#line 129 "parser.y"
                                 { if ((yyvsp[0].node)) push_stmt((yyvsp[0].node)); }
#line 1413 "y.tab.c"
    break;

  case 4: /* statement: declaration SEMICOLON  */
#line 134 "parser.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 1419 "y.tab.c"
    break;

  case 5: /* statement: show_stmt SEMICOLON  */
#line 135 "parser.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 1425 "y.tab.c"
    break;

  case 6: /* statement: func_def  */
#line 136 "parser.y"
                                 { (yyval.node) = (yyvsp[0].node); }
#line 1431 "y.tab.c"
    break;

  case 7: /* statement: return_stmt SEMICOLON  */
#line 137 "parser.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 1437 "y.tab.c"
    break;

  case 8: /* statement: func_call_expr SEMICOLON  */
#line 138 "parser.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 1443 "y.tab.c"
    break;

  case 9: /* statement: if_stmt  */
#line 139 "parser.y"
                                 { (yyval.node) = (yyvsp[0].node); }
#line 1449 "y.tab.c"
    break;

  case 10: /* statement: while_stmt  */
#line 140 "parser.y"
                                 { (yyval.node) = (yyvsp[0].node); }
#line 1455 "y.tab.c"
    break;

  case 11: /* data_type: BAFLE_INT_TYPE  */
#line 144 "parser.y"
                                 { (yyval.type_enum) = TYPE_INT;    }
#line 1461 "y.tab.c"
    break;

  case 12: /* data_type: BAFLE_FLOAT_TYPE  */
#line 145 "parser.y"
                                 { (yyval.type_enum) = TYPE_FLOAT;  }
#line 1467 "y.tab.c"
    break;

  case 13: /* data_type: BAFLE_CHAR_TYPE  */
#line 146 "parser.y"
                                 { (yyval.type_enum) = TYPE_CHAR;   }
#line 1473 "y.tab.c"
    break;

  case 14: /* data_type: BAFLE_STR_TYPE  */
#line 147 "parser.y"
                                 { (yyval.type_enum) = TYPE_STRING; }
#line 1479 "y.tab.c"
    break;

  case 15: /* data_type: BAFLE_BOOL_TYPE  */
#line 148 "parser.y"
                                 { (yyval.type_enum) = TYPE_BOOL;   }
#line 1485 "y.tab.c"
    break;

  case 16: /* condition: expression EQ expression  */
#line 153 "parser.y"
                                 { (yyval.node) = ast_compare(CMP_EQ,  (yyvsp[-2].node), (yyvsp[0].node)); (yyval.node)->line = yylineno; }
#line 1491 "y.tab.c"
    break;

  case 17: /* condition: expression NEQ expression  */
#line 154 "parser.y"
                                 { (yyval.node) = ast_compare(CMP_NEQ, (yyvsp[-2].node), (yyvsp[0].node)); (yyval.node)->line = yylineno; }
#line 1497 "y.tab.c"
    break;

  case 18: /* condition: expression LT expression  */
#line 155 "parser.y"
                                 { (yyval.node) = ast_compare(CMP_LT,  (yyvsp[-2].node), (yyvsp[0].node)); (yyval.node)->line = yylineno; }
#line 1503 "y.tab.c"
    break;

  case 19: /* condition: expression GT expression  */
#line 156 "parser.y"
                                 { (yyval.node) = ast_compare(CMP_GT,  (yyvsp[-2].node), (yyvsp[0].node)); (yyval.node)->line = yylineno; }
#line 1509 "y.tab.c"
    break;

  case 20: /* condition: expression LTE expression  */
#line 157 "parser.y"
                                 { (yyval.node) = ast_compare(CMP_LTE, (yyvsp[-2].node), (yyvsp[0].node)); (yyval.node)->line = yylineno; }
#line 1515 "y.tab.c"
    break;

  case 21: /* condition: expression GTE expression  */
#line 158 "parser.y"
                                 { (yyval.node) = ast_compare(CMP_GTE, (yyvsp[-2].node), (yyvsp[0].node)); (yyval.node)->line = yylineno; }
#line 1521 "y.tab.c"
    break;

  case 22: /* expression: INT_LITERAL  */
#line 163 "parser.y"
                                 { (yyval.node) = ast_lit_int((yyvsp[0].i_val));    (yyval.node)->line = yylineno; }
#line 1527 "y.tab.c"
    break;

  case 23: /* expression: FLOAT_LITERAL  */
#line 164 "parser.y"
                                 { (yyval.node) = ast_lit_float((yyvsp[0].f_val));  (yyval.node)->line = yylineno; }
#line 1533 "y.tab.c"
    break;

  case 24: /* expression: CHAR_LITERAL  */
#line 165 "parser.y"
                                 { (yyval.node) = ast_lit_char((yyvsp[0].c_val));   (yyval.node)->line = yylineno; }
#line 1539 "y.tab.c"
    break;

  case 25: /* expression: STRING_LITERAL  */
#line 166 "parser.y"
                                 { (yyval.node) = ast_lit_string((yyvsp[0].s_val)); (yyval.node)->line = yylineno; free((yyvsp[0].s_val)); }
#line 1545 "y.tab.c"
    break;

  case 26: /* expression: IDENTIFIER  */
#line 167 "parser.y"
                                 { (yyval.node) = ast_identifier((yyvsp[0].s_val)); (yyval.node)->line = yylineno; free((yyvsp[0].s_val)); }
#line 1551 "y.tab.c"
    break;

  case 27: /* expression: func_call_expr  */
#line 168 "parser.y"
                                 { (yyval.node) = (yyvsp[0].node); }
#line 1557 "y.tab.c"
    break;

  case 28: /* expression: expression PLUS expression  */
#line 169 "parser.y"
                                 { (yyval.node) = ast_binop(OP_ADD,(yyvsp[-2].node),(yyvsp[0].node)); (yyval.node)->line = yylineno; }
#line 1563 "y.tab.c"
    break;

  case 29: /* expression: expression SUB expression  */
#line 170 "parser.y"
                                 { (yyval.node) = ast_binop(OP_SUB,(yyvsp[-2].node),(yyvsp[0].node)); (yyval.node)->line = yylineno; }
#line 1569 "y.tab.c"
    break;

  case 30: /* expression: expression MUL expression  */
#line 171 "parser.y"
                                 { (yyval.node) = ast_binop(OP_MUL,(yyvsp[-2].node),(yyvsp[0].node)); (yyval.node)->line = yylineno; }
#line 1575 "y.tab.c"
    break;

  case 31: /* expression: expression DIV expression  */
#line 172 "parser.y"
                                 { (yyval.node) = ast_binop(OP_DIV,(yyvsp[-2].node),(yyvsp[0].node)); (yyval.node)->line = yylineno; }
#line 1581 "y.tab.c"
    break;

  case 32: /* expression: LPARENT expression RPARENT  */
#line 173 "parser.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 1587 "y.tab.c"
    break;

  case 33: /* func_call_expr: IDENTIFIER LPARENT RPARENT  */
#line 177 "parser.y"
                                 {
        (yyval.node) = ast_func_call((yyvsp[-2].s_val), NULL, 0); (yyval.node)->line = yylineno; free((yyvsp[-2].s_val));
    }
#line 1595 "y.tab.c"
    break;

  case 34: /* func_call_expr: IDENTIFIER LPARENT arg_list RPARENT  */
#line 180 "parser.y"
                                          {
        ArgList *al = (ArgList *)(yyvsp[-1].ptr_val);
        (yyval.node) = ast_func_call((yyvsp[-3].s_val), al->nodes, al->count); (yyval.node)->line = yylineno;
        free((yyvsp[-3].s_val)); free(al);
    }
#line 1605 "y.tab.c"
    break;

  case 35: /* arg_list: expression  */
#line 188 "parser.y"
                 {
        ArgList *al = arglist_new(); arglist_add(al, (yyvsp[0].node)); (yyval.ptr_val) = (void *)al;
    }
#line 1613 "y.tab.c"
    break;

  case 36: /* arg_list: arg_list COMMA expression  */
#line 191 "parser.y"
                                {
        ArgList *al = (ArgList *)(yyvsp[-2].ptr_val); arglist_add(al, (yyvsp[0].node)); (yyval.ptr_val) = (void *)al;
    }
#line 1621 "y.tab.c"
    break;

  case 37: /* declaration: SET IDENTIFIER COMO data_type ASSIGN expression  */
#line 197 "parser.y"
                                                      {
        (yyval.node) = ast_var_decl((yyvsp[-4].s_val), (DataType)(yyvsp[-2].type_enum), (yyvsp[0].node)); (yyval.node)->line = yylineno; free((yyvsp[-4].s_val));
    }
#line 1629 "y.tab.c"
    break;

  case 38: /* show_stmt: SHOW expression  */
#line 203 "parser.y"
                                 { (yyval.node) = ast_show_var((yyvsp[0].node)); (yyval.node)->line = yylineno; }
#line 1635 "y.tab.c"
    break;

  case 39: /* $@1: %empty  */
#line 208 "parser.y"
             { block_push_ctx(); }
#line 1641 "y.tab.c"
    break;

  case 40: /* then_body: LBRACE $@1 block_stmts RBRACE  */
#line 208 "parser.y"
                                                      { (yyval.ptr_val) = (void *)block_pop_ctx(); }
#line 1647 "y.tab.c"
    break;

  case 41: /* if_stmt: SI LPARENT condition RPARENT then_body  */
#line 212 "parser.y"
                                             {
        (yyval.node) = ast_if((yyvsp[-2].node), (ASTNode *)(yyvsp[0].ptr_val), NULL); (yyval.node)->line = yylineno;
    }
#line 1655 "y.tab.c"
    break;

  case 42: /* if_stmt: SI LPARENT condition RPARENT then_body SINO if_else_body  */
#line 215 "parser.y"
                                                               {
        (yyval.node) = ast_if((yyvsp[-4].node), (ASTNode *)(yyvsp[-2].ptr_val), (ASTNode *)(yyvsp[0].ptr_val)); (yyval.node)->line = yylineno;
    }
#line 1663 "y.tab.c"
    break;

  case 43: /* while_stmt: MIENTRAS LPARENT condition RPARENT then_body  */
#line 222 "parser.y"
                                                   {
        (yyval.node) = ast_while((yyvsp[-2].node), (ASTNode *)(yyvsp[0].ptr_val)); (yyval.node)->line = yylineno;
    }
#line 1671 "y.tab.c"
    break;

  case 44: /* $@2: %empty  */
#line 230 "parser.y"
             { block_push_ctx(); }
#line 1677 "y.tab.c"
    break;

  case 45: /* if_else_body: LBRACE $@2 block_stmts RBRACE  */
#line 230 "parser.y"
                                                      {
        (yyval.ptr_val) = (void *)block_pop_ctx();
    }
#line 1685 "y.tab.c"
    break;

  case 48: /* block_stmt: declaration SEMICOLON  */
#line 241 "parser.y"
                                 { block_add_stmt((yyvsp[-1].node)); }
#line 1691 "y.tab.c"
    break;

  case 49: /* block_stmt: show_stmt SEMICOLON  */
#line 242 "parser.y"
                                 { block_add_stmt((yyvsp[-1].node)); }
#line 1697 "y.tab.c"
    break;

  case 50: /* block_stmt: return_stmt SEMICOLON  */
#line 243 "parser.y"
                                 { block_add_stmt((yyvsp[-1].node)); }
#line 1703 "y.tab.c"
    break;

  case 51: /* block_stmt: func_call_expr SEMICOLON  */
#line 244 "parser.y"
                                 { block_add_stmt((yyvsp[-1].node)); }
#line 1709 "y.tab.c"
    break;

  case 52: /* block_stmt: if_stmt  */
#line 245 "parser.y"
                                 { block_add_stmt((yyvsp[0].node)); }
#line 1715 "y.tab.c"
    break;

  case 53: /* block_stmt: while_stmt  */
#line 246 "parser.y"
                                 { block_add_stmt((yyvsp[0].node)); }
#line 1721 "y.tab.c"
    break;

  case 54: /* $@3: %empty  */
#line 252 "parser.y"
             { block_push_ctx(); }
#line 1727 "y.tab.c"
    break;

  case 55: /* func_def: FUNC IDENTIFIER LPARENT RPARENT ARROW data_type LBRACE $@3 block_stmts RBRACE  */
#line 252 "parser.y"
                                                      {
        ASTNode *body = block_pop_ctx();
        (yyval.node) = ast_func_decl((yyvsp[-8].s_val), (DataType)(yyvsp[-4].type_enum), NULL, 0, body); (yyval.node)->line = yylineno;
        free((yyvsp[-8].s_val));
    }
#line 1737 "y.tab.c"
    break;

  case 56: /* $@4: %empty  */
#line 258 "parser.y"
             { block_push_ctx(); }
#line 1743 "y.tab.c"
    break;

  case 57: /* func_def: FUNC IDENTIFIER LPARENT param_list RPARENT ARROW data_type LBRACE $@4 block_stmts RBRACE  */
#line 258 "parser.y"
                                                      {
        ParmList *pl = (ParmList *)(yyvsp[-7].ptr_val);
        ASTNode  *body = block_pop_ctx();
        (yyval.node) = ast_func_decl((yyvsp[-9].s_val), (DataType)(yyvsp[-4].type_enum), pl->params, pl->count, body); (yyval.node)->line = yylineno;
        for(int i=0;i<pl->count;i++) free(pl->params[i].name);
        free(pl->params); free(pl); free((yyvsp[-9].s_val));
    }
#line 1755 "y.tab.c"
    break;

  case 58: /* param_list: data_type IDENTIFIER  */
#line 268 "parser.y"
                           {
        ParmList *pl = parmlist_new();
        parmlist_add(pl, (yyvsp[0].s_val), (DataType)(yyvsp[-1].type_enum)); free((yyvsp[0].s_val)); (yyval.ptr_val) = (void *)pl;
    }
#line 1764 "y.tab.c"
    break;

  case 59: /* param_list: param_list COMMA data_type IDENTIFIER  */
#line 272 "parser.y"
                                            {
        ParmList *pl = (ParmList *)(yyvsp[-3].ptr_val);
        parmlist_add(pl, (yyvsp[0].s_val), (DataType)(yyvsp[-1].type_enum)); free((yyvsp[0].s_val)); (yyval.ptr_val) = (void *)pl;
    }
#line 1773 "y.tab.c"
    break;

  case 60: /* return_stmt: RETORNA expression  */
#line 279 "parser.y"
                                 { (yyval.node) = ast_return((yyvsp[0].node)); (yyval.node)->line = yylineno; }
#line 1779 "y.tab.c"
    break;

  case 61: /* return_stmt: RETORNA  */
#line 280 "parser.y"
                                 { (yyval.node) = ast_return(NULL); (yyval.node)->line = yylineno; }
#line 1785 "y.tab.c"
    break;


#line 1789 "y.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 283 "parser.y"


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
