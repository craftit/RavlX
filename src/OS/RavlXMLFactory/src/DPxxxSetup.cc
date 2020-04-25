/*
 * DPxxxSetup.cc
 *
 *  Created on: Feb 10, 2013
 *      Author: charlesgalambos
 */


#include "Ravl/SysLog.hh"
#include "Ravl/XMLFactory.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/DP/Converter.hh"
#include "Ravl/DP/Entity.hh"
#include "Ravl/DP/Process.hh"
#include "Ravl/DP/Plug.hh"


namespace RavlN {


  //: XMLFactory constructor.

  DPEntityBodyC::DPEntityBodyC(const XMLFactoryContextC &factory)
  { Setup(factory); }

  //: Factory constructor.
  // You must include RavlXMLFactory to use

  void DPEntityBodyC::Setup(const XMLFactoryContextC &factory)
  {
    m_entityName = factory.AttributeString("entityName",factory.Path());
  }

  // ------------------------------------------------------------------------

  //: XML factory constructor.

  DPProcessBaseBodyC::DPProcessBaseBodyC(const XMLFactoryContextC &factory)
   : DPEntityBodyC(factory)
  {}

  // ------------------------------------------------------------------------


  void DPPlugBaseBodyC::Setup(const XMLFactoryContextC &factory)
  {
    DPEntityBodyC::Setup(factory);
    DPEntityBodyC::RefT ent;
    factory.UseChildComponent("Entity",ent,true);
    m_hold = ent;
  }

  static DPEntityBodyC::RefT ConvertDPPlugBase2DPEntityPtr(const DPPlugBaseC &plugBase)
  { return DPEntityBodyC::RefT(plugBase.BodyPtr()); }

  DP_REGISTER_CONVERSION(ConvertDPPlugBase2DPEntityPtr,1);

  // ------------------------------------------------------------------------

  //: Setup iplug with xml factory

  void DPIPlugBaseBodyC::Setup(const XMLFactoryContextC &factory)
  {
    DPPlugBaseBodyC::Setup(factory);
    //: Setup iplug with xml factory
    DPIPortBaseC iport;
    if(factory.UseChildComponent("IPort",iport,true)) {
      if(!ConnectPort(iport)) {
        RavlError("Failed to connect IPort at %s ",factory.Path().c_str());
        throw RavlN::ExceptionBadConfigC("Failed to connect IPort");
      }
    }
  }

  static XMLFactoryRegisterHandleConvertAbstractC<DPIPlugBaseC,DPPlugBaseC> g_register_DPIPlugBaseC("RavlN::DPIPlugBaseC");

  // ------------------------------------------------------------------------

  //: Setup oplug with xml factory

  void DPOPlugBaseBodyC::Setup(const XMLFactoryContextC &factory)
  {
    //: Setup oplug with xml factory
    DPOPortBaseC oport;
    if(factory.UseChildComponent("OPort",oport,true)) {
      if(!ConnectPort(oport)) {
        RavlError("Failed to connect OPort at %s ",factory.Path().c_str());
        throw RavlN::ExceptionBadConfigC("Failed to connect OPort");
      }
    }
  }

  static XMLFactoryRegisterHandleConvertAbstractC<DPOPlugBaseC,DPPlugBaseC> g_register_DPOPlugBaseC("RavlN::DPOPlugBaseC");

}
