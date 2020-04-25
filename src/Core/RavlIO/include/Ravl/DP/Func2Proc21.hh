// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FUNC2PROC21_HEADER
#define RAVL_FUNC2PROC21_HEADER 1
//! rcsid="$Id$"
//! lib=RavlIO
//! docentry="Ravl.API.Core.Data Processing.Processes" 
//! author="Charles Galambos"
//! date="23/7/2002"
//! file="Ravl/Core/IO/Func2Proc21.hh"

#include "Ravl/DP/Process21.hh"
#include "Ravl/FunctionRegister.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Access a 2 argument func to a process.
  
  template<class In1T,class In2T,class Out1T>
  class DPFunc2Proc21BodyC
    : public DPProcess21BodyC<In1T,In2T,Out1T>
  {
  public:
    typedef Out1T (*FuncT)(const In1T &,const In2T &);
    
    DPFunc2Proc21BodyC(FuncT nfunc,bool nstateless = true)
      : func(nfunc),
	stateless(nstateless)
    {}
    //: Constructor.
    
    DPFunc2Proc21BodyC(std::istream &in) 
      : DPProcess21BodyC<In1T,In2T,Out1T>(in)
    { LoadFunctionPointer(in,func); }
    //: Binary stream constructor.
    
    DPFunc2Proc21BodyC(BinIStreamC &in) 
      : DPProcess21BodyC<In1T,In2T,Out1T>(in)
    { LoadFunctionPointer(in,func); }
    //: Binary stream constructor.
    
    virtual bool Save(std::ostream &out) const {
      if(!DPProcess21BodyC<In1T,In2T,Out1T>::Save(out))
	return false;
      return SaveFunctionPointer(out,func); 
    }
    //: Save to std::ostream.
    
    virtual bool Save(BinOStreamC &out) const {
      if(!DPProcess21BodyC<In1T,In2T,Out1T>::Save(out))
	return false;
      return SaveFunctionPointer(out,func); 
    }
    //: Save to std::ostream.
    
    Out1T Apply(const In1T &dat1,const In2T &dat2)
    { return (*func)(dat1,dat2); }
    //: Apply operation.
    
    virtual bool IsStateless() const 
    { return stateless; }
    //: Is operation stateless ?
    
  protected:
    FuncT func;
    bool stateless;
  };

  //! userlevel=Develop
  //: Access a 2 argument func to a process.
  
  template<class In1T,class In2T,class Out1T>
  class DPFunc2Proc21C
    : public DPProcess21C<In1T,In2T,Out1T>
  {
  public:    
    typedef Out1T (*FuncT)(const In1T &,const In2T &);
    
    DPFunc2Proc21C(FuncT nfunc,bool stateless = true)
      : DPProcess21C<In1T,In2T,Out1T>(*new DPFunc2Proc21BodyC<In1T,In2T,Out1T>(nfunc,stateless))
    {}
    //: Constructor.
    
  };
  
  template<class In1T,class In2T,class Out1T>
  DPFunc2Proc21C<In1T,In2T,Out1T> Process(Out1T (*func)(const In1T &,const In2T &))
  { return DPFunc2Proc21C<In1T,In2T,Out1T>(func); }
  //: Create a process from a func.
  
}


#endif
