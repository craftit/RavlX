// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! rcsid="$Id$"
//! lib=RavlDMedia
//! file="Ravl/Contrib/DMedia/RingData.cc"

#include "Ravl/DMedia/RingData.hh"
#include "Ravl/DMedia/RingMemory.hh"
#include <string.h>

namespace RavlImageN
{
  //: Destructor.
  
  VLRingDataBodyC::~VLRingDataBodyC() 
   { Done(); }

  //: Done with data.
  
  void VLRingDataBodyC::Done() {
    if(seqNo == 0)
      return ;
    buff.DoneWith(*this);
    seqNo = 0;
    buff.Invalidate();
  }
  
  //: Copy data from another frame.
  
  bool VLRingDataBodyC::CopyFrom(VLRingDataC &oth) {
    RavlAssert(oth.IsValid());
    if(oth.BufferSize() != size) {
      cerr << "VLRingDataBodyC::CopyFrom(), Size mismatch " << oth.BufferSize() << " to " << size << "\n";
      return false;
    }
    char *from = (char *) oth.RawData();
    char *to = (char *) RawData();
    memcpy(to,from,size);
    return true;
  }
  
  
  
};
