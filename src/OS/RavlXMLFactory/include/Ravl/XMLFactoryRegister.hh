// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2009, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_XMLFACTORYREGISTER_HEADER
#define RAVL_XMLFACTORYREGISTER_HEADER 1
//! lib=RavlXMLFactory
//! author="Charles Galambos"
//! docentry=Ravl.API.Core.IO.XMLFactory

#include "Ravl/XMLFactory.hh"
#include "Ravl/DP/Converter.hh"

namespace RavlN {
  
  
  template<class DataT,class UsedAsT>
  class XMLFactoryRegisterConvertC {
  public:
    static typename UsedAsT::RefT ConvertToBase(const typename DataT::RefT &x) 
    { return x.BodyPtr(); }
    //! Method to convert to base class.
    
    XMLFactoryRegisterConvertC()
    { 
      if(typeid(DataT) != typeid(UsedAsT)) // Only needed if types are different
        RavlN::RegisterConversion(&ConvertToBase);
      XMLFactoryC::RegisterTypeFactory(typeid(DataT),&XMLFactoryC::DefaultFactoryFunc<DataT>); 
    }
    //! Typename already set, just register the factory
    
    XMLFactoryRegisterConvertC(const char *nameOfType) { 
      RavlN::AddTypeName(typeid(DataT),nameOfType);
      if(typeid(DataT) != typeid(UsedAsT)) // Only needed if types are different
        RavlN::RegisterConversion(&ConvertToBase);
      XMLFactoryC::RegisterTypeFactory(typeid(DataT),&XMLFactoryC::DefaultFactoryFunc<DataT>);
    }
    //! Construct and set typename.
    
  };


  //! Syntactic sugar for registering abstract classes in XMLFactory hierarchy.
  template<class DataT,class UsedAsT>
  class XMLFactoryRegisterConvertAbstractC {
  public:
    static typename UsedAsT::RefT ConvertToBase(const typename DataT::RefT &x)
    { return x.BodyPtr(); }
    //! Method to convert to base class.

    XMLFactoryRegisterConvertAbstractC()
    {
      if(typeid(DataT) != typeid(UsedAsT)) // Only needed if types are different
        RavlN::RegisterConversion(&ConvertToBase);
    }
    //! Typename already set, just register the factory

    XMLFactoryRegisterConvertAbstractC(const char *nameOfType) {
      RavlN::AddTypeName(typeid(DataT),nameOfType);
      if(typeid(DataT) != typeid(UsedAsT)) // Only needed if types are different
        RavlN::RegisterConversion(&ConvertToBase);
    }
    //! Construct and set typename.

  };

  
  template<class DataT,class UsedAsT>
  class XMLFactoryRegisterHandleConvertC {
  public:
    static UsedAsT ConvertToBase(const DataT &x) 
    { return UsedAsT(x); }
    //! Method to convert to base class.
    
    static RavlN::RCWrapAbstractC FactoryFunc(const XMLFactoryContextC &node)
    { return RavlN::RCWrapC<DataT>(DataT(node)); }
    //! Default factory function.
    
    XMLFactoryRegisterHandleConvertC() { 
      if(typeid(DataT) != typeid(UsedAsT)) // Only needed if types are different
        RavlN::RegisterConversion(&ConvertToBase);
      XMLFactoryC::RegisterTypeFactory(typeid(DataT),&FactoryFunc); 
    }
    //! Typename already set, just register the factory
    
    XMLFactoryRegisterHandleConvertC(const char *nameOfType) { 
      RavlN::AddTypeName(typeid(DataT),nameOfType);
      if(typeid(DataT) != typeid(UsedAsT)) // Only needed if types are different
        RavlN::RegisterConversion(&ConvertToBase);
      XMLFactoryC::RegisterTypeFactory(typeid(DataT),&FactoryFunc);
    }
    //! Construct and set typename.
    
  };

  //! Syntactic sugar for registering abstract classes in XMLFactory hierarchy.

  template<class DataT,class UsedAsT>
  class XMLFactoryRegisterHandleConvertAbstractC {
  public:
    static UsedAsT ConvertToBase(const DataT &x)
    { return UsedAsT(x); }
    //! Method to convert to base class.

    XMLFactoryRegisterHandleConvertAbstractC() {
      if(typeid(DataT) != typeid(UsedAsT)) // Only needed if types are different
        RavlN::RegisterConversion(&ConvertToBase);
    }
    //! Typename already set, just register the factory

    XMLFactoryRegisterHandleConvertAbstractC(const char *nameOfType) {
      RavlN::AddTypeName(typeid(DataT),nameOfType);
      if(typeid(DataT) != typeid(UsedAsT)) // Only needed if types are different
        RavlN::RegisterConversion(&ConvertToBase);
    }
    //! Construct and set typename.

  };

  
  //! userlevel=Normal
  //: Register class with a smart pointer 
  
  template<class DataT>
  class XMLFactoryRegisterC {
  public:
    
    XMLFactoryRegisterC()
    { XMLFactoryC::RegisterTypeFactory(typeid(DataT),&XMLFactoryC::DefaultFactoryFunc<DataT>); }
    //! Typename already set, just register the factory
    
    XMLFactoryRegisterC(const char *nameOfType) { 
      RavlN::AddTypeName(typeid(DataT),nameOfType);
      static StringC refName = StringC(nameOfType) + "::RefT";
      if(!HaveTypeName(typeid(typename DataT::RefT)))
        RavlN::AddTypeName(typeid(typename DataT::RefT),refName);
      XMLFactoryC::RegisterTypeFactory(typeid(DataT),&XMLFactoryC::DefaultFactoryFunc<DataT>);
    }
    //! Construct and set typename.
    
  };

  //! userlevel=Normal
  //: Register class with a big object style handle 
  
  template<class DataT>
  class XMLFactoryRegisterHandleC {
  protected:
    
    static RavlN::RCWrapAbstractC FactoryFunc(const XMLFactoryContextC &node)
    { return RavlN::RCWrapC<DataT>(DataT(node)); }
    //! Default factory function.
  public:
    
    XMLFactoryRegisterHandleC()
    { XMLFactoryC::RegisterTypeFactory(typeid(DataT),&FactoryFunc); }
    //! Typename already set, just register the factory
    
    XMLFactoryRegisterHandleC(const char *nameOfType) { 
      RavlN::AddTypeName(typeid(DataT),nameOfType);
      XMLFactoryC::RegisterTypeFactory(typeid(DataT),&FactoryFunc);
    }
    //! Construct and set typename.
    
  };
  
}

#endif
