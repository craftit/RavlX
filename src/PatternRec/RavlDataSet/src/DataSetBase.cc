// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlPatternRec
//! file="Ravl/PatternRec/DataSet/DataSetBase.cc"

#include "Ravl/PatternRec/DataSetBase.hh"
#include "Ravl/HSet.hh"

#define DODEBUG 0
#if DODEBUG
#define ONDEBUG(x) x
#else
#define ONDEBUG(x)
#endif

namespace RavlN {
  
  //: Generate a random index set from this collection of with the given size.
  // Size is limited to the size of this set.
  
  CollectionC<UIntT> DataSetBaseBodyC::RandomIndexSet(UIntT size) {
    CollectionC<UIntT> ret(size);
#if 0
    if(size < (UIntT) ((RealT) Size() * 0.8)) {
      for(UIntT i = 0;i < Size();i++)
	ret.InsertRandom(i);
      // Take the first size elements from the collection.
      return CollectionC<UIntT>(SArray1dC<UIntT>(ret.Array(),size));
    }
    
    HSetC<UIntT> done;
    while(ret.Size() < size) {
      UIntT ni = (UIntT)((RealT) Random1() * Size());
      if(done[ni])
	continue;
      ret.Insert(ni);
      done += ni;
    }
#endif
    return ret;
  }

  //: Shuffle the order of the dataset.
  
  void DataSetBaseBodyC::Shuffle() {
    RavlAssertMsg(0,"DataSetBaseBodyC::Shuffle(), Abstract method called. ");
  }
  
}
