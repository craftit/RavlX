/* This file is part of CxxDoc, The RAVL C++ Documentation tool 
/*! file="Ravl/SourceTools/CxxDoc/tokenizer.h" */
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here*/
/*! rcsid="$Id$" */
/*! lib=RavlCxxDoc */
#define _TOKENIZER_H_INCLUDED

#define CPCLASS        258
#define PUBLIC         259
#define PROTECTED      260
#define PRIVATE        261
#define ID             262
#define CLCL           263
#define CONST          264
#define VOLATILE       265
#define CPUNION        267
#define STRING         268
#define STATIC         269
#define CPVIRTUAL      270
#define QTSIGNAL       271
#define QTSLOT         272
#define CPGLOBAL       273
#define CPENUM         274
#define CPTYPEDEF      275
#define CPFRIEND       276
#define CPSTRUCT       277
#define CPOPERATOR     278
#define CPTEMPLATE     279
#define NUM            280
#define CPTHROW        281
#define NAMESPACE      282

typedef const char * YYSTYPE;

extern YYSTYPE yylval;

#endif
