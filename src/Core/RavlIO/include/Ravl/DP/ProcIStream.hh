// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPPROCISTREAM_HEADER
#define RAVL_DPPROCISTREAM_HEADER 1
/////////////////////////////////////////////////////
//! example=exDataProc.cc
//! file="Ravl/Core/IO/ProcIStream.hh"
//! lib=RavlIO
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Data Processing.Internal" 
//! date="06/07/1998"
//! userlevel=Default

#include "Ravl/DP/StreamOp.hh"
#include "Ravl/DP/Process.hh"
#include "Ravl/DP/IOJoin.hh"

namespace RavlN {
  //! userlevel=Develop
  //: Convert process into input stream operator, body.
  
  template<class InT,class OutT>
  class DPProcIStreamBodyC 
    : public DPProcessC<InT,OutT>,
      public DPIStreamOpBodyC<InT,OutT>
  {
  public:
    DPProcIStreamBodyC(const DPProcessC<InT,OutT> &bod,const DPIPortC<InT> &nin)
      : DPProcessC<InT,OutT>(bod),
	DPIStreamOpBodyC<InT,OutT>(nin)
    {}
    //: Constructor.

    DPProcIStreamBodyC(const DPProcessC<InT,OutT> &bod)
      : DPProcessC<InT,OutT>(bod)
    {}
    //: Constructor.
    
    DPProcIStreamBodyC(const DPProcIStreamBodyC<InT,OutT> &oth);
    //: Copy Constructor.
    
    virtual OutT Get()  { 
      RavlAssert(this->input.IsValid());
      return this->Apply(this->input.Get());
    }
    //: Process next piece of data.
    
    virtual bool Get(OutT &outbuff) { 
      InT buff;
      RavlAssert(this->input.IsValid());
      if(!this->input.Get(buff))
	return false;
      outbuff = this->Apply(buff);
      return true;
    }
    //: Process some data.  
    
    virtual IntT GetArray(SArray1dC<OutT> &dest) {
      SArray1dC<InT> src(dest.Size());
      IntT ret = this->input.GetArray(src);
      // we did not get any data so no point continuing
      if(ret <= 0)
        return ret;
      src = SArray1dC<InT>(src,ret);
//comment ap variable if assertions are not active
#if RAVL_CHECK
      IntT ap = 
#endif
	this->ApplyArray(src,dest);
      RavlAssert(ap == (IntT) src.Size()); // Expect the all to be processed.
      return ret;
    }
    //: Get Array of data.
    
    virtual RCBodyVC &Copy() const { 
      if(!this->IsStateless())
	return *new DPProcIStreamBodyC(*this); 
      return const_cast<RCBodyVC &>((RCBodyVC &)*this);
    }
    //: Creat a copy of this object.
    
    virtual bool Save(std::ostream &out) const 
    { return DPIStreamOpBodyC<InT,OutT>::Save(out); }
    //: Save to std::ostream.
    
  }; 
  
  //! userlevel=Normal
  //: Convert process into input stream operator, handle.
  
  template<class InT,class OutT>
  class DPProcIStreamC 
    : public DPIStreamOpC<InT,OutT> 
  {
  public:
    DPProcIStreamC() 
      : DPEntityC(true)
    {}
    //: Default Constructor.
    // Creates an invalid handle.
    
    DPProcIStreamC(const DPProcessC<InT,OutT> &bod,const DPIPortC<InT> &nin)
      : DPEntityC((DPIPortBodyC<OutT> &) *new DPProcIStreamBodyC<InT,OutT>(bod,nin))
    {}
    //: Constructor.

    DPProcIStreamC(const DPProcessC<InT,OutT> &bod)
      : DPEntityC((DPIPortBodyC<OutT> &) *new DPProcIStreamBodyC<InT,OutT>(bod))
    {}
    //: Constructor.
    
    DPProcIStreamC(const DPProcIStreamC<IntT,OutT> &oth) 
      : DPEntityC(oth),
	DPIStreamOpC<InT,OutT>(oth)
    {}
    //: Copy Constructor.
    
  };
  
  ///////////////////////////////
  //: Composition operator.
  
  template<class InT,class OutT>
  DPIPortC<OutT> operator>>(const DPIPortC<InT> &in,const DPProcessC<InT,OutT> &proc) 
  { return (const DPIPortC<OutT> &) DPProcIStreamC<InT,OutT> (proc,in); }
  
  template<class InT,class OutT,class InterT>
  DPIOPortC<OutT,InterT> operator>>(const DPIOPortC<InT,InterT> &in,const DPProcessC<InT,OutT> &proc) 
  { return DPIOPortJoin(DPProcIStreamC<InT,OutT> (proc,in),in); }
  
  //////////////////////////////////////////////////////////
  
  template<class InT,class OutT>
  DPProcIStreamBodyC<InT,OutT>::DPProcIStreamBodyC(const DPProcIStreamBodyC<InT,OutT> &oth)
    : DPProcessC<InT,OutT>(oth), //.Copy()
      DPIStreamOpBodyC<InT,OutT>(oth)
  {}
  //: Copy Constructor.
  
}
#endif
