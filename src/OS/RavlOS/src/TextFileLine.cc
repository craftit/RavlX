// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOS
//! file="Ravl/OS/Text/TextFileLine.cc"

#include "Ravl/Text/TextFileLine.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {
  
  // Dump to stdout.
  
  void TextFileLineC::Dump(ostream &strm) const {
    strm << lineNo << ":" << text;
  }

  BinOStreamC &operator<<(BinOStreamC &strm,const TextFileLineC &line) {
    ByteT version = 1;
    strm << version << line.LineNo() << line.Text();
    return strm;
  }
  // Write to output

  BinIStreamC &operator>>(BinIStreamC &strm,TextFileLineC &line) {
    ByteT version = 0;
    strm >> version;
    UIntT ln;
    StringC text;
    strm >> ln >> text;
    line = TextFileLineC(ln,text);
    return strm;
  }
  // Write to output

}


