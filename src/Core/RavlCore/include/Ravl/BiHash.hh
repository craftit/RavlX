// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BIHASH_HEADER
#define RAVL_BIHASH_HEADER 1
///////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Hash/BiHash.hh"
//! lib=RavlCore
//! author="Charles Galambos"
//! date="1/9/1996"
//! example=exBiHash.cc
//! docentry="Ravl.API.Core.Hash Tables"
//! rcsid="$Id$"
//! userlevel=Default

#include "Ravl/Hash.hh"
#include "Ravl/HashIter.hh"
#include "Ravl/BinStream.hh"

namespace RavlN {

  template<class D1T,class D2T> class BiHashIterC;
  
  //! userlevel=Normal
  //: Bi-directional hash table.
  // This allows a reversable mapping between two hashed items.
  // For quick mapping in both directions.
  // Forward is nominally from D1T to D2T.
  
  template<class D1T,class D2T>
  class BiHashC {
  public:
    BiHashC(UIntT nBins = 23) 
      : hmap1(nBins),
	hmap2(nBins)
    {}
    //: Constructor.
    
    void Save(BinOStreamC &strm) const
    { strm << hmap1; }
    //: Save to binary stream.
    
    void Save(std::ostream &strm) const
    { strm << hmap1; }
    //: Save to text stream.

    void Load(BinIStreamC &strm) { 
      hmap1.Empty();
      strm >> hmap1; 
      hmap2.Empty();
      // Build other hash table.
      for(HashIterC<D1T,D2T> it(hmap1);it;it++)
        hmap2[it.Data()] = it.Key();
    }
    //: Load from binary stream.
    // The existing table is replaced.
    
    void Load(std::istream &strm) { 
      hmap1.Empty();
      strm >> hmap1; 
      hmap2.Empty();
      // Build other hash table.
      for(HashIterC<D1T,D2T> it(hmap1);it;it++)
        hmap2[it.Data()] = it.Key();
    }
    //: Load from text stream.
    // The existing table is replaced.
    
    D2T &Map1(const D1T &key)
    { return hmap1[key]; }
    //: Do a lookup on data type 1
    
    D2T &Fwd(const D1T &key)
    { return hmap1[key]; }
    //: Do a lookup on data type 1
    
    bool LookupFwd(const D1T &key,D2T &data) const
    { return hmap1.Lookup(key,data); }
    //: Lookup forward.
    // Return true if entry is found and copy 
    // result into 'data'. Otherwise returns
    // false and data is unaffected.
    
    bool IsFwd(const D1T &key) const
    { return hmap1.IsElm(key); }
    //: Do we have a forward mapping.
    
    D1T &Map2(const D2T &key)
    { return hmap2[key]; }
    //: Do a lookup on data type 2;
    
    D1T &Bkw(const D2T &key)
    { return hmap2[key]; }
    //: Do a lookup on data type 2

    bool LookupBkw(const D2T &key,D1T &data) const
    { return hmap2.Lookup(key,data); }
    //: Lookup backward.
    // Return true if entry is found and copy 
    // result into 'data'. Otherwise returns
    // false and data is unaffected.
    
    bool IsBkw(const D2T &key) const
    { return hmap2.IsElm(key); }
    //: Do we have a forward mapping.
    
    bool Insert(const D1T &d1,const D2T &d2) {
      hmap1[d1] = d2;
      hmap2[d2] = d1;
      return true;
    }
    //: Insert a mapping into the table.
    // Should check that they are reversable ?
    
    bool DelMap1(const D1T &key) {
      D2T *d2 = hmap1.Lookup(key);
      if(d2 == 0)
	return false;
      bool ret = hmap2.Del(*d2);
      ret &= hmap1.Del(key);
      return ret;
    }
    //: Delete a mapping from the table, using d1 as the key.
    
    bool DelMap2(const D2T &key) {
      D1T *d1 = hmap2.Lookup(key);
      if(d1 == 0)
	return false;
      bool ret = hmap1.Del(*d1);
      ret &= hmap2.Del(key);
      return ret;
    }
    //: Delete a mapping from the table, using d2 as the key.
    
    SizeT Size() const
    { return hmap1.Size(); }
    //: Get number of mappings.

    void Empty() {
      hmap1.Empty();
      hmap2.Empty();
    }
    //: Clear table of all contents.
  protected:
    HashC<D1T,D2T> hmap1;
    HashC<D2T,D1T> hmap2;
    
    friend class BiHashIterC<D1T,D2T>;
  };
  
  //! userlevel=Normal
  //: Bi-directional hash table iterator
  
  template<class D1T,class D2T>
  class BiHashIterC
    : public HashIterC<D1T,D2T>
  {
  public:
    BiHashIterC(const BiHashC<D1T,D2T> &bihash)
      : HashIterC<D1T,D2T>(bihash.hmap1)
    {}
    
    D1T &Data1()
    { return this->Key(); }
    //: Access data1.
    
    D2T &Data2()
    { return this->Data(); }
    //: Access data1.
  };

  template<class D1T,class D2T>
  BinOStreamC &operator<<(BinOStreamC &strm,const BiHashC<D1T,D2T> &table) {
    table.Save(strm);
    return strm;
  }
  
  template<class D1T,class D2T>
  BinIStreamC &operator>>(BinIStreamC &strm,BiHashC<D1T,D2T> &table) {
    table.Load(strm);    
    return strm;
  }

  template<class D1T,class D2T>
  std::ostream &operator<<(std::ostream &strm,const BiHashC<D1T,D2T> &table) {
    table.Save(strm);
    return strm;
  }

  template<class D1T,class D2T>
  std::istream &operator>>(std::istream &strm,BiHashC<D1T,D2T> &table) {
    table.Load(strm);    
    return strm;
  }

}
  
#endif
