// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_FUNC2PROC31_HEADER
#define RAVL_FUNC2PROC31_HEADER 1
//! rcsid="$Id$"
//! lib=RavlIO
//! docentry="Ravl.API.Core.Data Processing.Processes" 
//! author="Charles Galambos"
//! date="23/7/2002"
//! file="Ravl/Core/IO/Func2Proc31.hh"

#include "Ravl/DP/Process31.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Access a 2 argument func to a process.
  
  template<class In1T,class In2T,class In3T,class Out1T>
  class DPFunc2Proc31BodyC
    : public DPProcess31BodyC<In1T,In2T,In3T,Out1T>
  {
  public:
    typedef Out1T (*FuncT)(const In1T &,const In2T &,const In3T &);
    
    DPFunc2Proc31BodyC(FuncT nfunc,bool nstateless = true)
      : func(nfunc),
	stateless(nstateless)
    {}
    //: Constructor.
    
    DPFunc2Proc31BodyC(std::istream &in) 
      : DPProcess31BodyC<In1T,In2T,In3T,Out1T>(in)
    { LoadFunctionPointer(in,func); }
    //: Binary stream constructor.
    
    DPFunc2Proc31BodyC(BinIStreamC &in) 
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
    { return (*func)(dat1,dat2,dat3); }
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
  
  template<class In1T,class In2T,class In3T,class Out1T>
  class DPFunc2Proc31C
    : public DPProcess31C<In1T,In2T,In3T,Out1T>
  {
  public:    
    typedef Out1T (*FuncT)(const In1T &,const In2T &,const In3T &);
    
    DPFunc2Proc31C(FuncT nfunc,bool stateless = true)
      : DPProcess31C<In1T,In2T,In3T,Out1T>(*new DPFunc2Proc31BodyC<In1T,In2T,In3T,Out1T>(nfunc,stateless))
    {}
    //: Constructor.
    
  };
  
  template<class In1T,class In2T,class In3T,class Out1T>
  DPFunc2Proc31C<In1T,In2T,In3T,Out1T> Process(Out1T (*func)(const In1T &,const In2T &,const In3T &))
  { return DPFunc2Proc31C<In1T,In2T,In3T,Out1T>(func); }
  //: Create a process from a func.
  
}


#endif
