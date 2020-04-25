// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_BINITER_HEADER
#define RAVL_BINITER_HEADER 1
///////////////////////////////////////////////////////////////
//! file="Ravl/Core/Container/Misc/BinIter.hh"
//! userlevel=Normal
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Misc"
//! date="27/11/1996"
//! rcsid="$Id$"
//! lib=RavlCore

#include "Ravl/BinTable.hh"
#include "Ravl/HashIter.hh"

namespace RavlN {

  //: Bin table iterator.
  // SMALL OBJECT
  
  template<class IT,class DIT,class BT>
  class BinIterC : public HashIterC<DIT,BT> {
  public:
    BinIterC(const BinTableC<IT,DIT,BT> &Tab)
      : HashIterC<DIT,BT>(Tab.bins) {}
    // Constructor.
    
    BinIterC(const BinIterC<IT,DIT,BT> &anoth)
      : HashIterC<DIT,BT>(anoth) {}
    // Copy Constructor.
    
    BinIterC<IT,DIT,BT> &operator=(const BinIterC<IT,DIT,BT> &oth) {
      HashIterC<DIT,BT>::operator=(oth);
      return *this;
    }
    // Assignment to another iterator.
    
    BinIterC<IT,DIT,BT> &operator=(const BinTableC<IT,DIT,BT> &oth) {
      HashIterC<DIT,BT>::operator=(oth.bins);
      return *this;
    }
    // Assignment to another table.
    
  };
  
  template<class IT,class DIT,class BT>
  std::ostream &operator<<(std::ostream &out,const BinIterC<IT,DIT,BT> &) {
    RavlAssertMsg(0,"operator<<(std::ostream &out,const BinIterC<IT,DIT,BT> &), NOT IMPLEMENTED ");
    return out;
  }
}

#endif


