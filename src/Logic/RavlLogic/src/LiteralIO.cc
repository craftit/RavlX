// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlLogic
//! file="Ravl/Logic/Base/LiteralIO.cc"

#include "Ravl/Logic/Literal.hh"
#include "Ravl/Logic/LiteralIO.hh"
#include "Ravl/Stack.hh"
#include "Ravl/Logic/Tuple.hh"
#include "Ravl/Logic/And.hh"
#include "Ravl/Logic/Or.hh"
#include "Ravl/Logic/Not.hh"
#include "Ravl/Logic/NamedVar.hh"
#include "Ravl/Logic/NamedLiteral.hh"
#include "Ravl/DList.hh"
#include "Ravl/Logic/State.hh"

namespace RavlLogicN {

  //! userlevel=Normal
  //: Parse error.
  
  class ExceptionParseErrorC 
    : public ExceptionC
  {
  public:
    ExceptionParseErrorC(const char *msg)
      : ExceptionC(msg)
    {}
    //: Constructor.
    
    ExceptionParseErrorC(const char *ntext,bool copy)
      : ExceptionC(ntext,copy)
    {}
    //: Constructor.
    // if copy is true, make a copy of ntext.

  };
  
  //: Load data from filename into state.
  
  bool LoadState(const StringC &filename,StateC &state,ContextC &context) {
    IStreamC is(filename);
    if(!is)
      return false;
    return LoadState(is,state,context);
  }
  
  bool LoadState(IStreamC &is,StateC &state,ContextC &loadContext) {
    StackC<Tuple2C<DListC<LiteralC>,bool> > context;
    int ln = 0;
    bool invertNext = false;
    try {
      while(!is.IsEndOfStream()) { // Any more characters.
	char let = is.GetChar();
	bool var = false;
	switch(let) {
	case '(': { // Open tuple
	  DListC<LiteralC> lits;
	  context.Push(Tuple2C<DListC<LiteralC>,bool>(lits,invertNext));
	  invertNext = false;
	} break;
	case ')': { // Close tuple
	  if(context.IsEmpty()) {
	    std::cerr << "ERROR: Parse error at line " << ln << ", unmatched ')' found. \n";
	    throw ExceptionParseErrorC("Unexpected ')' found. ");
	  }
	  if(invertNext) {
	    std::cerr << "ERROR: Parse error at line " << ln << ", Unexpect '!' found. \n";
	    throw ExceptionParseErrorC("Unexpected '!' found. ");
	  }
	  UIntT n = context.Top().Data1().Size();
	  if(n == 0) {
	    state.Tell(Literal());
	    break;
	  }
	  TupleC tup;
	  LiteralC first = context.Top().Data1().First();
	  if(first == literalAnd)
	    tup = AndC(n);
	  else if(first == literalOr)
	    tup = OrC(n);
	  else if(first == literalNot) {
	    if(n != 2) {
	      std::cerr << "ERROR: Parse error at line " << ln << ", Not must have exactly 1 argument. \n";
	      throw ExceptionParseErrorC("Not must have exactly 1 argument. ");
	    }
	    tup = NotC(true);
	  } else
	    tup = TupleC(n);
	  int i = 0;
	  for(DLIterC<LiteralC> it(context.Top().Data1());it;it++,i++)
	    tup.SetArg(i,*it);
	  if(context.Top().Data2())
	    tup = NotC(true,tup);
	  context.DelTop();
	  if(!context.IsEmpty())
	    context.Top().Data1().InsLast(tup);
	  else
	    state.Tell(tup);
	} break;
	case ';': // Comment until end of line.
	  is.SkipTo('\n');
	  /* no break */
	case '\n':
	  ln++; // Count lines.
	  /* no break */
	case '\r':
	case '\t':
	case ' ':
	  break; // Ignore whitespace.
	case '#':
	case '$':
	  break; // Ignore these characters silently for now.
	case '!':
	  invertNext = true;
	  break;
	case '@':
	case '~':
	case '{':
	case '}':
	case '[':
	case ']':
	case '^':
	case '*':
	case '%':
	case '`':
	case '+':
	case ':':
	case '>':
 	case '<':
 	case '.':
 	case ',':
 	case '\\':
 	case '/':
 	case '|':
 	case '=':
	  std::cerr << "WARNING: Illegal character '" << let << "' found in stream on line " << ln << ". \n";
 	  break;
	case '"': { // String constant.
	  StringC val = is.ClipTo('"');
	  // FIXME:- If the string constant contains new lines, we'll loose our line number count.
	  
	  LiteralC lit;
	  if(!loadContext.Lookup(val,lit))
	    lit = NamedLiteralC(val);
	  if(invertNext) {
	    lit = NotC(true,lit);
	    invertNext = false;
	  }
	  if(!context.IsEmpty())
	    context.Top().Data1().InsLast(lit);
	  else
	    state.Tell(lit);
	} break;
	case '?': // Variable marker.
	  var = true;
	  /* no break */
	default: { // Constant
	  StringC x(let);
	  x += is.ClipWord(" \t\n\r()\\/:;+<>{}[]|=%*^.,#$@!~");
	  // Check for numbers ?
	  LiteralC lit;
	  if(!loadContext.Lookup(x,lit)) {
	    if(var)
	      lit = NamedVarC(x);
	    else
	      lit = NamedLiteralC(x);
	  } else {
	    if(var && !lit.IsVariable())
	      throw ExceptionParseErrorC("Conflicting literal variable/literal types.");
	  }
	  if(invertNext) {
	    lit = NotC(true,lit);
	    invertNext = false;
	  }
	  if(!context.IsEmpty())
	    context.Top().Data1().InsLast(lit);
	  else
	    state.Tell(lit);
	  } break;
	}
      }
    } catch(ExceptionEndOfStreamC &) {
    } catch(ExceptionParseErrorC &) {
      return false;
    } 
    
    if(!context.IsEmpty()) {
      std::cerr << "Unexpected end of file. \n";
      return false;
    }
    return true;
  }

  //: Save data to file from state.
  
  bool SaveState(const StringC &filename,StateC &state) {
    OStreamC os(filename);
    return SaveState(os,state);
  }

  //: Save data into stream.
  
  bool SaveState(OStreamC &os,StateC &state) {
    if(!os)
      return false;
    state.Dump(os);
    return true;
  }
  
  
  
}
