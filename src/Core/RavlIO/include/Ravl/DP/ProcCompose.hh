// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPCOMPOSE_HEADER
#define RAVL_DPCOMPOSE_HEADER 1
///////////////////////////////////////////////
//! lib=RavlIO
//! docentry="Ravl.API.Core.Data Processing.Internal" 
//! author="Charles Galambos"
//! example=exDPMultiplex.cc
//! rcsid="$Id$"
//! file="Ravl/Core/IO/ProcCompose.hh"
//! date="04/07/1998"
//! userlevel=Default

#include "Ravl/DP/Process.hh"

namespace RavlN {
  //! userlevel=Develop
  //: Mix two processes together.
  
  template<class InT,class InterT,class OutT>
  class DPComposeProcessBodyC 
    : public DPProcessBodyC<InT,OutT> 
  {
  public:
    DPComposeProcessBodyC(const DPProcessC<InT,InterT> &np1,const DPProcessC<InterT,OutT> &np2)
      :  p1(np1),
	 p2(np2)
    {}
    //: Constructor.
    
    virtual bool Save(std::ostream &out) const;
    //: Save to std::ostream.
    
    virtual OutT Apply(const InT &di)
    { return p2.Apply(p1.Apply(di)); }
    //: Apply operation.
    
    virtual bool IsStateless() const
    { return p1.IsStateless() && p2.IsStateless(); }
    //: Is operation stateless ?
    
    virtual DPProcessBaseBodyC::ProcTypeT OpType() const;
    //: Operation type lossy/lossless.
    
    virtual RCBodyVC &Copy() const;
    //: Creat a copy of this object.
    
  private:
    DPProcessC<InT,InterT> p1;
    DPProcessC<InterT,OutT> p2;
  };
  
  ///////////////////////////////
  //: Mix two processes.
  //! userlevel=Normal
  
  template<class InT,class InterT,class OutT>
  class DPComposeProcessC 
    : public DPProcessC<InT,OutT> 
  {
  public:
    DPComposeProcessC(const DPProcessC<InT,InterT> &np1,const DPProcessC<InterT,OutT> &np2)
      : DPProcessC<InT,OutT>(*new DPComposeProcessBodyC<InT,InterT,OutT>(np1,np2))
    {}
    //: Constructor.
  };
  
  ///////////////////////////////
  //: Composition operator.
  
  template<class InT,class InterT,class OutT>
  DPProcessC<InT,OutT> operator>>(const DPProcessC<InT,InterT> &np1,const DPProcessC<InterT,OutT> &np2) 
  { return DPComposeProcessC<InT,InterT,OutT>(np1,np2); }
  
  //////////////////////////////////////////
  
  template<class InT,class InterT,class OutT>
  bool DPComposeProcessBodyC<InT,InterT,OutT>::Save(std::ostream &out) const {
    DPProcessBodyC<InT,OutT>::Save(out);  
    if(!p1.Save(out))
      return false;
    return p2.Save(out);
  }
  
  template<class InT,class InterT,class OutT>
  RCBodyVC &DPComposeProcessBodyC<InT,InterT,OutT>::Copy() const  { 
    if(p2.IsStateless() && p1.IsStateless()) 
      return const_cast<RCBodyVC &>((RCBodyVC &) *this);
    DPProcessC<InT,InterT> np1;
    DPProcessC<InterT,OutT> np2;
    if(p1.IsStateless())
      np1=p1;
    else
      np1=p1.Copy();
    if(p2.IsStateless())
      np2=p2;
    else
      np2=p2.Copy();  
    return *new DPComposeProcessBodyC<InT,InterT,OutT>(np1,np2);
  }
    
  template<class InT,class InterT,class OutT>
  DPProcessBaseBodyC::ProcTypeT DPComposeProcessBodyC<InT,InterT,OutT>::OpType() const  {
    if(p2.OpType() == DPProcessBaseBodyC::LossyT)
      return DPProcessBaseBodyC::LossyT;
    switch(p1.OpType()) {
    case DPProcessBaseBodyC::ConversionT:
      return p2.OpType();
    case DPProcessBaseBodyC::ConversionLossyT:
      return DPProcessBaseBodyC::ConversionLossyT;
    case DPProcessBaseBodyC::LossyT:
      return DPProcessBaseBodyC::LossyT;
    }
    return DPProcessBaseBodyC::LossyT;
  }
  
}
#endif
