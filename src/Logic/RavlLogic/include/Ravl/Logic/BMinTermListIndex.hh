// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_MINTERMLISTINDEX_HEADER
#define RAVL_MINTERMLISTINDEX_HEADER 1
////////////////////////////////////////////////////
//! userlevel=Normal
//! rcsid="$Id$"
//! file="Ravl/Logic/Index/BMinTermListIndex.hh"
//! lib=RavlLogic
//! author="Charles Galambos"
//! date="14/05/1998"
//! docentry="Ravl.API.Logic.Index"

#include "Ravl/Logic/BMinTermIndex.hh"
#include "Ravl/Logic/MinTermIter.hh"
#include "Ravl/Logic/Not.hh"
#include "Ravl/Logic/Or.hh"
#include "Ravl/BList.hh"

namespace RavlLogicN {
  
  //! userlevel=Normal
  //: This is a specialisation of VLNPIndex. 
  // Used in the Non-linear planner, but put
  // here in case it's usefull elsewhere.
  
  template <class DataT>
  class BMinTermListIndexC 
    : public BMinTermIndexC<BListC<DataT> > 
  {
  public:
    BMinTermListIndexC() 
    {}
    //: Default constructor.
    // Creates an empty index.
    
    BMinTermListIndexC(const BMinTermIndexC<BListC<DataT> > &oth)
      : BMinTermIndexC<BListC<DataT> >(oth)
    {}
    //: Base class constructor.
    
#if 0
    inline VLBNPIndexC<BListC<DataT> > DeepCopy() const
    { return VLBNPIndexC<BListC<DataT> >::DeepCopy(); }
    //: Make a deep copy of this class.
#endif  
    
    bool Insert(const LiteralC &symb,const DataT &dat);
    //: Insert data into all appropriate lists.
    
    bool Insert(const MinTermC &symb,const DataT &dat);
    //: Insert data into all appropriate lists.
  
  protected:
    void InsertEntry(bool negate,const LiteralC &as,const DataT &dat) {
      BListC<DataT> list;
      BMinTermIndexC<BListC<DataT> >::Lookup(negate,as,list);
      list.InsFirst(dat);
      BMinTermIndexC<BListC<DataT> >::Insert(negate,as,list);
    }
  };

  ///////////////////////////////////////////////////
  
  template <class DataT>
  bool BMinTermListIndexC<DataT>::Insert(const LiteralC &as,const DataT &dat) {
    AndC at(as);
    if(at.IsValid()) {
      InsertEntry(false,at,dat);
      for(SArray1dIterC<LiteralC> it(at.Terms());it;it++)
	Insert(as,dat);
      return true;
    }
    NotC nt(as);
    if(nt.IsValid()) {
      OrC ot(as);
      if(ot.IsValid()) {
	for(SArray1dIterC<LiteralC> it(ot.Terms());it;it++)
	  InsertEntry(true,*it,dat);
	return true;
      }
      InsertEntry(true,as,dat);
      return true;
    }
    InsertEntry(false,as,dat);
    return true;
  }
  
  template <class DataT>
  bool BMinTermListIndexC<DataT>::Insert(const MinTermC &mt,const DataT &dat) {
    for(MinTermIterC iter(mt);iter;iter++)
      InsertEntry(iter.IsNegated(),iter.Data(),dat);
    return true;
  }

}

#endif
