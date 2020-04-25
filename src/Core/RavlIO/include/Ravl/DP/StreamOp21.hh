// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPSTREAMOP21_HEADER
#define RAVL_DPSTREAMOP21_HEADER 1
/////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! date="16/7/2002"
//! author="Charles Galambos"
//! lib=RavlIO
//! docentry="Ravl.API.Core.Data Processing.Internal" 
//! file="Ravl/Core/IO/StreamOp21.hh"

#include "Ravl/DP/StreamProcess.hh"
#include "Ravl/DP/Process21.hh"

namespace RavlN {
  
  ////////////////////////////////////////
  //! userlevel=Develop
  //: Stream operation base class.
  
  template<class In1T,class In2T,class Out1T>
  class DPIStreamOp21BodyC 
    : public DPStreamOpBodyC,
      public DPIPortBodyC<Out1T>
  {
  public:
    DPIStreamOp21BodyC(const DPProcess21C<In1T,In2T,Out1T> &nproc,const DPIPortC<In1T> &input1,const DPIPortC<In2T> &input2)
      : DPIPortBodyC<Out1T>("Out1"),
	in1(input1),
	in2(input2),
	proc(nproc)
    {}
    //: Constructor.
    
    DPIStreamOp21BodyC(const DPProcess21C<In1T,In2T,Out1T> &nproc)
      : DPIPortBodyC<Out1T>("Out1"),
	proc(nproc)
    {}
    //: Constructor.
    
    virtual bool IsGetReady() const {
      if(!in1.IsValid() || !in2.IsValid())
	return false;
      return in1.IsGetReady() && in2.IsGetReady();
    }
    //: Is some data ready ?
    // true = yes.
    
    virtual bool IsGetEOS() const {
      if(!in1.IsValid() || !in2.IsValid())
	return true;
      return in1.IsGetEOS() && in2.IsGetEOS();
    }
    //: Has the End Of Stream been reached ?
    // true = yes.
    
    virtual DListC<DPIPortBaseC> IPorts() const {
      DListC<DPIPortBaseC> lst = DPStreamOpBodyC::IPorts();
      lst.InsLast(DPIPortBaseC((DPIPortBaseBodyC &)*this));
      return lst;
    }
    //: List input ports.
    
    virtual DListC<DPIPlugBaseC> IPlugs() const {
      DListC<DPIPlugBaseC> lst = DPStreamOpBodyC::IPlugs();
      lst.InsLast(DPIPlugC<In1T>(in1,"In1",DPEntityC((DPEntityBodyC &)*this)));
      lst.InsLast(DPIPlugC<In2T>(in2,"In2",DPEntityC((DPEntityBodyC &)*this)));
      return lst;
    }
    //: List input plugs.
    
    virtual Out1T Get()  { 
      RavlAssert(in1.IsValid() && in2.IsValid());
      return proc.Apply(in1.Get(),in2.Get());
    }
    //: Process next piece of data.
    
    virtual bool Get(Out1T &outbuff) { 
      RavlAssert(in1.IsValid() && in2.IsValid());
      In2T buff2;
      if(!in2.Get(buff2))
	return false;
      In1T buff1;
      if(!in1.Get(buff1))
	return false;
      outbuff = proc.Apply(buff1,buff2);
      return true;
    }
    //: Process some data.
    
    inline DPIPortC<In1T> &Input1() { return in1; }
    // Access input port 1.
    
    inline DPIPortC<In2T> &Input2() { return in2; }
    // Access input port 1.
    
  protected:
    DPIPortC<In1T> in1;
    DPIPortC<In2T> in2;
    
    DPProcess21C<In1T,In2T,Out1T> proc;
  }; 
  
  ///////////////////////////////////
  //! userlevel=Normal
  //: Stream operation handle class.
  
  template<class In1T,class In2T,class Out1T>
  class DPIStreamOp21C 
    : public DPStreamOpC,
      public DPIPortC<Out1T>
  {
  public:
    DPIStreamOp21C() 
      : DPEntityC(true)
    {}
    //: Default Constructor.
    
    DPIStreamOp21C(const DPProcess21C<In1T,In2T,Out1T> &nproc) 
      : DPEntityC(*new DPIStreamOp21BodyC<In1T,In2T,Out1T>(nproc))
    {}
    //: Construct from a process.
    
    DPIStreamOp21C(const DPProcess21C<In1T,In2T,Out1T> &nproc,const DPIPortC<In1T> &input1,const DPIPortC<In2T> &input2) 
      : DPEntityC(*new DPIStreamOp21BodyC<In1T,In2T,Out1T>(nproc,input1,input2))
    {}
    //: Construct from a process.
    
  protected:
    DPIStreamOp21C(const DPIStreamOp21BodyC<In1T,In2T,Out1T> &bod)
      : DPEntityC((DPIPortBodyC<Out1T> &) bod)
    {}
    //: Body constructor.
    
    DPIStreamOp21BodyC<In1T,In2T,Out1T> &Body()
    { return dynamic_cast<DPIStreamOp21BodyC<In1T,In2T,Out1T> &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPIStreamOp21BodyC<In1T,In2T,Out1T> &Body() const
    { return dynamic_cast<const DPIStreamOp21BodyC<In1T,In2T,Out1T> &>(DPEntityC::Body()); }
    //: Access body.
    
  public:  
    inline DPIPortC<In1T> &Input1() 
    { return Body().Input1(); }
    // Access input port 1.
    
    inline DPIPortC<In2T> &Input2()
    { return Body().Input2(); }
    // Access input port 2.
    
  };
  
  template<class In1T,class In2T,class Out1T>
  DPIStreamOp21C<In1T,In2T,Out1T> DPIStreamOp(const DPIPortC<In1T> &input1,
						   const DPIPortC<In2T> &input2,
						   const DPProcess21C<In1T,In2T,Out1T> &nproc)
  { return DPIStreamOp21C<In1T,In2T,Out1T>(nproc,input1,input2); }
  
  
}



#endif
