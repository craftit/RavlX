// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPSTREAMOP31_HEADER
#define RAVL_DPSTREAMOP31_HEADER 1
/////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! date="16/7/2002"
//! author="Charles Galambos"
//! lib=RavlIO
//! docentry="Ravl.API.Core.Data Processing.Internal" 
//! file="Ravl/Core/IO/StreamOp31.hh"

#include "Ravl/DP/StreamProcess.hh"
#include "Ravl/DP/Process31.hh"

namespace RavlN {
  
  ////////////////////////////////////////
  //! userlevel=Develop
  //: Stream operation base class.
  
  template<class In1T,class In2T,class In3T,class Out1T>
  class DPIStreamOp31BodyC 
    : public DPStreamOpBodyC,
      public DPIPortBodyC<Out1T>
  {
  public:
    DPIStreamOp31BodyC(const DPProcess31C<In1T,In2T,In3T,Out1T> &nproc,
		       const DPIPortC<In1T> &input1,
		       const DPIPortC<In2T> &input2,
		       const DPIPortC<In3T> &input3)
      : DPIPortBodyC<Out1T>("Out1"),
	in1(input1),
	in2(input2),
	in3(input3),
	proc(nproc)
    {}
    //: Constructor.
    
    DPIStreamOp31BodyC(const DPProcess31C<In1T,In2T,In3T,Out1T> &nproc)
      : DPIPortBodyC<Out1T>("Out1"),
	proc(nproc)
    {}
    //: Constructor.
    
    virtual bool IsGetReady() const {
      if(!in1.IsValid() || !in2.IsValid() || !in3.IsValid())
	return false;
      return in1.IsGetReady() && in2.IsGetReady() && in3.IsGetReady();
    }
    //: Is some data ready ?
    // true = yes.
    
    virtual bool IsGetEOS() const {
      if(!in1.IsValid() || !in2.IsValid() || !in3.IsValid())
	return true;
      return in1.IsGetEOS() && in2.IsGetEOS() && in3.IsGetEOS();
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
      lst.InsLast(DPIPlugC<In3T>(in3,"In3",DPEntityC((DPEntityBodyC &)*this)));
      return lst;
    }
    //: List input plugs.
    
    virtual Out1T Get()  { 
      RavlAssert(in1.IsValid() && in2.IsValid() && in3.IsValid());
      return proc.Apply(in1.Get(),in2.Get(),in3.Get()); 
    }
    //: Process next piece of data.
    
    virtual bool Get(Out1T &outbuff) { 
      RavlAssert(in1.IsValid() && in2.IsValid() && in3.IsValid());
      In3T buff3;
      if(!in3.Get(buff3))
	return false;
      In2T buff2;
      if(!in2.Get(buff2))
	return false;
      In1T buff1;
      if(!in1.Get(buff1))
	return false;
      outbuff = proc.Apply(buff1,buff2,buff3);
      return true;
    }
    //: Process some data.
    
    inline DPIPortC<In1T> &Input1() { return in1; }
    // Access input port 1.
    
    inline DPIPortC<In2T> &Input2() { return in2; }
    // Access input port 1.
    
    inline DPIPortC<In3T> &Input3() { return in3; }
    // Access input port 1.
    
  protected:
    DPIPortC<In1T> in1;
    DPIPortC<In2T> in2;
    DPIPortC<In3T> in3;
    
    DPProcess31C<In1T,In2T,In3T,Out1T> proc;
  }; 
  
  ///////////////////////////////////
  //! userlevel=Normal
  //: Stream operation handle class.
  
  template<class In1T,class In2T,class In3T,class Out1T>
  class DPIStreamOp31C 
    : public DPStreamOpC,
      public DPIPortC<Out1T>
  {
  public:
    DPIStreamOp31C() 
      : DPEntityC(true)
    {}
    //: Default Constructor.
    
    DPIStreamOp31C(const DPProcess31C<In1T,In2T,In3T,Out1T> &nproc) 
      : DPEntityC(*new DPIStreamOp31BodyC<In1T,In2T,In3T,Out1T>(nproc))
    {}
    //: Construct from a process.
    
    DPIStreamOp31C(const DPProcess31C<In1T,In2T,In3T,Out1T> &nproc,
		   const DPIPortC<In1T> &input1,
		   const DPIPortC<In2T> &input2,
		   const DPIPortC<In3T> &input3) 
      : DPEntityC(*new DPIStreamOp31BodyC<In1T,In2T,In3T,Out1T>(nproc,input1,input2,input3))
    {}
    //: Construct from a process.
    
  protected:
    DPIStreamOp31C(const DPIStreamOp31BodyC<In1T,In2T,In3T,Out1T> &bod)
      : DPEntityC((DPIPortBodyC<Out1T> &) bod)
    {}
    //: Body constructor.
    
    DPIStreamOp31BodyC<In1T,In2T,In3T,Out1T> &Body()
    { return dynamic_cast<DPIStreamOp31BodyC<In1T,In2T,In3T,Out1T> &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPIStreamOp31BodyC<In1T,In2T,In3T,Out1T> &Body() const
    { return dynamic_cast<const DPIStreamOp31BodyC<In1T,In2T,In3T,Out1T> &>(DPEntityC::Body()); }
    //: Access body.
    
  public:  
    inline DPIPortC<In1T> &Input1() 
    { return Body().Input1(); }
    // Access input port 1.
    
    inline DPIPortC<In2T> &Input2()
    { return Body().Input2(); }
    // Access input port 2.
    
    inline DPIPortC<In3T> &Input3()
    { return Body().Input3(); }
    // Access input port 2.
    
  };
  
  template<class In1T,class In2T,class In3T,class Out1T>
  DPIStreamOp31C<In1T,In2T,In3T,Out1T> DPIStreamOp(const DPIPortC<In1T> &input1,
						   const DPIPortC<In2T> &input2,
						   const DPIPortC<In3T> &input3,
						   const DPProcess31C<In1T,In2T,In3T,Out1T> &nproc)
  { return DPIStreamOp31C<In1T,In2T,In3T,Out1T>(nproc,input1,input2,input3); }
  
}



#endif
