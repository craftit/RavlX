// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPSTREAMPROCESS_HEADER
#define RAVL_DPSTREAMPROCESS_HEADER 1
/////////////////////////////////////////////////////
//! date="16/7/2002"
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Data Processing.Internal" 
//! lib=RavlIO
//! file="Ravl/Core/IO/StreamProcess.hh"

#include "Ravl/DP/StreamOp.hh"
#include "Ravl/Hash.hh"
#include "Ravl/RCAbstract.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Stream Process.
  
  class DPStreamProcessBodyC
    : public DPStreamOpBodyC
  {
  public:
    DPStreamProcessBodyC();
    //: Default constructor.

    DPStreamProcessBodyC(const StringC &name);
    //: Construct a stream process.
    
    virtual DListC<DPIPlugBaseC> IPlugs() const;
    //: Input plugs.
    
    virtual DListC<DPOPlugBaseC> OPlugs() const;
    //: Output plugs
    
    virtual DListC<DPIPortBaseC> IPorts() const;
    //: Input ports.
    
    virtual DListC<DPOPortBaseC> OPorts() const;
    //: Output ports

    DPIPlugBaseC IPlug(const StringC &name) const {
      DPIPlugBaseC ret;
      iplugs.Lookup(name,ret);
      return ret;
    }
    //: Lookup Input plug.
    
    DPOPlugBaseC OPlug(const StringC &name) const {
      DPOPlugBaseC ret;
      oplugs.Lookup(name,ret);
      return ret;
    }
    //: Lookup Output plug
    
    DPIPortBaseC IPort(const StringC &name) const {
      DPIPortBaseC ret;
      iports.Lookup(name,ret);
      return ret;
    }
    //: Lookup input port.
    
    DPOPortBaseC OPort(const StringC &name) const {
      DPOPortBaseC ret;
      oports.Lookup(name,ret);
      return ret;
    }
    //: Lookup output port
    
    void IPlug(const StringC &name,DPIPlugBaseC &obj)
    { iplugs[name] = obj; }
    //: Register input plug.
    
    void OPlug(const StringC &name,DPOPlugBaseC &obj)
    { oplugs[name] = obj; }
    //: Register output plug
    
    void IPort(const StringC &name,DPIPortBaseC &obj)
    { iports[name] = obj; }
    //: Register input port.
    
    void OPort(const StringC &name,DPOPortBaseC &obj)
    { oports[name] = obj; }
    //: Register output port
    
  protected:
    StringC procname;
    RCAbstractC ent; // Handle to processing class.
    HashC<StringC,DPIPlugBaseC> iplugs;
    HashC<StringC,DPOPlugBaseC> oplugs;
    HashC<StringC,DPIPortBaseC> iports;
    HashC<StringC,DPOPortBaseC> oports;
  };

  //! userlevel=Normal
  //: Stream Process.
  
  class DPStreamProcessC
    : public DPStreamOpC
  {
  public:
    DPStreamProcessC()
      : DPEntityC(true)
    {}
    //: Default constructor
    // Creates an invalid handle.
    
    DPStreamProcessC(const StringC &name)
      : DPEntityC(*new DPStreamProcessBodyC(name))
    {}
    //: Default constructor
    // Creates an invalid handle.

  protected:
    DPStreamProcessBodyC &Body()
    { return dynamic_cast<DPStreamProcessBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPStreamProcessBodyC &Body() const
    { return dynamic_cast<const DPStreamProcessBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    DPIPlugBaseC IPlug(const StringC &name) const
    { return Body().IPlug(name); }
    //: Lookup input plug.
    
    DPOPlugBaseC OPlug(const StringC &name) const
    { return Body().OPlug(name); }
    //: Lookup output plug
    
    DPIPortBaseC IPort(const StringC &name) const
    { return Body().IPort(name); }
    //: Lookup input port.
    
    DPOPortBaseC OPort(const StringC &name) const
    { return Body().OPort(name); }
    //: Lookup output port
    
    void IPlug(const StringC &name,DPIPlugBaseC &obj)
    { Body().IPlug(name,obj); }
    //: Register input plug.
    
    void OPlug(const StringC &name,DPOPlugBaseC &obj)
    { Body().OPlug(name,obj); }
    //: Register output plug
    
    void IPort(const StringC &name,DPIPortBaseC &obj)
    { Body().IPort(name,obj); }
    //: Register input port.
    
    void OPort(const StringC &name,DPOPortBaseC &obj)
    { Body().OPort(name,obj); }
    //: Register output port

  };
}


#endif
