// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPSTREAMOP_HEADER
#define RAVL_DPSTREAMOP_HEADER 1
/////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Data Processing.Internal" 
//! file="Ravl/Core/IO/StreamOp.hh"
//! lib=RavlIO
//! author="Charles Galambos"
//! date="09/07/1998"
//! userlevel=Normal

#include "Ravl/DP/Port.hh"
#include "Ravl/DP/Plug.hh"
#include "Ravl/DList.hh"
#include "Ravl/SysLog.hh"

namespace RavlN {

  class XMLFactoryContextC;
  
  //! userlevel=Develop
  //: Abstract stream operation.
  
  class DPStreamOpBodyC
    : virtual public DPEntityBodyC 
  {
  public:
    DPStreamOpBodyC();
    //: Default constructor.

    DPStreamOpBodyC(const StringC &entityName);
    //: Constructor.

    DPStreamOpBodyC(std::istream &in);
    //: Stream constructor.
    
    DPStreamOpBodyC(BinIStreamC &in);
    //: Binary stream constructor.

    void Setup(const XMLFactoryContextC &factory);
    //: Setup using an xml factory.
    // This can only be called after the class is constructed
    // and the IPlugs,OPlugs,IPorts and OPorts methods are functional.
    // As well as requiring libRavlXMLFactory; this method also requires
    // libRavlOSIO to be used.

    virtual StringC OpName() const;
    //: Op type name.
    
    virtual DListC<DPIPlugBaseC> IPlugs() const;
    //: Input plugs.
    
    virtual DListC<DPOPlugBaseC> OPlugs() const;
    //: Output plugs
    
    virtual DListC<DPIPortBaseC> IPorts() const;
    //: Input ports.
    
    virtual DListC<DPOPortBaseC> OPorts() const;
    //: Output ports

    virtual bool GetIPlug(const StringC &name,DPIPlugBaseC &port);
    //: Get input

    virtual bool GetOPlug(const StringC &name,DPOPlugBaseC &port);
    //: Get output

    virtual bool SetIPort(const StringC &name,const DPIPortBaseC &port);
    //: Set an input

    virtual bool SetOPort(const StringC &name,const DPOPortBaseC &port);
    //: Set an output

    virtual bool GetOPort(const StringC &name,DPOPortBaseC &port);
    //: Get output

    virtual bool GetIPort(const StringC &name,DPIPortBaseC &port);
    //: Get input

    bool GetOPort(const StringC &name,DPOPortBaseC &port,const std::type_info &dataType);
    //: Get output as specified type

    bool GetIPort(const StringC &name,DPIPortBaseC &port,const std::type_info &dataType);
    //: Get input as specified type

    template<typename TypeT>
    bool GetIPort(const StringC &name,DPIPortC<TypeT> &thePort) {
      DPIPortBaseC itmp;
      if(!GetIPort(name,itmp,typeid(TypeT)))
        return false;
      RavlAssert(itmp.IsValid());
      DPIPortC<TypeT> aPort(itmp);
      if(!aPort.IsValid()) {
        RavlError("Unexpected type '%s', expecting '%s' ",itmp.InputType().name(),typeid(TypeT).name());
        return false;
      }
      thePort = aPort;
      RavlAssert(thePort.IsValid());
      return true;
    }
    //: Get input port.

    template<typename TypeT>
    bool GetOPort(const StringC &name,DPOPortC<TypeT> &thePort) {
      DPOPortBaseC otmp;
      if(!GetOPort(name,otmp,typeid(TypeT)))
        return false;
      RavlAssert(otmp.IsValid());
      DPOPortC<TypeT> aPort(otmp);
      if(!aPort.IsValid()) {
        RavlError("Unexpected type '%s', expecting '%s' ",otmp.OutputType().name(),typeid(TypeT).name());
        return false;
      }
      thePort = aPort;
      RavlAssert(thePort.IsValid());
      return true;
    }
    //: Get input port.

    virtual bool Dump(std::ostream &strm) const;
    //: Dump information about the stream op.

    typedef RavlN::SmartPtrC<DPStreamOpBodyC> RefT;
    //: Handle to class
  };

