// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_BLITERALINDEX_HEADER
#define RAVLLOGIC_BLITERALINDEX_HEADER 1
///////////////////////////////////////////////////////
//! userlevel=Normal
//! docentry="Ravl.API.Logic.Index"
//! author="Charles Galambos"
//! rcsid="$Id$"
//! file="Ravl/Logic/Index/BLiteralIndex.hh"
//! lib=RavlLogic
//! date="14/05/1998"

#include "Ravl/BHash.hh"
#include "Ravl/Logic/LiteralIndex.hh"
#include "Ravl/IntC.hh"

namespace RavlLogicN {
  
  template<class DataT> class BMinTermIndexIterC;
  template<class DataT> class BLiteralIndexIterC;
  
  //! userlevel=Normal
  //: Branching literal index.
  // SMALL OBJECT
  
  template<class DataT>
  class BLiteralIndexC {
  public:
    inline BLiteralIndexC()
      : ids(true)
    {}
    //: Default constructor.
    // Creates a new table.
    
    inline BLiteralIndexC(const BLiteralIndexC<DataT> &oth,
			  bool shareIDOnly = false)
      : ids(oth.ids),
	tab(oth.tab)
    {
      if(!shareIDOnly)
	tab = oth.tab.Copy();
    }
    //: Copy constructor.
    // If shareIDOnly is true then a new index is created, which shares
    // the ID index of the old table but not its contents. Usefull if
    // you wish to make several indexes with similar sets of keys.
    
    inline BLiteralIndexC(const LiteralIndexC<IntC> &nids)
      : ids(nids)
    {}
    //: Construct using a given id Index.
    // Otherwise creates a new index, another way of doing share only.
    // see above.
    
#if 0
    BLiteralIndexC<DataT> DeepCopy() const
    { return BLiteralIndexC<DataT>(ids,tab.DeepCopy()); }
    //: Make a deep copy of the index.
#endif
    
    inline const DataT &operator[](const LiteralC &cnd) {
      IntC &it = ids[cnd];
      if(it == 0)
	it = ids.Size();
      return tab[it.v];
    }
    //: Lookup an entry.
    // Creates a new entry if one doesn't exist.

    bool Lookup(const LiteralC &key,DataT &data) { 
      IntC v;
      if(!ids.Lookup(key,v))
	return false;
      return tab.Lookup(v,data);     
    } 
    //: Copy data associated with 'key' to data if it exists.
    // Returns true if data has been found.
    
    bool IsElm(const LiteralC &key) {
      IntC v;
      if(!ids.Lookup(key,v))
	return false;
      return tab.IsElm(v);
    }
    //: Is 'key' in index ?
    
    inline void Insert(const LiteralC &cnd,const DataT &data) { 
      IntC &it = ids[cnd];
      if(it == 0)
	it = ids.Size();
      tab.Insert(it,data); 
    }
    //: Add a new item to the index.
    
    inline bool IsEmpty() const
    { return tab.IsEmpty(); }
    //: Is table empty ?
    
    UIntT Size() const
    { return tab.Size(); }
    //: How many entries ?
    
    bool UseIndex(const LiteralIndexC<IntC> &nIds) {
      RavlAssert(IsEmpty());
      if(!IsEmpty())
	return false;
      ids = nIds;
      return true;
    }
    //: Use another index, you can only use this
    // function if there are no entries in the table.
  
    const LiteralIndexC<IntC> &IdIndex() const
    { return ids; }
    //: Get ID index.

  protected:
    inline BLiteralIndexC(const LiteralIndexC<IntC> &nIds,const BHashC<IntT,DataT> &nTab)
      : ids(nIds), 
	tab(nTab)
    {}
    //: Construct using an index and a table.
    
  private:
    LiteralIndexC<IntC> ids;
    BHashC<IntT,DataT> tab;
    friend class BMinTermIndexIterC<DataT>;
    friend class BLiteralIndexIterC<DataT>;
  };
}
#endif
