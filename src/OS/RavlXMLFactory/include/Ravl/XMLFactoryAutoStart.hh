// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2009, OmniPerception Ltd
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_XMLFACTORYAUTOSTART
#define RAVL_XMLFACTORYAUTOSTART 1
//! lib=RavlXMLFactory
//! author="Charles Galambos"
//! docentry=Ravl.API.Core.IO.XMLFactory

#include "Ravl/SmartLayerPtr.hh"

namespace RavlN {

  //! Class to allow classes to run code after loaded in a preload section.
  
  class XMLFactoryAutoStartC
    : virtual public RavlN::RCLayerBodyC
  {
  public:
    //! Constructor
    XMLFactoryAutoStartC();
    
    //! Start object.
    //! This will be called automatically if the object
    //! is loaded in the XMLFactoryC 'preload' section. This is
    //! done after an initial reference has been taken, and so
    //! threads can be started safely from this method.
    virtual bool Init();
    
    //! Owner reference counted ptr to class
    typedef RavlN::SmartOwnerPtrC<XMLFactoryAutoStartC> RefT;
    
    //! Callback reference counter ptr to class
    typedef RavlN::SmartCallbackPtrC<XMLFactoryAutoStartC> CBRefT;
    
  protected:
    
  };

}

#endif
