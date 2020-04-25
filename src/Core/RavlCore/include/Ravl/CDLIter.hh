// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVLCDLITER_HEADER
#define RAVLCDLITER_HEADER
///////////////////////////////////////////////////////
//! rcsid="$Id$"
//! docentry="Ravl.API.Core.Lists"
//! file="Ravl/Core/Container/DList/CDLIter.hh"
//! lib=RavlCore
//! userlevel=Normal
//! author="Charles Galambos"
//! date="24/01/2001"

#include "Ravl/DLIter.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Constant iterator.
  
  template<class DataT>
  class ConstDLIterC
    : public DLIterC<DataT>
  {
  public:
    ConstDLIterC()
      {}
    //: Default constructor.
    
    ConstDLIterC(const DListC<DataT> &nlst)
      : DLIterC<DataT>(nlst)
      {}
    //: Constructor.
  };
}

#endif
