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

#line 158 "y.tab.c"

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
    BAFLE_INT_TYPE = 263,          /* BAFLE_INT_TYPE  */
    BAFLE_FLOAT_TYPE = 264,        /* BAFLE_FLOAT_TYPE  */
    BAFLE_CHAR_TYPE = 265,         /* BAFLE_CHAR_TYPE  */
    BAFLE_STR_TYPE = 266,          /* BAFLE_STR_TYPE  */
    BAFLE_BOOL_TYPE = 267,         /* BAFLE_BOOL_TYPE  */
    SEMICOLON = 268,               /* SEMICOLON  */
    ASSIGN = 269,                  /* ASSIGN  */
    LPARENT = 270,                 /* LPARENT  */
    RPARENT = 271,                 /* RPARENT  */
    LBRACE = 272,                  /* LBRACE  */
    RBRACE = 273,                  /* RBRACE  */
    COMMA = 274,                   /* COMMA  */
    ARROW = 275,                   /* ARROW  */
    PLUS = 276,                    /* PLUS  */
    SUB = 277,                     /* SUB  */
    MUL = 278,                     /* MUL  */
    DIV = 279,                     /* DIV  */
    IDENTIFIER = 280,              /* IDENTIFIER  */
    INT_LITERAL = 281,             /* INT_LITERAL  */
    FLOAT_LITERAL = 282,           /* FLOAT_LITERAL  */
    CHAR_LITERAL = 283,            /* CHAR_LITERAL  */
    STRING_LITERAL = 284           /* STRING_LITERAL  */
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
#define BAFLE_INT_TYPE 263
#define BAFLE_FLOAT_TYPE 264
#define BAFLE_CHAR_TYPE 265
#define BAFLE_STR_TYPE 266
#define BAFLE_BOOL_TYPE 267
#define SEMICOLON 268
#define ASSIGN 269
#define LPARENT 270
#define RPARENT 271
#define LBRACE 272
#define RBRACE 273
#define COMMA 274
#define ARROW 275
#define PLUS 276
#define SUB 277
#define MUL 278
#define DIV 279
#define IDENTIFIER 280
#define INT_LITERAL 281
#define FLOAT_LITERAL 282
#define CHAR_LITERAL 283
#define STRING_LITERAL 284

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 89 "parser.y"

    int    i_val;
    double f_val;
    char   c_val;
    char  *s_val;
    int    type_enum;
    void  *ptr_val;
    struct ASTNode *node;

