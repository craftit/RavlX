// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPFUNCP2PROCESS_HEADER
#define RAVL_DPFUNCP2PROCESS_HEADER 1
///////////////////////////////////////////////
//! docentry="Ravl.API.Core.Data Processing.Processes" 
//! lib=RavlIO
//! rcsid="$Id$"
//! file="Ravl/Core/IO/FuncP2Proc.hh"
//! author="Charles Galambos"
//! date="09/07/1998"
//! userlevel=Normal
//! example=exDataProc.cc

#include "Ravl/DP/Process.hh"
#include "Ravl/DP/ProcCompose.hh"
#include "Ravl/FunctionRegister.hh"
#include "Ravl/Exception.hh"

namespace RavlN {

  //! userlevel=Develop
  //: Function ptr to process  body.
  
  template<class InT,class OutT>
  class DPFuncP2ProcBodyC 
    : public DPProcessBodyC<InT,OutT>
  {
  public:
    typedef OutT (*FuncT)(InT);
    FuncT func;
    
    DPFuncP2ProcBodyC() 
    {}
    //: Default constructor.
    
    DPFuncP2ProcBodyC(FuncT afunc) 
      : func(afunc)
    {}
    //: Constructor.

    DPFuncP2ProcBodyC(std::istream &in) 
      : DPProcessBodyC<InT,OutT>(in)
    { LoadFunctionPointer(in,func); }
    //: Stream constructor.
    
    DPFuncP2ProcBodyC(BinIStreamC &in) 
      : DPProcessBodyC<InT,OutT>(in)
    { LoadFunctionPointer(in,func); }
    //: Binary stream constructor.
    
    virtual bool Save(std::ostream &out) const {
      if(!DPProcessBodyC<InT,OutT>::Save(out))
	return false;
      return SaveFunctionPointer(out,func); 
    }
    //: Save to std::ostream.
    
    virtual bool Save(BinOStreamC &out) const {
      if(!DPProcessBodyC<InT,OutT>::Save(out))
	return false;
      return SaveFunctionPointer(out,func); 
    }
    //: Save to binary stream.  

    virtual OutT Apply(const InT &dat) 
    { return func(dat); }
    //: Apply operation.

    IntT ApplyArray(const SArray1dC<InT> &in,SArray1dC<OutT>  &out) {
      RavlAssert(in.Size() <= out.Size());
      for(SArray1dIter2C<InT,OutT> it(in,out);it;it++)
	it.Data2() = func(it.Data1());
      return in.Size();
    }
    //: Apply function to an array.
    
    virtual bool IsStateless() const 
    { return true; }
    //: Is operation stateless ?
    
  };
  
  //! userlevel=Develop
  //: Function ptr to process, where function has a constant reg as argument.
  
  template<class InT,class OutT>
  class DPCPFuncP2ProcBodyC : public DPProcessBodyC<InT,OutT>
  {
  public:
    typedef OutT (*FuncT)(const InT &);
    
    DPCPFuncP2ProcBodyC() 
    {}
    //: Default constructor.
    
    DPCPFuncP2ProcBodyC(FuncT afunc) 
      : func(afunc)
    {}
    //: Constructor.
    
    virtual OutT Apply(const InT &dat) 
    { return func(dat); }
    //: Apply operation.
    
    virtual bool IsStateless() const 
    { return true; }
    //: Is operation stateless ?
    
  protected:
    FuncT func;
  };

  //! userlevel=Normal
  //: Function ptr to process handle.
  
  template<class InT,class OutT>
  class DPFuncP2ProcC 
    : public DPProcessC<InT,OutT>
  {
  public:
    typedef OutT (*FuncT)(InT);
    typedef OutT (*CPFuncT)(const InT &);
    
    DPFuncP2ProcC(FuncT func)
      : DPProcessC<InT,OutT>(*new DPFuncP2ProcBodyC<InT,OutT>(func))
    {}
    //: Constructor.  
    
    DPFuncP2ProcC(CPFuncT func)
      : DPProcessC<InT,OutT>(*new DPCPFuncP2ProcBodyC<InT,OutT>(func))
    {}
    //: Constructor.  
  };

  template<class InT,class OutT>
  DPFuncP2ProcC<InT,OutT> Process(OutT (*func)(const InT &))
  { return DPFuncP2ProcC<InT,OutT>(func); }
  //: Turn a function into a process.
  
  template<class InT,class InterT,class OutT>
  DPProcessC<InT,OutT> operator>>(const DPProcessC<InT,InterT> &np1,OutT (*func)(InterT)) 
  { return DPComposeProcessC<InT,InterT,OutT>(np1,DPFunc2Proc(func)); }
  
  template<class InT,class InterT,class OutT>
  DPProcessC<InT,OutT> operator>>(InterT (*func)(InT),const DPProcessC<InterT,OutT> &np1)
  { return DPComposeProcessC<InT,InterT,OutT>(DPFunc2Proc(func),np1); }
  
  // Constant reference versions.
  
  template<class InT,class OutT>
  DPProcessC<InT,OutT> DPFunc2Proc(OutT (*func)(const InT &))
  { return DPFuncP2ProcC<InT,OutT>(func); }
  
  template<class InT,class InterT,class OutT>
  DPProcessC<InT,OutT> operator>>(const DPProcessC<InT,InterT> &np1,OutT (*func)(const InterT &)) 
  { return DPComposeProcessC<InT,InterT,OutT>(np1,DPFuncP2ProcC<InterT,OutT>(func)); }

  template<class InT,class InterT,class OutT>
  DPProcessC<InT,OutT> operator>>(InterT (*func)(const InT &),const DPProcessC<InterT,OutT> &np1)
  { return DPComposeProcessC<InT,InterT,OutT>(DPFuncP2ProcC<InT,InterT>(func),np1); }
  
#if 0
  // Problem: No paramiter is a class type!
  // Solution: Use the DPFunc2Proc() on one of the func ptrs.
  template<class InT,class InterT,class OutT>
  DPProcessC<InT,OutT> operator>>(InterT (*func1)(InT),OutT (*func2)(InterT))
  { return DPComposeProcessC<InT,InterT,OutT>(DPFuncP2ProcC<InT,InterT>(func1),DPFuncP2ProcC<InterT,OutT>(func2)); }
#endif

}
#endif
