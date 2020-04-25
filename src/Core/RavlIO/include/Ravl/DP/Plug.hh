// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2002, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPPLUG_HEADER
#define RAVL_DPPLUG_HEADER 1
////////////////////////////////////////////////////
//! docentry="Ravl.API.Core.Data Processing.Ports" 
//! file="Ravl/Core/IO/Plug.hh"
//! lib=RavlIO
//! author="Charles Galambos"
//! date="8/6/2002"
//! userlevel=Normal

#include "Ravl/DP/Port.hh"

namespace RavlN {
  class XMLFactoryContextC;

  //! userlevel=Develop
  //: Input plug base.
  
  class DPPlugBaseBodyC 
    : virtual public DPEntityBodyC
  {
  public:
    DPPlugBaseBodyC();
    //: Default constructor.

    DPPlugBaseBodyC(const DPEntityC &nhold);
    //: Constructor.
    
    DPPlugBaseBodyC(const StringC &nPlugId,const DPEntityC &nhold);
    //: Constructor.

    void Setup(const XMLFactoryContextC &factory);
    //: Factory constructor.
    // You must include RavlXMLFactory to use
    
    const StringC &PlugId() const
    { return EntityName(); }
    //: Get ID for plug.
    
  protected:
    DPEntityC m_hold; // Make sure object is not deleted.
  };

  //! userlevel=Advanced
  //: Input plug base.
  
