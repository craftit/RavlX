// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPMETHODPTRIO_HEADER
#define RAVL_DPMETHODPTRIO_HEADER 1
//////////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Data Processing.IO"
//! example=exDataProc.cc
//! lib=RavlIO
//! author="Charles Galambos"
//! date="13//2008"
//! example=exMethodIO.cc
//! rcsid="$Id: MethodIO.hh 5240 2005-12-06 17:16:50Z plugger $"
//! file="Ravl/Core/IO/MethodIO.hh"

#include "Ravl/DP/Port.hh"

namespace RavlN {
  ////////////////////////////////////////////
  //! userlevel=Develop
  //: Send objects to a class method.
  
  template<typename DataT,typename ObjT,typename PtrT,class RetT = bool>
  class DPOMethodPtrBodyC 
    : public DPOPortBodyC<DataT>
  {
  public:
    typedef RetT (ObjT::*Func1T)(const DataT &dat);
    
    DPOMethodPtrBodyC() 
      : func(0)
    {}
    //: Default constructor.
    
    DPOMethodPtrBodyC(const PtrT &nobj,Func1T meth)
      : obj(nobj),
	func(meth)
    {}
    //: Construct from a filename.
    
    virtual bool Put(const DataT &dat) 
    { (obj->*func)(dat); return true; }
    //: Put data.
    
    virtual IntT PutArray(const SArray1dC<DataT> &data) {
      for(SArray1dIterC<DataT> it(data);it;it++)
	(obj->*func)(*it);
      return data.Size();
    }
    //: Get an array of data from stream.
    
    virtual bool IsPutReady() const 
    { return func != 0; }
    //: Is port ready for data ?
    
    virtual bool Save(std::ostream &sout) const 
    {  return false; } // Not implemented
    //: Save to std::ostream.
    
  private:
    PtrT obj;
    Func1T func;
  };
  
  /////////////////////////////////////
  //! userlevel=Develop
  //: Get objects from a class method.
  
  template<class DataT,class ObjT,typename PtrT>
  class DPIMethodPtrBodyC 
    : public DPIPortBodyC<DataT>
  {
  public:
    typedef DataT (ObjT::*Func1T)();
    
    DPIMethodPtrBodyC() 
      : func(0)
    {}
    //: Default constructor.
    
    DPIMethodPtrBodyC(const PtrT &nobj,Func1T meth)
      : obj(nobj),
	func(meth)
    {}
    //: Construct from a filename.
    
    virtual bool IsGetEOS() const 
    { return func == 0; }
    //: Is valid data ?
    
    virtual DataT Get()
    { return (obj->*func)(); }
    //: Get next piece of data.
    
    virtual IntT GetArray(SArray1dC<DataT> &data) {
      for(SArray1dIterC<DataT> it(data);it;it++)
	*it = (obj->*func)();
      return data.Size();
    }
    //: Get an array of data from stream.
    
    virtual bool Save(std::ostream &out) const 
    {  return false; }
    //: Save to std::ostream.
    
  private:
    PtrT obj;
    Func1T func;
  };
  
  ///////////////////////////////
  //! userlevel=Normal
  //: Send objects to a class method.
  
  template<typename DataT,typename ObjT,typename PtrT,class RetT = bool>
  class DPOMethodPtrC 
    : public DPOPortC<DataT> 
  {
  public:
    inline DPOMethodPtrC() 
    {}
    //: Default constructor.
    
    inline DPOMethodPtrC(const PtrT &nobj,typename DPOMethodPtrBodyC<DataT,ObjT,PtrT,RetT>::Func1T meth)
      : DPEntityC(*new DPOMethodPtrBodyC<DataT,ObjT,PtrT,RetT>(nobj,meth))
    {}
    //: Constructor.
    
  };
  
  //////////////////////////////////
  //! userlevel=Normal
  //: Get objects from a class method.
  
  template<typename DataT,typename ObjT,typename PtrT>
  class DPIMethodPtrC 
    : public DPIPortC<DataT> 
  {
  public:
    inline DPIMethodPtrC() 
    {}
    //: Default constructor.
    
    inline DPIMethodPtrC(const PtrT &nobj,typename DPIMethodPtrBodyC<DataT,ObjT,PtrT>::Func1T meth)
      : DPEntityC(*new DPIMethodPtrBodyC<DataT,ObjT,PtrT>(nobj,meth))
    {}
    //: Constructor.
    
  };
  
  template<class DataT,class ObjT,typename PtrT>
  DPIPortC<DataT> IMethodPtr(const PtrT &nobj,DataT (ObjT::*meth)())
  { return DPIMethodPtrC<DataT,ObjT,PtrT>(nobj,meth); }
  //: Turn a class method into an input port.
  
  template<class DataT,class ObjT,typename PtrT,typename RetT>
  DPOPortC<DataT> OMethodPtr(const PtrT &nobj,RetT (ObjT::*meth)(const DataT &))
  { return DPOMethodPtrC<DataT,ObjT,PtrT,RetT>(nobj,meth); }
  //: Turn a class method into an output port.
  
};

#endif
