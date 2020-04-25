// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_METHOD2PROC31_HEADER
#define RAVL_METHOD2PROC31_HEADER 1
//! rcsid="$Id$"
//! lib=RavlIO
//! docentry="Ravl.API.Core.Data Processing.Processes" 
//! author="Charles Galambos"
//! date="23/7/2002"
//! file="Ravl/Core/IO/Method2Proc31.hh"

#include "Ravl/DP/Process31.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Access a 2 argument method to a process.
  
  template<class In1T,class In2T,class In3T,class Out1T,class ObjT>
  class DPMethod2Proc31BodyC
    : public DPProcess31BodyC<In1T,In2T,In3T,Out1T>
  {
  public:
    typedef Out1T (ObjT::*FuncT)(const In1T &,const In2T &,const In3T &);
    
    DPMethod2Proc31BodyC(const ObjT &nobj,FuncT nfunc,bool nstateless = true)
      : obj(nobj),
	func(nfunc),
	stateless(nstateless)
    {}
    //: Constructor.
    
    DPMethod2Proc31BodyC(std::istream &in) 
      : DPProcess31BodyC<In1T,In2T,In3T,Out1T>(in)
    { LoadFunctionPointer(in,func); }
    //: Binary stream constructor.
    
    DPMethod2Proc31BodyC(BinIStreamC &in) 
      : DPProcess31BodyC<In1T,In2T,In3T,Out1T>(in)
    { LoadFunctionPointer(in,func); }
    //: Binary stream constructor.
    
    virtual bool Save(std::ostream &out) const {
      if(!DPProcess31BodyC<In1T,In2T,In3T,Out1T>::Save(out))
	return false;
      return SaveFunctionPointer(out,func);
    }
    //: Save to std::ostream.
    
    virtual bool Save(BinOStreamC &out) const {
      if(!DPProcess31BodyC<In1T,In2T,In3T,Out1T>::Save(out))
	return false;
      return SaveFunctionPointer(out,func);
    }
    //: Save to std::ostream.
    
    Out1T Apply(const In1T &dat1,const In2T &dat2,const In3T &dat3)
    { return (obj.*func)(dat1,dat2,dat3); }
    //: Apply operation.
    
    virtual bool IsStateless() const 
    { return stateless; }
    //: Is operation stateless ?
    
  protected:
    ObjT obj;
    FuncT func;
    bool stateless;
  };

  //! userlevel=Develop
  //: Access a 2 argument method to a process.
  
  template<class In1T,class In2T,class In3T,class Out1T,class ObjT>
  class DPMethod2Proc31C
    : public DPProcess31C<In1T,In2T,In3T,Out1T>
  {
  public:    
    typedef Out1T (ObjT::*FuncT)(const In1T &,const In2T &,const In3T &);
    
    DPMethod2Proc31C(const ObjT &nobj,FuncT nfunc,bool stateless = true)
      : DPProcess31C<In1T,In2T,In3T,Out1T>(*new DPMethod2Proc31BodyC<In1T,In2T,In3T,Out1T,ObjT>(nobj,nfunc,stateless))
    {}
    //: Constructor.
    
  };
  
  template<class In1T,class In2T,class In3T,class Out1T,class ObjT>
  DPMethod2Proc31C<In1T,In2T,In3T,Out1T,ObjT> Process(const ObjT &obj,Out1T (ObjT::*method)(const In1T &,const In2T &,const In3T &))
  { return DPMethod2Proc31C<In1T,In2T,In3T,Out1T,ObjT>(obj,method); }
  //: Create a process from a method.
  
}


#endif
