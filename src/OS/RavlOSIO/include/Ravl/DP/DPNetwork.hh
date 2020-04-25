// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2013, Charles Galambos
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_DPNETWORK_HEADER
#define RAVL_DPNETWORK_HEADER 1
/////////////////////////////////////////////////////////////
//! lib=RavlOSIO
//! userlevel=Normal
//! author="Charles Galambos"
//! date="8/2/2013"
//! docentry="Ravl.API.Core.Data Processing"

#include "Ravl/Collection.hh"
#include "Ravl/DP/StreamOp.hh"
#include "Ravl/XMLFactory.hh"

namespace RavlN {

  //! Network of stream processing operations.
  // The main benefit of this class is its constructor from an XML file.

  class DPNetworkC
   : public DPStreamOpBodyC
  {
  public:
    DPNetworkC();
    //: Default constructor.
    // Creates an empty network

    DPNetworkC(const XMLFactoryContextC &factory);
    //: Create from an XML factory.

    virtual DListC<DPIPlugBaseC> IPlugs() const;
    //: Input plugs.

    virtual DListC<DPOPlugBaseC> OPlugs() const;
    //: Output plugs

    virtual DListC<DPIPortBaseC> IPorts() const;
    //: Input ports.

    virtual DListC<DPOPortBaseC> OPorts() const;
    //: Output ports

    typedef SmartPtrC<DPNetworkC> RefT;
    //: Handle to network.

  protected:
    CollectionC<RCAbstractC> m_components;
    DListC<DPIPlugBaseC> m_iplugs;
    DListC<DPOPlugBaseC> m_oplugs;
    DListC<DPIPortBaseC> m_iports;
    DListC<DPOPortBaseC> m_oports;
  };
}

#endif