  class DPPlugBaseC 
    : virtual public DPEntityC
  {
  public:
    DPPlugBaseC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
  protected:
    DPPlugBaseC(DPPlugBaseBodyC &bod)
      : DPEntityC(bod)
    {}
    //: Body constructor.
    
    DPPlugBaseBodyC &Body()
    { return dynamic_cast<DPPlugBaseBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPPlugBaseBodyC &Body() const
    { return dynamic_cast<const DPPlugBaseBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    const StringC &PlugId() const
    { return Body().PlugId(); }
    //: Get ID for plug.
    
  };

  ///////////////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Input plug base.
  
  class DPIPlugBaseBodyC 
    : public DPPlugBaseBodyC
  {
  public:
    DPIPlugBaseBodyC();
    //: Default constructor.
    
    DPIPlugBaseBodyC(const DPEntityC &nhold);
    //: Constructor.

    DPIPlugBaseBodyC(const StringC &nPlugId,const DPEntityC &nhold);
    //: Constructor.

    void Setup(const XMLFactoryContextC &factory);
    //: Setup iplug with xml factory
    // You must include RavlXMLFactory to use
    
    virtual bool ConnectPort(const DPIPortBaseC &port);
    //: set port.
    
    virtual const std::type_info &InputType() const;
    //: Return type of port.
  protected:
  };
  
  //! userlevel=Advanced
  //: Input plug base.
  
  class DPIPlugBaseC 
    : public DPPlugBaseC
  {
  public:
    DPIPlugBaseC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
  protected:
    DPIPlugBaseC(DPIPlugBaseBodyC &bod)
      : DPEntityC(bod)
    {}
    //: Body constructor.
    
    DPIPlugBaseBodyC &Body()
    { return dynamic_cast<DPIPlugBaseBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPIPlugBaseBodyC &Body() const
    { return dynamic_cast<const DPIPlugBaseBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    bool ConnectPort(const DPIPortBaseC &port)
    { return Body().ConnectPort(port); }
    //: Set port.
    
    const std::type_info &InputType() const
    { return Body().InputType(); }
    //: Return type of port.
    
  };
  
  ///////////////////////////////////////////////////////////////////
  
  //! userlevel=Develop
  //: Input plug base.
  
  class DPOPlugBaseBodyC 
    : public DPPlugBaseBodyC
  {
  public:
    DPOPlugBaseBodyC();
    //: Default constructor.

    DPOPlugBaseBodyC(const DPEntityC &nhold);
    //: Constructor.
    
    DPOPlugBaseBodyC(const StringC &nPlugId,const DPEntityC &nhold);
    //: Constructor.

    void Setup(const XMLFactoryContextC &factory);
    //: Setup oplug with xml factory
    // You must include RavlXMLFactory to use

    virtual bool ConnectPort(const DPOPortBaseC &port);
    //: set port.

    virtual const std::type_info &OutputType() const;
    //: Return type of port.    
  };
  
  //! userlevel=Advanced
  //: Input plug base.
  
  class DPOPlugBaseC 
    : public DPPlugBaseC
  {
  public:
    DPOPlugBaseC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
  protected:
    DPOPlugBaseC(DPOPlugBaseBodyC &bod)
      : DPEntityC(bod)
    {}
    //: Body constructor.
    
    DPOPlugBaseBodyC &Body()
    { return dynamic_cast<DPOPlugBaseBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPOPlugBaseBodyC &Body() const
    { return dynamic_cast<const DPOPlugBaseBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
  public:
    bool ConnectPort(const DPOPortBaseC &port)
    { return Body().ConnectPort(port); }
    //: Set port.
    
    const std::type_info &OutputType() const
    { return Body().OutputType(); }
    //: Return type of port.
    
  };

  
  ////////////////////////////////////
  //! userlevel=Develop
  //: Plug.
  // Used for setting up inputs.
  
  template<class DataT>
  class DPIPlugBodyC 
    : public DPIPlugBaseBodyC
  {
  public:
    explicit DPIPlugBodyC(const DPIPortC<DataT> &nport,const DPEntityC &nhold)
      : DPIPlugBaseBodyC(nhold),
	port(const_cast<DPIPortC<DataT> &>(nport))
    {}
    //: Constructor.

    explicit DPIPlugBodyC(const DPIPortC<DataT> &nport,const StringC &nPlugId,const DPEntityC &nhold)
      : DPIPlugBaseBodyC(nPlugId,nhold),
	port(const_cast<DPIPortC<DataT> &>(nport))
    {}
    //: Constructor.
    
    inline const DPPlugC<DataT> &operator= (DPIPortC<DataT> &othport)
    { port = othport; return *this; }
    //: Assignment.
    
    DPIPortC<DataT> &Port()
    { return port; }
    //: Access port.
    
    virtual bool ConnectPort(const DPIPortBaseC &nport) { 
      port = DPIPortC<DataT>(const_cast<DPIPortBaseC &>(nport));
      return port.IsValid();
    }
    //: set port.
    
    virtual const std::type_info &InputType() const
    { return typeid(DataT); }
    //: Return type of port.
    
  private:
    DPIPortC<DataT> &port;
  };
  
  ////////////////////////////////////
  //! userlevel=Advanced
  //: Plug.
  // Used for setting up inputs.
  
  template<class DataT>
  class DPIPlugC 
    : public DPIPlugBaseC
  {
  public:
    explicit DPIPlugC(const DPIPortC<DataT> &nport,const DPEntityC &nhold = DPEntityC(true))
      : DPEntityC(*new DPIPlugBodyC<DataT>(nport,nhold))
    {}
    //: Constructor.

    DPIPlugC(const DPIPortC<DataT> &nport,const StringC &nPlugId,const DPEntityC &nhold = DPEntityC(true))
      : DPEntityC(*new DPIPlugBodyC<DataT>(nport,nPlugId,nhold))
    { RavlAssert(EntityName() == nPlugId); }
    //: Constructor.
    
  protected:
    DPIPlugBodyC<DataT> &Body()
    { return static_cast<DPIPlugBodyC<DataT> &>(DPIPlugBaseC::Body()); }
    //: Access body.
    
    const DPIPlugBodyC<DataT> &Body() const
    { return static_cast<const DPIPlugBodyC<DataT> &>(DPIPlugBaseC::Body()); }
    //: Access body.
  public:
    
    DPIPortC<DataT> &Port()
    { return Body().Port(); }
    //: Access handle to port.
    
    
  };
  
  /////////////////////////////
  //: Use a plug
  
  template<class DataT>
  void operator>> (DPIPortC<DataT> &source,DPIPlugC<DataT> &input)  { 
    input = source; 
  }
  
  
  //////////////////////////////////////////////////////////////////////////
  //! userlevel=Develop
  //: Plug.
  // Used for setting up inputs.
  
  template<class DataT>
  class DPOPlugBodyC 
    : public DPOPlugBaseBodyC
  {
  public:
    explicit DPOPlugBodyC(const DPOPortC<DataT> &nport,const DPEntityC &nhold)
      : DPOPlugBaseBodyC(nhold),
	port(const_cast<DPOPortC<DataT> &>(nport))
    {}
    //: Constructor.

    DPOPlugBodyC(const DPOPortC<DataT> &nport,const StringC &nPlugId,const DPEntityC &nhold)
      : DPOPlugBaseBodyC(nPlugId,nhold),
	port(const_cast<DPOPortC<DataT> &>(nport))
    {}
    //: Constructor.
    
    DPOPortC<DataT> &Port()
    { return port; }
    //: Access handle to port.
    
    virtual bool ConnectPort(const DPOPortBaseC &nport) { 
      port = DPOPortC<DataT>(const_cast<DPOPortBaseC &>(nport));
      return port.IsValid();
    }
    //: set port.
    
    virtual const std::type_info &OutputType() const
    { return typeid(DataT); }
    //: Return type of port.
    
  private:
    DPOPortC<DataT> &port;
  };
  
  ////////////////////////////////////
  //! userlevel=Advanced
  //: Plug.
  // Used for setting up inputs.
  
  template<class DataT>
  class DPOPlugC 
    : public DPOPlugBaseC
  {
  public:
    explicit DPOPlugC(const DPOPortC<DataT> &nport,const DPEntityC &nhold = DPEntityC(true))
      : DPEntityC(*new DPOPlugBodyC<DataT>(nport,nhold))
    {}
    //: Constructor.

    DPOPlugC(const DPOPortC<DataT> &nport,const StringC &nPlugId,const DPEntityC &nhold = DPEntityC(true))
      : DPEntityC(*new DPOPlugBodyC<DataT>(nport,nPlugId,nhold))
    {}
    //: Constructor.
    
  protected:
    DPOPlugBodyC<DataT> &Body()
    { return static_cast<DPOPlugBodyC<DataT> &>(DPOPlugBaseC::Body()); }
    //: Access body.
    
    const DPOPlugBodyC<DataT> &Body() const
    { return static_cast<const DPOPlugBodyC<DataT> &>(DPOPlugBaseC::Body()); }
    //: Access body.
    
  public:
    DPOPortC<DataT> &Port()
    { return Body().Port(); }
    //: Access handle to port.
    
  };
  
  /////////////////////////////
  //: Use a plug
  
  template<class DataT>
  void operator>> (DPOPortC<DataT> &source,DPOPlugC<DataT> &output)  { 
    output = source; 
  }

}

#endif
