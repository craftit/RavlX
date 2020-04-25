// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPIOCONNECT_HEADER
#define RAVL_DPIOCONNECT_HEADER 1
////////////////////////////////////////////////////////////////////
//! rcsid="$Id$"
//! lib=RavlIO
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Data Processing" 
//! date="06/07/1998"
//! example=exDataProc.cc
//! file="Ravl/Core/IO/IOConnect.hh"

#include "Ravl/DP/Port.hh"
#include "Ravl/DP/IOJoin.hh"
#include "Ravl/DP/StreamOp.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: IO Connector base
  
  class DPIOConnectBaseBodyC
    : public DPStreamOpBodyC,
      public AttributeCtrlBodyC
  {
  public:
    DPIOConnectBaseBodyC();
    //: Default constructor.
    
    virtual StringC OpName() const;
    //: Op type name.
    
    virtual bool Run();
    //: Run until a stream completes.

    virtual bool Stop();
    //: Attempt to stop stream processing.
    
    virtual bool Step();
    //: Do a single processing step.
    
    virtual bool IsReady() const;
    //: Check if we're ready to run.
    
    bool IsRunning() const
    { return running; }
    //: Check if we're already running.
    
    bool Running(bool state)
    { return (running = state); }
    //: Set running state.

    bool TryRun() {
      if(running)
	return false;
      running = true;
      return true;
    }
    //: Attempt to start running, fails if running already.
    // FIXME :- Turn into an atomic operation.
    
    virtual bool GetAttr(const StringC &attrName,bool &attrValue);
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const bool &attrValue);
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool GetAttr(const StringC &attrName,StringC &attrValue);
    //: Get a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
    virtual bool SetAttr(const StringC &attrName,const StringC &attrValue);
    //: Set a attribute.
    // Returns false if the attribute name is unknown.
    // This is for handling attributes such as frame rate, and compression ratios.
    
  protected:
    volatile bool running;
  };

  //! userlevel=Develop
  //: IO Connector base
  
  class DPIOConnectBaseC
    : public DPStreamOpC
  {
  public:
    DPIOConnectBaseC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Create an invalid handle.
    
  protected:
    DPIOConnectBaseC(DPIOConnectBaseBodyC &bod)
      : DPEntityC(bod)
    {}
    //: Body constructor.

    DPIOConnectBaseBodyC &Body() 
    { return dynamic_cast<DPIOConnectBaseBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPIOConnectBaseBodyC &Body() const
    { return dynamic_cast<const DPIOConnectBaseBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    bool Run()
    { return Body().Run(); }
    //: Do processing.
    
    bool Stop()
    { return Body().Stop(); }
    //: Attempt to stop stream processing.
    
    bool Step()
    { return Body().Step(); }
    //: Do a single processing step.
    
    bool IsReady() const
    { return Body().IsReady(); }
    //: Check if we're ready to run.
    
    bool IsRunning() const
    { return Body().IsRunning(); }
    //: Check if we're already running.
  };
  
  //////////////////////////
  //! userlevel=Develop
  //: Connect some IOPorts body.
  
  template<class DataT>
  class DPIOConnectBodyC 
    : public DPIOConnectBaseBodyC 
  {
  public:
    DPIOConnectBodyC(const DPIPortC<DataT> &from,const DPOPortC<DataT> &to);
    //: Constructor.
    
    bool Run(void);
    //: Do processing.
    
    bool Step();
    //: Do a single processing step.

    virtual DListC<DPIPlugBaseC> IPlugs() const {
      DListC<DPIPlugBaseC> ret;
      ret.InsLast(DPIPlugC<DataT>(from,DPEntityC(const_cast<DPIOConnectBodyC<DataT> &>(*this))));
      return ret;
    }
    //: Input plugs.
    
    virtual DListC<DPOPlugBaseC> OPlugs() const {
      DListC<DPOPlugBaseC> ret;
      ret.InsLast(DPOPlugC<DataT>(to,DPEntityC(const_cast<DPIOConnectBodyC<DataT> &>(*this))));
      return ret;
    }
    //: Output plugs
    
  private:
    DPIPortC<DataT> from;
    DPOPortC<DataT> to;
  };

  ////////////////////////////////
  //! userlevel=Normal
  //: Connect some IOPorts.
  
  template<class DataT>
  class DPIOConnectC
    : public DPIOConnectBaseC
  {
  public:
    DPIOConnectC(const DPIPortC<DataT> &from,const DPOPortC<DataT> &to)
      : DPEntityC(*new DPIOConnectBodyC<DataT>(from,to))
    {}
    //: Constructor.
    
  protected:
    inline DPIOConnectBodyC<DataT> &Body() 
    { return dynamic_cast<DPIOConnectBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body.
    
    inline const DPIOConnectBodyC<DataT> &Body() const
    { return dynamic_cast<const DPIOConnectBodyC<DataT> &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
  };
  
  //////////////////////////////
  
  template<class DataT>
  DPIOConnectC<DataT> Connect(const DPIPortC<DataT> &from,const DPOPortC<DataT> &to)
  { return DPIOConnectC<DataT>(from,to); }
  
  /////////////////////////////////////////
  
  template<class DataT>
  DPIOConnectBodyC<DataT>::DPIOConnectBodyC(const DPIPortC<DataT> &nfrom,const DPOPortC<DataT> &nto) 
    : from(nfrom),
      to(nto)
  {
    RavlAssert(from.IsValid());
    RavlAssert(to.IsValid());
  }
  
  template<class DataT>
  bool DPIOConnectBodyC<DataT>::Run(void) {
    if(!from.IsValid() || !to.IsValid())
      return false;
    DataT buff;
    Running(true);
    try {
      while(1) {
	if(!from.Get(buff))
	  break;
	if(!to.Put(buff))
	  break;
      }
    } catch(...) {
      Running(false);
      throw;
    }
    Running(false);
    to.PutEOS(); // Put a termination marker.
    return true;
  }
  
  template<class DataT>
  bool DPIOConnectBodyC<DataT>::Step() {
    if(!from.IsValid() || !to.IsValid())
      return false;
    if(!from.IsGetReady()) {
      to.PutEOS();
      return false;
    }
    if(!to.Put(from.Get()))
      return false;
    return true;
  }
  
  ///////////////////////////////
  
  //: Single Threaded composition operators.
  
  template<class DataT>
  DPIOConnectC<DataT> operator>>(const DPIPortC<DataT> &in,const DPOPortC<DataT> &out) { 
    DPIOConnectC<DataT> tmp(in,out);
    //cerr << "operator>>(const DPIPortC<DataT> &in,const DPOPortC<DataT> &out) Called. \n";
    tmp.Run();
    return tmp;
  }
  
  template<class DataT,class OutT>
  DPIOConnectC<DataT> operator>>(const DPIPortC<DataT> &in,const DPIOPortC<DataT,OutT> &out) { 
    DPIOConnectC<DataT> tmp(in,out); 
    //cerr << "operator>>(const DPIPortC<DataT> &in,const DPIOPortC<DataT,OutT> &out) Called. \n";
    tmp.Run();
    return tmp;
  }
  
  template<class InT,class DataT>
  DPOPortC<InT> operator>>(const DPIOPortC<DataT,InT> &in,const DPOPortC<DataT> &out)  { 
    DPIOConnectC<DataT> tmp(in,out); 
    //cerr << "operator>>(const DPIOPortC<DataT,InT> &in,const DPOPortC<DataT> &out) Called. \n";
    tmp.Run();
    return in;
  }

#if 0  
  template<class DataT>
  inline void operator>>= (const DPIPortC<DataT> &in,const DPOPortC<DataT> &out)
  { DPIOConnectC<DataT>(in,out).Dummy(); }
  
  template<class DataT,class OutT>
  inline DPIPortC<OutT> operator>>= (const DPIPortC<DataT> &in,const DPIOPortC<OutT,DataT> &out)
  { DPIOConnectC<DataT>(in,out).Dummy(); return out; }
  
  template<class InT,class DataT>
  inline DPOPortC<InT> operator>>= (const DPIOPortC<DataT,InT> &in,const DPOPortC<DataT> &out) 
  { DPIOConnectC<DataT>(in,out).Dummy(); return in; }
  
  template<class InT,class DataT,class OutT>
  inline DPIOPortC<InT,OutT> operator>>= (const DPIOPortC<DataT,OutT> &in,const DPIOPortC<InT,DataT> &out) { 
    DPIOConnectC<DataT>(in,out).Dummy(); 
    return DPIOPortJoinC<InT,OutT>(out,in); 
  }
#endif  
}
#endif
