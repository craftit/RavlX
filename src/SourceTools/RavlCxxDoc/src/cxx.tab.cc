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

/* Written by Richard Stallman by simplifying the original so called
   ``semantic'' parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



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




/* Copy the first part of user declarations.  */
#line 2 "cxx.y"

#include "Ravl/Stream.hh"
#include "Ravl/CxxDoc/CxxElements.hh"
#include "Ravl/CxxDoc/DocNode.hh"
#include "Ravl/CxxDoc/Strings.hh"

#include <stdlib.h>

#ifndef NDEBUG
#define YYDEBUG 1
#endif
  
#define YYSTYPE ObjectC 
  
  namespace RavlCxxDocN {
    extern ObjectC hookCommentObj;
    extern ObjectListC results;

  }
  
  using namespace RavlN;
  using namespace RavlCxxDocN;
  
  extern int yyerror(const char *str);
  extern int yylex();

  static int anonCount = 1;



/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined (YYSTYPE) && ! defined (YYSTYPE_IS_DECLARED)
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 219 of yacc.c.  */
#line 186 "cxx.tab.c"

#if ! defined (YYSIZE_T) && defined (__SIZE_TYPE__)
# define YYSIZE_T __SIZE_TYPE__
#endif
#if ! defined (YYSIZE_T) && defined (size_t)
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T) && (defined (__STDC__) || defined (__cplusplus))
# include <stddef.h> /* INFRINGES ON USER NAME SPACE */
# define YYSIZE_T size_t
#endif
#if ! defined (YYSIZE_T)
# define YYSIZE_T unsigned int
#endif

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

