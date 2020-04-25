// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SOURCECURSOR_HEADER
#define RAVL_SOURCECURSOR_HEADER 1
//! rcsid="$Id$"
//! lib=RavlClassWizard
//! author="Charles Galambos"
//! docentry="Ravl.API.Source Tools.Class Wizard"
//! file="Ravl/Applications/ClassWizard/SourceCursor.hh"

#include "Ravl/Text/TextBuffer.hh"
#include "Ravl/Text/TextCursor.hh"

namespace RavlN {

#if 0
  //! userlevel=develop
  //: Wooping class
  // To test auto handle generation.
  
  class Woop2BodyC
    : public RCBodyVC
  {
  public:
    Woop2BodyC(int nn)
      : n(nn)
    {}
    //: Constructor.
    
    ~Woop2BodyC();
    //: Destructor
    
    IntT N() const
    { return n; }
    //: Get n

  protected:
    int n;
  };
#endif
  
  class SourceCursorC 
    : public TextCursorC
  {
  public:
    SourceCursorC(TextBufferC &buffer,IntT indent = 0);
    //: Create cursor.
    
    bool operator+=(const StringC &text)
    { return InsertLine(Indent(indent) + text); }
    //: Add line to current position.
    
    bool SetIndent(IntT val)
    { indent = val; return true;}
    //: Set current indent. 
    
    bool AddIndent(IntT val)
    { indent += val; return indent >= 0;}
    //: Change current indent. 
    
    StringC Indent(IntT level);
    //: Get an indent string.
    
    bool FindLine(IntT lineNum);
    //: Find a specific line number (from original file.)
    // Will return true if exact line is found and false otherwise.
    // If the line is missing then the cursor will be left on the first line
    // after the gap.
    
    bool IsBlank() const;
    //: Return true if the current line is blank
    
  protected:
    IntT indent;
  };

}


#endif
