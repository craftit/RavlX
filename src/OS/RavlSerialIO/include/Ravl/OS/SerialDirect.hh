// This file is part of RAVL, Recognition And Vision Library
// Copyright (C) 2010-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_SERIALDIRECT_HEADER
#define RAVL_SERIALDIRECT_HEADER 1


/////////////////////////////////////////////////////////////
//! userlevel=Normal
//! date="16/09/2010"
//! author="Alex Kostin"
//! docentry="Ravl.API.OS.Hardware"
//! rcsid="$Id$"
//! lib=RavlSerialIO
//! file="Ravl/OS/Misc/SerialBase.hh"

#include "Ravl/OS/SerialAbstract.hh"
#if RAVL_OS_WIN32
#include "Ravl/OS/WinStreamIO.hh"
#else
#include "Ravl/OS/UnixStreamIO.hh"
#endif

struct termios ;


namespace RavlN {
  
  //! userlevel=Normal
  //: Base class to control serial ports.

  class SerialDirectC : public SerialAbstractC {
  public:
    SerialDirectC();
    //: default constructor;

    SerialDirectC(const char *dev, const char * perm = "RDWR",bool nonBlocking = true);
    //: open a device for initialize;

    virtual ~SerialDirectC(){ Close(); };
    //: destructor;

    virtual bool Open(const char *dev, const char * perm = "RDWR",bool nonBlocking = true);
    //: Open device.

    virtual void Close();
    //: Close the socket.
    // Note this will only actually close the socket if
    // m_dontClose is false.

    virtual bool Setup(IntT i_speed = 9600,
                       IntT o_speed = 9600,
                       IntT stop_bits = 1,
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

    virtual void SetReadTimeout(float timeout);
    //: Set the amount of time you should attempt to read from a file descriptor.
    // This limits the time spent attempting to write to a socket
    // without reading a single byte. The default is 120 seconds.

    virtual void SetWriteTimeout(float timeout);
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
  protected:
#if RAVL_OS_WIN32
    WinStreamIOC m_streamIo;
#else
    UnixStreamIOC m_streamIo;
#endif
    bool SetISpeed(termios &pb,IntT bitrate);
    //: Set input bit rate.

    bool SetOSpeed(termios &pb,IntT bitrate);
    //: Set output bit rate.

    bool SetCharSize(termios &pb,IntT bits);
    //: Set char size.

    bool SetStopBits(termios &pb,IntT bits);
    //: Set bit rate.

    bool SetParity(termios &pb,ParityT par);
    //: parity type: Odd or Even or None or 1
  };

}

#endif

