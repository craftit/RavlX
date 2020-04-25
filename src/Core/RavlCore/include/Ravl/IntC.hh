// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_INTC_HEADER
#define RAVL_INTC_HEADER 1
////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Misc"
//! userlevel=Normal
//! rcsid="$Id$"
//! file="Ravl/Core/Base/IntC.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="26/10/98"

#include "Ravl/Types.hh"
#include "Ravl/Assert.hh"

//: Ravl namespace.

namespace RavlN {

  class BinIStreamC;
  class BinOStreamC;

  //: Integer with a default constructor that assigns it a value of 0
  // Its main feature is the default constructor sets
  // its value to zero.  Usefull in things like histograms.

  class IntC {
  public:
    IntC()
      : v(0)
    {}
    //: Default value, 0.

    IntC(IntT nv)
      : v(nv)
    {}
    //: Construct an int.

    IntC(const SizeC &val)
     : v(val.V())
    { RavlAssertMsg(v >= 0,"Value overflow."); }

    IntC Copy() const { return IntC(v); }
    //: Make a copy.

    operator IntT() const { return v; }
    //: Convert to a plain int.

    bool operator==(const IntC &oth) const { return v == oth.v; }
    //: Comparison operator.

    bool operator==(IntT ov) const { return v == ov; }
    //: Comparison operator.

    SizeT Hash() const { return (SizeT) v; }
    //: Hash it.

    IntT operator++(int) { return v++; }
    //: Increment.

    IntT operator--(int) { return v--; }
    //: Decrement

    IntT operator++() { return ++v; }
    //: Increment.

    IntT operator--() { return --v; }
    //: Decrement


    IntT operator-=(IntT oth) {
      v -= oth;
      return v;
    }
    //: Take another UIntT from this one.

    IntT operator+=(IntT oth) {
      v += oth;
      return v;
    }
    //: Take another UIntT from this one.

    IntT operator/=(IntT oth) {
      v /= oth;
      return v;
    }
    //: Divide in place by another value.

    IntT operator*=(IntT oth) {
      v *= oth;
      return v;
    }
    //: Multiply in place by another value.

    IntT v;
  };

  std::ostream &operator<<(std::ostream &out,const IntC &x);
  std::istream &operator>>(std::istream &in,IntC &x);

  BinOStreamC &operator<<(BinOStreamC &out,const IntC &x);
  BinIStreamC &operator>>(BinIStreamC &in,IntC &x);

  //: unsigned integer with a default constructor that assigns it a value of 0.
  // Its main feature is the default constructor sets
  // its value to zero.  Usefull in things like histograms.

  class UIntC {
  public:
    UIntC()
      : v(0) {}
    //: Default value, 0.

    UIntC(UIntT nv)
      : v(nv) {}
    //: Construct an int.

    UIntC(const SizeC &val)
     : v(val.V())
    { RavlAssertMsg(v != val.V(),"Value overflow."); }

    UIntC Copy() const { return UIntC(v); }
    //: Make a copy.

    operator UIntT() const { return v; }
    //: Convert to a plain int.

    bool operator==(const UIntC &oth) const { return v == oth.v; }
    //: Comparison operator.

    bool operator==(UIntT ov) const { return v == ov; }
    //: Comparison operator.

    SizeT Hash() const { return v; }
    //: Hash it.

    UIntT operator++(int) { return v++; }
    //: Increment.

    UIntT operator--(int) { return v--; }
    //: Decrement

    UIntT operator++() { return ++v; }
    //: Increment.

    UIntT operator--() { return --v; }
    //: Decrement

    UIntT operator-=(UIntT oth) {
      RavlAssert(v >= oth);
      v -= oth;
      return v;
    }
    //: Take another UIntT from this one.

    UIntT operator+=(UIntT oth) {
      v += oth;
      return v;
    }
    //: Take another UIntT from this one.

    UIntT v;
  };

  inline int operator-(int v1,UIntC v2)
  { return v1 - (int) v2; }
  //: Subtraction.
  // This is included to avoid some confusing type casts which can
  // lead to odd results.

  inline UIntC operator-(UIntC v1,UIntC v2)
  { return v1.v - v2.v; }
  //: Subtraction.

  inline UIntC operator-(UIntC v1,int v2)
  { return v1.v - v2; }
  //: Subtraction.

  std::ostream &operator<<(std::ostream &out,const UIntC &x);
  std::istream &operator>>(std::istream &in,UIntC &x);

  BinOStreamC &operator<<(BinOStreamC &out,const UIntC &x);
  BinIStreamC &operator>>(BinIStreamC &in,UIntC &x);

}
#endif