#line 279 "y.tab.c"

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
  YYSYMBOL_BAFLE_INT_TYPE = 8,             /* BAFLE_INT_TYPE  */
  YYSYMBOL_BAFLE_FLOAT_TYPE = 9,           /* BAFLE_FLOAT_TYPE  */
  YYSYMBOL_BAFLE_CHAR_TYPE = 10,           /* BAFLE_CHAR_TYPE  */
  YYSYMBOL_BAFLE_STR_TYPE = 11,            /* BAFLE_STR_TYPE  */
  YYSYMBOL_BAFLE_BOOL_TYPE = 12,           /* BAFLE_BOOL_TYPE  */
  YYSYMBOL_SEMICOLON = 13,                 /* SEMICOLON  */
  YYSYMBOL_ASSIGN = 14,                    /* ASSIGN  */
  YYSYMBOL_LPARENT = 15,                   /* LPARENT  */
  YYSYMBOL_RPARENT = 16,                   /* RPARENT  */
  YYSYMBOL_LBRACE = 17,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 18,                    /* RBRACE  */
  YYSYMBOL_COMMA = 19,                     /* COMMA  */
  YYSYMBOL_ARROW = 20,                     /* ARROW  */
  YYSYMBOL_PLUS = 21,                      /* PLUS  */
  YYSYMBOL_SUB = 22,                       /* SUB  */
  YYSYMBOL_MUL = 23,                       /* MUL  */
  YYSYMBOL_DIV = 24,                       /* DIV  */
  YYSYMBOL_IDENTIFIER = 25,                /* IDENTIFIER  */
  YYSYMBOL_INT_LITERAL = 26,               /* INT_LITERAL  */
  YYSYMBOL_FLOAT_LITERAL = 27,             /* FLOAT_LITERAL  */
  YYSYMBOL_CHAR_LITERAL = 28,              /* CHAR_LITERAL  */
  YYSYMBOL_STRING_LITERAL = 29,            /* STRING_LITERAL  */
  YYSYMBOL_YYACCEPT = 30,                  /* $accept  */
  YYSYMBOL_program = 31,                   /* program  */
  YYSYMBOL_statement = 32,                 /* statement  */
  YYSYMBOL_data_type = 33,                 /* data_type  */
  YYSYMBOL_expression = 34,                /* expression  */
  YYSYMBOL_func_call_expr = 35,            /* func_call_expr  */
  YYSYMBOL_arg_list = 36,                  /* arg_list  */
  YYSYMBOL_declaration = 37,               /* declaration  */
  YYSYMBOL_show_stmt = 38,                 /* show_stmt  */
  YYSYMBOL_func_def = 39,                  /* func_def  */
  YYSYMBOL_40_1 = 40,                      /* $@1  */
  YYSYMBOL_41_2 = 41,                      /* $@2  */
  YYSYMBOL_param_list = 42,                /* param_list  */
  YYSYMBOL_func_body = 43,                 /* func_body  */
  YYSYMBOL_func_body_stmt = 44,            /* func_body_stmt  */
  YYSYMBOL_return_stmt = 45                /* return_stmt  */
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
#define YYLAST   94

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  30
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  16
/* YYNRULES -- Number of rules.  */
#define YYNRULES  44
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  85

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   284


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
      25,    26,    27,    28,    29
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   132,   132,   133,   137,   138,   139,   140,   141,   146,
     147,   148,   149,   150,   155,   156,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   170,   174,   183,   188,   197,
     205,   212,   211,   218,   217,   230,   236,   246,   247,   251,
     252,   253,   254,   258,   259
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
  "FUNC", "RETORNA", "BAFLE_INT_TYPE", "BAFLE_FLOAT_TYPE",
  "BAFLE_CHAR_TYPE", "BAFLE_STR_TYPE", "BAFLE_BOOL_TYPE", "SEMICOLON",
  "ASSIGN", "LPARENT", "RPARENT", "LBRACE", "RBRACE", "COMMA", "ARROW",
  "PLUS", "SUB", "MUL", "DIV", "IDENTIFIER", "INT_LITERAL",
  "FLOAT_LITERAL", "CHAR_LITERAL", "STRING_LITERAL", "$accept", "program",
  "statement", "data_type", "expression", "func_call_expr", "arg_list",
  "declaration", "show_stmt", "func_def", "$@1", "$@2", "param_list",
  "func_body", "func_body_stmt", "return_stmt", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-26)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -26,     9,   -26,   -23,    32,   -12,    32,     2,   -26,    10,
      18,    28,   -26,    31,    36,    32,     2,   -26,   -26,   -26,
     -26,    63,   -26,     7,    63,    27,   -26,   -26,   -26,   -26,
      71,    52,    32,    32,    32,    32,    55,   -26,    63,     5,
     -26,   -26,   -26,   -26,   -26,    34,   -26,    14,    14,   -26,
     -26,    25,    21,    17,   -26,    32,    32,    71,   -26,    29,
      71,    63,    63,    45,    71,    44,   -26,    60,   -26,   -26,
     -26,     0,   -26,   -26,    59,    75,    76,   -26,    77,     1,
     -26,   -26,   -26,   -26,   -26
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       3,     0,     1,     0,     0,     0,    44,     0,     2,     0,
       0,     0,     6,     0,     0,     0,    18,    14,    15,    16,
      17,    30,    19,     0,    43,     0,     8,     4,     5,     7,
       0,     0,     0,     0,     0,     0,     0,    25,    27,     0,
       9,    10,    11,    12,    13,     0,    24,    20,    21,    22,
      23,     0,     0,     0,    26,     0,     0,     0,    35,     0,
       0,    28,    29,     0,     0,     0,    31,     0,    36,    38,
      33,     0,    38,    32,     0,     0,     0,    37,     0,     0,
      42,    39,    40,    41,    34
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -26,   -26,   -26,   -25,    -5,    -1,   -26,    90,    91,   -26,
     -26,   -26,   -26,    22,   -26,    92
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     1,     8,    45,    21,    22,    39,    75,    76,    12,
      69,    72,    53,    71,    77,    78
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
       9,    24,    14,     3,     3,     4,     4,     6,     6,     2,
      31,    52,     3,    23,     4,     5,     6,    25,    73,    84,
      38,    54,    36,    26,    55,     7,     7,    47,    48,    49,
      50,    27,    63,    59,     7,    65,    60,    34,    35,    67,
      30,    28,    15,    37,    29,    57,    58,    15,    56,    64,
      61,    62,    16,    17,    18,    19,    20,    16,    17,    18,
      19,    20,    66,    40,    41,    42,    43,    44,    46,    68,
      74,    51,    80,    32,    33,    34,    35,    70,    74,    40,
      41,    42,    43,    44,    32,    33,    34,    35,    81,    82,
      83,    10,    11,    13,    79
};

