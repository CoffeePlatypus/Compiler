/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IDENT_TOK = 258,
    INT_TOK = 259,
    CHR_TOK = 260,
    BOOL_TOK = 261,
    VOID_TOK = 262,
    IF_TOK = 263,
    ELSE_TOK = 264,
    WHILE_TOK = 265,
    INTLIT_TOK = 266,
    CHRLIT_TOK = 267,
    BOOLLIT_TOK = 268,
    ARROW_TOK = 269,
    DBLCOLON_TOK = 270,
    PUT_TOK = 271,
    GET_TOK = 272,
    GTEQ_TOK = 273,
    LTEQ_TOK = 274,
    EQ_TOK = 275,
    NTEQ_TOK = 276,
    UMINUS = 277
  };
#endif
/* Tokens.  */
#define IDENT_TOK 258
#define INT_TOK 259
#define CHR_TOK 260
#define BOOL_TOK 261
#define VOID_TOK 262
#define IF_TOK 263
#define ELSE_TOK 264
#define WHILE_TOK 265
#define INTLIT_TOK 266
#define CHRLIT_TOK 267
#define BOOLLIT_TOK 268
#define ARROW_TOK 269
#define DBLCOLON_TOK 270
#define PUT_TOK 271
#define GET_TOK 272
#define GTEQ_TOK 273
#define LTEQ_TOK 274
#define EQ_TOK 275
#define NTEQ_TOK 276
#define UMINUS 277

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 18 "YGrammar.y" /* yacc.c:1909  */

  int Integer;
  char * Text;
  enum BaseTypes BaseType;
  struct IdList * IdList;
  struct LiteralDesc * LiteralDesc;
  struct InstrSeq * InstrSeq;
  struct ExprResult * ExprResult;
  struct CondResult * CondResult;

#line 109 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
