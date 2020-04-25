// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_PAIR_HEADER
#define RAVL_PAIR_HEADER 1
///////////////////////////////////////////////////////////////////////
//! file="Ravl/Core/Base/Pair.hh"
//! author="Radek Marik"
//! docentry="Ravl.API.Core.Tuples"
//! date="27/02/1997"
//! rcsid="$Id$"
//! lib=RavlCore
//! userlevel=Default

#include "Ravl/Types.hh"
#include "Ravl/Index.hh"
#include "Ravl/Assert.hh"

namespace RavlN {
  class BinOStreamC;
  class BinIStreamC;
  
  //! userlevel=Normal
  //: Pair of homogenious objects
  // The class PairC represents a pair of objects of the same type.
  // It has basic features of arrays. The object class must provide
  // the default constructor, the copy constructor, the assigment operator,
  // and the destructor.
  
  template <class DataC>
  class PairC {
  public:
    
    // Constructors, assigment, and destructor
    // ---------------------------------------
    
    inline PairC()
    {}
    // The default constructor.
    
    inline PairC(const PairC & p);
    // The copy constructor.
    
    inline PairC(const DataC & o1, const DataC & o2);
    // Creates the pair of the objects 'o1' and 'o2'.
    
    inline const PairC & operator=(const PairC & p);
    // The assigment.
    
    // Access to the objects
    // ---------------------
    
    inline const DataC & operator[](IndexC i) const;
    // Returns the i-th object of the pair.
    
    inline DataC & operator[](IndexC i);
    // Access to the i-th object of the pair.
    
    inline const DataC & A() const
    { return item[0]; }
    // Returns the first object of the pair.
    
    inline DataC & A()
    { return item[0]; }
    // Access to the first object of the pair.
    
    inline const DataC & B() const
    { return item[1]; }
    // Returns the second object of the pair.
  
    inline DataC & B()
    { return item[1]; }
    // Access to the second object of the pair.
    

    inline const DataC & Data1() const
    { return item[0]; }
    // Returns the first object of the pair.
    
    inline DataC & Data1()
    { return item[0]; }
    // Access to the first object of the pair.
    
    inline const DataC & Data2() const
    { return item[1]; }
    // Returns the second object of the pair.
  
    inline DataC & Data2()
    { return item[1]; }
    // Access to the second object of the pair.
    
    inline IndexC Size() const
    { return 2; }
    // Returns 2 as the number of elements of the pair.
    
    inline bool IsValid() const
    { return true; }
    // Returns always true confirming that the pair exists.
    
    inline bool Contains(const IndexC i) const
    { return (i>=0) && (i <= 1); }
    // Returns true if the pair contains an item with the index 'i'.
    
    // Special operations
    // ------------------
    
    inline PairC & Swap();
    // Exchanges the order of items in the pair.
    
    inline PairC & Reverse();
    // Exchanges the order of items in the pair.
    
  private:
    
    // Object representation
    // ---------------------
    
    DataC item[2]; // The pair of items.
  };
  
  template <class DataC>
  inline 
  PairC<DataC>::PairC(const PairC<DataC> & p) {
    item[0] = p.item[0];
    item[1] = p.item[1];
  }
  
  template <class DataC>
  inline 
  PairC<DataC>::PairC(const DataC & o1, const DataC & o2) {
    item[0] = o1;
    item[1] = o2;
  }
  
  template <class DataC>
  inline 
  const PairC<DataC> & 
  PairC<DataC>::operator=(const PairC<DataC> & p) {
    item[0] = p.item[0];
    item[1] = p.item[1];
    return *this;
  }
  
  template <class DataC>
  inline 
  const DataC & 
  PairC<DataC>::operator[](IndexC i) const {
    RavlAssert(Contains(i));
    return item[i.V()];
  }
  
  template <class DataC>
  inline 
  DataC & 
  PairC<DataC>::operator[](IndexC i) {
    RavlAssert(Contains(i));
    return item[i.V()];
  }
  
  template <class DataC>
  inline 
  PairC<DataC> & 
  PairC<DataC>::Swap() {
    DataC tmp = item[0];
    item[0]   = item[1];
    item[1]   = tmp;
    return *this;
  }  
  
  
  template <class DataC>
  std::ostream &operator<<(std::ostream & s, const PairC<DataC> & p) {
    s << p.A() << ' ' << p.B();
    return s;
  }
  
  template <class DataC>
  std::istream &operator>>(std::istream & s, PairC<DataC> & p) {
    s >> p.A() >> p.B();
    return s;
  }

  template <class DataC>
  BinOStreamC &operator<<(BinOStreamC & s, const PairC<DataC> & p) {
    s << p.A() << p.B();
    return s;
  }
  
  template <class DataC>
  BinIStreamC &operator>>(BinIStreamC & s, PairC<DataC> & p) {
    s >> p.A() >> p.B();
    return s;
  }
  
}

#endif

