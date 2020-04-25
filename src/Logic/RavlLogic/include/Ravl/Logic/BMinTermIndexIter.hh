// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLLOGIC_BMINTERMINDEXITER_HEADER
#define RAVLLOGIC_BMINTERMINDEXITER_HEADER 1
///////////////////////////////////////////
//! userlevel=Normal
//! rcsid="$Id$"
//! file="Ravl/Logic/Index/BMinTermIndexIter.hh"
//! lib=RavlLogic
//! author="Charles Galambos"
//! date="14/05/1998"
//! docentry="Ravl.API.Logic.Index"

#include "Ravl/Logic/BMinTermIndex.hh"
#include "Ravl/Logic/LiteralIndexFilter.hh"
#include "Ravl/Logic/Tuple.hh"
#include "Ravl/HSet.hh"

namespace RavlLogicN {
  
  //! userlevel=Normal
  //: Branching MinTerm Index iterator.
  
  template<class DataT>
  class BMinTermIndexIterC {
  public:
    inline BMinTermIndexIterC(const BMinTermIndexC<DataT> &index,const MinTermC &cond);
    //: Construct from an index.
    
    inline BMinTermIndexIterC(const BMinTermIndexC<DataT> &index,const LiteralC &lit);
    //: Construct from an index.

    inline BMinTermIndexIterC(const BMinTermIndexIterC<DataT> &);
    //: Copy constructor.
    
    inline DataT &Data() {
      RavlAssert(data != 0);
      return *data;
    }
    //: Access data.
    
    inline const DataT &Data() const {
      RavlAssert(data != 0);
      return *data;
    }
    //: Access data.
    
    DataT &operator*() 
    { return Data(); }
    //: Access data.
    
    const DataT &operator*() const
    { return Data(); }
    //: Access data.
    
    inline bool First();
    //: Goto first item.
  
    inline bool Next()  {
      iter.Next();
      return CheckValid();
    }
    //: goto next item.
  
    void operator++(int)
    { Next(); }
    //: goto next element.
    
    inline bool IsElm() const
    { return data != 0; }
    //: At a valid element ?
    
    operator bool() const
    { return data != 0; }
    //: At a valid element ?
  private:
    bool CheckValid();
    //: Goto next entry if not on valid one else return false.
    
    MinTermIterC mtiter;    // Iterate through minterms.
    LiteralIndexFilterC<IntC> iter; // Filtered index handle.
    HSetC<IntC> ndone;      // Only do each entry once.
    HSetC<IntC> pdone;      // Only do each entry once.
    BMinTermIndexC<DataT> ind; // Index handle.
    DataT *data;            // Short cut to data.
  };

  ///////////////////////////////////////////////////
  
  template<class DataT>
  inline
  BMinTermIndexIterC<DataT>::BMinTermIndexIterC(const BMinTermIndexC<DataT> &index,const MinTermC &cond) 
    : mtiter(cond),
      ind(index),
      data(0)
  {
    if(mtiter.IsElm()) {
      iter = LiteralIndexFilterC<IntC>(ind.IdIndex(),*mtiter);
      CheckValid();
    }
  }

  template<class DataT>
  inline
  BMinTermIndexIterC<DataT>::BMinTermIndexIterC(const BMinTermIndexC<DataT> &index,const LiteralC &symb)
    : mtiter(MinTermC(symb,false)),
      ind(index),
      data(0)
  {
    if(mtiter.IsElm()) {
      iter = LiteralIndexFilterC<IntC>(ind.IdIndex(),*mtiter);
      CheckValid();
    }
  }
  
  template<class DataT>
  inline 
  BMinTermIndexIterC<DataT>::BMinTermIndexIterC(const BMinTermIndexIterC<DataT> &old) 
    : mtiter(old.mtiter),
      iter(old.iter),
      ndone(old.ndone),
      pdone(old.pdone),
      ind(old.ind),
      data(old.data)
  {}
  
  template<class DataT>
  inline 
  bool BMinTermIndexIterC<DataT>::First() {
    if(!mtiter.First()) {
      data = 0; // Make sure IsElm() return false.
      return false;
    }
    ndone.Empty();
    pdone.Empty();
    iter = LiteralIndexFilterC<IntC>(ind.IdIndex(),*mtiter);
    RavlAssert(iter.IsValid());
    return CheckValid();
  }
  
  template<class DataT>
  bool BMinTermIndexIterC<DataT>::CheckValid() {
    RavlAssert(iter.IsValid());
    do {
      while(!iter) {
	if(!mtiter.Next()) {
	  data = 0;
	  return false; // Done.
	}
	iter = LiteralIndexFilterC<IntC>(ind.IdIndex(),*mtiter);
      }
      if(mtiter.IsNegated()) {
	// Look up in negated index.
	if(!ndone.Insert(iter.MappedData().v)) {
	  iter.Next();
	  continue;
	}
	data = ind.neg.tab.Lookup(iter.MappedData());
      } else {
	if(!pdone.Insert(iter.MappedData().v)) {
	  iter.Next();
	  continue;
	}
	data = ind.pos.tab.Lookup(iter.MappedData().v);
      }
      if(data != 0) 
	break;
      iter.Next();
    } while(true) ;
    return true;
  }
  
}

#endif
