// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLTEXTLINE_HEADER
#define RAVLTEXTLINE_HEADER 1
///////////////////////////////////////////////////////
//! file="Ravl/OS/Text/TextFileLine.hh"
//! lib=RavlOS
//! author="Charles Galambos"
//! docentry="Ravl.API.OS.Text Processing"
//! date="05/12/97"
//! rcsid="$Id$"

#include "Ravl/String.hh"

namespace RavlN {
  
  //! userlevel=Advanced
  //: Class for holding a single line of text.
  
  class TextFileLineC 
  {
  public:
    TextFileLineC()
      : lineNo((UIntT)-1)
    {}
    // Default constructor.
    
    TextFileLineC(UIntT lno,const StringC &str)
     : lineNo(lno),
       text(str)
    {}
    // Default constructor.
        
    StringC &Text() { return text; }
    // Access text of line.
    
    const StringC &Text() const { return text; }
    // Access text of line.
    
    void SetLineNo(UIntT no) { lineNo = no; }
    // Set the lineno.
    
    UIntT LineNo() const { return lineNo; }
    // Get the lineno.
    
    void Dump(ostream &strm) const;
    // Dump to stdout.
  protected:
    UIntT lineNo;   // Line number in file.
    StringC text; // Text.
  };

  BinOStreamC &operator<<(BinOStreamC &strm,const TextFileLineC &line);
  // Write to output

  BinIStreamC &operator>>(BinIStreamC &strm,TextFileLineC &line);
  // Write to output
}



#endif

