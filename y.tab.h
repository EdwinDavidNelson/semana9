/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

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

#line 153 "y.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
