// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TUPLE4_HEADER
#define RAVL_TUPLE4_HEADER 1
/////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Tuples"
//! userlevel=Normal
//! rcsid="$Id$"
//! file="Ravl/Core/Container/Misc/Tuple4.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="09/06/1998"

#include "Ravl/Types.hh"

//: Ravl namespace.

namespace RavlN {
  
  class BinIStreamC;
  class BinOStreamC;

  //! userlevel=Normal
  //: Class containing 4 other classes.
  
  template<class T1,class T2,class T3,class T4>
  class Tuple4C {
  public:
    Tuple4C() {}
    //: Default constructor.
    
    Tuple4C(std::istream &in) 
    { in >> d1 >> d2 >> d3 >> d4; }
    //: Stream constructor.
    
    Tuple4C(BinIStreamC &in) 
    { in >> d1 >> d2 >> d3 >> d4; }
    //: Binary Stream constructor.
    
    Tuple4C(const T1 &nd1,const T2 &nd2,const T3 &nd3,const T4 &nd4) 
      : d1(nd1),
	d2(nd2),
	d3(nd3),
	d4(nd4)
    {}
    //: Constructor.
    
    Tuple4C(const Tuple4C<T1,T2,T3,T4> &oth) 
      : d1(oth.d1),
	d2(oth.d2),
	d3(oth.d3),
	d4(oth.d4)
    {}
    //: Copy constructor.

    Tuple4C<T1,T2,T3,T4> Copy() const
    { return Tuple4C<T1,T2,T3,T4>(d1,d2,d3,d4); }
    //: Make a copy of this object.
    
    Tuple4C<T1,T2,T3,T4> DeepCopy(UIntT levels = ((UIntT) -1)) const { 
      if(levels <= 1) return Copy();
      return Tuple4C<T1,T2,T3,T4>(StdDeepCopy(d1,levels-1),
				  StdDeepCopy(d2,levels-1),
				  StdDeepCopy(d3,levels-1),
				  StdDeepCopy(d4,levels-1)); 
    }
    //: Make a deep copy of the container.
    
    T1 &Data1() { return d1; }
    //: Data access.
    
    const T1 &Data1() const { return d1; }
    //: Data access.
    
    T2 &Data2() { return d2; }
    //: Data access.
    
    const T2 &Data2() const { return d2; }
    //: Data access.
    
    T3 &Data3() { return d3; }
    //: Data access.
    
    const T3 &Data3() const { return d3; }
    //: Data access.
    
    T4 &Data4() { return d4; }
    //: Data access.
    
    const T4 &Data4() const { return d4; }
    //: Data access.

    SizeT Hash() const {
      SizeT h2 = StdHash(d2);
      SizeT h3 = StdHash(d3);
      SizeT h4 = StdHash(d4);
      return (StdHash(d1) + h2 + h3 + h4) ^ (h2 << 7)  ^ (h3 << 11) ^ (h3 << 13);
    }
    //: Get hash value for tuple.
    
    bool operator==(const Tuple4C<T1,T2,T3,T4> &oth) const
    { return (oth.d1 == d1) && (oth.d2 == d2) && (oth.d3 == d3)  && (oth.d4 == d4); }
    //: Are tuples equal ?
    
    bool operator!=(const Tuple4C<T1,T2,T3,T4> &oth) const
    { return  (oth.d1 != d1) || (oth.d2 != d2) || (oth.d3 != d3) || (oth.d4 != d4); }
    //: Are tuples unequal ?
    
  protected:
    T1 d1;
    T2 d2;
    T3 d3;
    T4 d4;
  };
  
  template<class T1,class T2,class T3,class T4>
  std::ostream &operator<<(std::ostream &out,const Tuple4C<T1,T2,T3,T4> &obj) {
    out << obj.Data1() << ' ' << obj.Data2() << ' ';
    out << obj.Data3() << ' ' << obj.Data4() << ' ';
    return out;
  }
  
  template<class T1,class T2,class T3,class T4>
  std::istream &operator>>(std::istream &in,Tuple4C<T1,T2,T3,T4> &obj) {
    obj = Tuple4C<T1,T2,T3,T4>(in);
    return in;
  }
  
  template<class T1,class T2,class T3,class T4>
  BinOStreamC &operator<<(BinOStreamC &out,const Tuple4C<T1,T2,T3,T4> &obj) {
    out << obj.Data1() << obj.Data2();
    out << obj.Data3() << obj.Data4();
    return out;
  }
  
  template<class T1,class T2,class T3,class T4>
  BinIStreamC &operator>>(BinIStreamC &in,Tuple4C<T1,T2,T3,T4> &obj) {
    obj = Tuple4C<T1,T2,T3,T4>(in);
    return in;
  }
  
}
#endif