#if ! defined (yyoverflow) || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if defined (__STDC__) || defined (__cplusplus)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     define YYINCLUDED_STDLIB_H
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning. */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2005 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM ((YYSIZE_T) -1)
#  endif
#  ifdef __cplusplus
extern "C" {
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if (! defined (malloc) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if (! defined (free) && ! defined (YYINCLUDED_STDLIB_H) \
	&& (defined (__STDC__) || defined (__cplusplus)))
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifdef __cplusplus
}
#  endif
# endif
#endif /* ! defined (yyoverflow) || YYERROR_VERBOSE */


#if (! defined (yyoverflow) \
     && (! defined (__cplusplus) \
	 || (defined (YYSTYPE_IS_TRIVIAL) && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  short int yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (short int) + sizeof (YYSTYPE))			\
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined (__GNUC__) && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (0)
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (0)

#endif

#if defined (__STDC__) || defined (__cplusplus)
   typedef signed char yysigned_char;
#else
   typedef short int yysigned_char;
#endif

/* YYFINAL -- State number of the termination state. */
#define YYFINAL  87
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   1289

/* YYNTOKENS -- Number of terminals. */
#define YYNTOKENS  53
/* YYNNTS -- Number of nonterminals. */
#define YYNNTS  68
/* YYNRULES -- Number of rules. */
#define YYNRULES  327
/* YYNRULES -- Number of states. */
#define YYNSTATES  545

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   285

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const unsigned char yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    52,     2,     2,     2,    49,    46,     2,
      37,    39,    48,    44,    32,    45,     2,    47,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    34,    33,
      42,    35,    43,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    38,     2,    41,    51,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    31,    50,    40,    36,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const unsigned short int yyprhs[] =
{
       0,     0,     3,     5,     7,    10,    11,    13,    15,    17,
      19,    25,    31,    33,    36,    38,    41,    46,    48,    50,
      54,    59,    67,    71,    81,    84,    93,   101,   105,   109,
     113,   122,   132,   137,   143,   146,   147,   149,   151,   155,
     157,   161,   163,   166,   168,   172,   176,   178,   179,   181,
     183,   186,   189,   191,   194,   199,   201,   204,   208,   214,
     219,   222,   226,   230,   239,   244,   247,   251,   252,   254,
     256,   258,   262,   266,   271,   275,   277,   281,   284,   287,
     290,   299,   303,   305,   309,   311,   315,   317,   321,   322,
     325,   329,   331,   333,   335,   337,   340,   344,   348,   350,
     351,   354,   357,   360,   363,   365,   368,   369,   371,   373,
     376,   380,   384,   388,   390,   392,   394,   396,   398,   400,
     402,   404,   406,   408,   410,   412,   414,   416,   418,   420,
     422,   424,   426,   428,   430,   432,   434,   436,   438,   440,
     442,   444,   446,   448,   450,   453,   457,   462,   465,   469,
     473,   476,   479,   483,   487,   492,   497,   501,   502,   504,
     506,   510,   514,   519,   521,   525,   528,   535,   538,   539,
     542,   545,   548,   550,   554,   559,   567,   568,   570,   572,
     575,   579,   581,   583,   585,   587,   589,   591,   593,   595,
     597,   599,   601,   603,   605,   607,   609,   611,   613,   615,
     617,   619,   621,   623,   625,   627,   629,   631,   633,   635,
     637,   639,   641,   643,   645,   647,   649,   651,   653,   655,
     657,   659,   662,   665,   670,   674,   677,   679,   681,   683,
     685,   687,   689,   691,   693,   695,   697,   699,   701,   703,
     706,   709,   712,   715,   717,   719,   720,   724,   728,   733,
     741,   745,   752,   761,   766,   773,   778,   779,   781,   783,
     787,   788,   790,   792,   796,   799,   801,   804,   806,   808,
     818,   830,   831,   833,   836,   838,   842,   844,   848,   849,
     852,   856,   858,   860,   862,   864,   866,   868,   870,   872,
     874,   876,   878,   880,   882,   884,   886,   888,   890,   892,
     894,   896,   898,   900,   902,   904,   906,   908,   910,   912,
     914,   916,   918,   920,   922,   924,   926,   928,   930,   932,
     934,   936,   938,   940,   942,   944,   946,   948
};

/* YYRHS -- A `-1'-separated list of the rules' RHS. */
static const yysigned_char yyrhs[] =
{
      54,     0,    -1,    56,    -1,    57,    -1,    55,    57,    -1,
      -1,    55,    -1,   102,    -1,    58,    -1,    72,    -1,    25,
      37,    62,    39,    33,    -1,     8,    65,    31,    56,    40,
      -1,    69,    -1,    71,    33,    -1,    70,    -1,     1,    33,
      -1,     1,    31,   119,    40,    -1,    33,    -1,    30,    -1,
      90,    93,    33,    -1,    13,    90,     3,    33,    -1,    13,
      90,     3,    38,    89,    41,    33,    -1,    13,   115,    33,
      -1,    13,    20,    65,    81,    31,    56,    40,     3,    33,
      -1,   115,    33,    -1,    13,    12,    65,    31,    59,    40,
       3,    33,    -1,    12,    65,    31,    59,    40,    92,    33,
      -1,    20,     3,    33,    -1,    12,     3,    33,    -1,    14,
      90,    33,    -1,    20,     3,    81,    31,    85,    40,    92,
      33,    -1,    26,    20,    65,    81,    31,    85,    40,    92,
      33,    -1,    23,    90,    93,    33,    -1,    23,     9,    31,
      56,    40,    -1,    68,    33,    -1,    -1,    60,    -1,    61,
      -1,    60,    32,    61,    -1,     3,    -1,     3,    35,    88,
      -1,     9,    -1,    62,     9,    -1,     3,    -1,    67,     4,
      63,    -1,    67,     4,    64,    -1,     3,    -1,    -1,    63,
      -1,    64,    -1,     4,    64,    -1,     4,    63,    -1,     3,
      -1,    36,     3,    -1,     3,    42,    73,    43,    -1,    67,
      -1,     4,    67,    -1,    68,     4,    67,    -1,     8,     3,
      35,    66,    33,    -1,    22,     8,    66,    33,    -1,    22,
      64,    -1,    22,     4,    64,    -1,    22,     4,    63,    -1,
      75,    20,     3,    81,    31,    85,    40,    33,    -1,    75,
      20,     3,    33,    -1,    75,   102,    -1,    75,    75,   102,
      -1,    -1,    74,    -1,    90,    -1,    10,    -1,    74,    32,
      90,    -1,    74,    32,    10,    -1,    16,    42,    76,    43,
      -1,    16,    42,    43,    -1,    80,    -1,    76,    32,    80,
      -1,    90,    65,    -1,    20,    65,    -1,    21,    66,    -1,
      90,    37,    65,    39,    37,   112,    39,   116,    -1,    75,
      20,    65,    -1,    78,    -1,    78,    35,    90,    -1,    77,
      -1,    77,    35,    89,    -1,    79,    -1,    79,    35,    90,
      -1,    -1,    34,    82,    -1,    82,    32,    84,    -1,    84,
      -1,     5,    -1,     7,    -1,     6,    -1,    83,    68,    -1,
      11,    83,    68,    -1,    83,    11,    68,    -1,    68,    -1,
      -1,    85,    86,    -1,     6,    34,    -1,     7,    34,    -1,
       5,    34,    -1,    57,    -1,    11,   102,    -1,    -1,    88,
      -1,    89,    -1,    88,    89,    -1,    37,    99,    39,    -1,
      38,    88,    41,    -1,    31,    88,    40,    -1,     3,    -1,
      10,    -1,    26,    -1,    29,    -1,     9,    -1,    28,    -1,
      42,    -1,    43,    -1,    35,    -1,    44,    -1,    45,    -1,
      36,    -1,    46,    -1,    47,    -1,    48,    -1,    49,    -1,
      50,    -1,    51,    -1,    52,    -1,    34,    -1,     4,    -1,
      11,    -1,    12,    -1,    13,    -1,    14,    -1,    15,    -1,
      16,    -1,    17,    -1,    21,    -1,    22,    -1,    91,    -1,
      26,    91,    -1,    26,    26,    91,    -1,    26,    26,    26,
      91,    -1,    68,    96,    -1,    20,    68,    96,    -1,    12,
      68,    96,    -1,    29,    96,    -1,    27,    96,    -1,    27,
      29,    96,    -1,    27,    27,    96,    -1,    27,    27,    29,
      96,    -1,    27,    27,    27,    96,    -1,    21,    66,    96,
      -1,    -1,    93,    -1,    95,    -1,    93,    32,    95,    -1,
      38,    87,    41,    -1,    94,    38,    87,    41,    -1,     3,
      -1,     3,    35,    88,    -1,     3,    94,    -1,     3,    94,
      35,    31,   119,    40,    -1,    35,    88,    -1,    -1,    96,
      48,    -1,    96,    46,    -1,    96,    26,    -1,    98,    -1,
      97,    32,    98,    -1,     3,    37,    99,    39,    -1,     3,
      42,    73,    43,    37,    99,    39,    -1,    -1,   100,    -1,
     101,    -1,   100,   101,    -1,    37,    99,    39,    -1,     3,
      -1,    10,    -1,    28,    -1,     4,    -1,     5,    -1,     6,
      -1,     7,    -1,     8,    -1,    26,    -1,     9,    -1,    11,
      -1,    12,    -1,    13,    -1,    29,    -1,    14,    -1,    15,
      -1,    16,    -1,    17,    -1,    21,    -1,    22,    -1,    34,
      -1,    42,    -1,    43,    -1,    35,    -1,    44,    -1,    45,
      -1,    36,    -1,    46,    -1,    47,    -1,    48,    -1,    49,
      -1,    50,    -1,    51,    -1,    52,    -1,    38,    -1,    41,
      -1,    33,    -1,    32,    -1,   104,    -1,   103,    -1,    14,
     103,    -1,   108,    33,    -1,   108,    35,    10,    33,    -1,
      23,   108,    33,    -1,   108,   118,    -1,    44,    -1,    45,
      -1,    36,    -1,    46,    -1,    47,    -1,    48,    -1,    49,
      -1,    50,    -1,    51,    -1,    52,    -1,    35,    -1,    42,
      -1,    43,    -1,    45,    45,    -1,    44,    44,    -1,    38,
      41,    -1,    37,    39,    -1,    28,    -1,     3,    -1,    -1,
      42,    73,    43,    -1,    37,   112,    39,    -1,    90,    67,
     107,   116,    -1,    90,    67,     4,    63,   106,   107,   116,
      -1,    90,   107,   116,    -1,    90,    15,   105,   106,   107,
     116,    -1,    90,    67,     4,    15,   105,   106,   107,   116,
      -1,    15,    90,   107,   116,    -1,    68,     4,    15,    90,
     107,   116,    -1,    17,    37,   110,    39,    -1,    -1,   111,
      -1,    68,    -1,   111,    32,    68,    -1,    -1,   113,    -1,
     114,    -1,   113,    32,   114,    -1,   113,    24,    -1,    90,
      -1,    90,    95,    -1,   115,    -1,    24,    -1,    90,    37,
      48,    65,    39,    37,   112,    39,   116,    -1,    90,    37,
      68,     4,    48,     3,    39,    37,   112,    39,   116,    -1,
      -1,   109,    -1,    26,   109,    -1,    26,    -1,    31,   119,
      40,    -1,   117,    -1,    34,    97,   117,    -1,    -1,   119,
     120,    -1,    31,   119,    40,    -1,     3,    -1,    10,    -1,
      20,    -1,    35,    -1,    44,    -1,    45,    -1,    36,    -1,
      46,    -1,    47,    -1,    48,    -1,    49,    -1,    50,    -1,
      51,    -1,    52,    -1,    34,    -1,    37,    -1,    39,    -1,
      38,    -1,    41,    -1,    33,    -1,    32,    -1,    42,    -1,
      43,    -1,    28,    -1,     4,    -1,     5,    -1,     6,    -1,
       7,    -1,     8,    -1,    26,    -1,     9,    -1,    11,    -1,
      12,    -1,    13,    -1,    27,    -1,    29,    -1,    14,    -1,
      15,    -1,    16,    -1,    17,    -1,    18,    -1,    19,    -1,
      21,    -1,    22,    -1,    23,    -1,    25,    -1,    24,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const unsigned short int yyrline[] =
{
       0,    79,    79,    84,    85,    88,    89,    92,    93,    94,
      95,    96,   105,   106,   107,   108,   109,   110,   111,   118,
     128,   133,   138,   143,   163,   164,   183,   190,   195,   200,
     203,   212,   225,   226,   227,   231,   232,   234,   235,   237,
     238,   244,   245,   250,   253,   254,   257,   258,   262,   263,
     264,   265,   270,   271,   272,   276,   277,   283,   293,   297,
     305,   306,   307,   316,   322,   327,   331,   341,   342,   345,
     346,   347,   348,   352,   353,   356,   357,   360,   369,   370,
     371,   375,   386,   387,   388,   389,   390,   391,   404,   405,
     408,   409,   411,   412,   413,   415,   419,   424,   429,   435,
     436,   438,   439,   440,   441,   442,   446,   447,   449,   450,
     452,   453,   454,   455,   455,   455,   455,   455,   455,   456,
     457,   458,   459,   460,   461,   462,   463,   464,   465,   466,
     467,   468,   469,   470,   471,   472,   473,   474,   475,   476,
     477,   478,   479,   483,   484,   485,   486,   489,   492,   496,
     500,   503,   506,   509,   512,   515,   518,   523,   524,   526,
     527,   529,   530,   532,   533,   534,   535,   537,   539,   540,
     541,   542,   546,   547,   549,   550,   552,   553,   555,   556,
     558,   559,   559,   559,   560,   560,   560,   560,   560,   560,
     561,   561,   561,   561,   561,   562,   562,   562,   562,   562,
     562,   563,   564,   565,   566,   567,   568,   569,   570,   571,
     572,   573,   574,   575,   576,   577,   578,   579,   580,   585,
     586,   587,   593,   597,   601,   607,   612,   613,   614,   615,
     616,   617,   618,   619,   620,   621,   622,   623,   624,   625,
     626,   627,   628,   629,   630,   632,   633,   635,   639,   646,
     653,   673,   680,   688,   693,   702,   704,   705,   707,   708,
     710,   711,   713,   714,   715,   717,   718,   719,   720,   723,
     737,   748,   749,   750,   751,   754,   756,   757,   759,   760,
     762,   763,   763,   763,   764,   764,   764,   764,   764,   764,
     764,   764,   764,   764,   764,   764,   765,   765,   765,   765,
     765,   765,   765,   765,   765,   766,   766,   766,   766,   766,
     766,   767,   767,   767,   767,   767,   767,   768,   768,   768,
     768,   768,   768,   768,   768,   769,   769,   769
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals. */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "CLCL", "PUBLIC",
  "PROTECTED", "PRIVATE", "NAMESPACE", "STRING", "CONSTANT", "CPVIRTUAL",
  "ENUM", "CPTYPEDEF", "CPFRIEND", "CPOPERATOR", "TEMPLATE", "CPTHROW",
  "CPTRY", "CPCATCH", "SCSPEC", "TYPENAME_KEYWORD", "USING", "EXTERN",
  "ELLIPSIS", "ASM_KEYWORD", "CV_QUALIFIER", "TYPEQUAL", "BINOP",
  "BUILTIN", "DOCSECTION", "'{'", "','", "';'", "':'", "'='", "'~'", "'('",
  "'['", "')'", "'}'", "']'", "'<'", "'>'", "'+'", "'-'", "'&'", "'/'",
  "'*'", "'%'", "'|'", "'^'", "'!'", "$accept", "program", "extdefs",
  "extdefs_opt", "extdef", "datadef", "enum_name_list_opt",
  "enum_name_list", "enum_name_elem", "string", "unqualified_id",
  "qualified_id", "maybe_identifier", "any_id", "scope_id",
  "scope_resolved_id", "namespace_alias", "using_directive", "using_decl",
  "template_def", "template_inst_args_opt", "template_inst_args",
  "template_header", "template_parm_list", "parm", "template_type_parm",
  "template_template_parm", "template_parm", "class_inherit_list_all",
  "class_inherit_list", "class_inherit_mode", "class_inherit_def",
  "class_lst", "class_def", "expr_no_commas_list_opt",
  "expr_no_commas_list", "expr_no_commas", "type_id", "type_id_bod",
  "var_name_list_opt", "var_name_list", "var_name_array", "var_name_elem",
  "type_id_qual", "base_class_constructors_list", "constructor_expr",
  "arg_expr_list_all", "arg_expr_list", "arg_expr_list_item", "fndef",
  "func_decl", "func_def", "operator_types", "func_arg_templ_qual",
  "func_arg_prototype", "func_prototype", "func_throw_spec",
  "list_scoped_ids_opt", "list_scoped_ids", "func_arg_list_all",
  "func_arg_list", "func_arg_def", "function_ptr_def", "func_qualifier",
  "func_body", "func_body_opt_constr", "func_body_contents_list",
  "func_body_contents", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const unsigned short int yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   123,    44,    59,    58,    61,   126,    40,    91,    41,
     125,    93,    60,    62,    43,    45,    38,    47,    42,    37,
     124,    94,    33
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const unsigned char yyr1[] =
{
       0,    53,    54,    55,    55,    56,    56,    57,    57,    57,
      57,    57,    57,    57,    57,    57,    57,    57,    57,    58,
      58,    58,    58,    58,    58,    58,    58,    58,    58,    58,
      58,    58,    58,    58,    58,    59,    59,    60,    60,    61,
      61,    62,    62,    63,    64,    64,    65,    65,    66,    66,
      66,    66,    67,    67,    67,    68,    68,    68,    69,    70,
      71,    71,    71,    72,    72,    72,    72,    73,    73,    74,
      74,    74,    74,    75,    75,    76,    76,    77,    78,    78,
      78,    79,    80,    80,    80,    80,    80,    80,    81,    81,
      82,    82,    83,    83,    83,    84,    84,    84,    84,    85,
      85,    86,    86,    86,    86,    86,    87,    87,    88,    88,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    89,    89,    89,    89,    89,    89,    89,
      89,    89,    89,    90,    90,    90,    90,    91,    91,    91,
      91,    91,    91,    91,    91,    91,    91,    92,    92,    93,
      93,    94,    94,    95,    95,    95,    95,    95,    96,    96,
      96,    96,    97,    97,    98,    98,    99,    99,   100,   100,
     101,   101,   101,   101,   101,   101,   101,   101,   101,   101,
     101,   101,   101,   101,   101,   101,   101,   101,   101,   101,
     101,   101,   101,   101,   101,   101,   101,   101,   101,   101,
     101,   101,   101,   101,   101,   101,   101,   101,   101,   102,
     102,   102,   103,   103,   103,   104,   105,   105,   105,   105,
     105,   105,   105,   105,   105,   105,   105,   105,   105,   105,
     105,   105,   105,   105,   105,   106,   106,   107,   108,   108,
     108,   108,   108,   108,   108,   109,   110,   110,   111,   111,
     112,   112,   113,   113,   113,   114,   114,   114,   114,   115,
     115,   116,   116,   116,   116,   117,   118,   118,   119,   119,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120,   120,   120,
     120,   120,   120,   120,   120,   120,   120,   120
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const unsigned char yyr2[] =
{
       0,     2,     1,     1,     2,     0,     1,     1,     1,     1,
       5,     5,     1,     2,     1,     2,     4,     1,     1,     3,
       4,     7,     3,     9,     2,     8,     7,     3,     3,     3,
       8,     9,     4,     5,     2,     0,     1,     1,     3,     1,
       3,     1,     2,     1,     3,     3,     1,     0,     1,     1,
       2,     2,     1,     2,     4,     1,     2,     3,     5,     4,
       2,     3,     3,     8,     4,     2,     3,     0,     1,     1,
       1,     3,     3,     4,     3,     1,     3,     2,     2,     2,
       8,     3,     1,     3,     1,     3,     1,     3,     0,     2,
       3,     1,     1,     1,     1,     2,     3,     3,     1,     0,
       2,     2,     2,     2,     1,     2,     0,     1,     1,     2,
       3,     3,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     3,     4,     2,     3,     3,
       2,     2,     3,     3,     4,     4,     3,     0,     1,     1,
       3,     3,     4,     1,     3,     2,     6,     2,     0,     2,
       2,     2,     1,     3,     4,     7,     0,     1,     1,     2,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     2,     4,     3,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     2,
       2,     2,     2,     1,     1,     0,     3,     3,     4,     7,
       3,     6,     8,     4,     6,     4,     0,     1,     1,     3,
       0,     1,     1,     3,     2,     1,     2,     1,     1,     9,
      11,     0,     1,     2,     1,     3,     1,     3,     0,     2,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const unsigned short int yydefact[] =
{
       0,     0,    52,     0,    47,    47,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   168,   168,    18,    17,
       0,     0,     0,     2,     3,     8,    55,   168,    12,    14,
       0,     9,     0,     0,   143,     7,   220,   219,     0,     0,
     278,    15,    67,    56,    46,     0,    52,     0,   168,    47,
      47,     0,   168,     0,     0,     0,     0,     0,   168,     0,
     221,     0,     0,     0,    52,   168,    43,     0,    48,    49,
     168,     0,     0,     0,    60,     0,     0,     0,     0,    47,
       0,   144,   168,   168,   151,   150,    53,     1,     4,     0,
      34,   147,    13,     0,     0,     0,     0,    65,    52,     0,
       0,   260,     0,     0,   159,   271,   278,   222,     0,     0,
     276,   225,    24,     0,    70,     0,    68,    69,     0,     0,
      28,    35,     0,   149,    52,     0,    88,     0,     0,    22,
      29,   260,   271,    47,     0,    74,     0,     0,    84,    82,
      86,    75,    47,    27,     0,     0,   148,    51,    50,   156,
       0,    62,    61,     0,     0,     0,   224,    41,     0,    88,
       0,   145,   168,   168,   153,   152,   171,   170,   169,     0,
      57,    52,    66,     0,   106,   165,   244,   243,   236,   228,
       0,     0,   237,   238,   226,   227,   229,   230,   231,   232,
     233,   234,   235,   245,   113,   133,   117,   114,   134,   135,
     136,   137,   138,   139,   140,   141,   142,   115,   118,   116,
       0,   132,   121,   124,   176,     0,   119,   120,   122,   123,
     125,   126,   127,   128,   129,   130,   131,   167,   108,   268,
      47,   168,   265,     0,   261,   262,   267,     0,   271,     0,
      19,     0,   274,   272,   250,     0,     0,     0,   172,     0,
     281,   305,   306,   307,   308,   309,   311,   282,   312,   313,
     314,   317,   318,   319,   320,   321,   322,   283,   323,   324,
     325,   327,   326,   310,   315,   304,   316,   278,   301,   300,
     295,   284,   287,   296,   298,   297,    16,   299,   302,   303,
     285,   286,   288,   289,   290,   291,   292,   293,   294,   279,
      54,     0,     0,     0,    39,     0,    36,    37,    35,     0,
      20,     0,     0,   253,    78,   168,    47,     0,    73,     0,
       0,     0,    46,    47,    77,    92,    94,    93,     0,    98,
      89,     0,    91,    99,    44,    45,    59,     0,    32,    42,
       0,     0,   146,   155,   154,     0,    64,     0,   164,     0,
     107,     0,   106,   242,   241,   240,   239,    67,     0,     0,
     181,   184,   185,   186,   187,   188,   190,   182,   191,   192,
     193,   195,   196,   197,   198,   199,   200,   189,   183,   194,
     218,   217,   201,   204,   207,   176,   215,   216,   202,   203,
     205,   206,   208,   209,   210,   211,   212,   213,   214,     0,
     177,   178,     0,   109,     0,     0,   163,   266,   247,   264,
       0,    43,     0,   245,   248,   160,   256,   273,   275,   176,
      67,     0,   277,   223,     0,    72,    71,    58,    11,     0,
     157,     0,     0,     0,     0,    81,    76,    85,    83,    87,
       0,     0,     0,     0,    95,     0,    33,    10,    99,   271,
      99,   161,   278,     0,     0,   271,   112,     0,   110,   179,
     111,     0,     0,   263,   245,     0,   258,     0,   257,     0,
       0,   173,   280,    40,     0,   158,    38,     0,     0,     0,
       0,    96,    90,    97,     0,     0,     0,     0,   157,   104,
     100,     0,   254,     0,     0,   162,   246,   251,   180,   260,
       0,     0,   271,   255,     0,   174,     0,    26,     0,     0,
      21,   260,   103,   101,   102,   105,     0,   157,     0,   166,
       0,     0,   271,   249,   259,   176,    25,     0,     0,    30,
       0,    63,   271,   260,   252,     0,    23,   271,    31,   269,
       0,   175,    80,   271,   270
};

/* YYDEFGOTO[NTERM-NUM]. */
static const short int yydefgoto[] =
{
      -1,    21,    22,    23,    24,    25,   305,   306,   307,   158,
      68,    69,    45,    70,    26,    52,    28,    29,    30,    31,
     115,   116,    32,   137,   138,   139,   140,   141,   145,   330,
     331,   332,   445,   490,   349,   350,   228,    33,    34,   474,
     475,   175,   104,    91,   247,   248,   399,   400,   401,    35,
      36,    37,   193,   358,   105,    38,   243,   467,   468,   233,
     234,   235,    39,   244,   110,   111,   113,   299
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -391
static const short int yypact[] =
{
    1057,   293,     0,    44,    49,   313,  1193,  1085,  1204,    23,
     343,   367,   302,   273,   122,  1223,   314,  -391,  -391,  -391,
     164,   189,   881,  -391,  -391,  -391,  -391,   146,  -391,  -391,
     140,  -391,   419,   151,  -391,  -391,  -391,  -391,   489,   171,
    -391,  -391,  1106,  -391,   172,   168,   270,   231,   275,   383,
     383,  1234,   275,    36,   241,   413,   413,  1119,   279,   159,
    -391,   323,   276,   369,   288,   275,    34,    88,  -391,  -391,
    -391,   333,    88,   367,  -391,   266,   166,   285,   331,   383,
    1253,  -391,   391,  -391,   244,   244,  -391,  -391,  -391,   128,
    -391,   244,  -391,  1085,   421,   463,   169,  -391,   479,   317,
     828,   260,   133,    68,  -391,    96,  -391,  -391,   341,   347,
    -391,  -391,  -391,   209,  -391,   348,   342,  -391,   367,  1026,
    -391,   385,    44,   244,   428,   363,   368,    17,    78,  -391,
    -391,  1140,    96,   383,   367,  -391,   377,    13,   371,   374,
     378,  -391,   104,  -391,   262,   395,   244,  -391,  -391,   244,
      88,  -391,  -391,   396,  1026,   345,  -391,  -391,    55,   368,
     372,  -391,  -391,  -391,   244,   244,  -391,  -391,  -391,  1204,
    -391,   476,  -391,   828,   828,   173,  -391,  -391,  -391,  -391,
     397,   400,  -391,  -391,   393,   398,  -391,  -391,  -391,  -391,
    -391,  -391,  -391,   408,  -391,  -391,  -391,  -391,  -391,  -391,
    -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,
     828,  -391,  -391,  -391,   678,   828,  -391,  -391,  -391,  -391,
    -391,  -391,  -391,  -391,  -391,  -391,  -391,   828,  -391,  -391,
     435,   447,    83,   425,     5,  -391,  -391,    25,    96,   105,
    -391,   431,   441,  -391,  -391,   528,   148,   351,  -391,   436,
    -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,
    -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,
    -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,
    -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,
    -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,
    -391,  1153,   439,   433,   444,   440,   450,  -391,   385,   454,
    -391,   828,   447,  -391,  -391,   296,   435,  1174,  -391,   828,
    1204,  1204,  -391,   435,  -391,  -391,  -391,  -391,   329,   275,
     455,   157,  -391,  -391,  -391,  -391,  -391,   448,  -391,  -391,
     458,   464,  -391,   244,   244,   276,  -391,   466,   828,   453,
     828,   469,   828,  -391,  -391,  -391,  -391,  1106,   276,   728,
    -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,
    -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,
    -391,  -391,  -391,  -391,  -391,   678,  -391,  -391,  -391,  -391,
    -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,  -391,   462,
     678,  -391,   778,  -391,   467,    99,   240,  -391,  -391,  -391,
    1140,  -391,   317,   408,  -391,  -391,   413,  -391,  -391,   678,
    1106,   341,  -391,  -391,   578,  -391,  -391,  -391,  -391,   828,
     105,   385,   473,  1026,   475,  -391,  -391,  -391,  -391,  -391,
     480,   413,   262,   413,   275,   919,  -391,  -391,  -391,    96,
    -391,  -391,  -391,   484,   460,    96,  -391,   487,  -391,  -391,
    -391,   491,   526,  -391,   408,   276,   275,   519,   576,   619,
     653,  -391,  -391,   828,   664,   666,  -391,   698,   662,   670,
     668,   275,  -391,   275,   674,   675,   683,   463,   105,  -391,
    -391,   957,  -391,   995,   628,  -391,  -391,  -391,  -391,  1140,
     679,   276,    96,  -391,   413,  -391,   696,  -391,   701,   732,
    -391,  1140,  -391,  -391,  -391,  -391,   703,   105,   713,  -391,
     708,   711,    96,  -391,   275,   678,  -391,   718,   714,  -391,
     719,  -391,    96,  1140,  -391,   716,  -391,    96,  -391,  -391,
     721,  -391,  -391,    96,  -391
};

/* YYPGOTO[NTERM-NUM].  */
static const short int yypgoto[] =
{
    -391,  -391,  -391,  -110,   -17,  -391,   459,  -391,   327,  -391,
     -40,     3,     9,   -19,    60,    11,  -391,  -391,  -391,  -391,
    -337,  -391,   -10,  -391,  -391,  -391,  -391,   452,    12,  -391,
     456,   319,   -18,  -391,   434,   -81,  -202,    -6,    -2,  -308,
       1,  -391,   -41,    14,  -391,   362,  -373,  -391,   401,   -26,
      10,  -391,   373,  -390,   -54,    28,   554,  -391,  -391,  -388,
    -391,   387,    -3,  -122,   551,  -391,  -102,  -391
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -164
static const short int yytable[] =
{
      53,    59,    62,    54,   245,    88,    97,    76,   132,   303,
     313,    27,   457,    81,    47,    74,    48,    60,    58,   227,
     454,    65,    95,   465,    58,   403,    96,   147,   411,   409,
      84,    85,   151,    27,   103,    61,   117,   410,   -52,   127,
     412,    77,    42,    58,   337,   317,   469,     2,   238,    81,
     310,    96,    44,   136,   153,   311,   318,   142,   125,   126,
      48,    65,   123,    43,   339,    63,    48,    65,    58,   172,
     148,    71,    71,   128,   501,   152,    42,   155,   161,   146,
      20,     2,     3,   470,   149,    77,   406,    96,   159,    96,
      65,    66,   348,   102,   340,   232,   164,   165,   236,   302,
     239,   240,     2,    60,    58,    65,    58,   322,   406,   434,
     334,   520,   231,   241,    20,   315,   414,   437,   100,   102,
     128,    61,   242,   528,    20,   232,   230,    71,   236,   359,
      27,     2,    71,    71,   402,    20,   102,   237,   309,   312,
     100,   323,   314,   169,    65,   540,   403,   462,   403,   170,
      89,   324,   535,   335,    98,   329,   102,   403,   342,    78,
       2,     3,     2,   345,    20,    27,    99,    86,   443,    98,
     131,   341,     2,    92,    99,   424,   343,   344,    71,    90,
     516,    99,   170,   347,    99,   419,   100,    20,   101,    87,
     420,   407,   130,    20,    71,    20,   131,   413,   415,   119,
     403,   100,    20,   131,   112,    20,   131,   118,   351,   530,
      71,   352,   250,   251,   252,   253,   254,   255,   256,   257,
     258,   259,   260,   261,   262,   263,   264,   265,   266,   267,
     268,   269,   270,   271,   272,   273,   274,   275,   276,   404,
     277,   278,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   121,     2,     3,     2,     3,   325,   326,   327,
     166,   403,    55,   328,   129,   173,     2,     3,   174,   122,
      56,    11,    75,    89,   229,    55,    51,    16,     8,    17,
     167,   449,   168,    56,    11,   426,    20,   154,    20,    51,
      16,   -46,    17,   120,   455,     2,    72,   136,   230,    20,
      73,   142,    42,   131,   438,   439,    46,     3,   156,   -88,
     176,   143,   144,   478,    40,   435,    41,   492,   -79,   149,
      42,   -79,   440,   497,   325,   326,   327,   150,    20,   -79,
     157,    82,   444,    83,   246,   177,    64,     3,   473,    20,
     494,   117,   178,   179,   180,   181,   107,   249,   109,   182,
     183,   184,   185,   186,   187,   188,   189,   190,   191,   192,
      66,    67,     2,     3,   301,     2,     3,   239,   338,    20,
     523,    55,   106,   421,    55,     9,   124,     3,   304,   133,
     134,   300,    56,    11,   308,    51,    16,   316,    17,    16,
     534,    17,   144,    20,   232,    20,   319,   236,    20,   320,
     539,   502,   135,   321,   117,   542,     2,     3,   162,    20,
     163,   544,     2,     3,   171,     3,   333,   466,   489,   336,
     491,    55,   493,    93,     8,     9,   353,   355,   322,    94,
      11,   354,    57,   356,    27,    51,    16,   522,    17,    20,
     357,   405,   481,   329,   483,    20,    27,    20,   241,   -46,
     -46,   515,   -46,   -46,   408,   170,     2,     3,   416,   423,
      42,   -46,   427,   428,   489,    55,   489,    93,     8,   429,
     430,    96,   431,    56,    11,   433,    57,   442,   446,    51,
      16,   447,    17,   232,   451,   448,   236,   450,    58,    20,
     452,   458,    27,   496,    27,   232,   461,   -88,   236,   346,
     144,  -163,  -163,   477,   173,   524,   479,   174,    42,   480,
     106,    42,   107,   108,   109,   495,   498,   232,   499,   500,
     236,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   503,   277,
     278,   279,   280,   281,   282,   283,   284,   285,   418,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   297,
     298,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   504,   277,
     278,   279,   280,   281,   282,   283,   284,   285,   472,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   297,
     298,   250,   251,   252,   253,   254,   255,   256,   257,   258,
     259,   260,   261,   262,   263,   264,   265,   266,   267,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   505,   277,
     278,   279,   280,   281,   282,   283,   284,   285,   519,   287,
     288,   289,   290,   291,   292,   293,   294,   295,   296,   297,
     298,   360,   361,   362,   363,   364,   365,   366,   367,   368,
     369,   370,   371,   372,   373,   374,   506,   507,   239,   375,
     376,   508,   509,   510,   377,   511,   378,   379,   512,   513,
     380,   381,   382,   383,   384,   385,   386,   514,   521,   387,
     388,   389,   390,   391,   392,   393,   394,   395,   396,   397,
     398,   194,   195,   525,   526,   527,   529,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   531,   532,   533,   205,
     206,   536,   538,   537,   207,   541,   208,   209,   476,   210,
     543,   482,   211,   212,   213,   214,   215,   432,   456,   436,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   194,   195,   471,   441,   464,   453,   196,   197,   198,
     199,   200,   201,   202,   203,   204,   417,   463,   422,   205,
     206,   459,     0,     0,   207,     0,   208,   209,     0,   210,
       0,     0,   211,   212,   213,   214,   215,     0,     0,   460,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,   194,   195,     0,     0,     0,     0,   196,   197,   198,
     199,   200,   201,   202,   203,   204,     0,     0,     0,   205,
     206,     0,     0,     0,   207,     0,   208,   209,     0,   210,
       0,     0,   211,   212,   213,   214,   215,     0,     0,     0,
     216,   217,   218,   219,   220,   221,   222,   223,   224,   225,
     226,    -6,     1,     0,     2,     3,     0,     0,     0,     4,
       0,     0,     0,     5,     6,     7,     8,     9,     0,     0,
       0,    10,    11,    12,    13,     0,    14,    15,    16,     0,
      17,    18,     0,     0,    19,     0,     0,    20,     0,     0,
       1,    -6,     2,     3,   484,   485,   486,     4,     0,     0,
     487,     5,     6,     7,     8,     9,     0,     0,     0,    10,
      11,    12,    13,     0,    14,    15,    16,     0,    17,    18,
       0,     0,    19,     0,     0,    20,     0,     0,     1,   488,
       2,     3,   484,   485,   486,     4,     0,     0,   487,     5,
       6,     7,     8,     9,     0,     0,     0,    10,    11,    12,
      13,     0,    14,    15,    16,     0,    17,    18,     0,     0,
      19,     0,     0,    20,     0,     0,     1,   517,     2,     3,
     484,   485,   486,     4,     0,     0,   487,     5,     6,     7,
       8,     9,     0,     0,     0,    10,    11,    12,    13,     0,
      14,    15,    16,     0,    17,    18,     0,     1,    19,     2,
       3,    20,     0,     0,     4,   518,     0,     0,     5,     6,
       7,     8,     9,     0,     0,     0,    10,    11,    12,    13,
       0,    14,    15,    16,     0,    17,    18,    -5,     1,    19,
       2,     3,    20,     0,     0,     4,    -5,     0,     0,     5,
       6,     7,     8,     9,     0,     0,     0,    10,    11,    12,
      13,     0,    14,    15,    16,     0,    17,    18,     2,     3,
      19,     0,     0,    20,     0,     0,     0,    55,     0,     0,
       8,     0,     0,     0,     0,    56,    11,     0,    57,     2,
       3,    51,    16,     0,    17,     0,   114,     0,    55,     0,
       0,    20,     2,     3,     0,     0,    56,    11,     0,     0,
       0,    55,    51,    16,     8,    17,     0,     0,     0,    56,
      11,     0,    20,     2,     3,    51,    16,     0,    17,     0,
       0,     0,    55,     0,     0,    20,     2,     3,     0,     0,
      56,    11,     0,   425,   229,    55,    51,    16,     0,    17,
       0,     0,     0,    56,    11,     0,    20,     2,     3,    51,
      16,     0,    17,     0,     0,     0,    55,     0,     0,    20,
       9,     0,     0,     0,   133,   134,     2,     3,     0,     0,
      51,    16,     0,    17,     0,    49,     0,     2,     3,     0,
      20,     0,     0,    50,    11,     0,    55,     0,     0,    51,
      16,     0,    17,     0,    56,    11,     2,     3,     0,    20,
      51,    16,     0,    17,     0,    55,     0,     2,     3,     0,
      20,     0,     0,    79,    11,     0,    55,     0,     0,    80,
      16,     0,    17,     0,    56,    11,     2,     3,     0,    20,
      80,    16,     0,    17,     0,    55,     0,     0,     0,     0,
      20,     0,     0,    56,    11,     0,     0,     0,     0,   160,
      16,     0,    17,     0,     0,     0,     0,     0,     0,    20
};

static const short int yycheck[] =
{
       6,     7,     8,     6,   106,    22,    32,    13,    62,   119,
     132,     0,   385,    15,     5,    12,     5,     7,     7,   100,
     357,    10,    32,   413,    13,   227,    32,    67,     3,    24,
      16,    17,    72,    22,    33,     7,    42,    32,     4,     3,
      15,    13,    42,    32,   154,    32,   419,     3,   102,    51,
      33,    57,     3,    63,    73,    38,    43,    63,    49,    50,
      49,    50,    48,     3,     9,    42,    55,    56,    57,    95,
      67,    11,    12,    37,   464,    72,    42,    76,    80,    65,
      36,     3,     4,   420,    70,    57,     3,    93,    79,    95,
      79,     3,   173,    33,    39,   101,    82,    83,   101,   118,
      32,    33,     3,    93,    93,    94,    95,     3,     3,   311,
     150,   499,   101,    17,    36,   134,   238,   319,    35,    59,
      37,    93,    26,   511,    36,   131,    48,    67,   131,   210,
     119,     3,    72,    73,   215,    36,    76,     4,   126,   128,
      35,    37,   133,    15,   133,   533,   348,    48,   350,    89,
       4,   142,   525,   150,     3,   144,    96,   359,   160,    37,
       3,     4,     3,   169,    36,   154,    15,     3,    11,     3,
      37,   159,     3,    33,    15,   277,   162,   163,   118,    33,
     488,    15,   122,   171,    15,    37,    35,    36,    37,     0,
      42,   232,    33,    36,   134,    36,    37,   237,   239,    31,
     402,    35,    36,    37,    33,    36,    37,    35,    35,   517,
     150,    38,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    17,    18,    19,    20,
      21,    22,    23,    24,    25,    26,    27,    28,    29,   230,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
      41,    42,    43,    44,    45,    46,    47,    48,    49,    50,
      51,    52,    31,     3,     4,     3,     4,     5,     6,     7,
      26,   473,    12,    11,    33,    35,     3,     4,    38,     4,
      20,    21,     9,     4,    24,    12,    26,    27,    15,    29,
      46,   345,    48,    20,    21,   301,    36,    31,    36,    26,
      27,    31,    29,    33,   358,     3,     4,   317,    48,    36,
       8,   317,    42,    37,   320,   321,     3,     4,    33,    31,
       3,    33,    34,   433,    31,   316,    33,   449,    32,   315,
      42,    35,   323,   455,     5,     6,     7,     4,    36,    43,
       9,    27,   331,    29,     3,    28,     3,     4,   429,    36,
     452,   357,    35,    36,    37,    38,    33,    10,    35,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
       3,     4,     3,     4,    32,     3,     4,    32,    33,    36,
     502,    12,    31,    32,    12,    16,     3,     4,     3,    20,
      21,    43,    20,    21,    31,    26,    27,    20,    29,    27,
     522,    29,    34,    36,   410,    36,    35,   410,    36,    35,
     532,   465,    43,    35,   420,   537,     3,     4,    27,    36,
      29,   543,     3,     4,     3,     4,    31,   416,   445,    33,
     448,    12,   450,    14,    15,    16,    39,    44,     3,    20,
      21,    41,    23,    45,   433,    26,    27,   501,    29,    36,
      42,     4,   441,   442,   443,    36,   445,    36,    17,    31,
      32,   487,    34,    35,    39,   405,     3,     4,    37,    33,
      42,    43,    33,    40,   491,    12,   493,    14,    15,    35,
      40,   487,    32,    20,    21,    31,    23,    32,    40,    26,
      27,    33,    29,   499,    41,    31,   499,    31,   487,    36,
      31,    39,   491,    43,   493,   511,    39,    31,   511,    33,
      34,    32,    33,    40,    35,   504,    41,    38,    42,    39,
      31,    42,    33,    34,    35,    41,    39,   533,    37,     3,
     533,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    39,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    32,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    39,    31,
      32,    33,    34,    35,    36,    37,    38,    39,    40,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    43,    33,    32,    21,
      22,     3,    40,    33,    26,    37,    28,    29,    34,    34,
      32,    33,    34,    35,    36,    37,    38,    34,    39,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,     3,     4,    37,    33,     3,    33,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    33,    39,    37,    21,
      22,    33,    33,    39,    26,    39,    28,    29,   431,    31,
      39,   442,    34,    35,    36,    37,    38,   308,    40,   317,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,     3,     4,   421,   328,   412,   352,     9,    10,    11,
      12,    13,    14,    15,    16,    17,   242,   410,   247,    21,
      22,   400,    -1,    -1,    26,    -1,    28,    29,    -1,    31,
      -1,    -1,    34,    35,    36,    37,    38,    -1,    -1,    41,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,     3,     4,    -1,    -1,    -1,    -1,     9,    10,    11,
      12,    13,    14,    15,    16,    17,    -1,    -1,    -1,    21,
      22,    -1,    -1,    -1,    26,    -1,    28,    29,    -1,    31,
      -1,    -1,    34,    35,    36,    37,    38,    -1,    -1,    -1,
      42,    43,    44,    45,    46,    47,    48,    49,    50,    51,
      52,     0,     1,    -1,     3,     4,    -1,    -1,    -1,     8,
      -1,    -1,    -1,    12,    13,    14,    15,    16,    -1,    -1,
      -1,    20,    21,    22,    23,    -1,    25,    26,    27,    -1,
      29,    30,    -1,    -1,    33,    -1,    -1,    36,    -1,    -1,
       1,    40,     3,     4,     5,     6,     7,     8,    -1,    -1,
      11,    12,    13,    14,    15,    16,    -1,    -1,    -1,    20,
      21,    22,    23,    -1,    25,    26,    27,    -1,    29,    30,
      -1,    -1,    33,    -1,    -1,    36,    -1,    -1,     1,    40,
       3,     4,     5,     6,     7,     8,    -1,    -1,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    20,    21,    22,
      23,    -1,    25,    26,    27,    -1,    29,    30,    -1,    -1,
      33,    -1,    -1,    36,    -1,    -1,     1,    40,     3,     4,
       5,     6,     7,     8,    -1,    -1,    11,    12,    13,    14,
      15,    16,    -1,    -1,    -1,    20,    21,    22,    23,    -1,
      25,    26,    27,    -1,    29,    30,    -1,     1,    33,     3,
       4,    36,    -1,    -1,     8,    40,    -1,    -1,    12,    13,
      14,    15,    16,    -1,    -1,    -1,    20,    21,    22,    23,
      -1,    25,    26,    27,    -1,    29,    30,     0,     1,    33,
       3,     4,    36,    -1,    -1,     8,    40,    -1,    -1,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    20,    21,    22,
      23,    -1,    25,    26,    27,    -1,    29,    30,     3,     4,
      33,    -1,    -1,    36,    -1,    -1,    -1,    12,    -1,    -1,
      15,    -1,    -1,    -1,    -1,    20,    21,    -1,    23,     3,
       4,    26,    27,    -1,    29,    -1,    10,    -1,    12,    -1,
      -1,    36,     3,     4,    -1,    -1,    20,    21,    -1,    -1,
      -1,    12,    26,    27,    15,    29,    -1,    -1,    -1,    20,
      21,    -1,    36,     3,     4,    26,    27,    -1,    29,    -1,
      -1,    -1,    12,    -1,    -1,    36,     3,     4,    -1,    -1,
      20,    21,    -1,    10,    24,    12,    26,    27,    -1,    29,
      -1,    -1,    -1,    20,    21,    -1,    36,     3,     4,    26,
      27,    -1,    29,    -1,    -1,    -1,    12,    -1,    -1,    36,
      16,    -1,    -1,    -1,    20,    21,     3,     4,    -1,    -1,
      26,    27,    -1,    29,    -1,    12,    -1,     3,     4,    -1,
      36,    -1,    -1,    20,    21,    -1,    12,    -1,    -1,    26,
      27,    -1,    29,    -1,    20,    21,     3,     4,    -1,    36,
      26,    27,    -1,    29,    -1,    12,    -1,     3,     4,    -1,
      36,    -1,    -1,    20,    21,    -1,    12,    -1,    -1,    26,
      27,    -1,    29,    -1,    20,    21,     3,     4,    -1,    36,
      26,    27,    -1,    29,    -1,    12,    -1,    -1,    -1,    -1,
      36,    -1,    -1,    20,    21,    -1,    -1,    -1,    -1,    26,
      27,    -1,    29,    -1,    -1,    -1,    -1,    -1,    -1,    36
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const unsigned char yystos[] =
{
       0,     1,     3,     4,     8,    12,    13,    14,    15,    16,
      20,    21,    22,    23,    25,    26,    27,    29,    30,    33,
      36,    54,    55,    56,    57,    58,    67,    68,    69,    70,
      71,    72,    75,    90,    91,   102,   103,   104,   108,   115,
      31,    33,    42,    67,     3,    65,     3,    65,    68,    12,
      20,    26,    68,    90,   115,    12,    20,    23,    68,    90,
     103,   108,    90,    42,     3,    68,     3,     4,    63,    64,
      66,    67,     4,     8,    64,     9,    90,   108,    37,    20,
      26,    91,    27,    29,    96,    96,     3,     0,    57,     4,
      33,    96,    33,    14,    20,    75,    90,   102,     3,    15,
      35,    37,    67,    93,    95,   107,    31,    33,    34,    35,
     117,   118,    33,   119,    10,    73,    74,    90,    35,    31,
      33,    31,     4,    96,     3,    65,    65,     3,    37,    33,
      33,    37,   107,    20,    21,    43,    75,    76,    77,    78,
      79,    80,    90,    33,    34,    81,    96,    63,    64,    96,
       4,    63,    64,    66,    31,    93,    33,     9,    62,    65,
      26,    91,    27,    29,    96,    96,    26,    46,    48,    15,
      67,     3,   102,    35,    38,    94,     3,    28,    35,    36,
      37,    38,    42,    43,    44,    45,    46,    47,    48,    49,
      50,    51,    52,   105,     3,     4,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    21,    22,    26,    28,    29,
      31,    34,    35,    36,    37,    38,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    88,    89,    24,
      48,    68,    90,   112,   113,   114,   115,     4,   107,    32,
      33,    17,    26,   109,   116,   119,     3,    97,    98,    10,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    28,    29,    31,    32,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,   120,
      43,    32,    66,    56,     3,    59,    60,    61,    31,    81,
      33,    38,    68,   116,    65,    66,    20,    32,    43,    35,
      35,    35,     3,    37,    65,     5,     6,     7,    11,    68,
      82,    83,    84,    31,    63,    64,    33,    56,    33,     9,
      39,    81,    91,    96,    96,    90,    33,    81,    88,    87,
      88,    35,    38,    39,    41,    44,    45,    42,   106,    88,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    21,    22,    26,    28,    29,
      32,    33,    34,    35,    36,    37,    38,    41,    42,    43,
      44,    45,    46,    47,    48,    49,    50,    51,    52,    99,
     100,   101,    88,    89,    65,     4,     3,    95,    39,    24,
      32,     3,    15,    63,   116,    95,    37,   109,    40,    37,
      42,    32,   117,    33,   119,    10,    90,    33,    40,    35,
      40,    32,    59,    31,    89,    65,    80,    89,    90,    90,
      65,    83,    32,    11,    68,    85,    40,    33,    31,   107,
      31,    41,    31,    87,    73,   107,    40,    99,    39,   101,
      41,    39,    48,   114,   105,   106,    68,   110,   111,    99,
      73,    98,    40,    88,    92,    93,    61,    40,    56,    41,
      39,    68,    84,    68,     5,     6,     7,    11,    40,    57,
      86,    85,   116,    85,   119,    41,    43,   116,    39,    37,
       3,   106,   107,    39,    32,    39,    43,    33,     3,    40,
      33,    37,    34,    34,    34,   102,    92,    40,    40,    40,
     112,    39,   107,   116,    68,    37,    33,     3,   112,    33,
      92,    33,    39,    37,   116,    99,    33,    39,    33,   116,
     112,    39,   116,    39,   116
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (0)


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (N)								\
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (0)
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
              (Loc).first_line, (Loc).first_column,	\
              (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (0)

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)		\
do {								\
  if (yydebug)							\
    {								\
      YYFPRINTF (stderr, "%s ", Title);				\
      yysymprint (stderr,					\
                  Type, Value);	\
      YYFPRINTF (stderr, "\n");					\
    }								\
} while (0)

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_stack_print (short int *bottom, short int *top)
#else
static void
yy_stack_print (bottom, top)
    short int *bottom;
    short int *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (/* Nothing. */; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yy_reduce_print (int yyrule)
#else
static void
yy_reduce_print (yyrule)
    int yyrule;
#endif
{
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu), ",
             yyrule - 1, yylno);
  /* Print the symbols being reduced, and their result.  */
  for (yyi = yyprhs[yyrule]; 0 <= yyrhs[yyi]; yyi++)
    YYFPRINTF (stderr, "%s ", yytname[yyrhs[yyi]]);
  YYFPRINTF (stderr, "-> %s\n", yytname[yyr1[yyrule]]);
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (Rule);		\
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined (__GLIBC__) && defined (_STRING_H)
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
#   if defined (__STDC__) || defined (__cplusplus)
yystrlen (const char *yystr)
#   else
yystrlen (yystr)
     const char *yystr;
#   endif
{
  const char *yys = yystr;

  while (*yys++ != '\0')
    continue;

  return yys - yystr - 1;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined (__GLIBC__) && defined (_STRING_H) && defined (_GNU_SOURCE)
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
#   if defined (__STDC__) || defined (__cplusplus)
yystpcpy (char *yydest, const char *yysrc)
#   else
yystpcpy (yydest, yysrc)
     char *yydest;
     const char *yysrc;
#   endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      size_t yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

#endif /* YYERROR_VERBOSE */



#if YYDEBUG
/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yysymprint (FILE *yyoutput, int yytype, YYSTYPE *yyvaluep)
#else
static void
yysymprint (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);


# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  switch (yytype)
    {
      default:
        break;
    }
  YYFPRINTF (yyoutput, ")");
}

#endif /* ! YYDEBUG */
/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

#if defined (__STDC__) || defined (__cplusplus)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  /* Pacify ``unused variable'' warnings.  */
  (void) yyvaluep;

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
        break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM);
# else
int yyparse ();
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
# if defined (__STDC__) || defined (__cplusplus)
int yyparse (void *YYPARSE_PARAM)
# else
int yyparse (YYPARSE_PARAM)
  void *YYPARSE_PARAM;
# endif
#else /* ! YYPARSE_PARAM */
#if defined (__STDC__) || defined (__cplusplus)
int
yyparse (void)
#else
int
yyparse ()
    ;
#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  short int yyssa[YYINITDEPTH];
  short int *yyss = yyssa;
  short int *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK   (yyvsp--, yyssp--)

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* When reducing, the number of symbols on the RHS of the reduced
     rule.  */
  int yylen;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed. so pushing a state here evens the stacks.
     */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack. Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	short int *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	short int *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

/* Do appropriate processing given the current state.  */
/* Read a look-ahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to look-ahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;


  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  yystate = yyn;
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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 79 "cxx.y"
    { results = (yyvsp[0]); ;}
    break;

  case 3:
#line 84 "cxx.y"
    { ScopeC ol(STR(extdefs)); (yyval)=ol; ol.Append((yyvsp[0]));  ;}
    break;

  case 4:
#line 85 "cxx.y"
    { ObjectListC ol((yyvsp[-1])); (yyval)=(yyvsp[-1]); ol.Append((yyvsp[0]));  ;}
    break;

  case 5:
#line 88 "cxx.y"
    { ScopeC ol(STR(extdefs)); (yyval)=ol; ;}
    break;

  case 6:
#line 89 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 7:
#line 92 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 8:
#line 93 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 9:
#line 94 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 10:
#line 95 "cxx.y"
    { (yyval)=ObjectC(); ;}
    break;

  case 11:
#line 96 "cxx.y"
    { 
                                                           StringC name;
                                                           if((yyvsp[-3]).IsValid())
                                                             name = (yyvsp[-3]).Name();
                                                           (yyval)=ScopeC(name,(yyvsp[-1])); 
							   (yyval).UpdateComment((yyvsp[-4]).Comment());
    	                                                   (yyval).SetVar(STR(dectype),STR(definition));
                                                           (yyval).SetVar(STR(storageType),STR(namespace));
	                                                 ;}
    break;

  case 12:
#line 105 "cxx.y"
    { (yyval)=ObjectC(); ;}
    break;

  case 13:
#line 106 "cxx.y"
    { (yyval)=ObjectC(); ;}
    break;

  case 14:
#line 107 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 15:
#line 108 "cxx.y"
    { (yyval)=ObjectC(); ;}
    break;

  case 16:
#line 109 "cxx.y"
    { (yyval)=ObjectC(); ;}
    break;

  case 17:
#line 110 "cxx.y"
    { (yyval)=ObjectC(); ;}
    break;

  case 18:
#line 111 "cxx.y"
    { 
	                     (yyval)=DocNodeC(StringC("anon?") + StringC(anonCount++)); 
          		     hookCommentObj = (yyval);
                           ;}
    break;

  case 19:
#line 118 "cxx.y"
    { ObjectListC ol((yyvsp[-1]));
						   ObjectListC ret(STR(varList),true);
						   for(DLIterC<ObjectC> it(ol.List());it.IsElm();it.Next()) {
						     VariableC avar((yyvsp[-2]),it.Data().Name());
						     ret.Append(avar);
						   }
          				           hookCommentObj = ret; 
                                                   (yyval).SetupLineNos((yyvsp[-2]),(yyvsp[0]));
						   (yyval)=ret;
                                                 ;}
    break;

  case 20:
#line 128 "cxx.y"
    { (yyval)=TypedefC((yyvsp[-1]).Name(),(yyvsp[-2]));
                                                   (yyval).SetupLineNos((yyvsp[-3]),(yyvsp[0]));
          				           hookCommentObj = (yyval); 
                                                 ;}
    break;

  case 21:
#line 133 "cxx.y"
    { (yyvsp[-5]).SetVar(STR(array),STR(1)); 
                                                               (yyval)=TypedefC((yyvsp[-4]).Name(),(yyvsp[-5])); 
          				                       hookCommentObj = (yyval); 
                                                               (yyval).SetupLineNos((yyvsp[-6]),(yyvsp[0]));
                                                             ;}
    break;

  case 22:
#line 138 "cxx.y"
    { DataTypeC dt((yyvsp[-1]));
                                                               (yyval)=TypedefC(dt.Alias(),dt);
                                                               (yyval).SetupLineNos((yyvsp[-2]),(yyvsp[0]));
          				                       hookCommentObj = (yyval); 
                                                             ;}
    break;

  case 23:
#line 144 "cxx.y"
    { 
							       StringC className;
							       if((yyvsp[-6]).IsValid())
								 className = (yyvsp[-6]).Name();
							       else 
								 className = StringC("anon?") + StringC(anonCount++);
							       ClassC aclass(className,(yyvsp[-3]),(yyvsp[-5]));
                                                               aclass.SetVar(STR(dectype),STR(definition));
                                                               aclass.SetVar(STR(storageType),(yyvsp[-7]).Name());
							       TypedefC td((yyvsp[-2]).Name(),DataTypeC(className));
							       td.SetDefinition(aclass);
							       ObjectListC ret(STR(x),true);
							       ret.Append(td);
							       if((yyvsp[-6]).IsValid())
								 ret.Append(aclass);
                                                               (yyval).SetupLineNos((yyvsp[-8]),(yyvsp[0]));
							       (yyval) = ret;
          				                       hookCommentObj = (yyval);
							     ;}
    break;

  case 24:
#line 163 "cxx.y"
    { (yyval)=ObjectC(); ;}
    break;

  case 25:
#line 164 "cxx.y"
    { 
							       StringC enumName;
							       if((yyvsp[-5]).IsValid())
								 enumName = (yyvsp[-5]).Name();
							       else 
								 enumName = StringC("anon_enum?") + StringC(anonCount++);
                                                               EnumC anEnum(enumName,(yyvsp[-3]));
                                                               anEnum.SetVar(STR(dectype),STR(definition));
                                                               anEnum.SetVar(STR(storageType),STR(enum));
							       TypedefC td((yyvsp[-1]).Name(),DataTypeC(enumName));
							       td.SetDefinition(anEnum);
							       ObjectListC ret("x",true);
							       ret.Append(td);
							       if((yyvsp[-5]).IsValid())
								 ret.Append(anEnum);
                                                               (yyval).SetupLineNos((yyvsp[-7]),(yyvsp[0]));
							       (yyval) = ret;
          				                       hookCommentObj = (yyval);
                                                             ;}
    break;

  case 26:
#line 183 "cxx.y"
    {
                                                  if((yyvsp[-5]).IsValid())
						    (yyval)=EnumC((yyvsp[-5]).Name(),(yyvsp[-3]));
						  else
						    (yyval)=EnumC(StringC("anon_enum?") + StringC(anonCount++),(yyvsp[-3]));
						  hookCommentObj = (yyval);
                                                ;}
    break;

  case 27:
#line 190 "cxx.y"
    { (yyval)=ClassC((yyvsp[-1]).Name());
						  (yyval).UpdateComment((yyvsp[-2]).Comment()); 
						  (yyval).SetVar(STR(storageType),(yyvsp[-2]).Name());
                                                  (yyval).SetVar(STR(dectype),STR(forward));
                                                ;}
    break;

  case 28:
#line 195 "cxx.y"
    { (yyval)=EnumC((yyvsp[-1]).Name());
						  (yyval).UpdateComment((yyvsp[-2]).Comment()); 
						  (yyval).SetVar(STR(storageType),STR(enum));
                                                  (yyval).SetVar(STR(dectype),STR(forward));
                                                ;}
    break;

  case 29:
#line 200 "cxx.y"
    { (yyval)=(yyvsp[-1]); 
                                                  (yyval).SetVar(STR(dectype),STR(friend));  
                                                ;}
    break;

  case 30:
#line 204 "cxx.y"
    { 
						  ClassC newClass((yyvsp[-6]).Name(),(yyvsp[-3]),(yyvsp[-5]));
						  (yyval) = newClass;
						  (yyval).UpdateComment((yyvsp[-7]).Comment()); 
						  (yyval).SetupLineNos((yyvsp[-7]),(yyvsp[0]));
						  newClass.SetVar(STR(dectype),STR(definition));
						  newClass.SetVar(STR(storageType),(yyvsp[-7]).Name());
						;}
    break;

  case 31:
#line 212 "cxx.y"
    {
                                                  StringC nname;
						  if((yyvsp[-6]).IsValid())
						    nname = (yyvsp[-6]).Name();
						  else
						    nname = StringC("anon_struct?") + StringC(anonCount++);
						  ClassC newClass(nname,(yyvsp[-3]),(yyvsp[-5]));
						  (yyval) = newClass;
						  (yyval).UpdateComment((yyvsp[-8]).Comment()); 
						  (yyval).SetupLineNos((yyvsp[-8]),(yyvsp[-1]));
						  newClass.SetVar(STR(dectype),STR(definition));
						  newClass.SetVar(STR(storageType),(yyvsp[-7]).Name());
                                                ;}
    break;

  case 32:
#line 225 "cxx.y"
    { (yyval)=ObjectC(); ;}
    break;

  case 33:
#line 226 "cxx.y"
    { (yyval) = (yyvsp[-1]); ObjectListC ol((yyvsp[-1])); ol.SetWrapper(true); ;}
    break;

  case 34:
#line 227 "cxx.y"
    { (yyval)=ObjectC(); ;}
    break;

  case 35:
#line 231 "cxx.y"
    { ObjectListC ol(STR(EnumNameList)); (yyval)=ol; ;}
    break;

  case 36:
#line 232 "cxx.y"
    { (yyval)=(yyvsp[0]);}
    break;

  case 37:
#line 234 "cxx.y"
    { ObjectListC ol(STR(VarNameList)); ol.Append((yyvsp[0])); (yyval) = ol;  ;}
    break;

  case 38:
#line 235 "cxx.y"
    { ObjectListC ol((yyvsp[-2])); ol.Append((yyvsp[0])); (yyval) = ol; ;}
    break;

  case 39:
#line 237 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 40:
#line 238 "cxx.y"
    { (yyval) = (yyvsp[-2]).Name() +  StringC(" = ") + (yyvsp[0]).Name(); ;}
    break;

  case 43:
#line 250 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 44:
#line 253 "cxx.y"
    { (yyval) = (yyvsp[-2]).Name() + strp_ColonColon + (yyvsp[0]).Name(); ;}
    break;

  case 45:
#line 254 "cxx.y"
    { (yyval) = (yyvsp[-2]).Name() + strp_ColonColon + (yyvsp[0]).Name(); ;}
    break;

  case 46:
#line 257 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 47:
#line 258 "cxx.y"
    { (yyval)=ObjectC(); ;}
    break;

  case 48:
#line 262 "cxx.y"
    { (yyval) = (yyvsp[0]); ;}
    break;

  case 49:
#line 263 "cxx.y"
    { (yyval) = (yyvsp[0]); ;}
    break;

  case 50:
#line 264 "cxx.y"
    { (yyval)=ObjectC(strp_ColonColon + (yyvsp[0]).Name()); ;}
    break;

  case 51:
#line 265 "cxx.y"
    { (yyval)=ObjectC(strp_ColonColon + (yyvsp[0]).Name()); ;}
    break;

  case 52:
#line 270 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 53:
#line 271 "cxx.y"
    { (yyval) = ObjectC(StringC("~") + (yyvsp[0]).Name()); (yyval).CopyLineNo((yyvsp[0])); ;}
    break;

  case 54:
#line 272 "cxx.y"
    { (yyval) = ObjectTemplateC((yyvsp[-3]).Name(),(yyvsp[-1])); ;}
    break;

  case 55:
#line 276 "cxx.y"
    { ObjectListC ol((yyvsp[0]).Name()); ol.Append((yyvsp[0])); (yyval)=ol; (yyval).CopyLineNo((yyvsp[0])); ;}
    break;

  case 56:
#line 277 "cxx.y"
    { ObjectListC ol((yyvsp[0]).Name()); 
                                       ObjectC newun(strp_ColonColon);
                                       ol.Append(newun);
				       ol.Append((yyvsp[0]));
				       (yyval) = ol;
                                     ;}
    break;

  case 57:
#line 283 "cxx.y"
    { ObjectListC ol((yyvsp[-2])); 
				       ol.Append((yyvsp[0]));
				       ol.Name() = (yyvsp[0]).Name();  // Give name of last component to list.
				       (yyval) = ol;
                                     ;}
    break;

  case 59:
#line 297 "cxx.y"
    {
	    InheritC inh(SAPublic,true);
	    inh.SetScopeDef((yyvsp[-1]));  
	    (yyval) = inh;
	  ;}
    break;

  case 63:
#line 317 "cxx.y"
    { (yyval) =  ClassTemplateC((yyvsp[-5]).Name(),(yyvsp[-2]),(yyvsp[-4]),(yyvsp[-7]));
						  (yyval).SetVar(STR(dectype),STR(definition));
						  (yyval).SetVar(STR(storageType),(yyvsp[-6]).Name());
						  (yyval).UpdateComment((yyvsp[-7]).Comment());
						;}
    break;

  case 64:
#line 322 "cxx.y"
    { (yyval) = ClassTemplateC((yyvsp[-1]).Name(),(yyvsp[-3])); 
						    (yyval).SetVar(STR(dectype),STR(forward));
						    (yyval).SetVar(STR(storageType),(yyvsp[-2]).Name());
						    (yyval).UpdateComment((yyvsp[-3]).Comment());
                                                  ;}
    break;

  case 65:
#line 327 "cxx.y"
    { (yyval) = (yyvsp[0]);
                                                    ObjectListC ol1((yyvsp[-1]));
						    (yyval).UpdateComment((yyvsp[-1]).Comment());
                                                  ;}
    break;

  case 66:
#line 331 "cxx.y"
    { (yyval) = (yyvsp[0]);
                                                    ObjectListC ol1((yyvsp[-2]));
                                                    ObjectListC ol2((yyvsp[-1]));
						    (yyval).UpdateComment((yyvsp[-2]).Comment());
                                                  ;}
    break;

  case 67:
#line 341 "cxx.y"
    { (yyval)=ObjectListC(STR(TemplateInstArgs)); ;}
    break;

  case 68:
#line 342 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 69:
#line 345 "cxx.y"
    { ObjectListC ol(STR(TemplateInstArgs)); (yyval)=ol; ol.Append((yyvsp[0])); ;}
    break;

  case 70:
#line 346 "cxx.y"
    { ObjectListC ol(STR(TemplateInstArgs)); (yyval)=ol; ol.Append((yyvsp[0])); ;}
    break;

  case 71:
#line 347 "cxx.y"
    { ObjectListC ol((yyvsp[-2])); (yyval)=(yyvsp[-2]); ol.Append((yyvsp[0])); ;}
    break;

  case 72:
#line 348 "cxx.y"
    { ObjectListC ol((yyvsp[-2])); (yyval)=(yyvsp[-2]); ol.Append((yyvsp[0])); ;}
    break;

  case 73:
#line 352 "cxx.y"
    { (yyval)=(yyvsp[-1]); (yyval).UpdateComment((yyvsp[-3]).Comment()); ;}
    break;

  case 74:
#line 353 "cxx.y"
    { (yyval)=ObjectListC(STR(TemplateDefArgs)); (yyval).UpdateComment((yyvsp[-2]).Comment()); ;}
    break;

  case 75:
#line 356 "cxx.y"
    { ObjectListC ol(STR(TemplateDefArgs)); (yyval)=ol; ol.Append((yyvsp[0])); ;}
    break;

  case 76:
#line 357 "cxx.y"
    { (yyval)=(yyvsp[-2]); ObjectListC ol((yyvsp[-2])); ol.Append((yyvsp[0])); ;}
    break;

  case 77:
#line 360 "cxx.y"
    { (yyval)=(yyvsp[-1]); 
                                      if((yyvsp[0]).IsValid()) { 
                                        DataTypeC dt((yyvsp[-1])); 
					dt.SetAlias((yyvsp[0]).Name()); 
                                      } 
                                    ;}
    break;

  case 78:
#line 369 "cxx.y"
    { (yyval)= DataTypeC(STR(class),(yyvsp[0])); ;}
    break;

  case 79:
#line 370 "cxx.y"
    { (yyval)= DataTypeC(STR(typename),(yyvsp[0]));  ;}
    break;

  case 80:
#line 371 "cxx.y"
    { (yyval)= DataTypeC((yyvsp[-7]).Name() + " (" + (yyvsp[-5]).Name() + ")(" + (yyvsp[-2]).Name() + ")" + (yyvsp[-1]).Name(),(yyvsp[-5])); ;}
    break;

  case 81:
#line 375 "cxx.y"
    { (yyval)=ObjectC("template<> class " + (yyvsp[-2]).Name()); ;}
    break;

  case 82:
#line 386 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 83:
#line 387 "cxx.y"
    { (yyval)=(yyvsp[-2]); ;}
    break;

  case 84:
#line 388 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 85:
#line 389 "cxx.y"
    { (yyval)=(yyvsp[-2]); ;}
    break;

  case 86:
#line 390 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 87:
#line 391 "cxx.y"
    { (yyval)=(yyvsp[-2]); ;}
    break;

  case 88:
#line 404 "cxx.y"
    { (yyval)=ObjectListC(STR(classInherit)); ;}
    break;

  case 89:
#line 405 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 90:
#line 408 "cxx.y"
    { (yyval) = (yyvsp[-2]); ObjectListC ol((yyvsp[-2])); ol.Append((yyvsp[0]));  ;}
    break;

  case 91:
#line 409 "cxx.y"
    { ObjectListC ol(STR(classInherit),true); ol.Append((yyvsp[0])); (yyval) = ol; ;}
    break;

  case 92:
#line 411 "cxx.y"
    { (yyval) = InheritC(SAPublic);    (yyval).CopyLineNo((yyvsp[0])); ;}
    break;

  case 93:
#line 412 "cxx.y"
    { (yyval) = InheritC(SAPrivate);   (yyval).CopyLineNo((yyvsp[0])); ;}
    break;

  case 94:
#line 413 "cxx.y"
    { (yyval) = InheritC(SAProtected); (yyval).CopyLineNo((yyvsp[0])); ;}
    break;

  case 95:
#line 415 "cxx.y"
    { (yyval) = (yyvsp[-1]);
							  InheritC inh((yyval));
							  inh.SetScopeDef((yyvsp[0]));
							;}
    break;

  case 96:
#line 419 "cxx.y"
    { (yyval) = (yyvsp[-1]);
                                                          InheritC inh((yyval));
							  inh.SetScopeDef((yyvsp[0]));
							  inh.SetVirtual(true);
							;}
    break;

  case 97:
#line 424 "cxx.y"
    { (yyval) = (yyvsp[-2]);
                                                          InheritC inh((yyval));
							  inh.SetScopeDef((yyvsp[0]));
							  inh.SetVirtual(true);
							;}
    break;

  case 98:
#line 429 "cxx.y"
    { (yyval) = InheritC(SAPrivate);
                                                          InheritC inh((yyval));
							  inh.SetScopeDef((yyvsp[0]));
							;}
    break;

  case 99:
#line 435 "cxx.y"
    { (yyval) = ObjectListC(STR(ClassContents)); ;}
    break;

  case 100:
#line 436 "cxx.y"
    { ObjectListC ol((yyvsp[-1])); ol.Append((yyvsp[0])); (yyval) = ol; ;}
    break;

  case 101:
#line 438 "cxx.y"
    { (yyval) = MarkerC(STR(protected)); (yyval).SetupLineNos((yyvsp[-1]),(yyvsp[0])); ;}
    break;

  case 102:
#line 439 "cxx.y"
    { (yyval) = MarkerC(STR(private));   (yyval).SetupLineNos((yyvsp[-1]),(yyvsp[0])); ;}
    break;

  case 103:
#line 440 "cxx.y"
    { (yyval) = MarkerC(STR(public));    (yyval).SetupLineNos((yyvsp[-1]),(yyvsp[0])); ;}
    break;

  case 104:
#line 441 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 105:
#line 442 "cxx.y"
    { (yyval)=(yyvsp[0]); (yyval).SetVar(STR(virtual),STR(1)); (yyval).IncludeLineNo((yyvsp[-1])) ;}
    break;

  case 106:
#line 446 "cxx.y"
    { (yyval)=ObjectC(""); ;}
    break;

  case 107:
#line 447 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 108:
#line 449 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 109:
#line 450 "cxx.y"
    { (yyval) = (yyvsp[-1]).Name() + ' ' +  (yyvsp[0]).Name(); ;}
    break;

  case 110:
#line 452 "cxx.y"
    { (yyval)=ObjectC(strp_OpenBracket + (yyvsp[-1]).Name() + strp_CloseBracket); ;}
    break;

  case 111:
#line 453 "cxx.y"
    { (yyval)=ObjectC(StringC("[") + (yyvsp[-1]).Name() + "]"); ;}
    break;

  case 112:
#line 454 "cxx.y"
    { (yyval)=ObjectC(StringC("{") + (yyvsp[-1]).Name() + "}"); ;}
    break;

  case 119:
#line 456 "cxx.y"
    { (yyval)=ObjectC("<"); ;}
    break;

  case 120:
#line 457 "cxx.y"
    { (yyval)=ObjectC(">"); ;}
    break;

  case 121:
#line 458 "cxx.y"
    { (yyval)=ObjectC(strp_equals); ;}
    break;

  case 122:
#line 459 "cxx.y"
    { (yyval)=ObjectC(strp_plus); ;}
    break;

  case 123:
#line 460 "cxx.y"
    { (yyval)=ObjectC("-"); ;}
    break;

  case 124:
#line 461 "cxx.y"
    { (yyval)=ObjectC("~"); ;}
    break;

  case 125:
#line 462 "cxx.y"
    { (yyval)=ObjectC(strp_ampersand); ;}
    break;

  case 126:
#line 463 "cxx.y"
    { (yyval)=ObjectC("/"); ;}
    break;

  case 127:
#line 464 "cxx.y"
    { (yyval)=ObjectC(strp_asterisk); ;}
    break;

  case 128:
#line 465 "cxx.y"
    { (yyval)=ObjectC("%"); ;}
    break;

  case 129:
#line 466 "cxx.y"
    { (yyval)=ObjectC("|"); ;}
    break;

  case 130:
#line 467 "cxx.y"
    { (yyval)=ObjectC("^"); ;}
    break;

  case 131:
#line 468 "cxx.y"
    { (yyval)=ObjectC("!"); ;}
    break;

  case 132:
#line 469 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 133:
#line 470 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 134:
#line 471 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 135:
#line 472 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 136:
#line 473 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 137:
#line 474 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 138:
#line 475 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 139:
#line 476 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 140:
#line 477 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 141:
#line 478 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 142:
#line 479 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 143:
#line 483 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 144:
#line 484 "cxx.y"
    { (yyval)=(yyvsp[0]); DataTypeC dt((yyvsp[0])); dt.SetPreQual((yyvsp[-1]).Name()); ;}
    break;

  case 145:
#line 485 "cxx.y"
    { (yyval)=(yyvsp[0]); DataTypeC dt((yyvsp[0])); dt.SetPreQual((yyvsp[-2]).Name() + " " + (yyvsp[-1]).Name()); ;}
    break;

  case 146:
#line 486 "cxx.y"
    { (yyval)=(yyvsp[0]); DataTypeC dt((yyvsp[0])); dt.SetPreQual((yyvsp[-3]).Name() + " " + (yyvsp[-2]).Name() + " " + (yyvsp[-1]).Name()); ;}
    break;

  case 147:
#line 489 "cxx.y"
    { (yyval)=DataTypeC(StringC(""),(yyvsp[-1]),(yyvsp[0]).Name()); 
	                                           (yyval).SetupLineNos((yyvsp[-1]),(yyvsp[0])); 
                                                 ;}
    break;

  case 148:
#line 492 "cxx.y"
    { (yyval)=DataTypeC((yyvsp[-2]).Name(),(yyvsp[-1]),(yyvsp[0]).Name()); 
	                                           (yyval).SetVar(STR(storageType),(yyvsp[-2]).Name());
	                                           (yyval).SetupLineNos((yyvsp[-2]),(yyvsp[-1]),(yyvsp[0])); 
	                                         ;}
    break;

  case 149:
#line 496 "cxx.y"
    { (yyval)=DataTypeC((yyvsp[-2]).Name(),(yyvsp[-1]),(yyvsp[0]).Name()); 
	                                           (yyval).SetVar(STR(storageType),(yyvsp[-2]).Name());
	                                           (yyval).SetupLineNos((yyvsp[-2]),(yyvsp[-1]),(yyvsp[0])); 
	                                         ;}
    break;

  case 150:
#line 500 "cxx.y"
    { (yyval)=DataTypeC(StringC(""),(yyvsp[-1]),(yyvsp[0]).Name()); 
	                                           (yyval).SetupLineNos((yyvsp[-1]),(yyvsp[0]));
	                                         ;}
    break;

  case 151:
#line 503 "cxx.y"
    { (yyval)=DataTypeC(StringC(""),(yyvsp[-1]), (yyvsp[0]).Name()); 
                                                   (yyval).SetupLineNos((yyvsp[-1]),(yyvsp[0])); 
                                                 ;}
    break;

  case 152:
#line 506 "cxx.y"
    { (yyval)=DataTypeC((yyvsp[-2]).Name(),(yyvsp[-1]),(yyvsp[0]).Name()); 
	                                           (yyval).SetupLineNos((yyvsp[-2]),(yyvsp[-1]),(yyvsp[0])); 
	                                         ;}
    break;

  case 153:
#line 509 "cxx.y"
    { (yyval)=DataTypeC((yyvsp[-2]).Name(),(yyvsp[-1]),(yyvsp[0]).Name()); 
	                                           (yyval).SetupLineNos((yyvsp[-2]),(yyvsp[-1]),(yyvsp[0])); 
	                                         ;}
    break;

  case 154:
#line 512 "cxx.y"
    { (yyval)=DataTypeC((yyvsp[-3]).Name() + " " + (yyvsp[-2]).Name(),(yyvsp[-1]),(yyvsp[0]).Name()); 
	                                           (yyval).SetupLineNos((yyvsp[-3]),(yyvsp[-1]),(yyvsp[0])); 
	                                         ;}
    break;

  case 155:
#line 515 "cxx.y"
    { (yyval)=DataTypeC((yyvsp[-3]).Name() + " " + (yyvsp[-2]).Name(),(yyvsp[-1]),(yyvsp[0]).Name()); 
	                                           (yyval).SetupLineNos((yyvsp[-3]),(yyvsp[-1]),(yyvsp[0]));
	                                         ;}
    break;

  case 156:
#line 518 "cxx.y"
    { (yyval)= DataTypeC(STR(typename),(yyvsp[-1]),(yyvsp[0]).Name());  
	                                           (yyval).SetupLineNos((yyvsp[-2]),(yyvsp[-1]),(yyvsp[0]));
	                                         ;}
    break;

  case 157:
#line 523 "cxx.y"
    { (yyval)=ObjectListC(STR(VarNameList)); ;}
    break;

  case 158:
#line 524 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 159:
#line 526 "cxx.y"
    { ObjectListC ol(STR(VarNameList)); ol.Append((yyvsp[0])); (yyval) = ol; ;}
    break;

  case 160:
#line 527 "cxx.y"
    { ObjectListC ol((yyvsp[-2])); ol.Append((yyvsp[0])); (yyval) = ol; ;}
    break;

  case 161:
#line 529 "cxx.y"
    { (yyval)=ObjectC("[]"); ;}
    break;

  case 162:
#line 530 "cxx.y"
    { (yyval)=(yyvsp[-3]); (yyvsp[-3]).Name() += "[]" ;}
    break;

  case 163:
#line 532 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 164:
#line 533 "cxx.y"
    { (yyval)=(yyvsp[-2]); (yyvsp[-2]).Name() += StringC(" = ") + (yyvsp[0]).Name(); ;}
    break;

  case 165:
#line 534 "cxx.y"
    { (yyval)=(yyvsp[-1]); (yyvsp[-1]).Name() += (yyvsp[0]).Name(); (yyval).SetVar(STR(array),STR(1)); ;}
    break;

  case 166:
#line 536 "cxx.y"
    { (yyval)=(yyvsp[-5]); (yyvsp[-5]).Name() += (yyvsp[-4]).Name(); (yyval).SetVar(STR(array),STR(1)); ;}
    break;

  case 167:
#line 537 "cxx.y"
    { (yyval)=(yyvsp[0]); (yyvsp[0]).Name() = StringC(" = ") + (yyvsp[0]).Name(); ;}
    break;

  case 168:
#line 539 "cxx.y"
    { (yyval)=ObjectC(""); ;}
    break;

  case 169:
#line 540 "cxx.y"
    { (yyval)=(yyvsp[-1]); (yyval).Name() += strp_asterisk;  ;}
    break;

  case 170:
#line 541 "cxx.y"
    { (yyval)=(yyvsp[-1]); (yyval).Name() += strp_ampersand;  ;}
    break;

  case 171:
#line 542 "cxx.y"
    { (yyval)=(yyvsp[-1]); (yyval).Name() += (yyvsp[0]).Name();  ;}
    break;

  case 176:
#line 552 "cxx.y"
    { (yyval) =ObjectC(""); ;}
    break;

  case 177:
#line 553 "cxx.y"
    { (yyval) = (yyvsp[0]); ;}
    break;

  case 178:
#line 555 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 179:
#line 556 "cxx.y"
    { (yyvsp[-1]) = (yyvsp[-1]).Name() + (yyvsp[0]).Name(); ;}
    break;

  case 180:
#line 558 "cxx.y"
    { (yyval)=ObjectC(strp_OpenBracket + (yyvsp[-1]).Name() + strp_CloseBracket); ;}
    break;

  case 201:
#line 563 "cxx.y"
    { (yyval)=ObjectC(strp_Colon); ;}
    break;

  case 202:
#line 564 "cxx.y"
    { (yyval)=ObjectC("<"); ;}
    break;

  case 203:
#line 565 "cxx.y"
    { (yyval)=ObjectC(">"); ;}
    break;

  case 204:
#line 566 "cxx.y"
    { (yyval)=ObjectC(strp_equals); ;}
    break;

  case 205:
#line 567 "cxx.y"
    { (yyval)=ObjectC(strp_plus); ;}
    break;

  case 206:
#line 568 "cxx.y"
    { (yyval)=ObjectC("-"); ;}
    break;

  case 207:
#line 569 "cxx.y"
    { (yyval)=ObjectC(strp_Tilda); ;}
    break;

  case 208:
#line 570 "cxx.y"
    { (yyval)=ObjectC(strp_ampersand); ;}
    break;

  case 209:
#line 571 "cxx.y"
    { (yyval)=ObjectC("/"); ;}
    break;

  case 210:
#line 572 "cxx.y"
    { (yyval)=ObjectC(strp_asterisk); ;}
    break;

  case 211:
#line 573 "cxx.y"
    { (yyval)=ObjectC("%"); ;}
    break;

  case 212:
#line 574 "cxx.y"
    { (yyval)=ObjectC("|"); ;}
    break;

  case 213:
#line 575 "cxx.y"
    { (yyval)=ObjectC("^"); ;}
    break;

  case 214:
#line 576 "cxx.y"
    { (yyval)=ObjectC("!"); ;}
    break;

  case 215:
#line 577 "cxx.y"
    { (yyval)=ObjectC("["); ;}
    break;

  case 216:
#line 578 "cxx.y"
    { (yyval)=ObjectC("]"); ;}
    break;

  case 217:
#line 579 "cxx.y"
    { (yyval)=ObjectC(strp_Semicolon); ;}
    break;

  case 218:
#line 580 "cxx.y"
    { (yyval)=ObjectC(strp_Comma); ;}
    break;

  case 219:
#line 585 "cxx.y"
    { (yyval)=(yyvsp[0]); hookCommentObj = (yyvsp[0]);  ;}
    break;

  case 220:
#line 586 "cxx.y"
    { (yyval)=(yyvsp[0]); hookCommentObj = (yyvsp[0]);  ;}
    break;

  case 221:
#line 587 "cxx.y"
    { (yyval)=(yyvsp[0]); 
                                          (yyval).SetVar(STR(dectype),STR(friend));
					  (yyval).IncludeLineNo((yyvsp[-1]));
					  hookCommentObj = (yyvsp[0]);
                                        ;}
    break;

  case 222:
#line 593 "cxx.y"
    { (yyval) = (yyvsp[-1]); 
                                          (yyval).SetVar(STR(dectype),STR(prototype)); 
					  (yyval).IncludeLineNo((yyvsp[0]));
                                        ;}
    break;

  case 223:
#line 597 "cxx.y"
    { (yyval)=(yyvsp[-3]);   /* "=0"; */
                                          (yyval).SetVar(STR(dectype),STR(abstract));
					  (yyval).IncludeLineNo((yyvsp[0]));
                                        ;}
    break;

  case 224:
#line 601 "cxx.y"
    { (yyval)=(yyvsp[-1]); 
                                          (yyval).SetVar(STR(dectype),STR(extern));    
					  (yyval).IncludeLineNo((yyvsp[0]));
                                        ;}
    break;

  case 225:
#line 607 "cxx.y"
    { (yyval) = (yyvsp[-1]);
                                             (yyval).SetVar(STR(dectype),STR(definition)); 
					     (yyval).IncludeLineNo((yyvsp[0]));
                                           ;}
    break;

  case 226:
#line 612 "cxx.y"
    { (yyval)=ObjectC(strp_plus); ;}
    break;

  case 227:
#line 613 "cxx.y"
    { (yyval)=ObjectC("-"); ;}
    break;

  case 228:
#line 614 "cxx.y"
    { (yyval)=ObjectC(strp_Tilda); ;}
    break;

  case 229:
#line 615 "cxx.y"
    { (yyval)=ObjectC(strp_ampersand); ;}
    break;

  case 230:
#line 616 "cxx.y"
    { (yyval)=ObjectC("/"); ;}
    break;

  case 231:
#line 617 "cxx.y"
    { (yyval)=ObjectC(strp_asterisk); ;}
    break;

  case 232:
#line 618 "cxx.y"
    { (yyval)=ObjectC("%"); ;}
    break;

  case 233:
#line 619 "cxx.y"
    { (yyval)=ObjectC("|"); ;}
    break;

  case 234:
#line 620 "cxx.y"
    { (yyval)=ObjectC("^"); ;}
    break;

  case 235:
#line 621 "cxx.y"
    { (yyval)=ObjectC("!"); ;}
    break;

  case 236:
#line 622 "cxx.y"
    { (yyval)=ObjectC(strp_equals); ;}
    break;

  case 237:
#line 623 "cxx.y"
    { (yyval)=ObjectC("<"); ;}
    break;

  case 238:
#line 624 "cxx.y"
    { (yyval)=ObjectC(">"); ;}
    break;

  case 239:
#line 625 "cxx.y"
    { (yyval)=ObjectC("--"); ;}
    break;

  case 240:
#line 626 "cxx.y"
    { (yyval)=ObjectC("++"); ;}
    break;

  case 241:
#line 627 "cxx.y"
    { (yyval)=ObjectC("[]"); ;}
    break;

  case 242:
#line 628 "cxx.y"
    { (yyval)=ObjectC("()"); ;}
    break;

  case 243:
#line 629 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 244:
#line 630 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 245:
#line 632 "cxx.y"
    { (yyval) = ObjectListC(STR(EmptyArgList)); ;}
    break;

  case 246:
#line 633 "cxx.y"
    { (yyval) = (yyvsp[-1]); ;}
    break;

  case 247:
#line 636 "cxx.y"
    { (yyval) = (yyvsp[-1]); ;}
    break;

  case 248:
#line 640 "cxx.y"
    { (yyval) = MethodC((yyvsp[-2]).Name(),DataTypeC((yyvsp[-3])),ObjectListC((yyvsp[-1])),(yyvsp[0])); 
	(yyval).IncludeLineNo((yyvsp[-3]));
	(yyval).IncludeLineNo((yyvsp[-2]));
	(yyval).IncludeLineNo((yyvsp[-1]));
	(yyval).IncludeLineNo((yyvsp[0]));
      ;}
    break;

  case 249:
#line 647 "cxx.y"
    { (yyval) = MethodC((yyvsp[-3]).Name(),DataTypeC((yyvsp[-6])),ObjectListC((yyvsp[-1])),(yyvsp[0]));
        (yyval).SetScope((yyvsp[-5]));
	(yyval).IncludeLineNo((yyvsp[-6]));
	(yyval).IncludeLineNo((yyvsp[-5]));
	(yyval).IncludeLineNo((yyvsp[0]));
      ;}
    break;

  case 250:
#line 654 "cxx.y"
    { MethodC amethod((yyvsp[-2]).Name(), DataTypeC(STR(void)),ObjectListC((yyvsp[-1])),(yyvsp[0])); 
        amethod.IncludeLineNo((yyvsp[-2]));
        amethod.IncludeLineNo((yyvsp[-1]));
	amethod.IncludeLineNo((yyvsp[0]));
        amethod.SetConstructor(true);
        (yyval) = amethod;
	ObjectC subScope = (yyvsp[-2]).GetScope();
	if(subScope.IsValid()) {
	  ObjectListC ol(subScope);
	  DListC<ObjectC> tmp = ol.List().Copy();
	  //cerr << "Got constructor:  SubScope '" << subScope.Name() << "' Path:" << PathName(tmp) << " \n";
	  tmp.DelLast();
	  if(!tmp.IsEmpty()) {
	    //cerr << "Got constructor:  SetScope to Path:" << PathName(tmp) << " \n";
	    ObjectListC oln(STR(ConstructorScopePath),tmp);
	    (yyval).SetScope(oln);
	  }
	}
      ;}
    break;

  case 251:
#line 674 "cxx.y"
    { (yyval) = MethodC(STR(operator) + (yyvsp[-3]).Name(), DataTypeC((yyvsp[-5])),ObjectListC((yyvsp[-1])),(yyvsp[0])); 
        (yyval).IncludeLineNo((yyvsp[-5]));
        (yyval).IncludeLineNo((yyvsp[-4]));
        (yyval).IncludeLineNo((yyvsp[0]));
      ;}
    break;

  case 252:
#line 681 "cxx.y"
    { (yyval) = MethodC(STR(operator) + (yyvsp[-3]).Name(), DataTypeC((yyvsp[-7])),ObjectListC((yyvsp[-1])),(yyvsp[0])); 
         (yyval).SetScope((yyvsp[-6]));
         (yyval).IncludeLineNo((yyvsp[-7]));
         (yyval).IncludeLineNo((yyvsp[-6]));
         (yyval).IncludeLineNo((yyvsp[0]));
       ;}
    break;

  case 253:
#line 689 "cxx.y"
    { (yyval) = MethodC(STR(operator), DataTypeC((yyvsp[-2])),ObjectListC((yyvsp[-1])),(yyvsp[0]),true);
        (yyval).IncludeLineNo((yyvsp[-3]));
        (yyval).IncludeLineNo((yyvsp[0]));
      ;}
    break;

  case 254:
#line 694 "cxx.y"
    { (yyval) = MethodC(STR(operator), DataTypeC((yyvsp[-2])),ObjectListC((yyvsp[-1])),(yyvsp[0]),true); 
        (yyval).SetScope((yyvsp[-5]));
        (yyval).IncludeLineNo((yyvsp[-5]));
        (yyval).IncludeLineNo((yyvsp[-4]));
        (yyval).IncludeLineNo((yyvsp[0]));
      ;}
    break;

  case 260:
#line 710 "cxx.y"
    {  (yyval)=ObjectListC(STR(FuncArgList)); ;}
    break;

  case 261:
#line 711 "cxx.y"
    {  (yyval)=(yyvsp[0]); ;}
    break;

  case 262:
#line 713 "cxx.y"
    { ObjectListC ol(STR(FuncArgList)); (yyval)= ol; ol.Append((yyvsp[0])); ;}
    break;

  case 263:
#line 714 "cxx.y"
    { (yyval)=(yyvsp[-2]); ObjectListC ol((yyvsp[-2])); ol.Append((yyvsp[0])); ;}
    break;

  case 264:
#line 715 "cxx.y"
    { (yyval)=(yyvsp[-1]); ObjectListC ol((yyvsp[-1])); DataTypeC ell("..."); ol.Append(ell); ;}
    break;

  case 265:
#line 717 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 266:
#line 718 "cxx.y"
    { (yyval)=(yyvsp[-1]); DataTypeC dt((yyvsp[-1])); dt.SetAlias((yyvsp[0]).Name()); ;}
    break;

  case 267:
#line 719 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 268:
#line 720 "cxx.y"
    { (yyval)= DataTypeC("..."); ;}
    break;

  case 269:
#line 724 "cxx.y"
    { 
	  StringC name;
	  if((yyvsp[-5]).IsValid())
	    name = (yyvsp[-5]).Name();
	  else
	    name = StringC("anon?") + StringC(anonCount++);
	  MethodC meth(name,DataTypeC((yyvsp[-8])),ObjectListC((yyvsp[-2])),(yyvsp[0]),false,true);
	  //$$=DataTypeC($1.Name() + StringC(" (*)(") + $7.Name() + strp_CloseBracket + $9.Name(),name); 
	  ObjectListC ol("(*)");
	  ol.Append(meth);
	  (yyval) = DataTypeC(name,"",ol);
	  (yyval).SetupLineNos((yyvsp[-8]),(yyvsp[-1]),(yyvsp[0]));
	;}
    break;

  case 270:
#line 738 "cxx.y"
    {
	  MethodC meth((yyvsp[-5]).Name(),DataTypeC((yyvsp[-10])),ObjectListC((yyvsp[-2])),(yyvsp[0]),false,true);
	  ObjectListC ol("(*)");
	  ol.Append(meth);	  
	  //$$=DataTypeC($1.Name() + StringC(" (*)(") + $9.Name() + strp_CloseBracket + $11.Name(),$6.Name());
	  (yyval) = DataTypeC((yyvsp[-4]).Name(),"",ol);
	  (yyval).SetupLineNos((yyvsp[-10]),(yyvsp[-1]),(yyvsp[0]));
	;}
    break;

  case 271:
#line 748 "cxx.y"
    { (yyval)=ObjectC(""); ;}
    break;

  case 272:
#line 749 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 273:
#line 750 "cxx.y"
    { (yyval)=(yyvsp[-1]); ;}
    break;

  case 274:
#line 751 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 275:
#line 754 "cxx.y"
    { (yyval)=(yyvsp[-1]); (yyval).SetupLineNos((yyvsp[-2]),(yyvsp[0])); ;}
    break;

  case 276:
#line 756 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;

  case 277:
#line 757 "cxx.y"
    { (yyval)=(yyvsp[0]); ;}
    break;


      default: break;
    }

/* Line 1126 of yacc.c.  */
#line 3270 "cxx.tab.c"

  yyvsp -= yylen;
  yyssp -= yylen;


  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (YYPACT_NINF < yyn && yyn < YYLAST)
	{
	  int yytype = YYTRANSLATE (yychar);
	  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
	  YYSIZE_T yysize = yysize0;
	  YYSIZE_T yysize1;
	  int yysize_overflow = 0;
	  char *yymsg = 0;
#	  define YYERROR_VERBOSE_ARGS_MAXIMUM 5
	  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
	  int yyx;

#if 0
	  /* This is so xgettext sees the translatable formats that are
	     constructed on the fly.  */
	  YY_("syntax error, unexpected %s");
	  YY_("syntax error, unexpected %s, expecting %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s");
	  YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
#endif
	  char *yyfmt;
	  char const *yyf;
	  static char const yyunexpected[] = "syntax error, unexpected %s";
	  static char const yyexpecting[] = ", expecting %s";
	  static char const yyor[] = " or %s";
	  char yyformat[sizeof yyunexpected
			+ sizeof yyexpecting - 1
			+ ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
			   * (sizeof yyor - 1))];
	  char const *yyprefix = yyexpecting;

	  /* Start YYX at -YYN if negative to avoid negative indexes in
	     YYCHECK.  */
	  int yyxbegin = yyn < 0 ? -yyn : 0;

	  /* Stay within bounds of both yycheck and yytname.  */
	  int yychecklim = YYLAST - yyn;
	  int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
	  int yycount = 1;

	  yyarg[0] = yytname[yytype];
	  yyfmt = yystpcpy (yyformat, yyunexpected);

	  for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	    if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	      {
		if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
		  {
		    yycount = 1;
		    yysize = yysize0;
		    yyformat[sizeof yyunexpected - 1] = '\0';
		    break;
		  }
		yyarg[yycount++] = yytname[yyx];
		yysize1 = yysize + yytnamerr (0, yytname[yyx]);
		yysize_overflow |= yysize1 < yysize;
		yysize = yysize1;
		yyfmt = yystpcpy (yyfmt, yyprefix);
		yyprefix = yyor;
	      }

	  yyf = YY_(yyformat);
	  yysize1 = yysize + yystrlen (yyf);
	  yysize_overflow |= yysize1 < yysize;
	  yysize = yysize1;

	  if (!yysize_overflow && yysize <= YYSTACK_ALLOC_MAXIMUM)
	    yymsg = (char *) YYSTACK_ALLOC (yysize);
	  if (yymsg)
	    {
	      /* Avoid sprintf, as that infringes on the user's name space.
		 Don't have undefined behavior even if the translation
		 produced a string with the wrong number of "%s"s.  */
	      char *yyp = yymsg;
	      int yyi = 0;
	      while ((*yyp = *yyf))
		{
		  if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		    {
		      yyp += yytnamerr (yyp, yyarg[yyi++]);
		      yyf += 2;
		    }
		  else
		    {
		      yyp++;
		      yyf++;
		    }
		}
	      yyerror (yymsg);
	      YYSTACK_FREE (yymsg);
	    }
	  else
	    {
	      yyerror (YY_("syntax error"));
	      goto yyexhaustedlab;
	    }
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror (YY_("syntax error"));
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
        {
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
        }
      else
	{
	  yydestruct ("Error: discarding", yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (0)
     goto yyerrorlab;

yyvsp -= yylen;
  yyssp -= yylen;
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping", yystos[yystate], yyvsp);
      YYPOPSTACK;
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token. */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK;
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  return yyresult;
}


#line 772 "cxx.y"



