// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLEMPTYCLASS_HEADER
#define RAVLEMPTYCLASS_HEADER 1
///////////////////////////////////////////////////////////////
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Misc"
//! rcsid="$Id$"
//! lib=RavlCore
//! file="Ravl/Core/Base/Empty.hh"
//! date="10/05/99"

#include "Ravl/Types.hh"

//: Ravl namespace.

namespace RavlN {
  class BinIStreamC;
  class BinOStreamC;
  
  //! userlevel=Advanced
  //: Empty data class.
  // For use in templates that expect a class, but we don't
  // wish to actually store data in.
  
  class EmptyC {
  public:
    EmptyC() {}
    //: Default constructor.
    
    EmptyC(std::istream &in) {}
    //: Stream constructor.
    
    EmptyC operator=(const EmptyC &)
      { return EmptyC(); }
    //: Assigment operator.
    
    EmptyC Copy() const 
    { return EmptyC(); }
    //: Make a copy.
    // Ohhh what hard work...
    
    EmptyC DeepCopy(UIntT levels) const
    { return EmptyC(); }
    //: Make a deep copy.
    // not far to go.
    
    bool operator==(const EmptyC &) const
    { return true; }
    //: Comparison, all empty classes are the same.
    
    bool operator!=(const EmptyC &) const
    { return false; }
    //: Comparison
    // all empty classes are the same.
    
    SizeT Hash() const
    { return 42; }
    //: One hash values as good as another.
    
  };
  
  inline std::ostream &operator <<(std::ostream &out,const EmptyC &)
  { return out; }

  inline std::istream &operator >>(std::istream &in,EmptyC &)
  { return in; }

  inline BinOStreamC &operator <<(BinOStreamC &out,const EmptyC &)
  { return out; }
  
  inline BinIStreamC &operator >>(BinIStreamC &in,EmptyC &)
  { return in; }
  
};

#endif
