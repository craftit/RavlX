// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DLISTEXTRA_HEADER
#define RAVL_DLISTEXTRA_HEADER 1
//! rcsid="$Id$"
//! lib=RavlCore
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Lists"
//! file="Ravl/Core/System/DListExtra.hh"

#include "Ravl/DList.hh"
#include "Ravl/DLIter.hh"
#include "Ravl/SArray1d.hh"
#include "Ravl/SArray1dIter.hh"

namespace RavlN {
  
  template<class DataT>
  SArray1dC<DataT> SArrayOf(const DListC<DataT> &list) {
    SArray1dC<DataT> ret(list.Size());
    DLIterC<DataT> lit(list);
    for(SArray1dIterC<DataT> it(ret);it;it++,lit++)
      *it = *lit;
    return ret;
  }
  //: Convert a DList to an SArray1dC.
  
  template<class DataT>
  DListC<DataT> DListOf(const SArray1dC<DataT> &arr) {
    DListC<DataT> ret;
    for(SArray1dIterC<DataT> it(arr);it;it++)
      ret.InsLast(*it);
    return ret;
  }
  //: Convert a SArray1dC to a DList
  
}



#endif
