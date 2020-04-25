// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! date="16/7/2002"
//! author="Charles Galambos"
//! lib=RavlIO
//! file="Ravl/Core/IO/StreamProcess.cc"

#include "Ravl/DP/StreamProcess.hh"
#include "Ravl/HashIter.hh"

namespace RavlN {

  DPStreamProcessBodyC::DPStreamProcessBodyC()
  {}
  //: Default constructor.

  DPStreamProcessBodyC::DPStreamProcessBodyC(const StringC &name)
    : procname(name)
  {}
  //: Construct a stream process.


  //: Input plugs.
  
  DListC<DPIPlugBaseC> DPStreamProcessBodyC::IPlugs() const {
    DListC<DPIPlugBaseC> ret;
    for(HashIterC<StringC,DPIPlugBaseC> it(iplugs);it;it++)
      ret.InsFirst(it.Data());
    return ret;
  }
  
  //: Output plugs
  
  DListC<DPOPlugBaseC> DPStreamProcessBodyC::OPlugs() const {
    DListC<DPOPlugBaseC> ret;
    for(HashIterC<StringC,DPOPlugBaseC> it(oplugs);it;it++)
      ret.InsFirst(it.Data());
    return ret;
  }
  
  //: Input ports.
  
  DListC<DPIPortBaseC> DPStreamProcessBodyC::IPorts() const {
    DListC<DPIPortBaseC> ret;
    for(HashIterC<StringC,DPIPortBaseC> it(iports);it;it++)
      ret.InsFirst(it.Data());
    return ret;
  }
  
  //: Output ports
  
  DListC<DPOPortBaseC> DPStreamProcessBodyC::OPorts() const {
    DListC<DPOPortBaseC> ret;
    for(HashIterC<StringC,DPOPortBaseC> it(oports);it;it++)
      ret.InsFirst(it.Data());
    return ret;
  }

}
