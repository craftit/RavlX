// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlCore
//! author="Charles Galambos"
//! file="Ravl/Core/System/IndexNd.cc"

#include "Ravl/IndexNd.hh"

namespace RavlN {
  
  SizeT IndexNdC::Hash() const {
    size_t base = Size();
    size_t BM = (sizeof(UIntT) * 8) / (static_cast<size_t>(Size())+1);
    if(BM < 1) BM = 1;
    IntT shift = (sizeof(UIntT) * 8)-BM;
    for(BufferAccessIterC<IndexC> it(*this);it;it++,shift -= BM)
      base += (it->V() << shift) + it->V();
    return base;
  }
}
