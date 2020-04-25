// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SERIALIO_HEADER
#define RAVL_SERIALIO_HEADER 1



/////////////////////////////////////////////////////////////
//! userlevel=Normal
//! date="31/12/1999"
//! author="Fangxiang Cheng"
//! docentry="Ravl.API.OS.Hardware"
//! rcsid="$Id$"
//! lib=RavlSerialIO
//! file="Ravl/OS/Misc/SerialIO.hh"

#include "Ravl/Stream.hh"
#include "Ravl/OS/SerialAbstract.hh"

namespace RavlN {
  
  //! userlevel=Normal
  //: Class to control serial ports.

  class SerialCtrlC : public RCHandleC<SerialAbstractC>, public SerialAbstractParityBaseC
  {
  public:
    SerialCtrlC();
    //: Default constructor.
    
    SerialCtrlC(SerialAbstractC &bod)
      : RCHandleC<SerialAbstractC>(bod)
    {}
    //: Body constructor.

    SerialCtrlC(const char *dev, const char * perm = "RDWR", bool nonBlocking = true);
    //: open a device for initialize;

    bool Open(const char *dev, const char * perm = "RDWR", bool nonBlocking = true);
    //: Open device.

    void Close() { Body().Close(); }
    //: Close port.
    // Note this will only actually close the port if
    // m_dontClose is false.

    bool Setup(IntT i_speed = 9600, IntT o_speed = 9600, IntT stop_bit = 1,
               ParityT par = SERIAL_PARITY_NONE, IntT char_size = 8)
    { return Body().Setup(i_speed, o_speed, stop_bit, par, char_size); }
    //: Setup the port.
    // with the given parameters: Input speed, Output speed,
    // stop bit(1,2), parity(exist or not exist), parity type(odd or even) and character size(5,6,7,8)

    void Reset() { Body().Reset(); }
    //: Reset interface.

    void Flush() { Body().Flush(); }
    //: Flush buffer.

    bool IsOpen() const { return IsValid() && Body().IsOpen(); }
    //: Is stream open ?

    IntT Read(char *buff,UIntT size) { return Body().Read(buff, size); }
    //: Read some bytes from a stream.

    IntT ReadV(char **buffer,IntT *len,int n) { return Body().ReadV(buffer, len, n); }
    //: Read some bytes from a stream.

    IntT Write(const char *buff,UIntT size) { return Body().Write(buff, size); }
    //: Write some bytes to a stream.

    IntT WriteV(const char **buffer,IntT *len,int n) { return Body().WriteV(buffer, len, n); }
    //: Write multiple buffers

    bool SetISpeed(const IntT i_speed) { return Body().SetISpeed(i_speed); }
    //: set the input speed of the port;

    bool SetOSpeed(const IntT o_speed) { return Body().SetOSpeed(o_speed); }
    //: set the output speed of the port;

    bool SetStopBits(const IntT stop_bit) { return Body().SetStopBits(stop_bit); }
    //: set the number of stop bits : 1 or 2;

    bool SetCharSize(const IntT char_size) { return Body().SetCharSize(char_size); }
    //: set the character size: 5,6,7,8;

    bool SetParity(ParityT par) { return Body().SetParity(par); }
    //: parity type: Odd or Even or None or 1

    IntT Fd() const { return Body().Fd(); }
    //: Access file descriptor.

    void SetReadTimeout(float timeout) { Body().SetReadTimeout(timeout); }
    //: Set the amount of time you should attempt to read from a file descriptor.
    // This limits the time spent attempting to write to a socket
    // without reading a single byte. The default is 120 seconds.

    void SetWriteTimeout(float timeout) { Body().SetWriteTimeout(timeout); }
    //: Set the amount of time you should attempt to write to a file descriptor.
    // This limits the time spent attempting to write to a socket
    // without sending a single byte. The default is 120 seconds.

    bool SetNonBlocking(bool block) { return Body().SetNonBlocking(block); }
    //: Enable non-blocking use of read and write.
    // true= read and write's won't do blocking waits.

    void SetDontClose(bool ndontClose) { Body().SetDontClose(ndontClose); }
    //: Setup don't close flag.
    
    void SetFailOnReadTimeout(bool val) { Body().SetFailOnReadTimeout(val); }
    //: Should read's fail on timeout ?
    // If false, the socket will be checked its
    // open and valid, if it is the read will be retried.

    void SetFailOnWriteTimeout(bool val) { Body().SetFailOnWriteTimeout(val); }
    //: Should write's fail on timeout ?
    // If false, the socket will be checked its
    // open and valid, if it is the write will be retried.
    
    void SetFillBufferOnRead(bool value) { Body().SetFillBufferOnRead(value); }
    //: Should read functions keep reading until the buffer is full?
    // If not the read will return immediately once any has been read.

  protected:
    SerialCtrlC(SerialAbstractC *bod)
      : RCHandleC<SerialAbstractC>(bod)
    {}
    //: Body constructor.

    SerialAbstractC &Body()
    { return RCHandleC<SerialAbstractC>::Body(); }
    //: Access body.

    const SerialAbstractC &Body() const
    { return RCHandleC<SerialAbstractC>::Body(); }
    //: Access body.
  };

  //////////////////////////////////////////////////////////////////////////////////////////

  //! userlevel=Normal
  //: Output serial stream.

  class OSerialC
    : public OStreamC,
      public SerialCtrlC
  {
  public:
    OSerialC()
    {}
    //: Default constructor.
    // Creates an invalid handle.

    OSerialC(const char *dev, bool buffered = false);
    //: Open a output serial stream.
    
    bool IsOpen() const
    { return SerialCtrlC::IsOpen(); }
    //: Test if the stream is open.
  };


  //! userlevel=Normal
  //: Input serial stream.

  class ISerialC
    : public IStreamC,
      public SerialCtrlC
  {
  public:
    ISerialC()
    {}
    //:Default constructor.
    // Creates an invalid handle.

    ISerialC (const char *dev, bool buffered = false);
    //: Open an input serial stream.

    bool IsOpen() const
    { return SerialCtrlC::IsOpen(); }
    //: Test if the stream is open.
  };

  //! userlevel=Normal
  //: Input/Output serial stream.

  class IOSerialC
    : public IStreamC,
      public OStreamC,
      public SerialCtrlC
  {
  public:
    IOSerialC()
    {}
    //:Default constructor.
    // Creates an invalid handle.

    IOSerialC (const char *dev, bool buffered = false);
    //: Open an input serial stream.

    bool good()
    { return IStreamC::good() && OStreamC::good(); }
    //: Test if stream is good

    bool operator!()
    { return IStreamC::operator!() || OStreamC::operator!(); }
    //: Test if stream is bad.

    bool Close();
    //: Close the stream.
    
    bool IsOpen() const
    { return SerialCtrlC::IsOpen(); }
    //: Test if the stream is open.
  };
}

#endif


