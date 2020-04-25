// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_TUPLE3_HEADER
#define RAVL_TUPLE3_HEADER 1
/////////////////////////////////////////////////
//! userlevel=Normal
//! rcsid="$Id$"
//! file="Ravl/Core/Container/Misc/Tuple3.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Tuples"
//! date="09/06/1998"

#include "Ravl/Types.hh"


namespace RavlN {
  class BinIStreamC;
  class BinOStreamC;
  
  //: Class containing 3 other classes.
  
  template<class T1,class T2,class T3>
  class Tuple3C {
  public:
    Tuple3C() {}
    //: Default constructor.
    
    Tuple3C(std::istream &in) 
    { in >> d1 >> d2 >> d3; }
    //: Stream constructor.
    
    Tuple3C(BinIStreamC &in) 
    { in >> d1 >> d2 >> d3; }
    //: Binary Stream constructor.
    
    Tuple3C(const T1 &nd1,const T2 &nd2,const T3 &nd3) 
      : d1(nd1),
	d2(nd2),
	d3(nd3)
    {}
    //: Constructor.
    
    Tuple3C(const Tuple3C<T1,T2,T3> &oth) 
      : d1(oth.d1),
	d2(oth.d2),
	d3(oth.d3)
    {}
    //: Copy constructor.

    Tuple3C<T1,T2,T3> Copy() const
    { return Tuple3C<T1,T2,T3>(d1,d2,d3); }
    //: Make a copy of this object.
    
    Tuple3C<T1,T2,T3> DeepCopy(UIntT levels = ((UIntT) -1)) const { 
      if(levels <= 1) return Copy();
      return Tuple3C<T1,T2,T3>(StdDeepCopy(d1,levels-1),
			       StdDeepCopy(d2,levels-1),
			       StdDeepCopy(d3,levels-1)); 
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

    SizeT Hash() const {
      SizeT h2 = StdHash(d2);
      SizeT h3 = StdHash(d3);
      return (StdHash(d1) + h2 + h3) ^ (h2 << 7)  ^ (h3 << 11);
    }
    //: Get hash value for tuple.
    
    bool operator==(const Tuple3C<T1,T2,T3> &oth) const
    { return (oth.d1 == d1) && (oth.d2 == d2) && (oth.d3 == d3); }
    //: Are tuples equal ?
    
    bool operator!=(const Tuple3C<T1,T2,T3> &oth) const
    { return  (oth.d1 != d1) || (oth.d2 != d2) || (oth.d3 != d3);}
    //: Are tuples unequal ?
    
  protected:
    T1 d1;
    T2 d2;
    T3 d3;
  };
  
  template<class T1,class T2,class T3>
  std::ostream &operator<<(std::ostream &out,const Tuple3C<T1,T2,T3> &obj) {
    out << obj.Data1() << ' ' << obj.Data2() << ' ' << obj.Data3() << ' ';
    return out;
  }
  
  template<class T1,class T2,class T3>
  std::istream &operator>>(std::istream &in,Tuple3C<T1,T2,T3> &obj) {
    obj = Tuple3C<T1,T2,T3>(in);
    return in;
  }
  
  // Binary stream operators.
  
  class BinIStreamC;
  class BinOStreamC;
  
  template<class T1,class T2,class T3>
  BinOStreamC &operator<<(BinOStreamC &out,const Tuple3C<T1,T2,T3> &obj) {
    out << obj.Data1() << obj.Data2() << obj.Data3();
    return out;
  }
  
  template<class T1,class T2,class T3>
  BinIStreamC &operator>>(BinIStreamC &in,Tuple3C<T1,T2,T3> &obj) {
    obj = Tuple3C<T1,T2,T3>(in);
    return in;
  }
}

#endif
