/* A Bison parser, made by GNU Bison 2.1.  */

/* Skeleton parser for Yacc-like parsing with Bison,
   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     IDENTIFIER = 258,
     CLCL = 259,
     PUBLIC = 260,
     PROTECTED = 261,
     PRIVATE = 262,
     NAMESPACE = 263,
     STRING = 264,
     CONSTANT = 265,
     CPVIRTUAL = 266,
     ENUM = 267,
     CPTYPEDEF = 268,
     CPFRIEND = 269,
     CPOPERATOR = 270,
     TEMPLATE = 271,
     CPTHROW = 272,
     CPTRY = 273,
     CPCATCH = 274,
     SCSPEC = 275,
     TYPENAME_KEYWORD = 276,
     USING = 277,
     EXTERN = 278,
     ELLIPSIS = 279,
     ASM_KEYWORD = 280,
     CV_QUALIFIER = 281,
     TYPEQUAL = 282,
     BINOP = 283,
     BUILTIN = 284,
     DOCSECTION = 285
   };
#endif
/* Tokens.  */
#define IDENTIFIER 258
#define CLCL 259
#define PUBLIC 260
#define PROTECTED 261
#define PRIVATE 262
#define NAMESPACE 263
#define STRING 264
#define CONSTANT 265
#define CPVIRTUAL 266
#define ENUM 267
#define CPTYPEDEF 268
#define CPFRIEND 269
#define CPOPERATOR 270
#define TEMPLATE 271
#define CPTHROW 272
#define CPTRY 273
#define CPCATCH 274
#define SCSPEC 275
#define TYPENAME_KEYWORD 276
#define USING 277
#define EXTERN 278
#define ELLIPSIS 279
#define ASM_KEYWORD 280
#define CV_QUALIFIER 281
#define TYPEQUAL 282
#define BINOP 283
#define BUILTIN 284
#define DOCSECTION 285




#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;



