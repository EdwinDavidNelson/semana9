%{
#include "y.tab.h"
#include "src/symbols.h"
#include <string.h>
%}

%option nounput
%option noinput
%option yylineno

DIGIT   [0-9]
LETTER  [a-zA-Z_]
ID      {LETTER}({LETTER}|{DIGIT})*
WS      [ \t\r\n]+
STRING  \"(\\.|[^\"\\])*\"

%%

{WS}                   { /* ignorar espacios */ }
"~~"[^\n]*             { /* ignorar comentarios */ }

"bafle_int"            { return BAFLE_INT_TYPE;   }
"bafle_float"          { return BAFLE_FLOAT_TYPE; }
"bafle_char"           { return BAFLE_CHAR_TYPE;  }
"bafle_str"            { return BAFLE_STR_TYPE;   }
"bafle_bool"           { return BAFLE_BOOL_TYPE;  }

"set"                  { return SET;       }
"como"                 { return COMO;      }
"show"                 { return SHOW;      }
"func"                 { return FUNC;      }
"retorna"              { return RETORNA;   }
"si"                   { return SI;        }
"sino"                 { return SINO;      }
"mientras"             { return MIENTRAS;  }

"="                    { return ASSIGN;    }
"=="                   { return EQ;        }
"!="                   { return NEQ;       }
"<="                   { return LTE;       }
">="                   { return GTE;       }
"<"                    { return LT;        }
">"                    { return GT;        }
";"                    { return SEMICOLON; }
"("                    { return LPARENT;   }
")"                    { return RPARENT;   }
"{"                    { return LBRACE;    }
"}"                    { return RBRACE;    }
","                    { return COMMA;     }
"->"                   { return ARROW;     }

"+"                    { return PLUS;      }
"-"                    { return SUB;       }
"*"                    { return MUL;       }
"/"                    { return DIV;       }

{DIGIT}*\.{DIGIT}+     { yylval.f_val = atof(yytext); return FLOAT_LITERAL; }
{DIGIT}+               { yylval.i_val = atoi(yytext); return INT_LITERAL;   }

\'.\' {
    yylval.c_val = yytext[1];
    return CHAR_LITERAL;
}

{STRING} {
    yylval.s_val = strndup(yytext + 1, strlen(yytext) - 2);
    return STRING_LITERAL;
}

{ID} {
    yylval.s_val = strdup(yytext);
    return IDENTIFIER;
}

. { fprintf(stderr, "[línea %d] Error léxico: caracter desconocido '%s'\n", yylineno, yytext); }

%%

int yywrap() { return 1; }
