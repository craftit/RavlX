// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPMETHODIO_HEADER
#define RAVL_DPMETHODIO_HEADER 1
//////////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Data Processing.IO"
//! example=exDataProc.cc
//! lib=RavlIO
//! author="Charles Galambos"
//! date="04/07/1998"
//! example=exMethodIO.cc
//! file="Ravl/Core/IO/MethodIO.hh"

#include "Ravl/DP/Port.hh"

namespace RavlN {
  ////////////////////////////////////////////
  //! userlevel=Develop
  //: Send objects to a class method.
  
  template<class DataT,class ObjT,class RetT = bool>
  class DPOMethodBodyC 
    : public DPOPortBodyC<DataT>
  {
  public:
    typedef RetT (ObjT::*Func1T)(const DataT &dat);
    
    DPOMethodBodyC() 
      : func(0)
    {}
    //: Default constructor.
    
    DPOMethodBodyC(const ObjT &nobj,Func1T meth)
      : obj(nobj),
	func(meth)
    {}
    //: Construct from a filename.
    
    virtual bool Put(const DataT &dat) 
    { (obj.*func)(dat); return true; }
    //: Put data.
    
    virtual IntT PutArray(const SArray1dC<DataT> &data) {
      for(SArray1dIterC<DataT> it(data);it;it++)
	(obj.*func)(*it);
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
    ObjT obj;
    Func1T func;
  };
  
  /////////////////////////////////////
  //! userlevel=Develop
  //: Get objects from a class method.
  
  template<class DataT,class ObjT>
  class DPIMethodBodyC 
    : public DPIPortBodyC<DataT>
  {
  public:
    typedef DataT (ObjT::*Func1T)();
    
    DPIMethodBodyC() 
      : func(0)
    {}
    //: Default constructor.
    
    DPIMethodBodyC(const ObjT &nobj,Func1T meth)
      : obj(nobj),
	func(meth)
    {}
    //: Construct from a filename.
    
    virtual bool IsGetEOS() const 
    { return func == 0; }
    //: Is valid data ?
    
    virtual DataT Get()
    { return (obj.*func)(); }
    //: Get next piece of data.
    
    virtual IntT GetArray(SArray1dC<DataT> &data) {
      for(SArray1dIterC<DataT> it(data);it;it++)
	*it = (obj.*func)();
      return data.Size();
    }
    //: Get an array of data from stream.
    
    virtual bool Save(std::ostream &out) const 
    {  return false; }
    //: Save to std::ostream.
    
  private:
    ObjT obj;
    Func1T func;
  };
  
  ///////////////////////////////
  //! userlevel=Normal
  //: Send objects to a class method.
  
  template<class DataT,class ObjT,class RetT = bool>
  class DPOMethodC 
    : public DPOPortC<DataT> 
  {
  public:
    inline DPOMethodC() 
    {}
    //: Default constructor.
    
    inline DPOMethodC(const ObjT &nobj,typename DPOMethodBodyC<DataT,ObjT,RetT>::Func1T meth)
      : DPEntityC(*new DPOMethodBodyC<DataT,ObjT,RetT>(nobj,meth))
    {}
    //: Constructor.
    
  };
  
  //////////////////////////////////
  //! userlevel=Normal
  //: Get objects from a class method.
  
  template<class DataT,class ObjT>
  class DPIMethodC 
    : public DPIPortC<DataT> 
  {
  public:
    inline DPIMethodC() 
    {}
    //: Default constructor.
    
    inline DPIMethodC(const ObjT &nobj,typename DPIMethodBodyC<DataT,ObjT>::Func1T meth)
      : DPEntityC(*new DPIMethodBodyC<DataT,ObjT>(nobj,meth))
    {}
    //: Constructor.
    
  };
  
  template<class DataT,class ObjT>
  DPIPortC<DataT> IMethod(const ObjT &nobj,DataT (ObjT::*meth)())
  { return DPIMethodC<DataT,ObjT>(nobj,meth); }
  //: Turn a class method into an input port.
  
  template<class DataT,class ObjT,class RetT>
  DPOPortC<DataT> OMethod(const ObjT &nobj,RetT (ObjT::*meth)(const DataT &dat))
  { return DPOMethodC<DataT,ObjT,RetT>(nobj,meth); }
  //: Turn a class method into an output port.
  
};

#endif
