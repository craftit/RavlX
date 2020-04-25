// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2003, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlOSIO
//! file="Ravl/OS/IO/EditSpec.cc"

#include "Ravl/DP/EditSpec.hh"

namespace RavlN {
  
  //: Constructor.
  
  EditSpecBodyC::EditSpecBodyC(const SubSequenceSpecC &from,const IndexRangeC &ins) 
    : SubSequenceSpecBodyC(from),
      insertRange(ins)
  {}
  
  //: Constructor.
  
  EditSpecBodyC::EditSpecBodyC(const StringC &nfn,
			       IndexC nfirstFrame,
			       IndexC nlastFrame,
			       IntT nchannelId, 
			       const StringC &fmt ,
			       const IndexRangeC &ins)
    : SubSequenceSpecBodyC(nfn,nfirstFrame,nlastFrame,nchannelId,fmt),
      insertRange(ins)
  {}
  

}
