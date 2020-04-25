// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2004, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SBYTESTREAM_HEADER
#define RAVL_SBYTESTREAM_HEADER 1
//! author="Charles Galambos"
//! docentry="Ravl.API.Core.Data Processing.IO" 
//! rcsid="$Id$"
//! lib=RavlIO
//! file="Ravl/Core/IO/SByteStream.hh"

#include "Ravl/DP/ByteStream.hh"
#include "Ravl/DP/SPort.hh"

namespace RavlN {
  
  //! userlevel=Develop
  //: Abstract class representing a seekable output byte stream
  
  class DPOSByteStreamBodyC
    : public DPOByteStreamBodyC,
      public DPSeekCtrlBodyC
  {
  public:
    DPOSByteStreamBodyC();
    //: Default constructor.

    virtual bool Save(std::ostream &out) const 
	{ return DPOPortBaseBodyC::Save(out) && DPSeekCtrlBodyC::Save(out); }
	//: Save stream
  protected:
  };
  
  //! userlevel=Normal
  //: Abstract class representing a seekable output byte stream
  
  class DPOSByteStreamC
    : public DPOByteStreamC,
      public DPSeekCtrlC 
  {
  public:
    DPOSByteStreamC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
  protected:
    DPOSByteStreamC(DPOSByteStreamBodyC &bod)
      : DPEntityC(bod)
    {}
    //: Body constructor.
    
    DPOSByteStreamBodyC &Body()
    { return dynamic_cast<DPOSByteStreamBodyC &>(DPEntityC::Body()); }
    //: Access body.

    const DPOSByteStreamBodyC &Body() const
    { return dynamic_cast<const DPOSByteStreamBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
  public:    
  };

  //! userlevel=Develop
  //: Abstract class representing a seekable input byte stream
  
  class DPISByteStreamBodyC
    : public DPIByteStreamBodyC,
      public DPSeekCtrlBodyC
  {
  public:
    DPISByteStreamBodyC();
    //: Default constructor.

    virtual bool Save(std::ostream &out) const 
	{ return DPIByteStreamBodyC::Save(out) && DPSeekCtrlBodyC::Save(out); }
    //: Save to std::ostream.
  protected:
  };
  
  //! userlevel=Normal
  //: Abstract class representing a seekable input byte stream
  
  class DPISByteStreamC
    : public DPIByteStreamC,
      public DPSeekCtrlC
  {
  public:
    DPISByteStreamC()
      : DPEntityC(true)
    {}
    //: Default constructor.
    // Creates an invalid handle.
    
  protected:
    DPISByteStreamC(DPISByteStreamBodyC &bod)
      : DPEntityC(bod)
    {}
    //: Body constructor.
    
    DPISByteStreamBodyC &Body()
    { return dynamic_cast<DPISByteStreamBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
    const DPISByteStreamBodyC &Body() const
    { return dynamic_cast<const DPISByteStreamBodyC &>(DPEntityC::Body()); }
    //: Access body.
    
  public:    
  };
  
}



#endif
