// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2009, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
//! lib=RavlXMLFactory
//! author="Charles Galambos"
//! docentry=Ravl.API.Core.IO.XMLFactory

#include "Ravl/STL.hh"
#include "Ravl/XMLFactoryRegister.hh"
#include "Ravl/Threads/Signal.hh"
#include "Ravl/Threads/Signal1.hh"

namespace RavlN {
  
  XMLFactoryRegisterC<XMLFactoryC> g_xmlFactoryRegisterXMLFactory("RavlN::XMLFactoryC");
  
  static RavlN::RCWrapAbstractC StdStringFactoryFunc(const XMLFactoryContextC &node)
  { return RavlN::RCWrapC<std::string>(node.AttributeString("value","").data()); }

  static RavlN::RCWrapAbstractC StringFactoryFunc(const XMLFactoryContextC &node)
  { return RavlN::RCWrapC<StringC>(node.AttributeString("value","")); }

  static RavlN::RCWrapAbstractC Signal0FactoryFunc(const XMLFactoryContextC &node)
  { return RavlN::RCWrapC<Signal0C>(Signal0C(true)); }

  static RavlN::RCWrapAbstractC Signal1BoolFactoryFunc(const XMLFactoryContextC &node)
  { return RavlN::RCWrapC<Signal1C<bool> >(Signal1C<bool>(node.AttributeBool("value",false))); }

  static RavlN::RCWrapAbstractC Signal1StringFactoryFunc(const XMLFactoryContextC &node)
  { return RavlN::RCWrapC<Signal1C<StringC> >(Signal1C<StringC>(node.AttributeString("value",""))); }


  static RavlN::RCWrapAbstractC RCHashStringCStringCFactoryFunc(const XMLFactoryContextC &node)
  {
    RCHashC<StringC,StringC> hashTable(true);
    DListC<XMLTreeC> children = node.Children();
    for(DLIterC<XMLTreeC> it(children);it;it++) {
      // Lookout for invalid attributes.
      RCHashC<StringC,StringC> attrs = it->Attributes();
      if(attrs.IsValid()) {
        for(HashIterC<StringC,StringC> ait(attrs);ait;ait++) {
          if(ait.Key() == "key" || ait.Key() == "value")
            continue;
          RavlSysLogf(SYSLOG_ERR,"Unused attribute '%s' in string hash table definition in %s ",ait.Key().data(),node.Path().data());
          throw RavlN::ExceptionBadConfigC("Unused attribute found ");
        }
      }
      StringC key = it->AttributeString("key",it->Name());
      StringC value = it->AttributeString("value",it->Name());
      if(hashTable.IsElm(key)) {
        RavlSysLogf(SYSLOG_ERR,"Duplicate definition of attribute '%s' in string hash table definition in %s ",it->Name().data(),node.Path().data());
        throw RavlN::ExceptionBadConfigC("Duplicate entry found ");
      }
      hashTable[key] = value;
    }
    return RavlN::RCWrapC<RCHashC<StringC,StringC> >(hashTable);
  }

  static RavlN::RCWrapAbstractC RCHashStringStringFactoryFunc(const XMLFactoryContextC &node)
  {
    RCHashC<std::string,std::string> hashTable;
    DListC<XMLTreeC> children = node.Children();
    for(DLIterC<XMLTreeC> it(children);it;it++) {
      // Lookout for invalid attributes.
      RCHashC<StringC,StringC> attrs = it->Attributes();
      for(HashIterC<StringC,StringC> ait(attrs);ait;ait++) {
        if(ait.Key() == "key" || ait.Key() == "value")
          continue;
        RavlSysLogf(SYSLOG_ERR,"Unused attribute '%s' in string hash table definition in %s ",ait.Key().data(),node.Path().data());
        throw RavlN::ExceptionBadConfigC("Unused attribute found ");
      }
      std::string key = it->AttributeString("key",it->Name()).data();
      std::string value = it->AttributeString("value",it->Content()).data();
      if(hashTable.IsElm(key)) {
        RavlSysLogf(SYSLOG_ERR,"Duplicate definition of attribute '%s' in string hash table definition in %s ",it->Name().data(),node.Path().data());
        throw RavlN::ExceptionBadConfigC("Duplicate entry found ");
      }
      hashTable[key] = value;
    }
    return RavlN::RCWrapC<RCHashC<std::string,std::string> >(hashTable);
  }


  static int InitStringFactory() {
    RavlN::AddTypeName(typeid(std::string),"std::string");
    RavlN::AddTypeName(typeid(RavlN::StringC),"RavlN::StringC");
    RavlN::AddTypeName(typeid(Signal0C),"RavlN::Signal0C");
    RavlN::AddTypeName(typeid(Signal1C<bool>),"RavlN::Signal1C<bool>");
    RavlN::AddTypeName(typeid(Signal1C<StringC>),"RavlN::Signal1C<RavlN::StringC>");
    RavlN::AddTypeName(typeid(RCHashC<StringC,StringC>),"RavlN::RCHashC<RavlN::StringC,RavlN::StringC>");
    RavlN::AddTypeName(typeid(RCHashC<std::string,std::string>),"RavlN::RCHashC<std::string,std::string>");
    XMLFactoryC::RegisterTypeFactory(typeid(std::string),&StdStringFactoryFunc);
    XMLFactoryC::RegisterTypeFactory(typeid(StringC),&StringFactoryFunc);
    XMLFactoryC::RegisterTypeFactory(typeid(Signal0C),&Signal0FactoryFunc);
    XMLFactoryC::RegisterTypeFactory(typeid(Signal1C<bool>),&Signal1BoolFactoryFunc);
    XMLFactoryC::RegisterTypeFactory(typeid(Signal1C<StringC>),&Signal1StringFactoryFunc);
    XMLFactoryC::RegisterTypeFactory(typeid(RCHashC<StringC,StringC>),&RCHashStringCStringCFactoryFunc);
    XMLFactoryC::RegisterTypeFactory(typeid(RCHashC<std::string,std::string>),&RCHashStringStringFactoryFunc);
    return 0;
  }

  int g_initStringFactory = InitStringFactory();

  void linkXMLFactoryRegister()
  {}

}