static const yytype_int8 yycheck[] =
{
       1,     6,    25,     3,     3,     5,     5,     7,     7,     0,
      15,    36,     3,    25,     5,     6,     7,    15,    18,    18,
      25,    16,    15,    13,    19,    25,    25,    32,    33,    34,
      35,    13,    57,    16,    25,    60,    19,    23,    24,    64,
       4,    13,    15,    16,    13,    20,    25,    15,    14,    20,
      55,    56,    25,    26,    27,    28,    29,    25,    26,    27,
      28,    29,    17,     8,     9,    10,    11,    12,    16,    25,
      71,    16,    13,    21,    22,    23,    24,    17,    79,     8,
       9,    10,    11,    12,    21,    22,    23,    24,    13,    13,
      13,     1,     1,     1,    72
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    31,     0,     3,     5,     6,     7,    25,    32,    35,
      37,    38,    39,    45,    25,    15,    25,    26,    27,    28,
      29,    34,    35,    25,    34,    15,    13,    13,    13,    13,
       4,    34,    21,    22,    23,    24,    15,    16,    34,    36,
       8,     9,    10,    11,    12,    33,    16,    34,    34,    34,
      34,    16,    33,    42,    16,    19,    14,    20,    25,    16,
      19,    34,    34,    33,    20,    33,    17,    33,    25,    40,
      17,    43,    41,    18,    35,    37,    38,    44,    45,    43,
      13,    13,    13,    13,    18
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    30,    31,    31,    32,    32,    32,    32,    32,    33,
      33,    33,    33,    33,    34,    34,    34,    34,    34,    34,
      34,    34,    34,    34,    34,    35,    35,    36,    36,    37,
      38,    40,    39,    41,    39,    42,    42,    43,    43,    44,
      44,    44,    44,    45,    45
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     0,     2,     2,     1,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     4,     1,     3,     6,
       2,     0,    10,     0,    11,     2,     4,     2,     0,     2,
       2,     2,     2,     2,     1
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
#line 132 "parser.y"
                                 { if ((yyvsp[0].node)) push_stmt((yyvsp[0].node)); }
#line 1355 "y.tab.c"
    break;

  case 4: /* statement: declaration SEMICOLON  */
#line 137 "parser.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 1361 "y.tab.c"
    break;

  case 5: /* statement: show_stmt SEMICOLON  */
#line 138 "parser.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 1367 "y.tab.c"
    break;

  case 6: /* statement: func_def  */
#line 139 "parser.y"
                                 { (yyval.node) = (yyvsp[0].node); }
#line 1373 "y.tab.c"
    break;

  case 7: /* statement: return_stmt SEMICOLON  */
#line 140 "parser.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 1379 "y.tab.c"
    break;

  case 8: /* statement: func_call_expr SEMICOLON  */
#line 141 "parser.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 1385 "y.tab.c"
    break;

  case 9: /* data_type: BAFLE_INT_TYPE  */
#line 146 "parser.y"
                                 { (yyval.type_enum) = TYPE_INT;    }
#line 1391 "y.tab.c"
    break;

  case 10: /* data_type: BAFLE_FLOAT_TYPE  */
#line 147 "parser.y"
                                 { (yyval.type_enum) = TYPE_FLOAT;  }
#line 1397 "y.tab.c"
    break;

  case 11: /* data_type: BAFLE_CHAR_TYPE  */
#line 148 "parser.y"
                                 { (yyval.type_enum) = TYPE_CHAR;   }
#line 1403 "y.tab.c"
    break;

  case 12: /* data_type: BAFLE_STR_TYPE  */
#line 149 "parser.y"
                                 { (yyval.type_enum) = TYPE_STRING; }
#line 1409 "y.tab.c"
    break;

  case 13: /* data_type: BAFLE_BOOL_TYPE  */
#line 150 "parser.y"
                                 { (yyval.type_enum) = TYPE_BOOL;   }
#line 1415 "y.tab.c"
    break;

  case 14: /* expression: INT_LITERAL  */
#line 155 "parser.y"
                                 { (yyval.node) = ast_lit_int((yyvsp[0].i_val));    }
#line 1421 "y.tab.c"
    break;

  case 15: /* expression: FLOAT_LITERAL  */
#line 156 "parser.y"
                                 { (yyval.node) = ast_lit_float((yyvsp[0].f_val));  }
#line 1427 "y.tab.c"
    break;

  case 16: /* expression: CHAR_LITERAL  */
#line 157 "parser.y"
                                 { (yyval.node) = ast_lit_char((yyvsp[0].c_val));   }
#line 1433 "y.tab.c"
    break;

  case 17: /* expression: STRING_LITERAL  */
#line 158 "parser.y"
                                 { (yyval.node) = ast_lit_string((yyvsp[0].s_val)); free((yyvsp[0].s_val)); }
#line 1439 "y.tab.c"
    break;

  case 18: /* expression: IDENTIFIER  */
#line 159 "parser.y"
                                 { (yyval.node) = ast_identifier((yyvsp[0].s_val)); free((yyvsp[0].s_val)); }
#line 1445 "y.tab.c"
    break;

  case 19: /* expression: func_call_expr  */
#line 160 "parser.y"
                                 { (yyval.node) = (yyvsp[0].node); }
#line 1451 "y.tab.c"
    break;

  case 20: /* expression: expression PLUS expression  */
#line 161 "parser.y"
                                 { (yyval.node) = ast_binop(OP_ADD, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1457 "y.tab.c"
    break;

  case 21: /* expression: expression SUB expression  */
#line 162 "parser.y"
                                 { (yyval.node) = ast_binop(OP_SUB, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1463 "y.tab.c"
    break;

  case 22: /* expression: expression MUL expression  */
#line 163 "parser.y"
                                 { (yyval.node) = ast_binop(OP_MUL, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1469 "y.tab.c"
    break;

  case 23: /* expression: expression DIV expression  */
#line 164 "parser.y"
                                 { (yyval.node) = ast_binop(OP_DIV, (yyvsp[-2].node), (yyvsp[0].node)); }
#line 1475 "y.tab.c"
    break;

  case 24: /* expression: LPARENT expression RPARENT  */
#line 165 "parser.y"
                                 { (yyval.node) = (yyvsp[-1].node); }
#line 1481 "y.tab.c"
    break;

  case 25: /* func_call_expr: IDENTIFIER LPARENT RPARENT  */
#line 170 "parser.y"
                                 {
        (yyval.node) = ast_func_call((yyvsp[-2].s_val), NULL, 0);
        free((yyvsp[-2].s_val));
    }
#line 1490 "y.tab.c"
    break;

  case 26: /* func_call_expr: IDENTIFIER LPARENT arg_list RPARENT  */
#line 174 "parser.y"
                                          {
        ArgList *al = (ArgList *)(yyvsp[-1].ptr_val);
        (yyval.node) = ast_func_call((yyvsp[-3].s_val), al->nodes, al->count);
        free((yyvsp[-3].s_val));
        free(al);
    }
#line 1501 "y.tab.c"
    break;

  case 27: /* arg_list: expression  */
#line 183 "parser.y"
                 {
        ArgList *al = arglist_new();
        arglist_add(al, (yyvsp[0].node));
        (yyval.ptr_val) = (void *)al;
    }
#line 1511 "y.tab.c"
    break;

  case 28: /* arg_list: arg_list COMMA expression  */
#line 188 "parser.y"
                                {
        ArgList *al = (ArgList *)(yyvsp[-2].ptr_val);
        arglist_add(al, (yyvsp[0].node));
        (yyval.ptr_val) = (void *)al;
    }
#line 1521 "y.tab.c"
    break;

  case 29: /* declaration: SET IDENTIFIER COMO data_type ASSIGN expression  */
#line 197 "parser.y"
                                                      {
        (yyval.node) = ast_var_decl((yyvsp[-4].s_val), (DataType)(yyvsp[-2].type_enum), (yyvsp[0].node));
        free((yyvsp[-4].s_val));
    }
#line 1530 "y.tab.c"
    break;

  case 30: /* show_stmt: SHOW expression  */
#line 205 "parser.y"
                                 { (yyval.node) = ast_show_var((yyvsp[0].node)); }
#line 1536 "y.tab.c"
    break;

  case 31: /* $@1: %empty  */
#line 212 "parser.y"
             { block_push_ctx(); }
#line 1542 "y.tab.c"
    break;

  case 32: /* func_def: FUNC IDENTIFIER LPARENT RPARENT ARROW data_type LBRACE $@1 func_body RBRACE  */
#line 212 "parser.y"
                                                    {
        ASTNode *body = block_pop_ctx();
        (yyval.node) = ast_func_decl((yyvsp[-8].s_val), (DataType)(yyvsp[-4].type_enum), NULL, 0, body);
        free((yyvsp[-8].s_val));
    }
#line 1552 "y.tab.c"
    break;

  case 33: /* $@2: %empty  */
#line 218 "parser.y"
             { block_push_ctx(); }
#line 1558 "y.tab.c"
    break;

  case 34: /* func_def: FUNC IDENTIFIER LPARENT param_list RPARENT ARROW data_type LBRACE $@2 func_body RBRACE  */
#line 218 "parser.y"
                                                    {
        ParmList *pl = (ParmList *)(yyvsp[-7].ptr_val);
        ASTNode  *body = block_pop_ctx();
        (yyval.node) = ast_func_decl((yyvsp[-9].s_val), (DataType)(yyvsp[-4].type_enum), pl->params, pl->count, body);
        for (int i = 0; i < pl->count; i++) free(pl->params[i].name);
        free(pl->params);
        free(pl);
        free((yyvsp[-9].s_val));
    }
#line 1572 "y.tab.c"
    break;

  case 35: /* param_list: data_type IDENTIFIER  */
#line 230 "parser.y"
                           {
        ParmList *pl = parmlist_new();
        parmlist_add(pl, (yyvsp[0].s_val), (DataType)(yyvsp[-1].type_enum));
        free((yyvsp[0].s_val));
        (yyval.ptr_val) = (void *)pl;
    }
#line 1583 "y.tab.c"
    break;

  case 36: /* param_list: param_list COMMA data_type IDENTIFIER  */
#line 236 "parser.y"
                                            {
        ParmList *pl = (ParmList *)(yyvsp[-3].ptr_val);
        parmlist_add(pl, (yyvsp[0].s_val), (DataType)(yyvsp[-1].type_enum));
        free((yyvsp[0].s_val));
        (yyval.ptr_val) = (void *)pl;
    }
#line 1594 "y.tab.c"
    break;

  case 39: /* func_body_stmt: declaration SEMICOLON  */
#line 251 "parser.y"
                                 { block_add_stmt((yyvsp[-1].node)); }
#line 1600 "y.tab.c"
    break;

  case 40: /* func_body_stmt: show_stmt SEMICOLON  */
#line 252 "parser.y"
                                 { block_add_stmt((yyvsp[-1].node)); }
#line 1606 "y.tab.c"
    break;

  case 41: /* func_body_stmt: return_stmt SEMICOLON  */
#line 253 "parser.y"
                                 { block_add_stmt((yyvsp[-1].node)); }
#line 1612 "y.tab.c"
    break;

  case 42: /* func_body_stmt: func_call_expr SEMICOLON  */
#line 254 "parser.y"
                                 { block_add_stmt((yyvsp[-1].node)); }
#line 1618 "y.tab.c"
    break;

  case 43: /* return_stmt: RETORNA expression  */
#line 258 "parser.y"
                                 { (yyval.node) = ast_return((yyvsp[0].node)); }
#line 1624 "y.tab.c"
    break;

  case 44: /* return_stmt: RETORNA  */
#line 259 "parser.y"
                                 { (yyval.node) = ast_return(NULL); }
#line 1630 "y.tab.c"
    break;


#line 1634 "y.tab.c"

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

#line 262 "parser.y"


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
