// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/Text/TemplateFile.cc"

#include "Ravl/Exception.hh"
#include "Ravl/Text/TemplateFile.hh"
#include "Ravl/EntryPnt.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  TemplateFileC::TemplateFileC(const StringC &templName,const OStreamC &anOut)
    : out(anOut),
      at(templName)
  {}
  
  TemplateFileC::TemplateFileC(const TextFileC &templFile,const OStreamC &anOut)
    : out(anOut),
      at(templFile)
  {}
  
  TemplateFileC::TemplateFileC(const TextFileC &templFile,const OStreamC &anOut,const RCHashC<StringC,StringC> &tab)
    : out(anOut),
      vars(tab),
      at(templFile)
  {}

  bool TemplateFileC::Build(bool userWillHandleExceptions) {
    if(userWillHandleExceptions) {
      if(!First())
	return true; // Finished with no trouble.
      ErrorLabel();  
    } else {
      try {
	if(!First())
	  return true; // Finished with no trouble.
	ErrorLabel();  
      } catch(ExceptionC &e) {
	if(UsingRavlMain()) // If error message not already printed.
	  cerr << "ERROR: " << e.what() << endl;
	else
	  throw ; // Pass it on.
      } catch(exception &e) {
	cerr << "C++ Exception :'" << e.what() << "'\n";
	cerr << "  while processing file template file at " << at.PositionTxt() << "\n";
      }
    }
    return false;
  }
  
  bool TemplateFileC::First()
  { return !(Next().IsEmpty()); }
  
  StringC &TemplateFileC::Next()  {
    ONDEBUG(cerr << "TemplateFileC::Next(), Called. \n");
    if(!at.IsElm()) {
      ONDEBUG(cerr << "TemplateFileC::Next(), Done. \n");
      point = StringC();
      return point;
    }
    int where;
    ONDEBUG(cerr << "TemplateFileC::Next(), Processing text.... \n");
    point = StringC(); // Empty string, in case we don't find a label.
    for(;at.IsElm();) {
      SubStringC cl = at.RestOfLine();
      // Do a quick check...
      if((where = cl.index("$(")) < 0) {
	ONDEBUG(cerr << "TemplateFileC::Next(), Passing on '" << cl << "'  Len=" << cl.length() << " (1) \n");
	out << cl; // Just output and goto the next line.
	if(!at.NextLine())
	  return point;
	continue;
      }
      if(where > 0) { // Stuffed ?
	out << cl.before(where); // Output begining of line.
	ONDEBUG(cerr << "TemplateFileC::Next(), Passing on '" << cl.before(where) << "' (2) \n");	
	if(cl[where-1] == '$') {
	  at.GotoCol(at.ColNum() + where+1); // Unstuff '$'
	  continue;
	}
      }
      at.GotoCol(at.ColNum() + where + 2);
      point = ClipVar(at);
      if(point.IsEmpty()) { // Check its not empty.
	if(!UsingRavlMain())
	  cerr << "ERROR: Empty macro in " << at.PositionTxt() << "\n";
	throw ExceptionC(StringC("Empty macro in ") + at.PositionTxt(),true);
      }
      // Known variable ?
      StringC *varVal = vars.Lookup(point);
      if(varVal == 0) {
	// Nope, hand it back to the user.
	//cerr << "Label : '"<<point << "' not preset. \n";
	break;
      }
      // Print out macro value. 
      ONDEBUG(cerr << "TemplateFileC::Next(), Subst label '" << point <<"' with '" << *varVal << "' \n");
      out << *varVal;
      point = StringC(); // Delt with this label.
    }
    ONDEBUG(cerr << "TemplateFileC::Next(), Returning label '" << point << "'. \n");
    return point;
  }
  
  //: Clip out variable.
  
  StringC TemplateFileC::ClipVar(TextCursorC &at) {
    static SArray1dC<bool> bracketClip(TextCursorC::BuildClipTable("()"));
    int depth = 1;
    //StringC ret("",0,127 - sizeof(StrRepC)); // Make sure there's some room...
    StringC ret;
    char last;
    while(depth > 0 && at.IsElm()) {
      SubStringC sub = at.ClipWord(bracketClip,false);
      ONDEBUG(cerr << "ClipVar:'" << sub << "'\n");
      if(at.IsElm()) {
	last = at.Char();
	ONDEBUG(cerr << "Next:'" << last << "'\n");
      } else {
	last = 0;
	ONDEBUG(cerr << "Next:0\n");
      }
      if(last == ')') { // Close bracket ?
	at.NextChar(); // Don't skip the last ')'
	depth--;
	ret += sub;
	if(depth > 0) 
	  ret += ')';
	continue;
      }
      if(last == '(') {
	at.NextChar();
	depth++;
	ret += sub;
	ret += '(';
	continue;
      }
      // last == 0 end of file.
      ret += sub;
      break;
    }
    if(depth != 0) {
      // Is there's a danger the exception won't be caught and printed.
      // So print out a message to StdErr.
      if(!UsingRavlMain())
	cerr << "ERROR: Unterminated macro in " << at.PositionTxt() << "\n";
      throw ExceptionC(StringC("Unterminated macro in ") + at.PositionTxt(),true);
    }
    
    return ret;
  }

  
  //: Print an error message about the label.
  
  void TemplateFileC::ErrorLabel() { 
    cerr << "ERROR: Unrecognised label '" << point << "' at "  << at.PositionTxt() << "\n";
  }
  
}
