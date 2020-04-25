// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the MIT
// License. See http://www.opensource.org/licenses/mit-license.html
// file-header-ends-here
#ifndef RAVL_SERIALABSTRACTPORT_HH
#define	RAVL_SERIALABSTRACTPORT_HH
//! lib=RavlSerialIO

#include "Ravl/SmartPtr.hh"
#include "Ravl/OS/SerialAbstract.hh"
#include "Ravl/DP/Port.hh"

namespace RavlN {

  //! Port containing abstract serial ports.
  //! User for SerialFormat's
  
  class SerialFormatAuxDPIPortBodyC
  : public RavlN::DPIPortBodyC<SmartPtrC<SerialAbstractC> >
  {
  public:
    //! Constructor.
    SerialFormatAuxDPIPortBodyC()
    {}

    //! Set port.

    void SetPort(SmartPtrC<SerialAbstractC> &port)
    { m_port = port; }

    //! Are we ready  ?

    virtual bool IsGetReady() const
    { return m_port.IsValid(); }

    //! Has an end of stream occurred (a terminal error in this case.)

    virtual bool IsGetEOS() const
    { return !m_port.IsValid(); }

    //! Get port

    virtual SmartPtrC<SerialAbstractC> Get()
    { return m_port; }

    //! Get an image from the camera.

    virtual bool Get(SmartPtrC<SerialAbstractC> &port) {
      port = m_port;
      return m_port.IsValid();
    }

  protected:
    SmartPtrC<SerialAbstractC> m_port;
  };

  //! Port containing abstract serial ports.
  //! User for SerialFormat's
  
  class SerialFormatAuxDPIPortC
  : public RavlN::DPIPortC<SmartPtrC<SerialAbstractC> >
  {
  public:

    SerialFormatAuxDPIPortC()
    : DPEntityC(false)
    {}
    //! Default constructor.

    SerialFormatAuxDPIPortC(SmartPtrC<SerialAbstractC> &port)
    : DPEntityC(new SerialFormatAuxDPIPortBodyC())
    { Body().SetPort(port); }

  protected:
    //! Access handle.

    SerialFormatAuxDPIPortBodyC &Body()
    { return dynamic_cast<SerialFormatAuxDPIPortBodyC &> (DPEntityC::Body()); }
  };

}

#endif	/* SERIALABSTRACTPORT_HH */

