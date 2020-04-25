// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
///////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlIO
//! file="Ravl/Core/IO/ProcInfo.cc"

#include "Ravl/DP/ProcInfo.hh"

#include "Ravl/DList.hh"
#include "Ravl/DLIter.hh"

namespace RavlN {
  ////////////////////////////
  //: Default constructor.
  
  DPProcInfoBodyC::DPProcInfoBodyC()
 { Procs().InsFirst(DPProcInfoC(*this)); }
  
  /////////////////////////////
  //: Get type of process
  
  const std::type_info &DPProcInfoBodyC::ProcType() const
  { return typeid(void); }
  
  /////////////////////////////
  //: Number of args for process.
  
  IntT DPProcInfoBodyC::Args() const
  { return 0; }
  
  /////////////////////////////
  //: Type of args.
  
  const std::type_info &DPProcInfoBodyC::ArgType(IntT) const
  { return typeid(void); }
  
  //////////////////////////////
  //: Output type.
  
  const std::type_info &DPProcInfoBodyC::Output() const
  { return typeid(void); }
  
  ////////////////////////////////
  //: Create instance of process.
  
  DPProcessBaseC DPProcInfoBodyC::CreateProc() const { 
    RavlAssert(0); // Should never be called.
    return DPProcessBaseC(); 
  }
  
  ////////////////////////////////
  //: Create instance of process.
  
  DPIPortBaseC DPProcInfoBodyC::CreateIStream(const DPIPortBaseC &inp) const {
    RavlAssert(0);
    return DPIPortBaseC();
  }
  
  ////////////////////////////////
  //: Create instance of process.
  
  DPOPortBaseC DPProcInfoBodyC::CreateOStream(const DPOPortBaseC &outp) const  {
    RavlAssert(0);
    return DPOPortBaseC();
  }
  
  /////////////////////////////////
  //: Apply process using abstract data handle.
  // These should be RCWrap's of the actual data.
  
  RCAbstractC 
  DPProcInfoBodyC::Apply(const DPProcessBaseC &proc,SArray1dC<RCAbstractC> dat) {
    RavlAssert(0);
    return RCAbstractC();
  }
  
  //: Apply process using abstract data handle.
  // Use only is you know the process uses 1 arg ! 
  
  RCAbstractC 
  DPProcInfoBodyC::Apply1(const DPProcessBaseC &proc,const RCAbstractC &arg)  {
    RavlAssert(0);
    return RCAbstractC();
  }
  
  //: Apply process using abstract data handle.
  // Use only is you know the process uses 2 args ! 
  
  RCAbstractC 
  DPProcInfoBodyC::Apply2(const DPProcessBaseC &proc,const RCAbstractC &arg1,const RCAbstractC &arg2) {
    RavlAssert(0);
    return RCAbstractC();
  }
  
  DListC<DPProcInfoC> &DPProcInfoBodyC::Procs()  {
    static DListC<DPProcInfoC> procList;
    return procList;
  }
  
}
