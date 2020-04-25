// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2009, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlXMLFactory
//! author="Charles Galambos"
//! docentry=Ravl.API.Core.IO.XMLFactory

#include "Ravl/XMLFactory.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/OS/SysLog.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/XMLTree.hh"

namespace RavlN {
  extern void linkXMLFactoryRegister();
  
  class TestClassBodyC 
    : public RavlN::RCBodyC
  {
  public:
    TestClassBodyC()
    { linkXMLFactoryRegister(); }
    
    TestClassBodyC(const XMLFactoryContextC &factory);
    
  protected:
    int value;
  };
  
  class TestClassC 
    : RavlN::RCHandleC<TestClassBodyC>
  {
  public:
    TestClassC()
    {}
    //: Default constructor.
    
    TestClassC(const XMLFactoryContextC &factory)
      : RavlN::RCHandleC<TestClassBodyC>(new TestClassBodyC(factory))
    {}
    //: Constructor
  };
  
  TestClassBodyC::TestClassBodyC(const XMLFactoryContextC &factory)
  {
    SysLog(SYSLOG_DEBUG,"Factory path '%s' XMLNode:'%s' ",factory.Path().chars(),factory.Node().Name().chars());
    
    // Read value from context, default to 10 if not specified.
    value = factory.AttributeInt("value",10);
    
    bool noChild = factory.AttributeBool("noChild",false);
    if(!noChild) {
      TestClassC child;
      if(!factory.UseComponent("child",child,noChild)) {
        SysLog(SYSLOG_ERR,"Failed to find expected child ");
      }
    }
  }
  
  //need to declare stream operators too
  inline std::istream &operator>>(std::istream &strm,TestClassC &obj) {
    RavlAssertMsg(0,"Not implemented. ");
    return strm;
  }
  //: Load from a stream.
  // Uses virtual constructor.

  inline std::ostream &operator<<(std::ostream &out,const TestClassC &obj) {
    RavlAssertMsg(0,"Not implemented. ");
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

  inline RavlN::BinIStreamC &operator>>(RavlN::BinIStreamC &strm,TestClassC &obj) {
    RavlAssertMsg(0,"Not implemented. ");
    return strm;
  }
  //: Load from a binary stream.
  // Uses virtual constructor.

  inline RavlN::BinOStreamC &operator<<(RavlN::BinOStreamC &out,const TestClassC &obj) {
    RavlAssertMsg(0,"Not implemented. ");
    return out;
  }
  //: Save to a stream.
  // Uses virtual constructor.

  
  XMLFactoryRegisterHandleC<TestClassC> g_register("RavlN::TestClassC");
  
  RavlN::TypeNameC g_regname(typeid(TestClassC),"RavlN::TestClassC");
}

int main() {
  RavlN::SysLogOpen("exXMLFactory",false);
  
  RavlN::StrIStreamC ss(
                        "<?xml version='1.0' encoding='UTF-8' ?>\n"
                        "<?RAVL class='RavlN::XMLTreeC' ?>\n"
                        "<Config verbose=\"false\" checkConfig=\"true\" >\n"
                        "  <Test typename=\"RavlN::TestClassC\" value=\"5\" >\n"
                        "    <child typename=\"RavlN::TestClassC\" noChild=\"true\" value=\"3\" />\n"
                        "  </Test>\n"
                        "</Config>\n"
                        );
  
  

  RavlN::XMLTreeC xmlTree(true);
  if(!xmlTree.Read(ss)) return __LINE__;
  
  RavlN::XMLFactoryHC factory("test.xml",xmlTree);
  
  RavlN::SysLog(RavlN::SYSLOG_DEBUG,"Requesting component 'Test' ");
  RavlN::TestClassC instance;
  
  if(!factory.UseComponent("Test",instance)) {
    SysLog(RavlN::SYSLOG_ERR,"Failed to find instance. ");
  }
    

}
