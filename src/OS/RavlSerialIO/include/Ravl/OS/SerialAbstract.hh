// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SERIALABSTRACT_HEADER
#define RAVL_SERIALABSTRACT_HEADER 1
/////////////////////////////////////////////////////////////
//! userlevel=Normal
//! date="15/09/2010"
//! author="Alex Kostin"
//! docentry="Ravl.API.OS.Hardware"
//! rcsid="$Id$"
//! lib=RavlSerialIO
//! file="Ravl/OS/Misc/SerialBase.hh"

#include "Ravl/RCHandleV.hh"
#include "Ravl/Types.hh"


namespace RavlN {

  class SerialAbstractParityBaseC {
  public:
    enum ParityT { 
      SERIAL_PARITY_ODD = 0,
      SERIAL_PARITY_EVEN = 1,
      SERIAL_PARITY_SET = 2,
      SERIAL_PARITY_NONE = 3, 
#if !RAVL_OS_WIN32
      // These are here for compatilibity with older code, they
      // unfortunately they conflict with define's under windows.
      PARITY_ODD = 0,
      PARITY_EVEN = 1,
      PARITY_SET = 2,
      PARITY_NONE = 3, 
#endif
    };
  };

  //! userlevel=Normal
  //: Base class to control serial ports.
  class SerialAbstractC : public RCBodyVC, public SerialAbstractParityBaseC {
  public:
    SerialAbstractC();
    //: default constructor;

    SerialAbstractC(const char *dev, const char * perm = "RDWR",bool nonBlocking = true);
    //: open a device for initialize;

    virtual ~SerialAbstractC(){};
    //: destructor;

    virtual bool Open(const char *dev, const char * perm = "RDWR",bool nonBlocking = true);
    //: Open device.

    virtual void Close();
    //: Close the socket.
    // Note this will only actually close the socket if
    // m_dontClose is false.

    virtual bool Setup(IntT i_speed = 9600,
                       IntT o_speed = 9600,
                       IntT stop_bit = 1,
                       ParityT par = SERIAL_PARITY_NONE,
                       IntT char_size = 8);
    //: Setup the port.
    // with the given parameters: Input speed, Output speed,
    // stop bit(1,2), parity(exist or not exist), parity type(odd or even) and character size(5,6,7,8)

    virtual void Reset();
    //: Reset interface.

    virtual void Flush();
    //: Flush buffer.

    virtual bool IsOpen() const;
    //: Is stream open ?

    virtual IntT Read(char *buff,UIntT size);
    //: Read some bytes from a stream.

    virtual IntT ReadV(char **buffer,IntT *len,int n);
    //: Read some bytes from a stream.

    virtual IntT Write(const char *buff,UIntT size);
    //: Write some bytes to a stream.

    virtual IntT WriteV(const char **buffer,IntT *len,int n);
    //: Write multiple buffers

    virtual bool SetISpeed(const IntT i_speed);
    //: set the input speed of the port;

    virtual bool SetOSpeed(const IntT o_speed);
    //: set the output speed of the port;

    virtual bool SetStopBits(const IntT stop_bit);
    //: set the number of stop bits : 1 or 2;

    virtual bool SetCharSize(const IntT char_size);
    //: set the character size: 5,6,7,8;

    virtual bool SetParity(ParityT par);
    //: parity type: Odd or Even or None or 1
    
    virtual IntT Fd() const;
    //: Access file descriptor.

    virtual void SetReadTimeout(float timeOut);
    //: Set the amount of time you should attempt to read from a file descriptor.
    // This limits the time spent attempting to write to a socket
    // without reading a single byte. The default is 120 seconds.

    virtual void SetWriteTimeout(float timeOut);
    //: Set the amount of time you should attempt to write to a file descriptor.
    // This limits the time spent attempting to write to a socket
    // without sending a single byte. The default is 120 seconds.

    virtual bool SetNonBlocking(bool block);
    //: Enable non-blocking use of read and write.
    // true= read and write's won't do blocking waits.
    
    virtual void SetDontClose(bool ndontClose);
    //: Setup don't close flag.
    
    virtual void SetFailOnReadTimeout(bool val);
    //: Should read's fail on timeout ?
    // If false, the socket will be checked its
    // open and valid, if it is the read will be retried.

    virtual void SetFailOnWriteTimeout(bool val);
    //: Should write's fail on timeout ?
    // If false, the socket will be checked its
    // open and valid, if it is the write will be retried.
    
    virtual void SetFillBufferOnRead(bool value);
    //: Should read functions keep reading until the buffer is full?
    // If not the read will return immediately once any has been read.

    typedef RavlN::SmartPtrC<SerialAbstractC> RefT;
    //: Handle to SerialAbtractC
  };

}

#endif
