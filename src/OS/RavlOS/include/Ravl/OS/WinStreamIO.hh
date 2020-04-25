// This file is part of RAVL, Recognition And Vision Library 
// Copyright (C) 2008, OmniPerception Ltd.
// This code may be redistributed under the terms of the GNU Lesser
// General Public License (LGPL). See the lgpl.licence file for details or
// see http://www.gnu.org/copyleft/lesser.html
// file-header-ends-here
#ifndef RAVL_WINSTREAMIO_HEADER
#define RAVL_WINSTREAMIO_HEADER 1
//! lib=RavlOS

#include "Ravl/config.h"
#if RAVL_OS_WIN32
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <windows.h> 
#undef WIN32_LEAN_AND_MEAN
#endif

namespace RavlN {

#if RAVL_OS_WIN32
  class WinStreamIOC 
  {
  public:
    WinStreamIOC(float writeTimeOut = 30,float readTimeOut = 30)
      : //m_fd(fd),
        m_writeTimeOut((int)(1000*writeTimeOut)), // MS
        m_readTimeOut((int)(1000*readTimeOut))    // MS
		//,
       // m_dontClose(dontClose)
		//,
        //m_failOnReadTimeout(false),
        //m_failOnWriteTimeout(true),
        //m_fillBufferOnRead(true)
    {}
    //: Constructor.
        
    ~WinStreamIOC();
    //: Destructor.
    
    bool Open(const char *fileName);
    //: Open a file.
    
    bool Close();
    //: Close the file handle.
    
    void SetReadTimeout(float timeOut)
    { m_readTimeOut = (int)(1000*timeOut); }
    //: Set the amount of time you should attempt to read from a file descriptor.
    // This limits the time spent attempting to write to a socket
    // without reading a single byte. The default is 120 seconds.
    
    void SetWriteTimeout(float timeOut)
    { m_writeTimeOut = (int)(1000*timeOut); }
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
    { return m_portHandle != INVALID_HANDLE_VALUE; }
    //: Test of port is open.

  private:
    int m_readTimeOut; // time in msec
    int m_writeTimeOut;// time in msec
    HANDLE m_portHandle;
  };
  
#endif
}

#endif

