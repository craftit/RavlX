// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2009, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlXMLFactory
//! author=Charles Galambos
//! docentry=Ravl.API.Core.IO.XMLFactory

#include "Ravl/Option.hh"
#include "Ravl/StrStream.hh"
#include "Ravl/TypeName.hh"
#include "Ravl/XMLFactory.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/OS/SysLog.hh"


extern int testBasic();

int main(int nargs,char **argv) {
  RavlN::OptionC opts(nargs,argv);
  opts.Check();
  
  int ln = 0;
  if((ln = testBasic()) != 0) {
    std::cerr << "Test failed on line " << ln << "\n";
    return 1;
  }
  
  std::cout << "Test passed ok. \n";
  return 0;
}

using namespace RavlN;

namespace RavlN {
  void linkXMLFactoryRegister();
  
  class TestClassC 
    : public RavlN::RCBodyVC
  {
  public:
    TestClassC()
     : m_count(0)
    { linkXMLFactoryRegister(); }
    
    TestClassC(const XMLFactoryContextC &factory)
     : m_count(0)
    {
      linkXMLFactoryRegister();
      RavlDebug("Factory path '%s' XMLNode:'%s' ",factory.Path().chars(),factory.Node().Name().chars());
      bool noChild = factory.AttributeBool("noChild",false);
      if(!noChild) {
        if(!factory.UseComponent("child",m_child,noChild)) {
          SysLog(SYSLOG_ERR,"Failed to find expected child ");
          //throw RavlN::ExceptionOperatorFailedC("Failed to find child.");
        }
      }
    }
    
    typedef RavlN::SmartPtrC<TestClassC> RefT;
    
    bool AddChildTest(TestClassC::RefT &theClass) {
      m_count++;
      return true;
    }
    
    bool AddChildTestConst(TestClassC::RefT &theClass) {
      m_count++;
      return true;
    }
    
    int Count() const
    { return m_count; }
    
  protected:
    TestClassC::RefT m_child;
    int m_count;
  };
  
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

  XMLFactoryRegisterC<TestClassC> g_register("RavlN::TestClassC");
  RavlN::TypeNameC g_regname(typeid(RavlN::SmartPtrC<TestClassC>),"RavlN::SmartPtrC<RavlN::TestClassC>");
}

int testBasic() {
  StrIStreamC ss(
                 "<?xml version='1.0' encoding='UTF-8' ?>\n"
                 "<?RAVL class='RavlN::XMLTreeC' ?>\n"
                 "<Config verbose=\"false\" >\n"
                 "  <Test typename=\"RavlN::XMLFactoryC\" />\n"
                 "  <Test2 typename=\"RavlN::TestClassC\" >\n"
                 "    <child typename=\"RavlN::TestClassC\" noChild=\"true\" />\n"
                 "  </Test2>\n"
                 "  <Nodes> \n"
                 "    <Test3 typename=\"RavlN::TestClassC\" noChild=\"true\" />\n"
                 "    <Test4 typename=\"RavlN::TestClassC\" noChild=\"true\" />\n"
                 "  </Nodes> \n"
                 "</Config>\n"
                 );
  
  
  XMLTreeC xmlTree(true);
  if(!xmlTree.Read(ss)) return __LINE__;
  
  RavlN::XMLFactoryC::RefT factory = new RavlN::XMLFactoryC("test.xml",xmlTree);
  RavlN::XMLFactoryC::RefT childFactory;
  
  RavlN::XMLFactoryContextC ctxt(*factory);
  RavlDebug("Requesting component 'Test' ");
  if(!ctxt.UseComponent("Test",childFactory))
    return __LINE__;
  if(!childFactory.IsValid())
    return __LINE__;
  
  RavlN::TestClassC::RefT m_testChild;
  if(!ctxt.UseComponent("Test2",m_testChild))
    return __LINE__;
  
  RavlN::XMLFactoryC::RefT childFactory2;
  if(!ctxt.CreateComponent("Test",childFactory2))
    return __LINE__;
  if(!childFactory2.IsValid())
    return __LINE__;
  
  if(!ctxt.SetComponent("Hello:One:Two",childFactory2))
    return __LINE__;

  RavlN::XMLFactoryC::RefT childFactory3;
  if(!ctxt.GetComponent("Hello:One:Two",childFactory3))
    return __LINE__;
  if(childFactory2 != childFactory3)
    return __LINE__;
  
  std::vector<RavlN::TestClassC::RefT> children;
  if(!ctxt.UseComponentGroup("Nodes",children))
    return __LINE__;
  if(children.size()!=2) 
    return __LINE__;
  
  if(!ctxt.UseComponentGroup("Nodes",*m_testChild,&TestClassC::AddChildTest))
    return __LINE__;
  if(m_testChild->Count() != 2)
    return __LINE__;
  if(!ctxt.UseComponentGroup("Nodes",*m_testChild,&TestClassC::AddChildTestConst))
    return __LINE__;
  if(m_testChild->Count() != 4)
    return __LINE__;
  return 0;
}
