// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/FeatureSelection/FieldInfo.cc"

#include "Ravl/PatternRec/FieldInfo.hh"
#include "Ravl/VirtualConstructor.hh"
#include "Ravl/Exception.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {

  //: Construct with name
  FieldInfoBodyC::FieldInfoBodyC(const StringC & name)
      : m_name(name) {
  }

  //: Load from stream.

  FieldInfoBodyC::FieldInfoBodyC(std::istream &strm)
      : RCBodyVC(strm) {
    char version;
    strm >> version;
    if (version != 1) {
      throw ExceptionUnexpectedVersionInStreamC("Unexpected version in FieldInfoC. ");
    }
    strm >> m_name;

  }

  //: Load from binary stream.

  FieldInfoBodyC::FieldInfoBodyC(BinIStreamC &strm)
      : RCBodyVC(strm) {
    char version;
    strm >> version;
    if (version != 1) {
      throw ExceptionUnexpectedVersionInStreamC("Unexpected version in FieldInfoC. ");
    }
    strm >> m_name;
  }

  //: Writes object to stream, can be loaded using constructor

  bool FieldInfoBodyC::Save(std::ostream &out) const {
    if (!RCBodyVC::Save(out))
      return false;
    char version = 1;
    out << version;
    out << m_name;
    return true;
  }

  //: Writes object to stream, can be loaded using constructor

  bool FieldInfoBodyC::Save(BinOStreamC &out) const {
    if (!RCBodyVC::Save(out))
      return false;
    char version = 1;
    out << version;
    out << m_name;
    return true;
  }

  //: Load from stream.

  FieldInfoC::FieldInfoC(std::istream &strm)
      : RCHandleVC<FieldInfoBodyC>(RAVL_VIRTUALCONSTRUCTOR(strm,FieldInfoBodyC)) {
  }

  //: Load from binary stream.

  FieldInfoC::FieldInfoC(BinIStreamC &strm)
      : RCHandleVC<FieldInfoBodyC>(RAVL_VIRTUALCONSTRUCTOR(strm,FieldInfoBodyC)) {
  }

  //: Setup virtual constructor.
  // As this is an abstract class its not really needed, but its usefull to
  // have an example of how it should work here.

  RAVL_INITVIRTUALCONSTRUCTOR(FieldInfoBodyC);

}
