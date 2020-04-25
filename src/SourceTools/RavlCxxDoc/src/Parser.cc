// This file is part of CxxDoc, The RAVL C++ Documentation tool 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU General 
// Public License (GPL). See the gpl.licence file for details or
// see http://www.gnu.org/copyleft/gpl.html
// file-header-ends-here
///////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlCxxDoc
//! file="Ravl/SourceTools/CxxDoc/Parser.cc"

#include "Ravl/CxxDoc/Object.hh"

#define YYSTYPE RavlCxxDocN::ObjectC 

#include "FlexLexer.h"
#include "Ravl/Stack.hh"
#include "Ravl/CxxDoc/Parser.hh"
#include "Ravl/OS/Directory.hh"

#if RAVL_HAVE_ANSICPPHEADERS
#include <fstream>
#else
#include <fstream.h>
#endif
#include <stdlib.h>

#include "cxx.tab.h"

#include <assert.h>
#include <ctype.h>

#define DODEBUG 0

#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

int yyparse();

extern RavlCxxDocN::CommentInfoC comment;
extern RavlCxxDocN::CommentInfoC commentLast;
extern RavlCxxDocN::ObjectC yylval;
extern int yylineno;

void CxxCheckForHook();

namespace RavlCxxDocN
{
  
  extern ObjectC hookCommentObj;
  
  ParserBodyC *currentParser = 0;   // Ptr to current parser.
  ObjectListC results;              // Results from parser...
  
  //: Constructor.
    
  ParserBodyC::ParserBodyC() 
    : lex(0),
      data("std")
  {}

  //: Destructor.
  
  ParserBodyC::~ParserBodyC()
  {
    // Clean up.
    results.Invalidate();
  }

  //: Parse a directory.
  
  bool ParserBodyC::ParseTree(const FilenameC &fn) {
    if(!fn.IsDirectory())
      return Parse(fn);
    std::cerr << "Parsing directory '" << fn << "'\n";
    if(rootFilename == "")
      rootFilename = fn;
    DirectoryC dir(fn);
    DListC<StringC> files = dir.List();
    for(DLIterC<StringC> it(files);it.IsElm();it.Next()) {
      if(it.Data() == "." || it.Data() == "..")
	continue;
      FilenameC newFn(fn + '/' + it.Data());
      if(newFn.IsDirectory()) {
	ParseTree(newFn);
	continue;
      }
      if(newFn.HasExtension(".hh"))
	Parse(newFn);
      if(newFn.HasExtension(".h"))
	Parse(newFn);
    }
    return true;
  }
  
  //: Parse a file.
  
  bool ParserBodyC::Parse(const FilenameC &fn) {
    std::cerr << "Parsing file '" << fn << "'\n";
    // Setup file.
    ONDEBUG(std::cerr << "Setting up file parse for '" << fn << "'\n");
    curFilename = fn;
    comment.FileReset();
    std::ifstream input(fn.c_str());
    yyFlexLexer tmp(&input,&cerr);
    lex = &tmp;
    currentParser = this;
    ONDEBUG(std::cerr << "Parsering file.\n");
    yyparse();
    //cerr << "Last :" << commentLast.Text() << " Com: " << comment.Text() << " " << CxxParser::hookCommentObj.IsValid() << "\n";
    currentParser = 0;
    // Finished with file..
    lex = 0;
    ONDEBUG(std::cerr << "Merging results.\n");
    StringC afn(fn);
    results.SetAll("filename",afn.after((int) rootFilename.length()));
    data.Merge(results);
    ONDEBUG(std::cerr << "Parse done.\n");
    return true;
  }

  //: Resolve references in input data.
  bool ParserBodyC::Resolve() {
    data.Resolve();
    return true;
  }
  
  //: Dump database to 'out'
  
  void ParserBodyC::Dump(std::ostream &out) {
    data.Dump(out);
  }
  
  int ParserBodyC::LineNo() { return lex->lineno(); }
  //: Get current lineno
  
  //: Get the next token;
  
  int ParserBodyC::NextToken() {
    assert(lex != 0);
    int tok = lex->yylex();
    //cerr << "Token:" << tok << "\n";
    return tok;
  }

  
  
} 

extern "C" {
  int yywrap();
}

using namespace RavlN;

bool stopOnError;
bool verbose;

int yyerror(const char *txt);
int yylex();

int yyerror(const char *txt) {
  std::cerr << RavlCxxDocN::currentParser->CurrentFilename() << ":" << RavlCxxDocN::currentParser->LineNo() << " Parse error \n";
  if(stopOnError)
    exit(1);
  return 0;
}

void CxxCheckForHook() {
  if(RavlCxxDocN::hookCommentObj.IsValid()) {
    if(RavlCxxDocN::ObjectListC::IsA(RavlCxxDocN::hookCommentObj)) {
      RavlCxxDocN::ObjectListC ol(RavlCxxDocN::hookCommentObj);
      if(ol.IsWrapper()) {
	for(DLIterC<RavlCxxDocN::ObjectC> it(ol.List());it.IsElm();it.Next())
	  it.Data().UpdateComment(::commentLast);
      }
    }
    if(verbose)
      std::cout << "Hooking to '" << RavlCxxDocN::hookCommentObj.Name() << "' Comment: '" << ::commentLast.Header() << "'\n";
    RavlCxxDocN::hookCommentObj.UpdateComment(::commentLast);
    RavlCxxDocN::hookCommentObj.Invalidate();
  }
}


int yylex() {
  assert(RavlCxxDocN::currentParser != 0);
    
  
  int tok =  RavlCxxDocN::currentParser->NextToken();
  if(tok == 0) { // End of file ?
    commentLast = comment;
  }
  
  CxxCheckForHook();
  
  if(verbose) {
    //cerr << "Comment:" << ::comment.Header() << "\n";
    //  std::cerr << "LastComment:" << : << "\n";
    std::cerr << "-- Tok:" << tok << " ";
    switch(tok)
      {

      case IDENTIFIER: std::cerr << "ID (" << yylval.Name() << ")" ; break;
      case CLCL: std::cerr << "CLCL"; break;
      case SCSPEC: std::cerr << "SCSPEC"; break;
      case PUBLIC: std::cerr << "PUBLIC"; break;
      case PROTECTED: std::cerr << "PROTECTED"; break;
      case PRIVATE: std::cerr << "PRIVATE"; break;
      case NAMESPACE: std::cerr << "NAMESPACE"; break;
	//      case STATIC: std::cerr << "STATIC"; break;
      case STRING: std::cerr << "STRING"; break;
      case CPVIRTUAL: std::cerr << "CPVIRTUAL"; break;
      case ENUM: std::cerr << "ENUM"; break;
      case CPTYPEDEF: std::cerr << "CPTYPEDEF"; break;
      case CPFRIEND: std::cerr << "CPFRIEND"; break;
      case EXTERN: std::cerr << "EXTERN"; break;
      case CONSTANT: std::cerr << "CONSTANT"; break;
      case TEMPLATE: std::cerr << "TEMPLATE"; break;
      case CPOPERATOR: std::cerr << "OPERATOR"; break;
      case CV_QUALIFIER: std::cerr << "CV_QUALIFIER"; break;
      default:
	if(isprint(tok) && tok > 16) 
	  std::cerr << " '" << ((char) tok) << "' ";
	break;
      }
    std::cerr <<" Line:" << RavlCxxDocN::currentParser->LineNo() << "\n";
  }
  return tok;
}

int yywrap() {
  return 1;
}
