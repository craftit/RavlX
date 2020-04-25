// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlClassWizard
//! docentry="Ravl.Source Tools.Class Wizard"
//! file="Ravl/Applications/ClassWizard/SourceCursor.cc"

#include "Ravl/SourceTools/SourceCursor.hh"
#include <ctype.h>

namespace RavlN {
  
  //: Create cursor.
  
  SourceCursorC::SourceCursorC(TextBufferC &buffer,IntT nindent)
    : TextCursorC(buffer),
      indent(nindent)
  {}
  
  //: Get an indent string.
  
  StringC SourceCursorC::Indent(IntT level) {
    StringC ret;
    for(int i = 0;i < level;i++)
      ret += "  ";
    return ret;
  }

  //: Find a specific line number (from original file.)
  
  bool SourceCursorC::FindLine(IntT lineNo) {
    //cerr << "SourceCursorC::FindLine() LN= " << lineNo << "\n";
    if(line.List().IsEmpty())
      return false;
    if(lineNo > (IntT) line.List().Last().LineNo()) {
      line.Last();
      return false;
    }
    if(lineNo < (IntT) line.List().First().LineNo()) {
      line.First();
      return false;
    }
    for(line.First();line;line++) {
      if((IntT) line->LineNo() >= lineNo && line->LineNo() != ((UIntT) -1))
	break;
    }
    if(!line) return false;
    //cerr << "SourceCursorC::FindLine() Got " << line->LineNo() << "\n";
    return (IntT) line->LineNo() == lineNo;
  }

  //: Return true if the current line is blank
  
  bool SourceCursorC::IsBlank() const {
    StringC t = LineText();
    IntT s = t.Size();
    for(int i = 0;i < s;i++)
      if(!isspace(t[i]))
	return false;
    return true;
  }
  
}
