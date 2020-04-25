// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_RCHASH_HEADER
#define RAVL_RCHASH_HEADER 1
///////////////////////////////////////////////////
//! userlevel=Normal
//! file="Ravl/Core/Container/Hash/RCHash.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="06/05/1998"
//! docentry="Ravl.API.Core.Hash Tables"
//! example=WordFreq.cc
//! rcsid="$Id$"

#include "Ravl/Hash.hh"
//#include "Ravl/RCWrap.hh"

namespace RavlN {
  //! userlevel=Develop
  //: Reference counted auto-resizing hash table.
  // This is a BIG OBJECT.
  // See <a href="RavlN.HashC.html">HashC</a> for more details. <p>
  // NB. This class requires the standard stream operators to
  // be implemented. ie.  operator<<(std::ostream &os,xxx) etc...
  
  template<class Key,class Dat >
  class RCHashBodyC
    : public RCBodyC
  {
  public:
    RCHashBodyC(const HashC<Key,Dat> &oth)
      : m_data(oth)
    {}
    //: Copy constructor.

    RCHashBodyC()
    {}
    //: Default constructor.
    // Will make a body by default.
    
    /*RCHashC(const RCWrapC<HashC<Key,Dat> > &base)
      : RCWrapC<HashC<Key,Dat> > (base)
    {}
    //: Base constructor.*/
    
    RCHashBodyC(std::istream &in)
      : m_data(in)
    {}
    //: Stream constructor.
    
    RCHashBodyC(BinIStreamC &in)
      : m_data(in)
    {}
    //: Stream constructor.

    RCHashBodyC(SizeT nBins)
      : m_data(nBins)
    {}

    bool Save(std::ostream &strm) const {
      strm << m_data;
      //RavlAssertMsg(0,"Not implemented");
      return true;
    }
    //: Save to text stream.
    // To avoid wrapped classes having to support a
    // IO implementation this method does nothing.

    bool Save(BinOStreamC &strm) const {
      strm << m_data;
      //RavlAssertMsg(0,"Not implemented");
      return true;
    }
    //: Save to binary stream.
    // To avoid wrapped classes having to support a
    // IO implementation this method does nothing.

    HashC<Key,Dat> &Data()
    { return m_data; }
    //: Access data.

    const HashC<Key,Dat> &Data() const
    { return m_data; }
    //: Access data.

  protected:
    HashC<Key,Dat> m_data;
  };

  //! userlevel=Normal
  //: Reference counted auto-resizing hash table.
  // This is a BIG OBJECT.
  // See <a href="RavlN.HashC.html">HashC</a> for more details. <p>
  // NB. This class requires the standard stream operators to
  // be implemented. ie.  operator<<(std::ostream &os,xxx) etc...