  //! userlevel=Normal
  //: Abstract stream operation.
  
  class DPStreamOpC
    : virtual public DPEntityC 
  {
  public:
    DPStreamOpC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
    DPStreamOpC(DPEntityC &ent)
      : DPEntityC(dynamic_cast<const DPStreamOpBodyC *>(BodyPtr(ent)))
    {}
    //: Base class constructor.
    // If object is not a DPStreamOpC then an invalid handle will
    // be created.
    
    DPStreamOpC(const RCAbstractC &bod)
     : DPEntityC(dynamic_cast<const DPStreamOpBodyC *>(bod.BodyPtr()))
    {}
    //: Construct from an abstract handle
    // This handle will be invalid if handle types don't match.

    DPStreamOpC(const DPStreamOpBodyC *bod)
      : DPEntityC(bod)
    {}
    //: Body ptr constructor.
  protected:
    DPStreamOpC(DPStreamOpBodyC &bod)
      : DPEntityC(bod)
    {}
    //: Body constructor.
    
    DPStreamOpBodyC &Body()
    { return dynamic_cast<DPStreamOpBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPStreamOpBodyC &Body() const
    { return dynamic_cast<const DPStreamOpBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    StringC OpName() const
    { return Body().OpName(); }
    //: Op type name.
    
    DListC<DPIPlugBaseC> IPlugs() const
    { return Body().IPlugs(); }
    //: Input plugs.
    
    DListC<DPOPlugBaseC> OPlugs() const
    { return Body().OPlugs(); }
    //: Output plugs
    
    DListC<DPIPortBaseC> IPorts() const
    { return Body().IPorts(); }
    //: Input ports.
    
    DListC<DPOPortBaseC> OPorts() const
    { return Body().OPorts(); }
    //: Output ports

    bool GetIPlug(const StringC &name,DPIPlugBaseC &port)
    { return Body().GetIPlug(name,port); }
    //: Get input

    bool GetOPlug(const StringC &name,DPOPlugBaseC &port)
    { return Body().GetOPlug(name,port); }
    //: Get output

    bool SetIPort(const StringC &name,const DPIPortBaseC &port)
    { return Body().SetIPort(name,port); }
    //: Set an input

    bool SetOPort(const StringC &name,const DPOPortBaseC &port)
    { return Body().SetOPort(name,port); }
    //: Set an output

    bool GetOPort(const StringC &name,DPOPortBaseC &port)
    { return Body().GetOPort(name,port); }
    //: Get output

    bool GetIPort(const StringC &name,DPIPortBaseC &port)
    { return Body().GetIPort(name,port); }
    //: Get input

    template<typename TypeT>
    bool GetIPort(const StringC &name,DPIPortC<TypeT> &thePort)
    { return Body().GetIPort(name,thePort); }
    //: Get input port.

    template<typename TypeT>
    bool GetOPort(const StringC &name,DPOPortC<TypeT> &thePort)
    { return Body().GetOPort(name,thePort); }
    //: Get output port.

    bool Dump(std::ostream &strm) const
    { return Body().Dump(strm); }
    //: Dump information about the stream op.

  };

  
  ////////////////////////////////////////
  //! userlevel=Develop
  //: Stream operation base class.
  
  template<class InT,class OutT>
  class DPIStreamOpBodyC 
    : public DPIPortBodyC<OutT>,
      public DPStreamOpBodyC
  {
  public:
    DPIStreamOpBodyC()
      : DPIPortBodyC<OutT>("Out1")
    {
      DPEntityBodyC::SetEntityName("Out1");
    }
    //: Default constructor.
    
    DPIStreamOpBodyC(const DPIPortC<InT> &nin)
      : DPIPortBodyC<OutT>("Out1"),
        input(nin)
    {
      DPEntityBodyC::SetEntityName("Out1");
    }
    //: Constructor.
    
    DPIStreamOpBodyC(std::istream &in) 
      : DPEntityBodyC(in),
        DPIPortBodyC<OutT>(in),
        DPStreamOpBodyC(in)
    {}
    //: Stream constructor.
    
    DPIStreamOpBodyC(BinIStreamC &in) 
      : DPEntityBodyC(in),
        DPIPortBodyC<OutT>(in),
        DPStreamOpBodyC(in)
    {}
    //: Binary stream constructor.
    
    virtual bool IsGetReady() const {
      if(!input.IsValid())
        return false;
      return input.IsGetReady(); 
    }
    //: Is some data ready ?
    // true = yes.
    
    virtual bool IsGetEOS() const {
      if(!input.IsValid())
        return true;
      return input.IsGetEOS(); 
    }
    //: Has the End Of Stream been reached ?
    // true = yes.
    
    virtual DPPortC ConnectedTo() const
    { return input; }
    //: What does this connect to ?
    
    virtual DListC<DPIPortBaseC> IPorts() const {
      DListC<DPIPortBaseC> lst = DPStreamOpBodyC::IPorts();
      lst.InsLast(DPIPortBaseC((DPIPortBaseBodyC &)*this));
      return lst;
    }
    //: Input ports.

    virtual DListC<DPIPlugBaseC> IPlugs() const {
      DListC<DPIPlugBaseC> lst = DPStreamOpBodyC::IPlugs();
      DPIPlugC<InT> plug(input,"In1",DPEntityC((DPEntityBodyC &)*this));
      lst.InsLast(plug);
      return lst;
    }
    //: Input plugs.
    
    virtual bool Save(std::ostream &out) const 
    { return DPIPortBodyC<OutT>::Save(out); }
    //: Save to std::ostream.
    
    virtual AttributeCtrlC ParentCtrl() const 
    { return AttributeCtrlC(input); }
    //: Get Parent attribute control.
    
  protected:
    DPIPortC<InT> input; // Where to get data from.
    
  public:
    inline DPIPortC<InT> &Input() { return input; }
    //: Access input port.
    
    virtual void Input(const DPIPortC<InT> &ins) {
      input = ins; 
      this->ReparentAttributeCtrl(input); // Make sure changed signals are changed appropriately.
    }
    //: Setup input port.
  }; 
  
  ///////////////////////////////////
  //! userlevel=Normal
  //: Stream operation handle class.
  
  template<class InT,class OutT>
  class DPIStreamOpC 
    : public DPIPortC<OutT>,
      public DPStreamOpC
  {
  public:
    DPIStreamOpC() 
      : DPEntityC(true)
    {}
    //: Default Constructor.
    
    DPIStreamOpC(const DPIStreamOpC<InT,OutT> &oth) 
      : DPEntityC(oth),
	DPIPortC<OutT>(oth)
    {}
    //: Copy Constructor.
    
    DPIStreamOpC<InT,OutT> &operator=(const DPIStreamOpC<InT,OutT> &oth) {
      DPEntityC::operator=(oth);
      return *this;
    }
    //: Assignment.
    
  protected:
    DPIStreamOpC(const DPIStreamOpBodyC<InT,OutT> &bod)
      : DPEntityC((DPIPortBodyC<OutT> &) bod)
    {}
    //: Body constructor.
    
    DPIStreamOpBodyC<InT,OutT> &Body()
    { return dynamic_cast<DPIStreamOpBodyC<InT,OutT> & >(DPEntityC::Body()); }
    //: Access body.
    
    const DPIStreamOpBodyC<InT,OutT> &Body() const
    { return dynamic_cast<const DPIStreamOpBodyC<InT,OutT> & >(DPEntityC::Body()); }
    //: Access body.
    
  public:  
    inline DPIPortC<InT> &Input() 
    { return Body().Input(); }
    //: Access input port.
    
    void Input(const DPIPortC<InT> &ins) 
    { Body().Input(ins); }
    //: Setup input port.
    
  };
  
  ////////////////////////////////////////
  //! userlevel=Develop
  //: Stream operation base class.
  
  template<class InT,class OutT>
  class DPOStreamOpBodyC 
    : public DPOPortBodyC<InT>,
      public DPStreamOpBodyC
  {
  public:
    DPOStreamOpBodyC()
      : DPOPortBodyC<InT>("In1")
    {}
    //: Default constructor.
    
    DPOStreamOpBodyC(const DPOPortC<OutT> &nout)
      : DPOPortBodyC<InT>("In1"),
        output(nout)
    {}
    //: Constructor.
    
    virtual bool IsPutReady() const {
      RavlAssert(output.IsValid());
      return output.IsPutReady(); 
    }
    //: Is some data ready ?
    // true = yes.
    
    virtual void PutEOS() {
      if(output.IsValid())
        output.PutEOS();
    }
    //: Put End Of Stream marker.
    
    virtual DPPortC ConnectedTo() const
    { return output; }
    //: What does this connect to ?
    
    virtual AttributeCtrlC ParentCtrl() const 
    { return AttributeCtrlC(output); }
    //: Get Parent attribute control.
    
    virtual DListC<DPOPortBaseC> OPorts() const {
      DListC<DPOPortBaseC> lst = DPStreamOpBodyC::OPorts();
      lst.InsLast(DPOPortBaseC(const_cast<DPOStreamOpBodyC<InT,OutT> &>(*this)));
      return lst;
    }
    //: Output ports.
    
    virtual DListC<DPOPlugBaseC> OPlugs() const {
      DListC<DPOPlugBaseC> lst = DPStreamOpBodyC::OPlugs();
      lst.InsLast(DPOPlugC<OutT>(output,"Out1",DPEntityC((DPEntityBodyC &)*this)));
      return lst;
    }
    //: Output plugs.
    
    virtual bool Save(std::ostream &out) const 
    { return DPOPortBodyC<InT>::Save(out); }
    //: Save to std::ostream.
    
  protected:
    DPOPortC<OutT> output; // Where to put data to.
    
  public:
    inline DPOPortC<OutT> &Output() { return output; }
    //: Access output port.
    
    virtual void Output(const DPOPortC<InT> &oport) {
      output = oport; 
      this->ReparentAttributeCtrl(output); // Make sure changed signals are changed appropriately.
    }
    //: Setup new output port.
  }; 
  
  ///////////////////////////////////
  //! userlevel=Normal
  //: Stream operation handle class.
  
  template<class InT,class OutT>
  class DPOStreamOpC 
    : public DPOPortC<InT>,
      public DPStreamOpC
  {
  public:
    DPOStreamOpC() 
      : DPEntityC(true)
    {}
    //: Default Constructor.
    
    DPOStreamOpC(const DPOStreamOpC<InT,OutT> &oth) 
      : DPEntityC(oth),
	DPOPortC<InT>(oth)
    {}
    //: Copy Constructor.
    
    DPOStreamOpC<InT,OutT> &operator=(const DPOStreamOpC<InT,OutT> &oth) {
      // The compile can get confused about this, so make it clear.
      DPEntityC::operator=(oth); 
      return *this;
    }
    //: Assignment.
    
  protected:
    DPOStreamOpC(const DPOStreamOpBodyC<InT,OutT> &bod)
      : DPEntityC((DPOPortBodyC<InT> &) bod)
    {}
    //: Body constructor.
    
    DPOStreamOpBodyC<InT,OutT> &Body()
    { return dynamic_cast<DPOStreamOpBodyC<InT,OutT> & >(DPEntityC::Body()); }
    //: Access body.
    
    const DPOStreamOpBodyC<InT,OutT> &Body() const
    { return dynamic_cast<const DPOStreamOpBodyC<InT,OutT> & >(DPEntityC::Body()); }
    //: Access body.

    inline DPOPortC<OutT> &Output() 
    { return dynamic_cast<DPOStreamOpBodyC<InT,OutT> & >(DPEntityC::Body()).Output(); }
    // Access input port.
    
  };
  
  template<class InT,class OutT>
  DPIPortC<OutT> operator>>(const DPIPortC<InT> &in,DPIStreamOpC<InT,OutT> dat) {
    dat.Input(in);
    return dat;
  }

}
#endif
