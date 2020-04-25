// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPMETHOD2PROC_HEADER
#define RAVL_DPMETHOD2PROC_HEADER 1
///////////////////////////////////////////////
//! docentry="Ravl.API.Core.Data Processing.Processes"
//! lib=RavlIO
//! file="Ravl/Core/IO/Method2Proc.hh"
//! author="Charles Galambos"
//! date="09/07/1998"
//! rcsid="$Id$"

#include "Ravl/DP/Process.hh"
#include "Ravl/FunctionRegister.hh"

namespace RavlN {

  ////////////////////////////
  //! userlevel=Develop
  //: Methodtion body.
  
  template<typename InT,typename OutT,class ObjT,typename FuncT>
  class DPMethod2ProcBodyC 
    : public DPProcessBodyC<InT,OutT>
  {
  public:
    
    DPMethod2ProcBodyC(const ObjT &nobj,FuncT nfunc,bool stateless = true)
      : obj(nobj),
	func(nfunc),
	stateless(true)
    {}
    //: Constructor.
    
    DPMethod2ProcBodyC(std::istream &in) 
      : DPProcessBodyC<InT,OutT>(in)
    { LoadFunctionPointer(in,func); }
    //: Stream constructor.
    
    DPMethod2ProcBodyC(BinIStreamC &in) 
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
    { return (obj.*func)(dat); }
    //: Apply operation.
    
    virtual IntT ApplyArray(const SArray1dC<InT> &in,SArray1dC<OutT>  &out) { 
      RavlAssert(out.Size() >= in.Size());
      for(SArray1dIter2C<InT,OutT> it(in,out);it;it++)
	it.Data2() = (obj.*func)(it.Data1());
      return in.Size();
    }
    //: Apply operation to an array of elements.
    
    virtual bool IsStateless() const 
    { return stateless; }
    //: Is operation stateless ?
    
  protected:
    ObjT obj;
    FuncT func;
    bool stateless;
  };
  
  ///////////////////////////////
  //! userlevel=Advanced
  //: Methodtion handle.
  
  template<typename InT,typename OutT,class ObjT,typename FuncT>
  class DPMethod2ProcC
    : public DPProcessC<InT,OutT>
  {
  public:
    DPMethod2ProcC(const ObjT &nobj,FuncT func,bool stateless = true)
      : DPProcessC<InT,OutT>(*new DPMethod2ProcBodyC<InT,OutT,ObjT,FuncT>(nobj,func,stateless))
    {}
    //: Default constructor.
  };


  //! userlevel=Normal
  
  template<typename InT,typename OutT,class ObjT>
  inline DPProcessC<InT,OutT> Process(const ObjT &nclass,OutT (ObjT::*nmethod)(const InT &))
  { 
    typedef OutT (ObjT::*DPMethod2ProcCFuncT)(const InT &);
    return DPMethod2ProcC<InT,OutT,ObjT,DPMethod2ProcCFuncT>(nclass,nmethod); 
  }
  //: Turn a function into a process.

  template<typename InT,typename OutT,class ObjT>
  inline DPProcessC<InT,OutT> Process(const ObjT &nclass,OutT (ObjT::*nmethod)(const InT &) const)
  { 
    typedef OutT (ObjT::*DPMethod2ProcCConstFuncT)(const InT &) const;
    return DPMethod2ProcC<InT,OutT,ObjT,DPMethod2ProcCConstFuncT>(nclass,nmethod); 
  }
  //: Turn a function into a process.
  
}

#endif
