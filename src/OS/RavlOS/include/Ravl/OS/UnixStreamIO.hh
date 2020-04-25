// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2001-12, University of Surrey
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_UNIXSTREAMIO_HEADER
#define RAVL_UNIXSTREAMIO_HEADER 1
//! docentry="Ravl.API.OS"
//! author="Charles Galambos"
//! lib=RavlOS

#include "Ravl/Types.hh"

namespace RavlN {
 
  
  //: Helper class for dealing with IO on file descriptors.
  // Note: The default behaviour is to wait for reads until the file descriptor is closed,
  // but to return an error if the write fails after the given timeout.
  
  class UnixStreamIOC {
  public:
    UnixStreamIOC(int fd = -1,float writeTimeOut = 30,float readTimeOut = 30,bool dontClose = false)
      : m_fd(fd),
        m_writeTimeOut(writeTimeOut),
        m_readTimeOut(readTimeOut),
        m_dontClose(dontClose),
        m_failOnReadTimeout(false),
        m_failOnWriteTimeout(true),
        m_fillBufferOnRead(true)
    {}
    //: Constructor.
    
    ~UnixStreamIOC();
    //: Destructor.
    // Closes the file descriptor.
    
    void SetReadTimeout(float timeOut)
    { m_readTimeOut = timeOut; }
    //: Set the amount of time you should attempt to read from a file descriptor.
    // This limits the time spent attempting to write to a socket
    // without reading a single byte. The default is 120 seconds.
    
    void SetWriteTimeout(float timeOut)
    { m_writeTimeOut = timeOut; }
    //: Set the amount of time you should attempt to write to a file descriptor.
    // This limits the time spent attempting to write to a socket
    // without sending a single byte. The default is 120 seconds.
    
    IntT Read(char *buff,UIntT size);
    //: Read some bytes from a stream.
    
    IntT ReadV(char **buffer,IntT *len,int n);
    //: Read some bytes from a stream.
    
    IntT Write(const char *buff,UIntT size);
    //: Write some bytes to a stream.
    
    IntT WriteV(const char **buffer,IntT *len,int n);
    //: Write multiple buffers
    
    bool IsOpen() const
    { return m_fd >= 0; }
    //: Test of port is open.
    
    IntT Fd() const
    { return m_fd; }
    //: Access file descriptor.

    IntT& Fd()
    { return m_fd; }
    //: Access file descriptor.

    IntT Getfd() const
    { return m_fd; }
    //: Get the file descriptor of the port;

    bool SetNonBlocking(bool block);
    //: Enable non-blocking use of read and write.
    // true= read and write's won't do blocking waits.
    
    void SetDontClose(bool ndontClose)
    { m_dontClose = ndontClose; }
    //: Setup don't close flag.
    
    void SetFailOnReadTimeout(bool val)
    { m_failOnReadTimeout = val; }
    //: Should read's fail on timeout ?
    // If false, the socket will be checked its
    // open and valid, if it is the read will be retried.

    void SetFailOnWriteTimeout(bool val)
    { m_failOnWriteTimeout = val; }
    //: Should write's fail on timeout ?
    // If false, the socket will be checked its
    // open and valid, if it is the write will be retried.
    
    void SetFillBufferOnRead(bool value)
    { m_fillBufferOnRead = value; }
    //: Should read functions keep reading until the buffer is full?
    // If not the read will return immediately once any has been read.

    void Close();
    //: Close the socket.
    // Note this will only actually close the socket if
    // m_dontClose is false.
    
    bool WaitForRead();
    //: Wait for read to be ready.
    // Returns false on error.

    bool WaitForWrite();
    //: Wait for write to be ready.
    // Returns false on error.
  protected:
    
    bool CheckErrors(const char *opName);
    //: Check for recoverable errors.

    int m_fd;
    float m_writeTimeOut;
    float m_readTimeOut;
    bool m_dontClose;
    bool m_failOnReadTimeout;
    bool m_failOnWriteTimeout;
    bool m_fillBufferOnRead;
  };

}

#endif
