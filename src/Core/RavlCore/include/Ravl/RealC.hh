// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_REALC_HEADER
#define RAVL_REALC_HEADER 1
////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Misc"
//! userlevel=Normal
//! rcsid="$Id$"
//! file="Ravl/Core/Base/RealC.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="15/02/2002"

#include "Ravl/Types.hh"
#include "Ravl/Assert.hh"
#include "Ravl/Stream.hh"

//: Ravl namespace.

namespace RavlN {
  
  class BinIStreamC;
  class BinOStreamC;
  
  //: Real value with a default constructor that assigns it a value of 0.
  
  // Its main feature is the default constructor sets 
  // its value to zero.  Usefull in things like histograms.
  
  class RealC {
  public:
    RealC() 
      : v(0) {}
    //: Default value, 0.
    
    RealC(RealT nv) 
      : v(nv) {}
    //: Construct an int.
    
    RealC Copy() const { return RealC(v); }
    //: Make a copy.
    
    operator RealT() const { return v; }
    //: Convert to a plain int.
    
    bool operator==(const RealC &oth) const { return v == oth.v; }
    //: Comparison operator.
    
    bool operator==(RealT ov) const { return v == ov; }
    //: Comparison operator.
    
    RealT operator++(int) { return v++; }
    //: Increment.
    
    RealT operator--(int) { return v--; }
    //: Decrement
    
    RealT operator++() { return ++v; }
    //: Increment.
    
    RealT operator--() { return --v; }
    //: Decrement
    
    
    RealT operator-=(RealT oth) {
      v -= oth;
      return v;
    }
    //: Take another URealT from this one.
    
    RealT operator+=(RealT oth) {
      v += oth;
      return v;
    }
    //: Take another URealT from this one.
    
    RealT operator/=(RealT oth) {
      v /= oth;
      return v;
    }
    //: Divide in place by another value.
    RealT v;
  };
  
  std::ostream &operator<<(std::ostream &out,const RealC &x);  
  std::istream &operator>>(std::istream &in,RealC &x);

  BinOStreamC &operator<<(BinOStreamC &out,const RealC &x);
  BinIStreamC &operator>>(BinIStreamC &in,RealC &x);

}
#endif
