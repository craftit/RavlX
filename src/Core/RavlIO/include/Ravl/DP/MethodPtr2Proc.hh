// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPMETHODPTR2PROC_HEADER
#define RAVL_DPMETHODPTR2PROC_HEADER 1
///////////////////////////////////////////////
//! docentry="Ravl.API.Core.Data Processing.Processes"
//! lib=RavlIO
//! file="Ravl/Core/IO/Method2Proc.hh"
//! author="Charles Galambos"
//! date="09/07/1998"

#include "Ravl/DP/Process.hh"
#include "Ravl/FunctionRegister.hh"

namespace RavlN {

  ////////////////////////////
  //! userlevel=Develop
  //: Method body.
  
  template<typename InT,typename OutT,class ObjPtrT,typename FuncT>
  class DPMethodPtr2ProcBodyC
    : public DPProcessBodyC<InT,OutT>
  {
  public:
    
    DPMethodPtr2ProcBodyC(const ObjPtrT &nobj,FuncT nfunc,bool stateless = true)
      : m_obj(nobj),
	m_func(nfunc),
	m_stateless(true)
    {}
    //: Constructor.
    
    DPMethodPtr2ProcBodyC(std::istream &in)
      : DPProcessBodyC<InT,OutT>(in)
    { LoadFunctionPointer(in,m_func); }
    //: Stream constructor.
    
    DPMethodPtr2ProcBodyC(BinIStreamC &in)
      : DPProcessBodyC<InT,OutT>(in)
    { LoadFunctionPointer(in,m_func); }
    //: Binary stream constructor.
    
    virtual bool Save(std::ostream &out) const {
      if(!DPProcessBodyC<InT,OutT>::Save(out))
	return false;
      return SaveFunctionPointer(out,m_func); 
    }
    //: Save to std::ostream.
    
    virtual bool Save(BinOStreamC &out) const {
      if(!DPProcessBodyC<InT,OutT>::Save(out))
	return false;
      return SaveFunctionPointer(out,m_func); 
    }
    //: Save to binary stream.  
    
    virtual OutT Apply(const InT &dat) 
    { return (m_obj->*m_func)(dat); }
    //: Apply operation.
    
    virtual IntT ApplyArray(const SArray1dC<InT> &in,SArray1dC<OutT>  &out) { 
      RavlAssert(out.Size() >= in.Size());
      for(SArray1dIter2C<InT,OutT> it(in,out);it;it++)
	it.Data2() = (m_obj->*m_func)(it.Data1());
      return in.Size();
    }
    //: Apply operation to an array of elements.
    
    virtual bool IsStateless() const 
    { return m_stateless; }
    //: Is operation stateless ?
    
  protected:
    ObjPtrT m_obj;
    FuncT m_func;
    bool m_stateless;
  };
  
  ///////////////////////////////
  //! userlevel=Advanced
  //: Method ptr handle.
  
  template<typename InT,typename OutT,class ObjPtrT,typename FuncT>
  class DPMethodPtr2ProcC
    : public DPProcessC<InT,OutT>
  {
  public:
    DPMethodPtr2ProcC(const ObjPtrT &nobj,FuncT func,bool stateless = true)
      : DPProcessC<InT,OutT>(*new DPMethodPtr2ProcBodyC<InT,OutT,ObjPtrT,FuncT>(nobj,func,stateless))
    {}
    //: Default constructor.
  };


  //! userlevel=Normal
  
  template<typename InT,typename OutT,class ObjT,class ObjPtrT>
  inline DPProcessC<InT,OutT> ProcessPtr(const ObjPtrT &nclass,OutT (ObjT::*nmethod)(const InT &))
  { 
    typedef OutT (ObjT::*DPMethodPtr2ProcCFuncT)(const InT &);
    return DPMethodPtr2ProcC<InT,OutT,ObjPtrT,DPMethodPtr2ProcCFuncT>(nclass,nmethod);
  }
  //: Turn a function into a process.

  template<typename InT,typename OutT,class ObjT,class ObjPtrT>
  inline DPProcessC<InT,OutT> ProcessPtr(const ObjPtrT &nclass,OutT (ObjT::*nmethod)(const InT &) const)
  { 
    typedef OutT (ObjT::*DPMethodPtr2ProcCConstFuncT)(const InT &) const;
    return DPMethodPtr2ProcC<InT,OutT,ObjPtrT,DPMethodPtr2ProcCConstFuncT>(nclass,nmethod);
  }
  //: Turn a function into a process.
  
}

#endif