  template<class Key,class Dat >
  class RCHashC
    : public RCHandleC<RCHashBodyC<Key,Dat> >
  {
  public:
    RCHashC(const RCHashC<Key,Dat> &oth)
      : RCHandleC<RCHashBodyC<Key,Dat> >(oth.BodyPtr())
    {}
    //: Copy constructor.

    RCHashC(const HashC<Key,Dat> &oth)
      : RCHandleC<RCHashBodyC<Key,Dat> >(new RCHashBodyC<Key,Dat>(oth))
    {}
    //: Copy constructor.

    RCHashC(bool makeBod = true)
      : RCHandleC<RCHashBodyC<Key,Dat> > (makeBod ? new RCHashBodyC<Key,Dat>() : NULL)
    {}
    //: Default constructor.
    // Will make a body by default.

    RCHashC(const RCHandleC<RCHashBodyC<Key,Dat> > &base)
      : RCHandleC<RCHashBodyC<Key,Dat> > (base)
    {}
    //: Base constructor.

    RCHashC(std::istream &in)
      : RCHandleC<RCHashBodyC<Key,Dat> > (new RCHashBodyC<Key,Dat>(in))
    {}
    //: Stream constructor.

    RCHashC(BinIStreamC &in)
      : RCHandleC<RCHashBodyC<Key,Dat> > (new RCHashBodyC<Key,Dat>(in))
    {}
    //: Stream constructor.

    RCHashC(SizeT nBins)
      : RCHandleC<RCHashBodyC<Key,Dat> > (new RCHashBodyC<Key,Dat>(nBins))
    {}

    Dat &operator[] (const  Key &a) 
    { return this->Data()[a]; }
    //: Access an element.
    //!param: a - Key to lookup in table.
    //!return: Element corresponding to 'a' in the table.
    // Will create an empty element with the default constructor
    // and return a reference to it, if it doesn't already exist.
    
    const Dat &operator[] (const  Key &a) const
    { return this->Data()[a]; }
    //: Access an element.
    //!param: a - Key to lookup in table.
    //!return: Element corresponding to 'a' in the table.
    // The element must exist in the hash table otherwise
    // it will cause an assertion failure. Note: It will
    // just crash in optimised builds.
    
    inline bool Del(const Key &aKey)
    { return this->Data().Del(aKey); }
    //: Delete member from table.
    //!param: aKey - Key to check 
    //!return: true if Key was in hash table.
    
    inline bool IsElm(const Key &aKey) const 
    { return this->Data().IsElm(aKey); }
    //: Is key used in the table.
    //!param: aKey - Key to check 
    //!return: true if aKey is in the table.
    
    inline void Move(RCHashC<Key,Dat> &oth)
    { this->Data().Move(oth.Data()); }
    //: Move contents of another table into this one.
    // The previous contents of this table are removed.
    //!param: oth - Table to move elements from
    
    inline bool IsEmpty(void) const
    { return this->Data().IsEmpty(); }
    //: Is table empty ?
    
    inline void Empty(void)
    { this->Data().Empty(); }
    //: Empty table
    
    inline bool Insert(const Key &aKey,const Dat &data) 
    { return this->Data().Update(aKey,data); }
    //: Insert Data with Key.
    // Returns: True=Member existed already. False=New one was added.
    
    inline SizeT Size() const
    { return this->Data().Size(); }
    //: Get number of elements in table.
    
    inline Dat *Lookup(const Key &aKey)
    { return this->Data().Lookup(aKey); }
    //: Look to see if data is present in the table.
    // Do not use, Try Lookup(key,data);
    // If data is present return a ptr to it, otherwise
    // return a 0 ptr.
    
    inline const Dat *Lookup(const Key &aKey) const
    { return this->Data().Lookup(aKey); }
    //: Look to see if data is present in the table.
    // Do not use, Try Lookup(key,data);
    //!return: If data is present return a ptr to it, otherwise return a 0 ptr.

    inline bool Lookup(const Key &aKey,Dat &data) const
    { return this->Data().Lookup(aKey,data); }
    //: Lookup data for key.
    //!param: aKey - Key for entry in the hash table.
    //!param: data - Place to hold results of lookup.
    //!return:true if entry is found, and is assigned to 'data', otherwise 'data' is left unmodified.
    
    inline bool Update(const Key &key,const Dat &data)
    { return this->Data().Update(key,data); }
    //: Update member of HashCable, will create new one if it doesn't exist.
    // OBSOLETE: Use Insert(key,data)
    // Returns: True=Member existed already. False=New one was added.
    
    inline Dat &Update(const Key &key)
    { return this->Data().Update(key); }
    //: Get value, add an entry created with the types default constructor if its not in the table. 
    // OBSOLETE: Use operator[].
    
    inline Dat &Access(const Key &key,const Dat &def = Dat())
    { return this->Data().Access(key,def); }
    //: Access key, if it does exists create a new bin with value 'def'
    //!param: key - Key to lookup.
    //!param: def - Default value to assign to entry if it doesn't exist.
    //!return: Reference to the new entry.
    
    void AddFrom(RCHashC<Key,Dat> &oth,bool replace = true)
    { this->Data().AddFrom(oth,replace); }
    //: Add contents of another table into this one.
    //!param: oth - Table to remove elements from.
    //!param: replace - If true replace elements from this table with those from 'oth', otherwise keep entries from this table.
    // leave other empty.  if replace is false the contents of the 
    // old table are not replace by the new entries. 
    
    bool NormaliseKey(Key &value) const
    { return this->Data().NormaliseKey(value); }
    //: Normalise an equivalent key to one used the the table.
    // This function is useful when you want to normalise the use
    // of equivalent keys (think strings.) to save memory.
    // Returns true if key exists in the table, false otherwise.
    
    SizeT Hash() const
    { return this->Data().Hash(); }
    //: Compute hash value for table.
    
    bool operator==(const RCHashC<Key,Dat> &oth) const
    { return this->Data() == oth.Data(); }
    //: Test if this hash table is equal to another.
    
    bool operator!=(const RCHashC<Key,Dat> &oth) const
    { return this->Data() != oth.Data(); }
    //: Test if this hash table is different to another.

    bool operator==(const HashC<Key,Dat> &oth) const
    { return this->Data() == oth; }
    //: Test if this hash table is equal to another.
    
    bool operator!=(const HashC<Key,Dat> &oth) const
    { return this->Data() != oth; }
    //: Test if this hash table is different to another.

    HashC<Key,Dat> &Data()
    { return Body().Data(); }
    //: Access data.

    const HashC<Key,Dat> &Data() const
    { return Body().Data(); }
    //: Access data.

    operator HashC<Key,Dat> &()
    { return Body().Data(); }
    //: Default conversion to data type.

    operator const HashC<Key,Dat> &() const
    { return Body().Data(); }
    //: Default conversion to data type.

    RCHashC Copy() const
    { return RCHashC(Body().Data()); }
    //: Make a copy of this handle.
    // NB. This assumes the wrapped object is SMALL, and so
    // just using the copy constructor is sufficient.

    RCHashC DeepCopy(UIntT levels = ((UIntT) -1)) const {
      switch(levels) {
      case 0: return *this;
      case 1: return Copy();
      case 2: return RCHashC(StdCopy(Body().Data()));
      default: break;
      }
      levels--;
      return RCHashC(StdDeepCopy(Body().Data(),levels));
    }
    //: Make a copy of this handle.
  protected:
    RCHashC(RCHashBodyC<Key,Dat> &bod)
      : RCHandleVC<RCHashBodyC<Key,Dat> >(bod)
    {}
    //: Body constructor.

    RCHashC(const RCHashBodyC<Key,Dat> *bod)
      : RCHandleVC<RCHashBodyC<Key,Dat> >(bod)
    {}
    //: Body constructor.

    RCHashBodyC<Key,Dat> &Body()
    { return  RCHandleC<RCHashBodyC<Key,Dat> >::Body(); }
    //: Access body.

    const RCHashBodyC<Key,Dat> &Body() const
    { return RCHandleC<RCHashBodyC<Key,Dat> >::Body(); }
    //: Access body.

    RCHashBodyC<Key,Dat> *BodyPtr()
    { return  RCHandleC<RCHashBodyC<Key,Dat> >::BodyPtr(); }
    //: Access body.

    const RCHashBodyC<Key,Dat> *BodyPtr() const
    { return RCHandleC<RCHashBodyC<Key,Dat> >::BodyPtr(); }
    //: Access body.
  };

  template<class KeyT,class DataT >
  std::ostream &operator<<(std::ostream &strm,const RCHashC<KeyT,DataT> &table) {
    RavlAssert(table.IsValid());
    strm << table.Data();
    return strm;
  }

  template<class KeyT,class DataT >
  std::istream &operator>>(std::istream &strm,RCHashC<KeyT,DataT> &table) {
    table = RCHashC<KeyT,DataT>();
    strm >> table.Data();
    return strm;
  }

  template<class KeyT,class DataT >
  BinOStreamC &operator<<(BinOStreamC &strm,const RCHashC<KeyT,DataT> &table) {
    RavlAssert(table.IsValid());
    strm << table.Data();
    return strm;
  }
  
  template<class KeyT,class DataT >
  BinIStreamC &operator>>(BinIStreamC &strm,RCHashC<KeyT,DataT> &table) {
    table = RCHashC<KeyT,DataT>();
    strm >> table.Data();
    return strm;
  }
  
}

#endif
